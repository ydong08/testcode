

	
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