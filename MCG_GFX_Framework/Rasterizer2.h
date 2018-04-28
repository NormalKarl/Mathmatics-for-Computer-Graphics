#pragma once

#include "Geometry.h"
#include <GLM\gtc\matrix_transform.hpp>

class Texture;
class Surface;

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
};

namespace Render {
	void DrawTriangle(const Context& context, const Vertex& a, const Vertex& b, const Vertex& c);
}