/*
 * 最长回文的算法
 *
 */
#include <stdio.h>
#include <stdlib.h>


/* O(n)解法 */
#define MIN(a,b) ((a) < (b) ? (a) : (b))

int maxid;        // 最长回文子串下标
int LPS_rb[100];  // i为中心的回文子串右边界下标right border
char str[100];    // 原字符串处理后的副本

void LPS_linear(char * X, int xlen)
{
  maxlen = maxid = 0;

  str[0] = '$';  // 将原串处理成所需的形式
  char *p = str;
  *(++p)++ = '#';
  while ((*p++ = *X++) != '\0') {
    *p++ = '#';
  }

  for (int i = 1; str[i]; ++i) {  // 计算LPS_rb的值 
    if (maxlen > i) {          // 初始化LPS[i]
      LPS_rb[i] = MIN(LPS_rb[2*maxid-i],(maxlen-i));
    } else {
      LPS_rb[i] = 1;
    }

    while (str[i-LPS_rb[i]] == str[i+LPS_rb[i]]) { // 扩展
      ++LPS_rb[i];
    }

    if (LPS_rb[i]-1 > maxlen) {
      maxlen = LPS_rb[i]-1;
      maxid = i;
    }
  }
}

//给出测试用例：
void main()
{
  char X[30];  // 设串不超过30
  /* test case
   * aaaa
   * abab
   * */
  while (cin.getline(X,30)) {
    /* 后缀数组方法 */
    LPS_suffix(X,strlen(X));
    printf("%d\n", maxlen);

    /* O(n)方法 */
    LPS_linear(X,strlen(X));
    printf("%d\n", maxlen);
  }
}






