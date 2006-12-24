        .text
        .align 16
        .global GCROOTS_save_regs_in_stack
        .proc GCROOTS_save_regs_in_stack
GCROOTS_save_regs_in_stack:
        .body
        flushrs
        ;;
        mov r8=ar.bsp
        br.ret.sptk.few rp
        .endp GCROOTS_save_regs_in_stack

