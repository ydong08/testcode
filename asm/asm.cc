assume cs:code,ds:data

data segment
	db "hello world!"
data ends

code segment
	start:
	mov ax,data
	mov ds,ax
	
	mov bx,1400ffh
	mov es,bx
	mov cx,11
	mov si,0
	mov bx,0
	mov ah,01000111B
	
  lp:mov al,ds:[si]
  	 mov es:[bx],al
  	 mov es:[bx+1],ah 
	 inc si
	 add bx,2
	 loop lp
	 
	 mov ax,4c00h
	 int 21h
code ends
	end start

