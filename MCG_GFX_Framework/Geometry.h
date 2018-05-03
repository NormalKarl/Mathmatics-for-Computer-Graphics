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

class Texture;

struct Vertex
{
	union {
		glm::vec3 m_position;
		glm::vec4 m_posExt;
	};

	glm::vec4 m_colour;
	glm::vec3 m_normal;
	glm::vec2 m_textureCoords;

	inline Vertex()
	{
		m_position = { 0.0f, 0.0f, 0.0f };
		m_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_textureCoords = { 0.0f, 0.0f };
	}

	inline Vertex(float x, float y)
	{
		m_position = { x, y, 0.0f };
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
