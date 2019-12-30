
/*队列的数组实现*/

#include <iostream>
using namespace std;
class Queue {
public:
	// 构造函数中分配内存空间
	Queue (size_t size) :
		m_data (new int[size]), m_size (size),
		m_rear (0), m_front (0), m_count (0) {}
	// 析构函数中释放内存空间
	~Queue (void) {
		if (m_data) {
			delete[] m_data;
			m_data = NULL;
		}
	}
	// 压入
	void push (int data) {
	if (full ())
			throw OverFlow ();
		if (m_rear >= m_size)//队列第一位置空
			m_rear = 0;
		m_count++;
		m_data[m_rear++] = data;
	}
	// 弹出
	int pop (void) {
		if (empty ())
			/*UnderFlow();调用构造,临时对象
			  catch()抓取*/
			throw UnderFlow ();
		if (m_front >= m_size)//
			m_front = 0;
		m_count--;
		return m_data[m_front++];
	}
	// 判空
	bool empty (void) {
		return ! m_count;
	}
	// 判满
	bool full (void) {
		return m_count >= m_size;
	}
private:
	// 上溢异常
	class OverFlow : public exception {
	public:
		/*what()虚函数,重写*/
		const char* what (void) const throw () {
			return "队列上溢！";
		}
	};
	// 下溢异常
	class UnderFlow : public exception {
	public:
		const char* what (void) const throw () {
			return "队列下溢！";
		}
	};
	int*   m_data;  // 数组
	size_t m_size;  // 容量
	size_t m_rear;  // 后端
	size_t m_front; // 前端
	size_t m_count; // 计数,队列中的现存有效元素个数
};
int main (void) {
	try {
		Queue queue (5);
		queue.push (10);
		queue.push (20);
		queue.push (30);
		queue.push (40);
		queue.push (50);
//		queue.push (60);
		cout << queue.pop () << endl; // 10
		queue.push (60);
		cout << queue.pop () << endl; // 20
		cout << queue.pop () << endl; // 30
		queue.push (70);
		queue.push (80);
		cout << queue.pop () << endl; // 40
		cout << queue.pop () << endl; // 50
		cout << queue.pop () << endl; // 60
		while (! queue.empty ())
			cout << queue.pop () << endl;
//		queue.pop ();
	}
	/*基类或基类的派生类,均可用基类类型接收*/
	catch (exception& ex) {
		cout << ex.what () << endl;
		return -1;
	}
	return 0;
}
