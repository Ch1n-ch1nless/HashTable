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
        crc32 rax, QWORD [rdi]
        crc32 rax, QWORD [rdi+8]
        crc32 rax, QWORD [rdi+16]
        crc32 rax, QWORD [rdi+24]
        not     eax
        ret
;===============================================