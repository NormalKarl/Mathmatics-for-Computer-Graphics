#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>
#include <map>

#include "Surface.h"
#include "Vertex.h"

class Texture;

/*
Context is a struct simmilar in nature to the OpenGL context which is used by the Rasterizer
below. The rasterizer can use this to project the vertices as well as performing lighting or
look up texture data.
*/
struct Context {
	Surface* m_surface;
	Texture* m_texture;
	glm::mat4 m_projection;
	glm::mat4 m_view;
	glm::mat4 m_world;
	glm::mat4 m_model;
	glm::vec3 m_cameraPosition;
	bool m_lighting;

	Context(Surface* _surface = NULL);
	void lookAt(float _eyeX, float _eyeY, float _eyeZ, float _centerX, float _centerY, float _centerZ, float _upX, float _upY, float _upZ);
	void ortho(float _left, float _right, float _bottom, float _top, float _near, float _far);
	void perspective(float _fovy, float _aspect, float _near, float _far);
	void reset();
	inline int getWidth() { return m_surface->getWidth(); }
	inline int getHeight() { return m_surface->getHeight(); }
};

/*
The Rasterizer namespace hold all the functions needed for rendering in both 3d and 2d
DrawPoint, DrawLine and DrawTriangle are the basic functions and all the other functions are
helpful uses of them.
*/
namespace Rasterizer {
	void DrawPoint(const Context& _context, const Vertex& _a);
	void DrawLine(const Context& _context, const Vertex& _a, const Vertex& _b);
	void DrawTriangle(const Context& _context, const Vertex& _a, const Vertex& _b, const Vertex& _c);
	void DrawQuad(const Context& _context, const Vertex& _a, const Vertex& _b, const Vertex& _c, const Vertex& _d);
	void FillRect(Context& _context, float _x, float _y, float _width, float _height, const glm::uvec4& _colour);
	void FillRect(Context& _context, float _x, float _y, float _width, float _height, const glm::vec4& _colour);
	void DrawImage(Context& _context, Texture* _texture, float _x, float _y);
	void DrawImage(Context& _context, Texture* _texture, float _x, float _y, float _width, float _height, float _tx = 0.0f, float _ty = 0.0f, float _tw = 1.0f, float _th = 1.0f);
}

#endif