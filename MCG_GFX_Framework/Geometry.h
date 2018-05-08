#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <GLM\glm.hpp>
#include "Rasterizer.h"
#include "Texture.h"

//Primitives for VertexArray
enum class Primitive
{
	Point,
	Line,
	Triangle
};

/*
VertexArray is a class which will hold an std::vector of vertices.
perfect for models and anything with more then a few Rasterizer calls.
*/
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
	void appendVertex(const Vertex& _vertex);
	void appendVertices(const std::vector<Vertex>& _vertices);
	void appendIndice(unsigned int _index);
	void appendIndices(std::vector<unsigned int> _indices);
	void offsetIndices(unsigned int _offset, std::vector<unsigned int> _indices);
	Vertex& operator[](int _index);
	void render(Context& _context);
	inline int getIndiceCount() { return m_indices.size(); }
	inline int getVertexCount() { return m_vertices.size(); }
	inline Texture* getTexture() { return m_texture; }
	inline void setTexture(Texture* _texture) { m_texture = _texture; }
};

/*
The Model name space hold functions for loading a model or making one.
*/
namespace Model
{
	VertexArray LoadOBJ(std::string _path);
	VertexArray CreateBox(Texture* _texture);
}

#endif