;; 
;; C Run-time startup module for MPLAB XC16 C compiler.
;; (c) Copyright 2002,2004,2007 - 2016 Microchip Technology, All rights reserved
;;
;; Alternate version, without data initialization support.
;; The linker loads this version when the --no-data-init
;; option is selected.
;; 
;; See file crt0.s for the primary version with
;; data initialization support.
;; 
;; Entry __reset takes control at device reset and
;; performs the following:
;;
;;  1. initialize stack and stack limit register
;;  2. initialize PSV window if __const_length > 0
;;  3. call the .user_init section, if it exists
;;  4. call the user's _main entry point
;;
;; Assigned to section .init, which may be allocated
;; at a specific address in linker scripts. 
;;
;; If a local copy of this file is customized, be sure
;; to choose a file name other than crt0.s or crt1.s.
;;
;;	.section *, code, boot, address(0x400)
	.section *, code
        .global __resetALT
        .ifdef __C30ELF
        .type   __resetALT,@function
        .endif
__resetALT:     
        .weak  __reset
        .ifdef __C30ELF
        .type   __reset,@function
        .endif
__reset:
;;
;; Initialize stack and PSV window
;; 
;; registers used:  w1
;;  (w0 is cleared by device reset, so ARGC = 0)
;;
;; Inputs (defined by user or linker):
;;  __SP_init         = initial value of stack pointer
;;  __SPLIM_init      = initial value of stack limit register

;; Inputs (defined by linker):
;;  __const_length    = length of section .const
;;  __const_psvpage   = PSVPAG setting for section .const
;; 
;; Outputs:
;;  (does not return - resets the processor)
;; 
;; Calls:
;;  _main
;; 
        .weak    __user_init, __has_user_init

        mov      #__SP_init,w15    ; initialize w15
        mov      #__SPLIM_init,w14

;; Uncomment to pre-initialize all RAM
;;
;; Start at the beginning of RAM, writing the value __DATA_INIT_VAL to memory
;;   write __STACK_INIT_VAL if the memory is part of stack space
;; cycle through to the end of RAM
;;
;; define initialization values as equate:
;; .global __DATA_INIT_VAL, __STACK_INIT_VAL
;; .equ __DATA_INIT_VAL, 0xDEAD
;; .equ __STACK_INIT_VAL, 0xA1DE
;;

;
;        mov      #__DATA_BASE,w0
;        mov      #__DATA_LENGTH,w1
;        mov      #__DATA_INIT_VAL,w3 ; start of initializing RAM
;        add      w0,w1,w1
;
;1:      cp       w0,w15
;        bra      geu, 2f             ; move to initializing STACK
;        mov      w3,[w0++]
;        cp       w0, w1
;        bra      ltu, 1b
;        bra      1f
;
;2:      mov      #__STACK_INIT_VAL,w3
;        setm     w15
;3:      mov      w3,[w0++]
;        cp       w0,w14
;        bra      nz,3b
;        mov      #__DATA_INIT_VAL,w3
;        cp       w0,w1
;        bra      ltu,1b
;1:      mov      #__SP_init,w15    ; (RE) initialize w15
;
;
;;  end RAM PRE-init

        mov      w14,_SPLIM        ; initialize SPLIM

        mov      #__const_length,w1  ; 
        cp0      w1                  ; test length of constants
        bra      z,1f                ; br if zero

        mov      #__const_psvpage,w1 ; 
        mov      w1,_DSRPAG          ; PSVPAG = psvpage(constants)
1:
        mov      #__has_user_init,w0 ;
        cp0      w0                  ; user init functions?
        bra      z,2f                ; br if not
        call     __user_init         ; else call them
2:
        call  _main                  ; call user's main()

        .pword 0xDA4000              ; halt the simulator
        reset                        ; reset the processor


.include "null_signature_boot.s"

