#pragma once

#include <GLM/glm.hpp>
#include <vector>

#include "buffer.h"
#include "Geometry.h"
#include "Surface.h"

enum class Filter
{
	Point,
	Bilinear
};

class Texture
{
public:
	int width;
	int height;
	int bytesPerPixel;
	unsigned char* data;
	Filter filter;

	inline Texture()
	{
		filter = Filter::Bilinear;
	}

	Texture(const char* filename);

	~Texture() {
		delete[] data;
	};

	glm::vec4 getPixelApprox(glm::vec2 uv);
	glm::vec4 getPixelAt(int x, int y);
};

/*class IShader
{
private:
	friend class Rasterizer;
	Rasterizer* m_context;
	glm::vec4 m_vertex;
	glm::vec4 m_frag;
	glm::vec3 m_weights;
public:
	glm::vec4 sample() const;

	virtual glm::vec4 vertex() = 0;
	virtual glm::vec4 frag() = 0;

	inline Rasterizer& getContext() {
		return *m_context;
	}

	inline glm::mat4 getModelViewProjection()
	{
		return m_context->getMVP();
	}

	inline glm::vec4 getVertex()
	{
		return m_vertex;
	}

	inline glm::vec4 getFrag() const
	{
		return m_frag;
	}

	inline glm::vec3 getWeights() const
	{
		return m_weights;
	}

};

class BasicShader : IShader {
public:
	glm::vec4 vertex();
	glm::vec4 frag();
};*/

class Rasterizer
{
public:
	enum class Culling {
		None,
		Backface
	};

	enum class WindingOrder {
		Clockwise,
		CounterClockwise
	};

private:
	Surface* m_surface;
	Texture* m_textures[16];

	glm::mat4 m_model;
	glm::mat4 m_world;
	glm::mat4 m_view;
	glm::mat4 m_projection;

	//Taken from Wikipedia
	void drawLineLow(float x0, float y0, float x1, float y1);
	void drawLineHigh(float x0, float y0, float x1, float y1);
	void drawLine(float x0, float y0, float x1, float y1);

	Culling m_culling;
	WindingOrder m_windingOrder;

	static const std::vector<glm::vec2> CLIP_COORDS;
public:
	Rasterizer() {}
	Rasterizer(Surface* _surface);
	~Rasterizer();

	bool transform(const Vertex& a, const Vertex& b, const Vertex& c, glm::vec4& target_a, glm::vec4& target_b, glm::vec4& target_c);
	void drawLine(Vertex& a, Vertex& b);
	void drawTriangle(Vertex& a, Vertex& b, Vertex& c);
	void ortho(float left, float right, float bottom, float top, float near, float far);
	void perspective(float fovy, float aspect, float near, float far);
	glm::vec4 transform(glm::vec3 _position);
	void lookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);

	void bindTexture(Texture* texture, int index = 0);


	std::vector<glm::vec4> transform(std::vector<Vertex>& vertices);

	glm::mat4 getMVP();

	inline void setCulling(Culling _culling, WindingOrder _windingOrder) {
		m_culling = _culling;
		m_windingOrder = _windingOrder;
	}

	inline glm::mat4 getModel()
	{
		return m_model;
	}

	inline glm::mat4 getWorld()
	{
		return m_world;
	}

	inline glm::mat4 getView()
	{
		return m_view;
	}

	inline glm::mat4 getProjection()
	{
		return m_projection;
	}

	inline void setModel(glm::mat4 _model)
	{
		m_model = _model;
	}

	inline void setWorld(glm::mat4 _world)
	{
		m_view = _world;
	}

	inline void setView(glm::mat4 _view)
	{
		m_view = _view;
	}

	inline void setProjection(glm::mat4 _projection)
	{
		m_projection = _projection;
	}
	
};
