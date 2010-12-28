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

#ifndef _GCROOTS_H
#define _GCROOTS_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GCROOTS_VERSION_MAJOR      (0)
#define GCROOTS_VERSION_MINOR      (2)
#define GCROOTS_VERSION_PATCHLEVEL (4)
#define GCROOTS_API_REVISION       (1)

#define GCROOTS_VERSION_REQUIRE(major, minor, patchlevel)                    \
  ((major) < GCROOTS_VERSION_MAJOR                                           \
   || ((major) == GCROOTS_VERSION_MAJOR && (minor) < GCROOTS_VERSION_MINOR)  \
   || ((major) == GCROOTS_VERSION_MAJOR && (minor) == GCROOTS_VERSION_MINOR  \
       && (patchlevel) <= GCROOTS_VERSION_PATCHLEVEL))


typedef struct _GCROOTS_context GCROOTS_context;

typedef void (*GCROOTS_mark_proc)(void *start, void *end,
                                  int is_certain, int is_aligned);
typedef void *(*GCROOTS_user_proc)(void *arg);
typedef void *(*GCROOTS_context_alloc_proc)(size_t ctx_size);

/* may return NULL */
GCROOTS_context *GCROOTS_init(GCROOTS_context_alloc_proc allocator,
                              GCROOTS_mark_proc marker,
                              int scan_entire_system_stack);
void GCROOTS_fin(GCROOTS_context *ctx);

void *GCROOTS_call_with_gc_ready_stack(GCROOTS_context *ctx,
                                       GCROOTS_user_proc proc,
                                       void *arg);
void GCROOTS_mark(GCROOTS_context *ctx);

int GCROOTS_is_protected_context(GCROOTS_context *ctx);
int GCROOTS_is_protected(GCROOTS_context *ctx, void *obj);

#ifdef __cplusplus
}
#endif

#endif /* _GCROOTS_H */
