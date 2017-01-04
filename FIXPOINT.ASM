; FIXPOINT.ASM
    .CODE
    .386p
    PUBLIC fixmul_, fixmul2_,fixdiv_, shf1_, qrshift_

fixmul_       PROC 
              PUSH    EDX
              IMUL    EDX
              SHLD    EDX,EAX,16      ; shift ax hi to lo
              MOV     EAX,EDX
              POP     EDX
              RET

fixmul_       ENDP

fixmul2_      PROC 
              PUSH    EDX
              IMUL    EDX
              SHLD    EDX,EAX,24      ; shift ax hi to lo
              MOV     EAX,EDX
              POP     EDX
              RET

fixmul2_      ENDP

fixdiv_       PROC
              PUSH    ECX
              PUSH    EDX
              test    eax,eax
              jz      fixdivjp
              test    edx,edx
              jz      fixdivjp
              mov     ecx,edx
              mov     edx,eax
              sar     edx,16
              shl     eax,16
              idiv    ecx              ;eax now holds result
              POP     EDX
              POP     ECX
              Ret
fixdivjp:     xor     eax,eax  
              POP     EDX
              POP     ECX
              Ret

fixdiv_       ENDP

fixdiv2_      PROC
              PUSH    ECX
              PUSH    EDX
              mov     cx,dx
              xor     dx,dx  
              idiv    cx              ;eax now holds result
              shl     eax,16
              mov     ax,dx
              POP     EDX
              POP     ECX
              Ret
fixdiv2_      ENDP

shf1_         PROC 
              PUSH    EDX
              XOR     EDX,EDX
              SHLD    EDX,EAX,10      ; shift ax hi to lo
              MOV     EAX,EDX
              POP     EDX
              RET

shf1_         ENDP

qrshift_      PROC 
              PUSH    CX
              MOV     CL,DL
              SHR     EAX,CL
              POP     CX
              RET

qrshift_      ENDP

              END
