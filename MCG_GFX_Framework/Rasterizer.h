#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>
#include <map>

#include "Surface.h"
#include "Vertex.h"

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
	glm::vec3 m_cameraPosition;

	bool m_lighting;

	inline Context(Surface* _surface = NULL) {
		m_surface = _surface;
		m_texture = NULL;
		m_projection = m_view = m_world = m_model = glm::mat4(1.0f);

		if (_surface != NULL) {
			ortho(0, _surface->getWidth(), _surface->getHeight(), 0, 0.0f, 1.0f);
		}

		m_lighting = false;
	}

	inline void lookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
	{
		m_cameraPosition = glm::vec3(eyeX, eyeY, eyeZ);
		m_view = glm::lookAt(m_cameraPosition, glm::vec3(centerX, centerY, centerZ), glm::vec3(upX, upY, upZ));
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

	inline int getWidth() {
		return m_surface->getWidth();
	}

	inline int getHeight() {
		return m_surface->getHeight();
	}
};

namespace Rasterizer {
	void DrawPoint(const Context& context, const Vertex& a);
	void DrawLine(const Context& context, const Vertex& a, const Vertex& b);
	void DrawTriangle(const Context& context, const Vertex& a, const Vertex& b, const Vertex& c);
	void DrawQuad(const Context& context, const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d);
	void FillRect(Context& context, float x, float y, float width, float height, const glm::uvec4& colour);
	void FillRect(Context& context, float x, float y, float width, float height, const glm::vec4& colour);
	void DrawImage(Context& context, Texture* texture, float x, float y);
	void DrawImage(Context& context, Texture* texture, float x, float y, float width, float height, float tx = 0.0f, float ty = 0.0f, float tw = 1.0f, float th = 1.0f);
}