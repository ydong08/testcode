tfn:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	8(%ebp), %eax
	movb	%al, -12(%ebp)
	movsbl	-12(%ebp), %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	nop
	leave
	ret
pfn:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	addl	%edx, %eax
	popl	%ebp
	ret
main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ecx
	subl	$20, %esp
	movb	$97, -9(%ebp)
	movl	$1, -16(%ebp)
	movl	$2, -20(%ebp)
	movsbl	-9(%ebp), %eax
	subl	$12, %esp
	pushl	%eax
	call	tfn
	addl	$16, %esp
	subl	$8, %esp
	pushl	-20(%ebp)
	pushl	-16(%ebp)
	call	pfn
	addl	$16, %esp
	movl	%eax, -24(%ebp)
	pushl	-20(%ebp)
	pushl	-16(%ebp)
	pushl	-24(%ebp)
	pushl	$.LC1
	call	printf
	addl	$16, %esp
	movl	$0, %eax
	movl	-4(%ebp), %ecx
	leave
	leal	-4(%ecx), %esp
	ret
