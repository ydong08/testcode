
#include <stdio.h>
#include <string.h>

#if 0
	
int  ChildString(char*p) {   
 	char *q=p;
	int  stringlen=0, i=0,j=1,len=0,maxlen=1;   
	while(*q!='\0') {
 		 Stringlen++;
		q++;
	}
  	while( i< Stringlen )   {   
	       if(*(p+i)==*(p+j) && j< Stringlen)   {   
		  len++;          
		  i++;
		  j++;   
	        }   
	       else    {   
		 if(len>maxlen) {   
		      maxlen=len+1;   
		      len=0;
		   }  	
		   else
		       len=0;
	       	 i++;
	       	 j++;
                   
	         }   
 	 }  
	return   maxlen;   
}
#endif
int main()
{
	double d = 3.141592627;
	double f = 3141592627.01;
	const char *p = "abc,def,hijk";
	const char *s = "/";
	char *t = NULL;
	printf("p:%s\n", strrchr(p, ','));
	printf("s:%s\n", strtok_r((char*)s, "/", &t));

	printf("[%%*s]:%*s\n", 6, p);
	printf("[%%.0s]:%.0s\n", p);
	printf("[%%.*s]:%.*s\n", 6, p);
	printf("[%%*.*s]:%*.*s\n", 6, 10, p);

	printf("[%%.0lf]:%.0lf\n", d);
	printf("[%%.*lf]:%.*lf\n", 6, d);
	printf("[%%.0lf]:%.0lf\n", f);
	printf("[%%.*lf]:%.*lf\n", 6, f);
	return 0;
}
