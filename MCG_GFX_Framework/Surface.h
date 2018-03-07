#pragma once

#include "Buffer.h"

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

	inline float getDepthAt(int _x, int _y)
	{
		return m_depthBuffer->Get(_x, _y);
	}

	inline void setDepthAt(int _x, int _y, float _depth)
	{
		m_depthBuffer->Set(_x, _y, _depth);
	}

	inline void setColourAt(int _x, int _y, glm::vec4 _colour)
	{
		m_frameBuffer->Set(_x, _y, _colour);
		m_frameFlagBuffer->Set(_x, _y, true);
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
};

