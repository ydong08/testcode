
#include <cstdint>
#include <iostream>
#include <cstring>
#include <stdio.h>
/*
	ģ��Linux�ں�kfifoʵ�ֵ�ѭ������
*/

using namespace std;

/*
	�ж�n�Ƿ���2����
	��nΪ2�Ĵ���,   �� n & (n-1) == 0��Ҳ����n��n-1�ĸ���λ������ͬ������ 8(1000)��7(0111��
	��n����2�Ĵ���, �� n & (n-1) != 0,Ҳ����n��n-1�ĸ���λ�϶�����ͬ�ģ�����7(0111)��6(0110)
*/
static inline bool is_power_of_2(uint32_t n)
{
	return (n != 0 && ((n & (n - 1)) == 0));
}


static inline uint32_t min(uint32_t a, uint32_t b)
{
	return a > b ? b : a;
}

static inline uint32_t max(uint32_t a, uint32_t b)
{
	return a > b ? a : b;
}

static inline uint32_t roundup_power_of_2(uint32_t a)
{
	if (a == 0)
		return 0;

	uint32_t position = 0;
	for (int i = a; i != 0; i >>= 1)
		position++;

	return static_cast<uint32_t>(1 << position);
}

/*
	linux kfifo���ŵ㣺
	1. ʹ���ڴ����ϣ�����ʵ�ֵ������ߺ͵������ߵ��������ʡ�����̲߳����Ļ���������Ҫ�����ġ�
	2. ���ݴ�������ʹ���޷�����������in��out��ֵ�������ڸ���in��outֵʱ������ģ����
		(a). һֱ����ȥ��֪��in��out��ֵ�������ͷ��ʼ
		(b). ��ʹin��out����ˣ���Ȼ�ܱ�֤in - outΪ�������Ѵ�����ݣ�size - (in - out)Ϊ�������Ŀ��г���
		(c). ������ÿռ䣬���Է�����жϻ�������״̬�Ϳ�״̬
		(d). ����ܼ��
	3. ��ʼ��ʱ����ȥȡ������֤size��2�Ĵ��ݣ���������ʹ�� in & (size - 1)������ in %(size -1)��ģ����
*/

struct kfifo{
	uint8_t *buffer;
	//uint32_t in; // ����ָ��
	//uint32_t out; // ���ָ��
	//uint32_t size; // ��������С������Ϊ2�Ĵ���
	uint8_t in;
	uint8_t out;
	uint8_t size;

	kfifo(uint32_t _size)
	{
		if (!is_power_of_2(_size))
			_size = roundup_power_of_2(_size);

		buffer = new uint8_t[_size];
		in = 0;
		out = 0;
		size = _size;
	}

	// ����ʵ��д�뻺�����е�����
	uint32_t put(const uint8_t *data, uint32_t len)
	{
		// ��ǰ���������пռ�
		len = min(len,size - in + out);

		// ��ǰinλ�õ�bufferĩβ�ĳ���
		auto l = min(len, size - (in  & (size - 1)));

		// ���ȸ������ݵ�[in��buffer��ĩβ]
		memcpy(buffer + (in & (size - 1)), data, l);

		// ����ʣ������ݣ�����У���[buffer����ʼλ��,...]
		memcpy(buffer, data + l, len - l);

		in += len; // ֱ�Ӽӣ�����ģ���㡣�����ʱ����buffer�Ŀ�ʼλ�����¿�ʼ

		return len;
	}

	// ����ʵ�ʶ�ȡ�����ݳ���
	uint32_t get(uint8_t *data, uint32_t len)
	{
		// �������е����ݳ���
		len = min(len, in - out);
		
		// ���ȴ�[out,buffer end]��ȡ����
		auto l = min(len, size - (out & (size - 1)));
		memcpy(data, buffer + (out & (size - 1)), l);

		// ��[buffer start,...]��ȡ����
		memcpy(data + l, buffer, len - l);

		out += len; // ֱ�Ӽӣ�����ģ���㡣����󣬴�buffer����ʼλ�����¿�ʼ

		return len;
	}

	// �����������ݵĳ���
	uint32_t length()
	{
		return in - out;
	}

	// �������е�ʣ��ռ�
	uint32_t left()
	{
		return size - (in - out);
	}

	// �Ƿ�Ϊ��
	uint32_t isEmpty()
	{
		return in == out;
	}

	// �Ƿ�����
	uint32_t isFull()
	{
		return size == (in - out);
	}

	~kfifo()
	{
		if (buffer)
			delete[] buffer;
		buffer = nullptr;
	}
};

int main()
{
	uint8_t output[512] = { 0 };
	uint8_t data[256] = { 0 };
	for (int i = 0; i < 256; i++)
		data[i] = i;

	kfifo fifo(128);
	fifo.put(data, 100);

	fifo.get(output, 50);

	fifo.put(data, 30);

	auto c = fifo.put(data + 10, 92);

	cout << "Empty:" << fifo.isEmpty() << endl;
	cout << "Left Space:" << fifo.left() << endl;
	cout << "Length:" << fifo.length() << endl;
	uint8_t a = fifo.size - fifo.in + fifo.out;
	uint8_t b = fifo.in - fifo.out;

	cout << "=======================================" << endl;
	fifo.get(output, 128);
	cout << "Empty:" << fifo.isEmpty() << endl;
	cout << "Left Space:" << fifo.left() << endl;
	cout << "Length:" << fifo.length() << endl;

	cout << "======================================" << endl;
	fifo.put(output, 100);
	cout << "Empty:" << fifo.isEmpty() << endl;
	auto d = static_cast<uint8_t>(fifo.left());
	auto e = static_cast<uint8_t>(fifo.length());
	printf("Left Space:%d\n", d); 
	printf("Length:%d\n", e);

	getchar();
	return 0;
}