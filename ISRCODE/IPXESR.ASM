;*****************************************************************************
; (C) Chris Wood 1995.
;
; IPXESR.ASM - Send and receive ESR handlers for the IPX code.
;
;***************************************************************************** 

;*****************************************************************************
; We are using the large memory model.
     .MODEL    LARGE

;*****************************************************************************
; The two handlers defined here.
     public    _IPXSentESRHandler
     public    _IPXRecvESRHandler

;*****************************************************************************
; The two callback functions for adding the packets into ECB lists.
     EXTRN     _IPXSentPacket:PROC
     EXTRN     _IPXRecvPacket:PROC

;*****************************************************************************
; The data segment contains an area which is to be used for a temporary stack
; while the ESR is processing the interrupt. It also contains an area for
; storing the old stack segment and pointer.
     .DATA
     stack_seg DW   0
     stack_ptr DW   0
     esr_stack DW   512  ; 1K stack.
     stack_top DW   0    ; Stack grows downwards.

;*****************************************************************************
; The ESR handler receives the address of the ECB into ES:SI. It first loads
; the DATA segemnt into DS, and saves the old stack pointer. It then sets up
; a temporary stack, pushes the ECB address onto it and makes the call to the
; real ESR handler. Finally it restores the old stack pointer and returns.
; Two ESR handlers are used, one for handling packets that are sent, and the
; other for packets when they are received.
     .CODE

;*****************************************************************************
; This routine passes ECBs that have been sent onto IPXSentPacket() which will
; add them back into the free 'sending' packet list.
_IPXSentESRHandler PROC FAR

     ; First load DS with the DATA segment.
     mov  ax,DGroup
     mov  ds,ax

     ; Save the old stack pointer.
     mov  stack_seg,ss
     mov  stack_ptr,sp

     ; Set up our 1K temporary stack.
     mov  ax,ds
     mov  ss,ax
     mov  sp,OFFSET stack_top

     ; Push ECB address onto stack and call real ESR handler.
     push es
     push si
     call _IPXSentPacket

     ; Restore the old stack pointer.
     mov  ss,stack_seg
     mov  sp,stack_ptr

     ; Return.
     retf
_IPXSentESRHandler ENDP

;*****************************************************************************
; This routine passes ECBs that have been recieved onto IPXRecvPacket() which
; will add them into the 'recieved' packet list.
_IPXRecvESRHandler PROC FAR

     ; First load DS with the DATA segment.
     mov  ax,DGroup
     mov  ds,ax

     ; Save the old stack pointer.
     mov  stack_seg,ss
     mov  stack_ptr,sp

     ; Set up our 1K temporary stack.
     mov  ax,ds
     mov  ss,ax
     mov  sp,OFFSET stack_top

     ; Push ECB address onto stack and call real ESR handler.
     push es
     push si
     call _IPXRecvPacket

     ; Restore the old stack pointer.
     mov  ss,stack_seg
     mov  sp,stack_ptr

     ; Return.
     retf
_IPXRecvESRHandler ENDP

;*****************************************************************************
; End of module.
     END
