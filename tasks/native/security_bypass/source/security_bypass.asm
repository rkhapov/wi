bits 64
default rel


extern ExitProcess
extern GetStdHandle
extern WriteFile

global main
global security_cookie
global security_fail


segment .data

flag: db 83, 69, 95, 88, 10, 76, 70, 75, 77, 10, 67, 89, 16, 10, 78, 79, 75, 78, 72, 79, 79, 76, 117, 73, 75, 76, 79, 72, 75, 72, 79, 39, 32
flag_len equ $ - flag
db 0, 0, 0, 0

segment .text

security_cookie: dd 0xdeadbeef

main:
    cmp     dword [security_cookie], 0xcafebabe
    je      .security_checked

    call    security_fail

.security_checked:
    lea     rsi, [flag]
    lea     rdi, [flag]
    cld

    mov     rcx, flag_len

.xoring:
    lodsb
    xor     al, 42
    stosb

    loop    .xoring

    sub     rsp, 0x30
    mov     rcx, -11
    call    GetStdHandle
    add     rsp, 0x30

    sub     rsp, 0x30
    mov     rcx, rax
    lea     rdx, [flag]
    mov     r8, flag_len
    mov     r9, 0
    push    r9
    call    WriteFile
    pop     r9
    add     rsp, 0x30

    xor     rcx, rcx
    call    ExitProcess


broken_cookie_msg: db "Security cookie is broken :(", 10, 13, 0
broken_cookie_msg_len equ $ - broken_cookie_msg

security_fail:
    sub     rsp, 0x28
    mov     rcx, -11
    call    GetStdHandle
    add     rsp, 0x28

    sub     rsp, 0x28
    mov     rcx, rax
    lea     rdx, [broken_cookie_msg]
    mov     r8, broken_cookie_msg_len
    mov     r9, 0
    push    r9
    call    WriteFile
    pop     r9
    add     rsp, 0x28

    mov     rcx, 1
    call    ExitProcess
