.386


_TEXT16 SEGMENT BYTE PUBLIC USE16 'CODE'

        ASSUME  cs:_TEXT16

        PUBLIC  rmhandler_

rmhandler_:
        push    ax
        push    dx
        mov     dx,20h
        mov     al,dl
        out     dx,al
        pop     dx
        pop     ax
        iret

        _TEXT16  ENDS


END

