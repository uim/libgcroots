/*
 * Copyright (c) 2006-2008 YAMAMOTO Kengo <yamaken AT bp.iij4u.or.jp>
 * All rights reserved.
 * 
 * THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY EXPRESSED
 * OR IMPLIED.  ANY USE IS AT YOUR OWN RISK.
 * 
 * Permission is hereby granted to use or copy this program
 * for any purpose,  provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 */

#include <assert.h>

#include "gcroots.h"
#include "private/gcroots_priv.h"
#include "private/gc_priv.h"

#if ((defined(__MWERKS__) && !defined(POWERPC))                              \
     || (!defined(USE_ASM_PUSH_REGS) && defined(M68K))                       \
     || defined(ALPHA) || defined(MIPS) || defined(POWERPC))
#define EXTRA_CONTEXT_IN_REGS
#endif

#ifdef THREADS
#error "threaded configuration is not supported (yet)"
#endif


#ifdef EXTRA_CONTEXT_IN_REGS
/* must always be capable of all push */
#define PUSHED_REGS_SIZE 64
#endif

struct _GCROOTS_context {
    void *stack_base;
#ifdef IA64
    void *register_stack_bs_base;
#endif
    GCROOTS_mark_proc mark;
    GC_bool use_system_stack_bottom;
};

static void find_obj(void *start, void *end, int is_certain, int is_aligned);

static void *findee;
static int found;

#if defined(SPARC) || defined(IA64)
extern ptr_t GC_save_regs_ret_val;
#endif

#ifdef EXTRA_CONTEXT_IN_REGS
static int n_pushed_regs;
static word pushed_regs[PUSHED_REGS_SIZE];
#endif

GCROOTS_context *
GCROOTS_init(GCROOTS_context_alloc_proc allocator, GCROOTS_mark_proc marker,
             int scan_entire_system_stack)
{
    GCROOTS_context *ctx;

    assert(allocator);
    assert(marker);
    /* scan_entire_system_stack is not supported yet */
    assert(!scan_entire_system_stack);

    ctx = (*allocator)(sizeof(GCROOTS_context));
    if (ctx) {
        ctx->mark = marker;
        ctx->use_system_stack_bottom = scan_entire_system_stack;
        ctx->stack_base = NULL;
#ifdef IA64
        ctx->register_stack_bs_base = NULL;
#endif
#ifdef EXTRA_CONTEXT_IN_REGS
        n_pushed_regs = 0;
#endif
    }

    return ctx;
}

void
GCROOTS_fin(GCROOTS_context *ctx)
{
    assert(ctx);

    /* Nothing to do for this implementation. Caller must free ctx. */
}

void *
GCROOTS_call_with_gc_ready_stack(GCROOTS_context *ctx,
                                 GCROOTS_user_proc proc, void *arg)
{
    void *ret;
    word stack_top; /* approx */
    volatile GCROOTS_user_proc anti_inline_proc;

    assert(ctx);
    assert(proc);

    if (!ctx->stack_base) {
        ctx->stack_base = &stack_top;
#ifdef IA64
        ctx->register_stack_bs_base = GC_save_regs_in_stack();
#endif
    }

    anti_inline_proc = proc;
    ret = (*anti_inline_proc)(arg);

    if (ctx->stack_base == &stack_top) {
        ctx->stack_base = NULL;
#ifdef IA64
        ctx->register_stack_bs_base = NULL;
#endif
    }

    return ret;
}

void
GCROOTS_mark(GCROOTS_context *ctx)
{
    assert(ctx);

    GC_push_regs_and_stack((ptr_t)ctx);
}

int
GCROOTS_is_protected_context(GCROOTS_context *ctx)
{
    assert(ctx);

    return (ctx->stack_base) ? TRUE : FALSE;
}

int
GCROOTS_is_protected(GCROOTS_context *ctx, void *obj)
{
    GCROOTS_context tmp_ctx;

    assert(ctx);

    if (!GCROOTS_is_protected_context(ctx))
        return FALSE;

    tmp_ctx = *ctx;
    tmp_ctx.mark = find_obj; /* not actually a mark function */
    findee = obj;
    found = FALSE;
    GCROOTS_mark(&tmp_ctx);

    return found;
}

/* thread unsafe */
static void
find_obj(void *start, void *end, int is_certain, int is_aligned)
{
    void **p;
    int offset;

    offset = 0;
    do {
        for (p = (void **)start + offset; p < (void **)end; p++) {
            if (*p == findee) {
                found = TRUE;
                return;
            }
        }
        offset += ALIGNOF_VOID_P;
    } while (!is_aligned
             && SIZEOF_VOID_P != ALIGNOF_VOID_P
             && offset % SIZEOF_VOID_P);
}

/*
 * Substitution functions for original Boehm GC
 */

/* mark immediately despite the name 'push_current_stack' */
void
GC_push_current_stack(ptr_t cold_gc_frame, void *context)
{
    word stack_top; /* approx */
#ifdef IA64
    void *bsp;
#endif
    GCROOTS_context *ctx;

    ctx = (GCROOTS_context *)cold_gc_frame;
    assert(ctx->stack_base || ctx->use_system_stack_bottom);

#ifdef STACK_GROWS_DOWN
    (*ctx->mark)(&stack_top + 1, ctx->stack_base, FALSE, FALSE);
#else
    (*ctx->mark)((word *)ctx->stack_base + 1, &stack_top, FALSE, FALSE);
#endif

#ifdef IA64
    /* register stack backing store grows upward */
    bsp = GC_save_regs_ret_val;
    (*ctx->mark)(ctx->register_stack_bs_base, bsp, FALSE, TRUE);
#endif

#ifdef EXTRA_CONTEXT_IN_REGS
    assert(n_pushed_regs <= PUSHED_REGS_SIZE);
    (*ctx->mark)(pushed_regs, &pushed_regs[n_pushed_regs], FALSE, TRUE);
    n_pushed_regs = 0;
#endif
}

#ifdef EXTRA_CONTEXT_IN_REGS
# if defined(MSWIN32) || defined(MSWINCE)
void __cdecl GC_push_one(word p)
# else
void GC_push_one(word p)
# endif
{
    pushed_regs[n_pushed_regs++] = p;
}
#endif /* EXTRA_CONTEXT_IN_REGS */
