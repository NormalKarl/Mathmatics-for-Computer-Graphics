#pragma once

#include <GLM\glm.hpp>
#include "Rasterizer.h"
#include "Texture.h"

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

	void draw(Context& context);
};