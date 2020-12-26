gfn:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	addl	$3, %eax
	popl	%ebp
	ret
ffn:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	8(%ebp)
	call	gfn
	addl	$4, %esp
	leave
	ret
main:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$8
	call	ffn
	addl	$4, %esp
	addl	$1, %eax
	leave
	ret
