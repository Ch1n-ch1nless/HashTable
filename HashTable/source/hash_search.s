global FastHashTableSearch

section .text

;===============================================
;Fast assembler implementation of HashTableSearch
;Entry: RDI =   address of hash table
;       RSI =   address of string 'key'
;       RDX =   len of string 'key'
;Exit:  RAX =   1, if elem is in hash table, else 0
;Dstr:  RCX, RBX
;===============================================
FastHashTableSearch:
;v~~~Calculate FastHashCrc32~~~v
            mov rcx, rdx                        ; RCX = len of cycle
            mov rax, 0xFFFFFFFF                 ; EAX = (uint32_t)-1



            crc32 
