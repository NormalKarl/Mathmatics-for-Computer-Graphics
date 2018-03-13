#pragma once

#include <GLM\glm.hpp>

/*
Vertex
IGeometry
Triangle
Plane
Rectangle
Sphere
Circle (Can be used to make Octagon, Hexagon, Pentagon etc.)
*/

class Rasterizer;
class Texture;

struct Vertex
{
	glm::vec3 m_position;
	glm::vec4 m_colour;
	glm::vec2 m_textureCoords;

	inline Vertex()
	{
		m_position = { 0.0f, 0.0f, 0.0f };
		m_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_textureCoords = { 0.0f, 0.0f };
	}

	inline Vertex(float x, float y, float z)
	{
		m_position = { x, y, z };
		m_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_textureCoords = { 0.0f, 0.0f };
	}

	inline Vertex(float x, float y, float z, float r, float g, float b, float a)
	{
		m_position = { x, y, z };
		m_colour = { r, g, b, a };
		m_textureCoords = { 0.0f, 0.0f };
	}

	inline Vertex(float x, float y, float z, float u, float v)
	{
		m_position = { x, y, z };
		m_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_textureCoords = { u, v };
	}

	inline Vertex(float x, float y, float z, float r, float g, float b, float a, float u, float v)
	{
		m_position = { x, y, z };
		m_colour = { r, g, b, a };
		m_textureCoords = { u, v };
	}
};

class Triangle
{
private:
	Vertex m_a;
	Vertex m_b;
	Vertex m_c;
	glm::vec3 m_normal;
public:
	Triangle() {}
	Triangle(Vertex _a, Vertex _b, Vertex _c);
	~Triangle();
};

class Rectangle
{
private:
	Vertex m_points[4];
	float m_x;
	float m_y;
	float m_width;
	float m_height;
	Texture* m_texture;
public:
	Rectangle(float x, float y, float width, float height, float minU = 0.0f, float minV = 0.0f, float maxU = 1.0f, float maxV = 1.0f);

	void draw(Rasterizer* renderer);
	void setColour(glm::vec4 colour);
	void setTexture(Texture* texture);
};

class Sphere {
public:
	glm::vec3 m_position;
	float m_radius;
};

class Plane {
public:
	glm::vec3 point;
};