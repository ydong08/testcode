
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_DIGITS 1024


void            gradeSchool(int *a, int *b, int *ret, int d);
void            doCarry(int *a, int d);
void            getNum(int *a, int *d_a);
void            printNum(int *a, int d);

int main() {
    int             a[MAX_DIGITS]; // first multiplicand
    int             b[MAX_DIGITS]; // second multiplicand
    int             r[6 * MAX_DIGITS]; // result goes here
    int             d_a; // length of a
    int             d_b; // length of b
    int             d; // maximum length
    int             i; // counter
   
    getNum(a, &d_a);
    getNum(b, &d_b);

    if(d_a < 0 || d_b < 0) {
        printf("0\n");
        exit(0);
        return 0;
    }

    // let d be the smallest power of 2 greater than d_a and d_b,
    // and zero out the rest of a and b.
    i = (d_a > d_b) ? d_a : d_b;
    for(d = 1; d < i; d *= 2);
    for(i = d_a; i < d; i++) a[i] = 0;
    for(i = d_b; i < d; i++) b[i] = 0;

    gradeSchool(a, b, r, d); // compute product in old way
	doCarry(r, 2 * d); // now do any carrying
    printNum(r, 2 * d);
}

void gradeSchool(int *a, int *b, int *ret, int d) {
    int             i, j;

    for(i = 0; i < 2 * d; i++) ret[i] = 0;
    for(i = 0; i < d; i++) {
        for(j = 0; j < d; j++) ret[i + j] += a[i] * b[j];
    }
}

void doCarry(int *a, int d) {
    int             c;
    int             i;

    c = 0;
    for(i = 0; i < d; i++) {
        a[i] += c;
        if(a[i] < 0) {
            c = -(-(a[i] + 1) / 10 + 1);
        } else {
            c = a[i] / 10;
        }
        a[i] -= c * 10;
    }
 //   if(c != 0) fprintf(stderr, "Overflow %d\n", c);
}

void getNum(int *a, int *d_a) {
    int             c;
    int             i;
    *d_a = 0;
    while(true) {
        c = getchar();
        if(c == '\n' || c == EOF) break;
        if(*d_a >= MAX_DIGITS) {
            fprintf(stderr, "using only first %d digits\n", MAX_DIGITS);
            while(c != '\n' && c != EOF) c = getchar();
        }
        a[*d_a] = c - '0';
        ++(*d_a);
    }
    // reverse the number so that the 1's digit is first
    for(i = 0; i * 2 < *d_a - 1; i++) {
        c = a[i], a[i] = a[*d_a - i - 1], a[*d_a - i - 1] = c;
    }
}

void printNum(int *a, int d) {
    int i;
    for(i = d - 1; i > 0; i--) if(a[i] != 0) break;
    for(; i >= 0; i--) printf("%d", a[i]);
    printf("\n"); 
}