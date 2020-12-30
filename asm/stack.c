
/*
 *         堆栈框架创建(enter 指令)
 * *************************************
 * 	pushl %ebp
 * 	movl %esp, %ebp
 *
 *        堆栈框架拆除(leave 指令)
 * *************************************
 *  movl %ebp, %esp
 *  popl %ebp
 *
 *        参数传递
 * *************************************
 *
 *
 * *************************************
 *        反汇编方法
 * *************************************
 * 1. objdump
 * 	gcc -m32 -g -o stack stack.c
 * 	objdump -S stack > stack.s
 *
 * 2. gcc
 * 	gcc -m32 -S stack.s stack.c
 * */

#include <stdio.h>

int tfn(char c)
{
	printf("%c\n", c);
}

int pfn(int x, int y)
{
	return x + y;
}

int main()
{
	char c='a';
	int x,y,z;

	x = 1;
	y = 2;

	tfn(c);
	z = pfn(x, y);
	printf("%d=%d+%d\n", z, x, y);
	
	return  0;
}

