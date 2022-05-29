section .data
    asterisk db "*"
    equals db "="
    enterrrr db 0ah
section .text
    global _start

_start:
    

    mov r10, 1
loop1:
    cmp r10, 10
    je exit

    mov r9, 1
loop2:
    cmp r9, 10
    je incloop1

    
    mov rdi, r10
    call print_int

    
    mov rcx,asterisk
    call pritn_char

    
    mov rdi, r9
    call print_int

    
    mov rcx,equals
    call pritn_char


    mov rax, r9
    mul r10
    mov rdi, rax
    call print_int

    mov rcx, enterrrr
    call pritn_char

    inc r9
    jmp loop2

incloop1:
    mov rcx, enterrrr
    call pritn_char

    inc r10
    jmp loop1
    
    


exit:
    mov rax, 60
    mov rdi, 0
    syscall


print_int:      ;print_int reference: https://stackoverflow.com/questions/13166064/how-do-i-print-an-integer-in-assembly-level-programming-without-printf-from-the
    push rbp
    mov rbp, rsp
    sub rsp, 0x10

    mov eax, edi

    mov    ecx, 0xa
    mov    rsi, rsp

num_to_ascii:
    mov rdx, 0
    div    rcx
    add    edx, 0x30
    sub rsi, 0x04
    mov [rsi], edx

    cmp eax, 0
    jne num_to_ascii

    mov    rax, 1
    mov    rdi, 1
    lea    edx, [rsp+16]
    sub    edx, esi
    syscall

    leave
    ret

pritn_char:
    push rbp
    mov rbp, rsp
    sub rsp, 0x10 

    mov rax, 1
    mov rdi, 1
    mov rsi, rcx
    mov rdx, 1

    syscall

    leave
    ret
