
/*���е�����ʵ��*/

#include <iostream>
using namespace std;
class Queue {
public:
	// ���캯���з����ڴ�ռ�
	Queue (size_t size) :
		m_data (new int[size]), m_size (size),
		m_rear (0), m_front (0), m_count (0) {}
	// �����������ͷ��ڴ�ռ�
	~Queue (void) {
		if (m_data) {
			delete[] m_data;
			m_data = NULL;
		}
	}
	// ѹ��
	void push (int data) {
	if (full ())
			throw OverFlow ();
		if (m_rear >= m_size)//���е�һλ�ÿ�
			m_rear = 0;
		m_count++;
		m_data[m_rear++] = data;
	}
	// ����
	int pop (void) {
		if (empty ())
			/*UnderFlow();���ù���,��ʱ����
			  catch()ץȡ*/
			throw UnderFlow ();
		if (m_front >= m_size)//
			m_front = 0;
		m_count--;
		return m_data[m_front++];
	}
	// �п�
	bool empty (void) {
		return ! m_count;
	}
	// ����
	bool full (void) {
		return m_count >= m_size;
	}
private:
	// �����쳣
	class OverFlow : public exception {
	public:
		/*what()�麯��,��д*/
		const char* what (void) const throw () {
			return "�������磡";
		}
	};
	// �����쳣
	class UnderFlow : public exception {
	public:
		const char* what (void) const throw () {
			return "�������磡";
		}
	};
	int*   m_data;  // ����
	size_t m_size;  // ����
	size_t m_rear;  // ���
	size_t m_front; // ǰ��
	size_t m_count; // ����,�����е��ִ���ЧԪ�ظ���
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
	/*���������������,�����û������ͽ���*/
	catch (exception& ex) {
		cout << ex.what () << endl;
		return -1;
	}
	return 0;
}
