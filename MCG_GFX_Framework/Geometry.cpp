#include "Geometry.h"
#include "Pipeline.h"

Triangle::Triangle(Vertex _a, Vertex _b, Vertex _c)
{
	m_a = _a;
	m_b = _b;
	m_c = _c;
	m_normal = glm::cross((_b.m_position - _a.m_position), (_c.m_position - _a.m_position));
}


Rectangle::Rectangle(float x, float y, float width, float height, float minS, float minT, float maxS, float maxT) : m_x(x), m_y(y), m_width(width), m_height(height), m_texture(NULL)
{
	m_points[0] = { x, y, 0, minS, minT };
	m_points[1] = { x + width, y, 0, maxS, minT };
	m_points[2] = { x + width, y + height, 0, maxS, maxT };
	m_points[3] = { x, y + height, 0, minS, maxT };
}

void Rectangle::draw(Rasterizer* renderer)
{
	renderer->bindTexture(m_texture);
	renderer->drawTriangle(m_points[0], m_points[1], m_points[3]);
	renderer->drawTriangle(m_points[1], m_points[2], m_points[3]);
}

void Rectangle::setColour(glm::vec4 colour)
{
	for (int i = 0; i < 4; i++)
		m_points[i].m_colour = colour;
}

void Rectangle::setTexture(Texture* texture)
{
	m_texture = texture;
}