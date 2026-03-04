	.file	"main.c"
	.text
	.section	.rodata
	.align 8
.LC0:
	.string	"Error : Skipped adding node because nodes_container is null.\n"
	.align 8
.LC1:
	.string	"Error : Skipped adding node because label is null.\n"
	.align 8
.LC2:
	.string	"Error : Memory re-allocation failed. Will not store item %s. \n"
	.text
	.globl	NODES_ADD
	.type	NODES_ADD, @function
NODES_ADD:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	cmpq	$0, -24(%rbp)
	jne	.L2
	movq	stderr(%rip), %rax
	leaq	.LC0(%rip), %rdi
	movq	%rax, %rcx
	movl	$61, %edx
	movl	$1, %esi
	call	fwrite@PLT
	jmp	.L1
.L2:
	cmpq	$0, -32(%rbp)
	jne	.L4
	movq	stderr(%rip), %rax
	leaq	.LC1(%rip), %rdi
	movq	%rax, %rcx
	movl	$51, %edx
	movl	$1, %esi
	call	fwrite@PLT
	jmp	.L1
.L4:
	movq	-24(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	cmpq	%rax, %rdx
	jb	.L5
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	jne	.L6
	movq	-24(%rbp), %rax
	movq	$256, (%rax)
	jmp	.L7
.L6:
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	leaq	(%rax,%rax), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, (%rax)
.L7:
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	(%rax), %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	realloc@PLT
	movq	-24(%rbp), %rdx
	movq	%rax, 16(%rdx)
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	testq	%rax, %rax
	jne	.L5
	movq	-32(%rbp), %rax
	leaq	.LC2(%rip), %rdx
	movq	%rax, %rsi
	movq	%rdx, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-24(%rbp), %rax
	movq	-8(%rbp), %rdx
	movq	%rdx, 16(%rax)
	jmp	.L1
.L5:
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	movl	%eax, -12(%rbp)
	movq	-40(%rbp), %rax
	movl	$15, %edx
	cmpq	%rdx, %rax
	cmova	%rdx, %rax
	movq	%rax, -40(%rbp)
	movq	-24(%rbp), %rax
	movq	16(%rax), %rcx
	movl	-12(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	addq	%rcx, %rax
	leaq	8(%rax), %rcx
	movq	-40(%rbp), %rdx
	movq	-32(%rbp), %rax
	movq	%rax, %rsi
	movq	%rcx, %rdi
	call	memcpy@PLT
	movq	-24(%rbp), %rax
	movq	16(%rax), %rcx
	movl	-12(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	leaq	(%rcx,%rax), %rdx
	movq	-40(%rbp), %rax
	movq	%rax, (%rdx)
	movq	-24(%rbp), %rax
	movq	16(%rax), %rcx
	movl	-12(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	leaq	(%rcx,%rax), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	addq	$8, %rax
	movb	$0, (%rax)
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	leaq	1(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, 8(%rax)
.L1:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	NODES_ADD, .-NODES_ADD
	.section	.rodata
.LC3:
	.string	"Error : nodes is NULL.\n"
.LC4:
	.string	"Error : Index out of range.\n"
	.text
	.globl	NODES_READ
	.type	NODES_READ, @function
NODES_READ:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L9
	movq	stderr(%rip), %rax
	leaq	.LC3(%rip), %rdi
	movq	%rax, %rcx
	movl	$23, %edx
	movl	$1, %esi
	call	fwrite@PLT
	movl	$0, %eax
	jmp	.L10
.L9:
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	cmpq	%rax, -16(%rbp)
	jb	.L11
	movq	stderr(%rip), %rax
	leaq	.LC4(%rip), %rdi
	movq	%rax, %rcx
	movl	$28, %edx
	movl	$1, %esi
	call	fwrite@PLT
	movl	$0, %eax
	jmp	.L10
.L11:
	movq	-8(%rbp), %rax
	movq	16(%rax), %rcx
	movq	-16(%rbp), %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	addq	%rcx, %rax
.L10:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	NODES_READ, .-NODES_READ
	.globl	NODES_FREE
	.type	NODES_FREE, @function
NODES_FREE:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, %rdi
	call	free@PLT
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	NODES_FREE, .-NODES_FREE
	.section	.rodata
	.align 8
.LC5:
	.string	"Error : Malloc failed, couldn't create nodes \n"
	.text
	.globl	NODES_NEW
	.type	NODES_NEW, @function
NODES_NEW:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$24, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L14
	movq	stderr(%rip), %rax
	leaq	.LC5(%rip), %rdi
	movq	%rax, %rcx
	movl	$46, %edx
	movl	$1, %esi
	call	fwrite@PLT
	movl	$0, %eax
	jmp	.L15
.L14:
	movq	-8(%rbp), %rax
	movl	$24, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	memset@PLT
	movq	-8(%rbp), %rax
.L15:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	NODES_NEW, .-NODES_NEW
	.section	.rodata
.LC6:
	.string	"rb"
.LC7:
	.string	"File %s not found.\n"
	.align 8
.LC8:
	.string	"Error allocating \"%s\" in a buffer\n"
	.text
	.globl	read_file
	.type	read_file, @function
read_file:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	leaq	.LC6(%rip), %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	fopen@PLT
	movq	%rax, -24(%rbp)
	cmpq	$0, -24(%rbp)
	jne	.L17
	movq	stderr(%rip), %rax
	movq	-40(%rbp), %rdx
	leaq	.LC7(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$0, %eax
	jmp	.L18
.L17:
	movq	-24(%rbp), %rax
	movl	$2, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	fseek@PLT
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	ftell@PLT
	movq	%rax, -16(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	rewind@PLT
	movq	-16(%rbp), %rax
	addq	$1, %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L19
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	movq	stderr(%rip), %rax
	movq	-40(%rbp), %rdx
	leaq	.LC8(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$0, %eax
	jmp	.L18
.L19:
	movq	-16(%rbp), %rdx
	movq	-24(%rbp), %rcx
	movq	-8(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	fread@PLT
	movq	-16(%rbp), %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movb	$0, (%rax)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	movq	-8(%rbp), %rax
.L18:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	read_file, .-read_file
	.section	.rodata
.LC9:
	.string	"mock-response.txt"
	.text
	.globl	AI_RUN_MOCK
	.type	AI_RUN_MOCK, @function
AI_RUN_MOCK:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	leaq	.LC9(%rip), %rax
	movq	%rax, %rdi
	call	read_file
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	AI_RUN_MOCK, .-AI_RUN_MOCK
	.section	.rodata
.LC10:
	.string	"\"nodes\""
.LC11:
	.string	"\"connections\""
	.align 8
.LC12:
	.string	"Error, response has no nodes param %s\n"
	.align 8
.LC13:
	.string	"Error, response has no connections param %s\n"
	.align 8
.LC14:
	.string	"Error : couldn't identify nodes block"
	.text
	.globl	ADD_DATA_FROM_RESPONSE
	.type	ADD_DATA_FROM_RESPONSE, @function
ADD_DATA_FROM_RESPONSE:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$176, %rsp
	movq	%rdi, -168(%rbp)
	movq	%rsi, -176(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	.LC10(%rip), %rdx
	movq	-176(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strstr@PLT
	movq	%rax, -136(%rbp)
	leaq	.LC11(%rip), %rdx
	movq	-176(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strstr@PLT
	movq	%rax, -128(%rbp)
	cmpq	$0, -136(%rbp)
	jne	.L23
	movq	stderr(%rip), %rax
	movq	-176(%rbp), %rdx
	leaq	.LC12(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$0, %eax
	jmp	.L24
.L23:
	cmpq	$0, -128(%rbp)
	jne	.L25
	movq	stderr(%rip), %rax
	movq	-176(%rbp), %rdx
	leaq	.LC13(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$0, %eax
	jmp	.L24
.L25:
	movq	-136(%rbp), %rax
	movl	$91, %esi
	movq	%rax, %rdi
	call	strchr@PLT
	movq	%rax, -152(%rbp)
	movq	-136(%rbp), %rax
	movl	$93, %esi
	movq	%rax, %rdi
	call	strchr@PLT
	movq	%rax, -120(%rbp)
	cmpq	$0, -152(%rbp)
	je	.L26
	cmpq	$0, -120(%rbp)
	jne	.L27
.L26:
	movq	stderr(%rip), %rax
	leaq	.LC14(%rip), %rdi
	movq	%rax, %rcx
	movl	$37, %edx
	movl	$1, %esi
	call	fwrite@PLT
	movl	$0, %eax
	jmp	.L24
.L27:
	movq	-152(%rbp), %rax
	movl	$34, %esi
	movq	%rax, %rdi
	call	strchr@PLT
	movq	%rax, -112(%rbp)
	movq	-112(%rbp), %rax
	addq	$1, %rax
	movl	$34, %esi
	movq	%rax, %rdi
	call	strchr@PLT
	movq	%rax, -104(%rbp)
	movq	-112(%rbp), %rax
	cmpq	-120(%rbp), %rax
	jnb	.L39
	cmpq	$0, -112(%rbp)
	je	.L29
	cmpq	$0, -104(%rbp)
	je	.L29
	movq	-104(%rbp), %rax
	subq	-112(%rbp), %rax
	subq	$1, %rax
	movq	%rax, %rdx
	movq	-112(%rbp), %rax
	leaq	1(%rax), %rcx
	movq	-168(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	NODES_ADD
	movq	-104(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -152(%rbp)
	jmp	.L27
.L39:
	nop
.L29:
	movq	-128(%rbp), %rax
	movl	$91, %esi
	movq	%rax, %rdi
	call	strchr@PLT
	movq	%rax, -144(%rbp)
	cmpq	$0, -144(%rbp)
	jne	.L30
	movq	stderr(%rip), %rax
	leaq	.LC14(%rip), %rdi
	movq	%rax, %rcx
	movl	$37, %edx
	movl	$1, %esi
	call	fwrite@PLT
	movl	$0, %eax
	jmp	.L24
.L30:
	movl	$0, -160(%rbp)
	jmp	.L31
.L37:
	cmpq	$0, -144(%rbp)
	je	.L40
	movq	-144(%rbp), %rax
	addq	$1, %rax
	movl	$91, %esi
	movq	%rax, %rdi
	call	strchr@PLT
	movq	%rax, -96(%rbp)
	movq	-96(%rbp), %rax
	addq	$1, %rax
	movl	$93, %esi
	movq	%rax, %rdi
	call	strchr@PLT
	movq	%rax, -88(%rbp)
	cmpq	$0, -96(%rbp)
	je	.L40
	cmpq	$0, -88(%rbp)
	je	.L40
	movq	-96(%rbp), %rax
	cmpq	%rax, -88(%rbp)
	jb	.L41
	movq	-88(%rbp), %rax
	subq	-96(%rbp), %rax
	movl	%eax, -156(%rbp)
	cmpl	$63, -156(%rbp)
	jle	.L35
	movl	$63, -156(%rbp)
.L35:
	movq	-88(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -144(%rbp)
	addl	$1, -160(%rbp)
.L31:
	cmpl	$50, -160(%rbp)
	jle	.L37
	jmp	.L36
.L40:
	nop
	jmp	.L36
.L41:
	nop
.L36:
	movl	$1, %eax
.L24:
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L38
	call	__stack_chk_fail@PLT
.L38:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	ADD_DATA_FROM_RESPONSE, .-ADD_DATA_FROM_RESPONSE
	.section	.rodata
.LC15:
	.string	"Mock response %s\n"
.LC16:
	.string	"Found node : %s\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movabsq	$7597061734094825519, %rax
	movabsq	$7147061311504867700, %rdx
	movq	%rax, -64(%rbp)
	movq	%rdx, -56(%rbp)
	movabsq	$7453001534313295984, %rax
	movabsq	$8317134093887024997, %rdx
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
	movabsq	$4047948189317689647, %rax
	movabsq	$8459843873184755042, %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movw	$102, -16(%rbp)
	call	NODES_NEW
	movq	%rax, -88(%rbp)
	call	AI_RUN_MOCK
	movq	%rax, -80(%rbp)
	movq	-80(%rbp), %rax
	leaq	.LC15(%rip), %rdx
	movq	%rax, %rsi
	movq	%rdx, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-80(%rbp), %rdx
	movq	-88(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	ADD_DATA_FROM_RESPONSE
	movq	-88(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	NODES_READ
	movq	%rax, -72(%rbp)
	movq	-72(%rbp), %rax
	leaq	8(%rax), %rdx
	leaq	.LC16(%rip), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-88(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	NODES_READ
	movq	%rax, -72(%rbp)
	movq	-72(%rbp), %rax
	leaq	8(%rax), %rdx
	leaq	.LC16(%rip), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-88(%rbp), %rax
	movl	$2, %esi
	movq	%rax, %rdi
	call	NODES_READ
	movq	%rax, -72(%rbp)
	movq	-72(%rbp), %rax
	leaq	8(%rax), %rdx
	leaq	.LC16(%rip), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-88(%rbp), %rax
	movq	%rax, %rdi
	call	NODES_FREE
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L44
	call	__stack_chk_fail@PLT
.L44:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	main, .-main
	.ident	"GCC: (GNU) 15.2.1 20260209"
	.section	.note.GNU-stack,"",@progbits
