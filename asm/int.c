/* 生成的汇编代码中所有以"."开始的语句都是链接辅助语句
 * 非链接使用时可以全部删除
 * enter:
 * 		pushl %ebp
 * 		movl %esp %ebp
 *
 * leave:
 * 		movl %ebp %esp
 * 		pop %ebp
 *
 * 函数调用参数压栈顺序是从右向左
 **/

int ffn(int x, int y)
{
	return x + y + 6;
}

int main()
{
	return ffn(2,3) + 1;
}
