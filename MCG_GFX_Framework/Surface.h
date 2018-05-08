#ifndef _SURFACE_H_
#define _SURFACE_H_

#include <GLM\glm.hpp>
#include "Texture.h"

/*
Buffer is a template class that is mostly used to represent the pixels on screen
in different, for example a frame buffer, depth buffer or texture.
*/
template<typename T>
class Buffer
{
public:
	unsigned int m_width;
	unsigned int m_height;
	T* m_data;

	Buffer<T>()
	{
		m_width = 0;
		m_height = 0;
		m_data = NULL;
	}

	Buffer<T>(unsigned int _width, unsigned int _height) : m_width(_width), m_height(_height)
	{
		int size = m_width * m_height;
		m_data = new T[size];
		clear((T)0);
		return;
	};

	~Buffer<T>() { delete[] m_data; }

	void clear(T _value)
	{
		for (size_t i = 0; i < m_width * m_height; i++)
		{
			m_data[i] = _value;
		}
	}

	T get(int _x, int _y);
	void set(int _x, int _y, T _value);

};

template <typename T>
T Buffer<T>::get(int _x, int _y)
{
	return m_data[(_y * m_width) + _x];
}

template <typename T>
void Buffer<T>::set(int _x, int _y, T _value)
{
	int index = (_y * m_width) + _x;
	this->m_data[index] = _value;
}

//Predfine different types of the buffer.
typedef Buffer<unsigned int> Image;
typedef Buffer<float> DepthBuffer;
typedef Buffer<glm::vec4> FrameBuffer;
typedef Buffer<bool> FlagBuffer;

/*
Viewport is used to define the width and height of the view aswell
as its position.
*/
struct Viewport
{
	int m_x;
	int m_y;
	int m_width;
	int m_height;

	inline Viewport() { m_x = m_y = m_width = m_height = 0; }
	inline Viewport(int _x, int _y, int _width, int _height) : m_x(_x), m_y(_y), m_width(_width), m_height(_height) { }
};

/*
Surface is a target for the Raytracer, Rasterizer aswell as the UI components and almost everything else.
Surfaces are uncompressed image data with each pixel containing 16 bytes per pixel (Component Count * Float Size).
*/
class Surface
{
private:
	Viewport m_viewport;
	FrameBuffer* m_frameBuffer;
	//This is used to indicated what has been changed.
	//This saves time when iterating over all pixels at the end of the frame.
	FlagBuffer* m_frameFlagBuffer;
	DepthBuffer* m_depthBuffer;
	glm::vec3 m_clearColour;
	bool m_blur;
public:
	Surface(const Viewport& _viewport);
	virtual ~Surface();
	void clear();
	void draw();
	Texture* getAsTexture();
	void writeToTexture(Texture* texture);
	inline int getWidth() { return m_viewport.m_width; }
	inline int getHeight() { return m_viewport.m_height; }
	inline float getDepthAt(int _x, int _y) { return m_depthBuffer->get(_x, _y); }
	inline void setDepthAt(int _x, int _y, float _depth) { m_depthBuffer->set(_x, _y, _depth); }
	void setColourAt(int _x, int _y, glm::vec4 _colour);
	inline glm::vec4 getColourAt(int _x, int _y) { return m_frameBuffer->get(_x, _y); }
	inline glm::vec3 getClearColour() { return m_clearColour; }
	void setClearColour(unsigned char r, unsigned char g, unsigned char b);
	inline void setClearColour(float r, float g, float b) { m_clearColour = { r, g, b }; }

	inline Viewport getViewport() { return m_viewport; }
	inline void clearFlagBuffer() { m_frameFlagBuffer->clear(false); }

	inline void clearFrameBuffer()
	{
		m_frameBuffer->clear(glm::vec4(m_clearColour, 1.0f));
		m_frameFlagBuffer->clear(false);
	}

	inline void clearDepthBuffer() { m_frameFlagBuffer->clear(false); }
	inline bool isBlurEnabled() { return m_blur; }
	inline void setBlurEnabled(bool _enabled) { m_blur = _enabled; }
};

#endif