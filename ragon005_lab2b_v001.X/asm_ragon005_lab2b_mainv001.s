.include "xc.inc"
.text                       ;BP (put the following data in ROM(program memory))
 ; This is a library, thus it can *not* contain a _main function: the C file will
 ; define main().  However, we ; we will need a .global statement to make available ASM functions to C code.
 ; All functions utilized outside of this file will need to have a leading
 ; underscore (_) and be included in a comment delimited list below.
.global _write_0a, _write_1a, _oneMilliSec
 
    
;20 cycles - 12 high 8 low
_write_1a:
    ;call 2
    inc LATA	    ;high 1
  
    repeat #9	    ;high 1
    nop		    ;high 9
    clr LATA	    ;low 1
    nop		    ;low 1
    nop		    ;low 1
    return	    ;low 3 + 2 for call

;20 cycles - 1 high 19 low
_write_0a:
    ;call 2
    inc LATA	    ;high 1
    clr LATA	    ;low 1
    repeat #11	    ;low 1
    nop		    ;low 12
    return	    ;low 3 + 2 for call
    
_oneMilliSec:
		    ;if needed reduce 2 cycle on repeat for bra
		    ;2 cycles call
    repeat #15993   ;1 cycle
    nop		    ;15996 cycle
    return	    ;3 cycle
    .end