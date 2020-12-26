ffn:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	addl	%edx, %eax
	addl	$6, %eax
	popl	%ebp
	ret
main:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$3
	pushl	$2
	call	ffn
	addl	$8, %esp
	addl	$1, %eax
	leave
	ret
