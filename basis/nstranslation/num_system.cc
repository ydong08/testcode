
/*不同进制数的求法*/
void printb (unsigned int numb, int base) {
     Stack stack (100);
    do {
        stack.push (numb % base);
   }  while (numb /= base);
    while (! stack.empty ()) {
       int digit = stack.pop ();
        if (digit < 10)
           cout << digit;
        else
           cout << char ('A' + (digit - 10));
    }
     cout << endl;
  }

