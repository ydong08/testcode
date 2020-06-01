#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>

using namespace std;

void print(const char* pstr) {
    printf("str: %s, len: %d\n", pstr, strlen(pstr));
}
int main() {
    string str = string("0987654321qwertyyuiioopplkjhgfdsazxcvbnm");
    string str1 = string("# 10:05:27 0x42E69490 CommTcp.cpp-00920:getsockopt nRet:0, status:7, info:TCP_CLOSE Host:[192.168.31.124:5020]");

    const char* p = str.c_str();
    const char* p1 = str1.c_str();

    print(p);
    print(p1);

	const char* ptr = "network\r\n\r\n";
	printf("prt: %s", ptr);
	printf("----\n");
	const char* ptr1 = "k\r\n";
	printf("prt1: %s", ptr1);
	printf("----\n");

    return 0;

}
