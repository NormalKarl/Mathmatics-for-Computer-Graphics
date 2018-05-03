#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>
#include <map>

#include "Geometry.h"

class Texture;
class Surface;

enum class Primitive {
	Point,
	Line,
	Triangle
};

struct Context {
	Surface* m_surface;
	Texture* m_texture;
	glm::mat4 m_projection;
	glm::mat4 m_view;
	glm::mat4 m_world;
	glm::mat4 m_model;

	inline Context() {
		m_surface = NULL;
		m_texture = NULL;
		m_projection = m_view = m_world = m_model = glm::mat4(1.0f);
	}

	inline void lookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
	{
		m_view = glm::lookAt(glm::vec3(eyeX, eyeY, eyeZ), glm::vec3(centerX, centerY, centerZ), glm::vec3(upX, upY, upZ));
	}

	inline void ortho(float left, float right, float bottom, float top, float near, float far)
	{
		m_projection = glm::ortho(left, right, bottom, top, near, far);
	}

	inline void perspective(float fovy, float aspect, float near, float far)
	{
		m_projection = glm::perspective(fovy, aspect, near, far);
	}

	inline void reset() {
		m_texture = NULL;
		m_world = glm::mat4();
		m_model = glm::mat4();
	}
};

class VertexArray
{
private:
	Primitive m_primitive;
	Texture* m_texture;
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
public:
	VertexArray(Primitive _primitive = Primitive::Triangle, Texture* _texture = NULL, int _initalVertexSize = 0, int _initalIndiceSize = 0);
	~VertexArray();
	void appendVertex(const Vertex& vertex);
	void appendVertices(const std::vector<Vertex>& _vertices);
	void appendIndice(unsigned int index);
	void appendIndices(std::vector<unsigned int> _indices);
	void offsetIndices(unsigned int _offset, std::vector<unsigned int> _indices);

	Vertex& operator[](int index);

	void render(Context& context);

	inline int getIndiceCount() {
		return m_indices.size();
	}

	inline int getVertexCount() {
		return m_vertices.size();
	}

	inline Texture* getTexture() {
		return m_texture;
	}

	inline void setTexture(Texture* _texture) {
		m_texture = _texture;
	}
};

class Model {
public:
	std::vector<VertexArray> arrays;
	Model(std::string name);

	void draw(const Context& context);
};

namespace Rasterizer {
	void DrawLine(const Context& context, const Vertex& a, const Vertex& b);
	void DrawTriangle(const Context& context, const Vertex& a, const Vertex& b, const Vertex& c);
	void DrawQuad(const Context& context, const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d);
	void FillRect(Context& context, float x, float y, float width, float height, const glm::uvec4& colour);
	void FillRect(Context& context, float x, float y, float width, float height, const glm::vec4& colour);
	void DrawImage(Context& context, Texture* texture, float x, float y, float width, float height, float tx = 0.0f, float ty = 0.0f, float tw = 1.0f, float th = 1.0f);
}