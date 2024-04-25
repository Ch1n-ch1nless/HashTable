global FastHashTableSearch

section .text

;===============================================
;Fast assembler implementation of HashTableSearch
;Entry: RDI =   address of hash table
;       RSI =   address of string 'key'
;       RDX =   len of string 'key'
;Exit:  RAX =   1, if elem is in hash table, else 0
;Dstr:  No 
;===============================================
FastHashTableSearch:
;v~~~~~~~~~~Save values of registers~~~~~~~~~v
                push rbx
                push rcx
                push r8
;^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^

;v~~~~~~~~~Prepare data for calculate~~~~~~~~~~v
                mov r8,  rsi                    ; R8  = address of string key
                mov rcx, rdx                    ; RCX = len of cycle
                mov rbx, rdx                    ; RBX = len of cycle
                mov eax, 0xFFFFFFFF             ; EAX = (uint32_t)-1
                cld
;^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^

;v~~~~~~~~~~~Calculate FastHashCrc32~~~~~~~~~~~v
        .FastHashCrc32:                         ;<---- Begin of cycle
                mov dl, BYTE [rsi]
                test dl, dl
                je .StopHash
                crc32 eax, dl
                inc rsi
                loop .FastHashCrc32             ;----> End of cycle
;^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^

        .StopHash:

                xor     edx, edx
                div     qword [rdi+8]           ; EDX = index of list in array

                lea     rax, [rdx+rdx*2]        ;<\
                sal     rax, 4                  ;<-+ RAX = address of list in which the word will be searched
                add     rax, qword [rdi]        ;</

;v~~~~~~~~~~~Search in List the key~~~~~~~~~~~~v
                vmovdqu      ymm0, [r8]         ; Load key to YMM0

                mov     rsi, QWORD [rax+32]     ; RSI = size of list
                test    rsi, rsi                ; 
                je      .IndexIsNotFound        ; if (RSI == 0) go to .IndexIsNotFound

                mov     rdx, QWORD [rax]
                xor     eax, eax

        .BeginOfCycle:                          ;<---- Begin of search cycle
                cmp     rbx, QWORD [rdx+32]
                jne     .GoToNextNode           ; if (RBX != list->data[i].size) i = list->next[i]

                vmovdqu      ymm1, [rdx]        ; Load the list->data[i].key to YMM1
                vpcmpeqb     ymm1, ymm1, ymm0   ; Compare YMM1 and YMM0
                vpmovmskb    edi, ymm1          ; EDI = mask(YMM1)

                cmp     edi, -1
                je      .EndOfFunction          ; if (key == list->data[i].key) return eax
        .GoToNextNode:
                add     rax, 1                  ;<-+ i = list->next[i]
                add     rdx, 40                 ;</

                cmp     rsi, rax                ; if (RSI != RAX) go to next node in list
                jne     .BeginOfCycle           ;----> End of search cycle

        .IndexIsNotFound:
                mov     eax, -666               ; EAX = LIST_INVALID_INDEX

;^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^

;v~~~~~~~~Return old values of registers~~~~~~~v
        .EndOfFunction:        
                pop r8
                pop rcx
                pop rbx
;^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^

                vzeroupper
                ret

;===============================================