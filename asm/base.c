
/* 
 *                寻址
 * ********************************************
 * 1. movl %eax, %edx    ==> edx = eax                register mode,寄存器寻址模式
 * 2. movl $0x123, %edx  ==> edx = 0x123              immediate,直接寻址
 * 3. movl 0x123, %edx   ==> edx = *(int32_t*)0x123   direct,直接寻址
 * 4. movl (%ebx), %edx  ==> edx = *(int32_t*)ebx     indirect,间接寻址
 * 5. movl 4(%ebx), %edx ==> edx = *(int32_t*)(ebx+4) displaced,变址寻址
 *   Note: b,w,l,q 分别代表8位，16位，32位，64位
 * ********************************************
 *
 *               基础指令
 * ********************************************
 * 1. pushl %eax
 * 		subl $4, %esp
 * 		movl %eax, (%esp
 * 2. popl %eax
 * 		movl (%esp), %eax
 * 		addl $4, %esp
 * 3. call 0x12345
 * 		pushl %eip  *
 * 		movl 0x12345, %eip *
 * 4. ret
 * 		popl %eip  *)
 * 5. enter
 * 		pushl %ebp
 * 		movl %esp %ebp
 * 6. leave
 * 		movl %ebp %esp
 * 		pop %ebp
 *   Note: %eip 寄存器不可使用指令改变，由CPU自动计算赋值
 * ********************************************
 *
 *               base.s 文件说明
 * ********************************************
 * 生成的汇编代码中所有以"."开始的语句都是链接辅助语句
 * 非链接使用时可以全部删除
 * ********************************************
 *
 *               其他              
 * ********************************************
 * 1. 函数调用参数压栈顺序是从右向左
 * ********************************************
 *
 **/

int ffn(int x, int y)
{
	return x + y + 6;
}

int main()
{
	return ffn(2,3) + 1;
}
