
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<vector>
#include<stdexcept>
#include<string>
using namespace std;


#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#define LOWWORD(l)          ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIGHWORD(l)         ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOWBYTE(w)          ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIGHBYTE(w)         ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))
#define MAKEWORD(low,high) (((unsigned short)((unsigned char)low))|(((unsigned short)((unsigned char)high))<<8))

int pi[10] = {0, 3, 1, 5, 2, 9, 8, 4, 6, 7};

typedef struct tagnode{
	int data;
	struct tagnode* next;
} Node;

Node* create(void)
{
	Node* head, *p;
	int i  = 0;
	p = (Node*)malloc(sizeof(Node));
	p->data = pi[i++];
	head = p;
	while(p){
		p->next = (Node*)malloc(sizeof(Node));
		p = p->next;
		p->data = pi[i++] ;
		if(i ==  9){
			p->next = NULL;
			p = NULL;
		}
	}
	return head;
}

Node* reverse(Node* p){
	Node* ph, *pn, *pt;
	ph = p;
	pn = NULL;
	while(ph){
		pt = ph->next;//get next node
		ph->next = pn;//
		pn = ph;
		ph = pt;
	}
	return pn;
}

//
Node* sortlist(Node* head, bool asce){

	Node* first;     //排列后有序链的表头指针  
	Node* tail;      //排列后有序链的表尾指针  
	Node* p_min;     //保留键值更小的节点的前驱节点的指针  
	Node* min;       //存储最小节点  
	Node* p;         //当前比较的节点 

	while(head != NULL)       
	{  
		for (p = head, min = head; p->next != NULL; p = p->next) 
		{  
			if (p->next->data < min->data)      
			{  
				//保存找到节点的前驱节点
				p_min = p;        
				min = p->next;     
			}  
		}  

		if (! first)
		{  
			first = min;     
			tail = min;        
		}  
		else 
		{  
			tail->next = min;   
			tail = min;           //???
		}  
 
		if (min == head)           
		{  
			head = head->next;      
		}  
		else 
		{  
			//将最小节点删除
			p_min->next = min->next;  
		}  
	}  

	if (first != NULL)      
	{   
		tail->next = NULL;   
	}  
	head = first;  
	return head;  
}

void freelist(Node* p){
	Node* pt, *pn = p;
	while(pt){
		pt = pn->next;
		free(pn);
		pn=pn->next;
	}
}

Node* sort_list(Node* p, bool asce){
	Node* pt;
	Node *ph=p;
	int loop = 0;
	Node* pmin;
	Node* pb;
	Node* pn; 
	Node* head=NULL;

	while(ph){
		loop++;
		for(pt = ph, pmin = ph; pt->next != NULL; pt=pt->next){
			if(pt->next->data < pmin->data){
				pb = pt;
				pmin = pt->next;
			}	
		}
		if(head){
			pn->next = pmin;
			pn=pmin;//pn的next指针实际上就是pmin的next
		}else{
			pn = pmin;
			head = pmin;
		}
		//重点
		if(ph == pmin)
			ph=ph->next;
		else{
			//free(pb->next);
			pb->next = pmin->next;
		}
	}
	if(head)
		pn->next = NULL;
	return head;
}

void SelectionSort(int *num,int n)  
{  
	int i = 0;  
	int min = 0;  
	int j = 0;  
	int tmp = 0;  
	for(i = 0;i < n-1;i++)  
	{  
		min = i;//每次将min置成无序组起始位置元素下标   
		for(j = i;j < n;j++)//遍历无序组，找到最小元素。   
		{  
			if(num[min]>num[j])  
			{  
				min = j;  
			}  
		}  
		if(min != i)//如果最小元素不是无序组起始位置元素，则与起始元素交换位置 
		{  
			tmp = num[min];  
			num[min] = num[i];  
			num[i] = tmp;  
		}  
	}  
}  

void select_sort(int*  p, int n)
{
	int* pt = p;
	int min, tmp;
	for(int i = 0; i<n -1; i++){
		min = i;
		for(int j = i; j < n; j++){
			if(pt[min] > pt[j])
				min = j;
		}

		if(min != i){
			tmp = pt[min];
			pt[min] = pt[i];
			pt[i] = tmp;
		}
	}
}

string mergestr(string& src, const string& sep)
{
	string nstr,tmp;
	size_t tpos=0,npos=0;
	try//stdexcept
	{
		while(true){
			npos =src.find_first_of(sep, tpos);
			if(string::npos == npos){
				tmp = src.substr(tpos);
				nstr += tmp;
				break;
			}
			if(npos == tpos){
				tpos++;
				continue;
			}
			tmp = src.substr(tpos, npos);
			nstr += tmp;
			tpos = npos+1;
		}
	}
	catch (std::out_of_range& e)
	{
		cout<<e.what()<<", Line: " << __LINE__ << ", file: " << __FILE__ << endl;
	}
	return nstr;

}

char* cmergestr(const char* src, int srclen, const char* sep, int seplen){
	char* srcbuf = (char*)malloc(sizeof(char)*srclen);
	char* sepbuf = (char*)malloc(sizeof(char)*seplen);
	char* selbuf = (char*)malloc(sizeof(char)*srclen);
	for(int i = 0; i<srclen;++i){
		for(int j = 0; j < seplen; ++j){
			if(srcbuf[i] == sepbuf[j]){


			}

		}

	}



	free(srcbuf);
	free(sepbuf);
	return selbuf;
}

void modifystr(char* p){

	p[3] = 'K';
	return;
}

class CBase
{
public:
	CBase() {};
	virtual ~CBase() {cout << "destructor ClxBase!" << endl; };
	virtual void DoSomething() { cout << "Do something in class ClxBase!" << endl; };
};

class CDerived : public CBase
{
public:
	CDerived() {};
	~CDerived() { cout << "destructor ClxDerived!" << endl; }; 
	void DoSomething() { cout << "Do something in class ClxDerived!" << endl; };
};

class B1
{
public:
	B1(){ cout<<"B1"<<endl;}
};
class B2
{
public:
	B2(){cout<<"B2"<<endl;}
};
class C:public B1,public B2
{
public:
	C(){cout<<"C"<<endl;}
};

int main(int argc,  char* argv[])
{
	{
		Node* pm = create();
		Node* pt = pm;
		while(pm)
			cout<<"data: " << pm->data <<endl, pm=pm->next;
		cout<<endl;

		pm = reverse(pt);
		pt = pm;
		while(pm)
			cout<<"data: " << pm->data <<endl, pm=pm->next;
		cout<<endl;

		pm=sort_list(pt, false);
		pt = pm;
		while(pm)
			cout<<"data: " << pm->data <<endl, pm=pm->next;
		cout<<endl;
		freelist(pt);

		int n = 10;
		select_sort(pi, n);
		while(n>0){
			cout<<pi[10-n]<<", ";
			n--;
		}
		cout<<endl;
	}

	string str = "张三|3456123, 湖南";
	string sep="|, ";
	cout<< str.c_str() <<" | " <<  str.length() << endl;
	cout<<"Str: " << mergestr(str, sep).c_str()<<endl;

	vector<int> vit;
	vit.reserve(100);
	vit.push_back(10);
	cout<<"MAX_SIZE: " << vit.capacity() << endl;


	vector<int> iVec;
	for(int i = 0; i < 5; i ++)
	{
	iVec.push_back(i);
	cout << "容器 大小为: " << iVec.size() << endl;
	cout << "容器 容量为: " << iVec.capacity() << endl; //0个元素， 容器容量为0
	cout<<endl;
	}
	//iVec.resize(500);
	iVec.reserve(10);
	cout << "容器 大小为: " << iVec.size() << endl;
	cout << "容器 容量为: " << iVec.capacity() << endl; //0个元素， 容器容量为0
	cout<<endl;
	for(int i = 0; i < 10; ++i)
		cout<<iVec[i]<<", ";
	cout<<endl;
	/* vs2005/8 容量增长不是翻倍的，如 
		9个元素   容量9 
		10个元素 容量13 */
	/* 测试effective stl中的特殊的交换 swap() */
	cout << "当前vector 的大小为: " << iVec.size() << endl;
	cout << "当前vector 的容量为: " << iVec.capacity() << endl;
	cout<<endl;
	vector<int>(iVec).swap(iVec);
	cout << "临时的vector<int>对象 的大小为: " << (vector<int>(iVec)).size() << endl;
	cout << "临时的vector<int>对象 的容量为: " << (vector<int>(iVec)).capacity() << endl;
	cout << "交换后，当前vector 的大小为: " << iVec.size() << endl;
	cout << "交换后，当前vector 的容量为: " << iVec.capacity() << endl;
	/*
	cout<<iVec.front()<<endl;
	iVec.pop_back();
	iVec.push_back(99);
	cout<<iVec.back()<<endl;
	cout<< iVec.at(0)<<endl;
	iVec.assign(4, 100);
	cout<<iVec.back()<<endl;
	
	vector<int> mp;
	for(int i = 103; i < 110; i ++)
	{
		mp.push_back(i);
	}
	iVec.assign(mp.begin(), mp.end());
	iVec.assign(mp.rbegin(), mp.rend());
	iVec.insert(mp.begin()+10, 100);

	//iVec.clear();
	//vector<int>().swap(iVec);

	//iVec.~vector<int>();
		*/

	string pstr("abcdef");
	modifystr((char*)pstr.c_str());
	const char* pp = pstr.c_str();
	cout<<pp<<endl;
	string ppt(pp);
	cout<<ppt<<endl;
	string ppt2(pp);
	cout<<ppt2<<endl;

	const string wstr("qwert");
	cout<<wstr[wstr.length()+10] <<endl;

	string strinfo("this is Winter"); 
	//最好的方式是:  
	string foo(strinfo.c_str());  
	cout<<foo<<endl;
	//也可以这么用:  
	const char* tpstr=strinfo.c_str();  
	string tfoo(tpstr);  
	cout<<tfoo<<endl;
	//不要再使用了pstr了, 下面的操作已经使pstr无效了。  
	strinfo += " Hello!";  
	string pfoo(tpstr);//tpstr指向的数据还是未增加"hello"的strinfo
	cout<<pfoo<<endl;

	CBase *pTest = new CDerived;
	pTest->DoSomething();
	delete pTest;
	/*
	CDerived *pTest = new CDerived;
	pTest->DoSomething();
	delete pTest;
	*/

	C obj;

	return 0;
}
