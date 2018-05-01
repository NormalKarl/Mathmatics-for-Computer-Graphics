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
		clear((T)0);
		return;
	};

	~Buffer<T>()
	{
		delete[] data;
	}

	void clear(T value)
	{
		for (size_t i = 0; i < width * height; i++)
		{
			data[i] = value;
		}
	}

	T get(int x, int y);
	void set(int x, int y, T value);

};

template <typename T>
T Buffer<T>::get(int x, int y)
{
	int index = (y * width) + x;
	return data[index];
}

template <typename T>
void Buffer<T>::set(int x, int y, T t)
{
	this->data[(y * width) + x] = t;
}

typedef Buffer<unsigned int> Image;
typedef Buffer<float> DepthBuffer;
typedef Buffer<glm::vec4> FrameBuffer;
typedef Buffer<bool> FlagBuffer;

class Viewport
{
public:
	int x;
	int y;
	int width;
	int height;

	inline Viewport()
	{
		x = y = width = height = 0;
	}

	inline Viewport(int x, int y, int width, int height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
};

/*
Surface is a target for the Raytracer, Rasterizer aswell as the UI components and anything else. 
Surfaces are uncompressed image data with each pixel containing 16 bytes per pixel (Component Count * Float Size).
*/
class Surface
{
private:
	Viewport m_viewport;
	FrameBuffer* m_frameBuffer;
	//This is used to indicated which been changed.
	//This saves time when iterating over all pixels at the end of the frame.
	FlagBuffer* m_frameFlagBuffer;
	DepthBuffer* m_depthBuffer;
	glm::vec3 m_clearColour;
public:
	Surface(const Viewport& _viewport);
	//Surface::Surface(int _width, int _height) : Surface({ 0, 0, _width, _height }) {}
	//Surface::Surface(int _x, int _y, int _width, int _height) : Surface(Viewport(_x, _y, _width, _height)) {}

	virtual ~Surface();
	void clear();
	void draw();
	glm::vec3 getFlattenedPixel(int x, int y);
	glm::vec3 performFXAA(int x, int y);
	float luma(int x, int y);

	inline int getWidth()
	{
		return m_viewport.width;
	}

	inline int getHeight()
	{
		return m_viewport.height;
	}

	inline float getDepthAt(int _x, int _y)
	{
		return m_depthBuffer->get(_x, _y);
	}

	inline void setDepthAt(int _x, int _y, float _depth)
	{
		m_depthBuffer->set(_x, _y, _depth);
	}

	/*inline void setColourAt(int _x, int _y, glm::vec4 _colour)
	{
		//m_frameBuffer->Set(_x, _y, _colour);
		addColourAt(_x, _y, _colour);
		m_frameFlagBuffer->Set(_x, _y, true);
	}*/

	void setColourAt(int _x, int _y, glm::vec4 _colour);

	inline glm::vec4 getColourAt(int _x, int _y) {
		return m_frameBuffer->get(_x, _y);
	}

	inline glm::vec3 getClearColour()
	{
		return m_clearColour;
	}

	void setClearColour(unsigned char r, unsigned char g, unsigned char b);

	inline void setClearColour(float r, float g, float b)
	{
		m_clearColour = { r, g, b };
	}

	inline Viewport getViewport()
	{
		return m_viewport;
	}

	inline void clearFlagBuffer()
	{
		m_frameFlagBuffer->clear(false);
	}

	inline void clearFrameBuffer()
	{
		m_frameBuffer->clear(glm::vec4(m_clearColour, 1.0f));
		m_frameFlagBuffer->clear(false);
	}

	inline void clearDepthBuffer()
	{
		m_frameFlagBuffer->clear(false);
	}
};

