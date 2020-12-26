/* 生成的汇编代码中所有以"."开始的语句都是链接辅助语句
 * 非链接使用时可以全部删除
 * enter:
 * 		pushl %ebp
 * 		movl %esp %ebp
 *
 * leave:
 * 		movl %ebp %esp
 * 		pop %ebp
 **/
int gfn(int x)
{
	return x + 3;
}

int ffn(int x)
{
	return gfn(x);
}

int main()
{
	return ffn(8) + 1;
}
