#include <stdio.h>
#include <stdlib.h>

#include <gcroots.h>

#if defined(__SANITIZE_ADDRESS__)
#  define ADDRESS_SANITIZER 1
#elif defined(__has_feature)
#  if __has_feature(address_sanitizer)
#    define ADDRESS_SANITIZER 1
#  endif
#endif

#if defined(_MSC_VER) && !defined(__clang__)
#  ifdef ADDRESS_SANITIZER
#    define NO_SANITIZE_ADDRESS __declspec(no_sanitize_address)
#  else
#    define NO_SANITIZE_ADDRESS
#  endif
#  define NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
#  ifdef ADDRESS_SANITIZER
#    define NO_SANITIZE_ADDRESS __attribute__((no_sanitize_address))
#  else
#    define NO_SANITIZE_ADDRESS
#  endif
#  define NOINLINE __attribute__((noinline))
#else
#  define NO_SANITIZE_ADDRESS
#  define NOINLINE
#endif

static GCROOTS_context *ctx;
static void *target;
static int n_found;

/* A mark procedure scans the stack conservatively. So it must not be
 * instrumented by AddressSanitizer. */
NO_SANITIZE_ADDRESS
static void
mark(void *start, void *end, int is_certain, int is_aligned)
{
    void **p;

    (void)is_certain;
    (void)is_aligned;
    for (p = (void **)start; p < (void **)end; p++) {
        if (*p == target)
            n_found++;
    }
}

/* The allocated object is referred only from *slot (and target). */
NOINLINE static void
allocate(void **slot)
{
    *slot = malloc(16);
    target = *slot;
}

NOINLINE static void
release(void **slot)
{
    free(*slot);
    *slot = NULL;
}

NOINLINE static long
clear_registers(long a, long b, long c, long d, long e, long f)
{
    return a + b + c + d + e + f;
}

/* Clear copies of the reference that allocate() left in unused stack
 * and registers. Then the reference is only in slots of user_proc().
 * This must not be instrumented by AddressSanitizer to clear the real
 * stack not a fake stack. */
NO_SANITIZE_ADDRESS
NOINLINE static void
clear_garbage(void)
{
    volatile char garbage[4096];
    size_t i;

    for (i = 0; i < sizeof(garbage); i++)
        garbage[i] = 0;
    (void)clear_registers(0, 0, 0, 0, 0, 0);
}

static void *
user_proc(void *arg)
{
    /* This is placed in a fake stack with AddressSanitizer's
     * detect_stack_use_after_return=1 because its address is taken. */
    void *slots[4] = {NULL, NULL, NULL, NULL};
    int *n_failures = arg;

    if (!GCROOTS_is_protected_context(ctx)) {
        fprintf(stderr, "must be protected context in user_proc\n");
        (*n_failures)++;
    }

    allocate(&slots[1]);
    clear_garbage();
    n_found = 0;
    GCROOTS_mark(ctx);
    if (n_found == 0) {
        fprintf(stderr, "object on stack must be marked\n");
        (*n_failures)++;
    }
    if (!GCROOTS_is_protected(ctx, target)) {
        fprintf(stderr, "object on stack must be protected\n");
        (*n_failures)++;
    }
    release(&slots[1]);

    return NULL;
}

int
main(void)
{
    int n_failures = 0;

    ctx = GCROOTS_init(malloc, mark, 0);
    if (!ctx) {
        fprintf(stderr, "failed to initialize\n");
        return EXIT_FAILURE;
    }
    if (GCROOTS_is_protected_context(ctx)) {
        fprintf(stderr, "must not be protected context before call\n");
        n_failures++;
    }
    GCROOTS_call_with_gc_ready_stack(ctx, user_proc, &n_failures);
    if (GCROOTS_is_protected_context(ctx)) {
        fprintf(stderr, "must not be protected context after call\n");
        n_failures++;
    }
    GCROOTS_fin(ctx);
    free(ctx);

    return n_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
