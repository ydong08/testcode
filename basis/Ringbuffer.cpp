
#include <cstdint>
#include <iostream>
#include <cstring>
#include <stdio.h>
/*
	模仿Linux内核kfifo实现的循环缓存
*/

using namespace std;

/*
	判断n是否是2的幂
	若n为2的次幂,   则 n & (n-1) == 0，也就是n和n-1的各个位都不相同。例如 8(1000)和7(0111）
	若n不是2的次幂, 则 n & (n-1) != 0,也就是n和n-1的各个位肯定有相同的，例如7(0111)和6(0110)
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
	linux kfifo的优点：
	1. 使用内存屏障，可以实现单消费者和单生产者的无锁访问。多个线程并发的话，还是需要加锁的。
	2. 数据处理很巧妙，使用无符号整数保存in和out的值，这样在更新in和out值时不进行模运算
		(a). 一直加下去，知道in和out的值溢出，从头开始
		(b). 即使in和out溢出了，仍然能保证in - out为缓冲区已存的数据；size - (in - out)为缓冲区的空闲长度
		(c). 充分利用空间，可以方便的判断缓冲区满状态和空状态
		(d). 代码很简洁
	3. 初始化时向上去取整，保证size是2的次幂，这样可以使用 in & (size - 1)来代替 in %(size -1)的模运算
*/

struct kfifo{
	uint8_t *buffer;
	//uint32_t in; // 输入指针
	//uint32_t out; // 输出指针
	//uint32_t size; // 缓冲区大小，必须为2的次幂
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

	// 返回实际写入缓冲区中的数据
	uint32_t put(const uint8_t *data, uint32_t len)
	{
		// 当前缓冲区空闲空间
		len = min(len,size - in + out);

		// 当前in位置到buffer末尾的长度
		auto l = min(len, size - (in  & (size - 1)));

		// 首先复制数据到[in，buffer的末尾]
		memcpy(buffer + (in & (size - 1)), data, l);

		// 复制剩余的数据（如果有）到[buffer的起始位置,...]
		memcpy(buffer, data + l, len - l);

		in += len; // 直接加，不作模运算。当溢出时，从buffer的开始位置重新开始

		return len;
	}

	// 返回实际读取的数据长度
	uint32_t get(uint8_t *data, uint32_t len)
	{
		// 缓冲区中的数据长度
		len = min(len, in - out);
		
		// 首先从[out,buffer end]读取数据
		auto l = min(len, size - (out & (size - 1)));
		memcpy(data, buffer + (out & (size - 1)), l);

		// 从[buffer start,...]读取数据
		memcpy(data + l, buffer, len - l);

		out += len; // 直接加，不错模运算。溢出后，从buffer的起始位置重新开始

		return len;
	}

	// 缓冲区中数据的长度
	uint32_t length()
	{
		return in - out;
	}

	// 缓冲区中的剩余空间
	uint32_t left()
	{
		return size - (in - out);
	}

	// 是否为空
	uint32_t isEmpty()
	{
		return in == out;
	}

	// 是否满了
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