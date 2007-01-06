/*
 * Copyright (c) 2006-2007 YAMAMOTO Kengo <yamaken AT bp.iij4u.or.jp>
 * 
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

#ifndef _GCROOTS_PRIV_H
#define _GCROOTS_PRIV_H

#ifdef __cplusplus
extern "C" {
#endif

#define GC_save_regs_ret_val        GCROOTS_save_regs_ret_val
#define GC_jmp_buf                  GCROOTS_jmp_buf

#define GC_push_regs                GCROOTS_push_regs
#define GC_push_one                 GCROOTS_push_one
#define GC_push_current_stack       GCROOTS_push_current_stack
#define GC_push_regs_and_stack      GCROOTS_push_regs_and_stack
#define GC_save_regs_in_stack       GCROOTS_save_regs_in_stack
#define GC_clear_stack_inner        GCROOTS_clear_stack_inner
#define GC_with_callee_saves_pushed GCROOTS_with_callee_saves_pushed
#define GC_noop1                    GCROOTS_noop1

#ifdef __cplusplus
}
#endif

#endif /* _GCROOTS_PRIV_H */
