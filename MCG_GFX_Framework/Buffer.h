#pragma once

#include <GLM\glm.hpp>

template<typename T>
class Buffer
{
public:
	unsigned int width;
	unsigned int height;
	T* data;

	Buffer<T>()
	{
		width = 0;
		height = 0;
		data = NULL;
	}

	Buffer<T>(unsigned int _width, unsigned int _height) : width(_width), height(_height)
	{
		int size = width * height;
		data = new T[size];
		//clear((T)int[sizeof(T)]{0});
		return;
	};

	/*~Buffer<T>()
	{
	delete[] data;
	}*/

	void clear(T value)
	{
		for (size_t i = 0; i < width * height; i++)
		{
			data[i] = value;
		}
	}

	T Get(int x, int y);
	void Set(int x, int y, T value);

};

template <typename T>
T Buffer<T>::Get(int x, int y)
{
	int index = (y * width) + x;
	return data[index];
}

template <typename T>
void Buffer<T>::Set(int x, int y, T t)
{
	this->data[(y * width) + x] = t;
}

typedef Buffer<unsigned int> Image;
typedef Buffer<float> DepthBuffer;
typedef Buffer<glm::vec4> FrameBuffer;
typedef Buffer<bool> FlagBuffer;