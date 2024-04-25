global FastHashCRC32

section .text

;===============================================
;Fast assembler implementation of HashCRC32
;Entry: RDI =   address of string 'key'
;       RSI =   len of string 'key'
;Exit:  EAX =   value of hash
;Dstr:  No 
;===============================================
FastHashCRC32:
        mov eax, -1
        movzx  edx, BYTE [rdi]
        test    dl, dl
        je      .end_of_cycle
        add     rdi, 1
        mov     eax, -1
    .next_char:
        add     rdi, 1
        crc32   eax, dl
        movzx   edx, BYTE [rdi-1]
        test    dl, dl
        jne     .next_char
    .end_of_cycle:
        not     eax
        ret
;===============================================