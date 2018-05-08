#include "Geometry.h"

#include <iostream>
#include "tiny_obj_loader.h"

VertexArray::VertexArray(Primitive _primitive, Texture* _texture, int _initalVertexSize, int _initalIndiceSize)
	: m_primitive(_primitive)
	, m_vertices(std::vector<Vertex>(_initalVertexSize))
	, m_indices(std::vector<unsigned int>(_initalIndiceSize))
	, m_texture(_texture)
{
}

VertexArray::~VertexArray()
{
}

void VertexArray::appendVertex(const Vertex& vertex)
{
	m_vertices.push_back(vertex);
}

void VertexArray::appendVertices(const std::vector<Vertex>& _vertices)
{
	m_vertices.insert(m_vertices.end(), _vertices.begin(), _vertices.end());
}

void VertexArray::appendIndice(unsigned int index)
{
	m_indices.push_back(index);
}

void VertexArray::appendIndices(std::vector<unsigned int> _indices)
{
	m_indices.insert(m_indices.end(), _indices.begin(), _indices.end());
}

void VertexArray::offsetIndices(unsigned int _offset, std::vector<unsigned int> _indices) {
	for (size_t i = 0; i < _indices.size(); i++) {
		_indices[i] = _offset + _indices[i];
	}

	appendIndices(_indices);
}

Vertex& VertexArray::operator[](int index)
{
	return m_vertices[index];
}

void VertexArray::render(Context& context)
{
	context.m_texture = m_texture;

	if (m_indices.size() != 0)
	{
		size_t i = 0;

		while (i < m_indices.size())
		{
			switch (m_primitive)
			{
			case Primitive::Line:
				Rasterizer::DrawLine(context, m_vertices[m_indices[i]], m_vertices[m_indices[i + 1]]);
				i += 2;
				break;
			case Primitive::Triangle:
				Rasterizer::DrawTriangle(context, m_vertices[m_indices[i]], m_vertices[m_indices[i + 1]], m_vertices[m_indices[i + 2]]);
				i += 3;
				break;
			}
		}
	}
	else {
		size_t i = 0;

		while (i < m_vertices.size())
		{
			switch (m_primitive)
			{
			case Primitive::Line:
				Rasterizer::DrawLine(context, m_vertices[i], m_vertices[i + 1]);
				i += 2;
				break;
			case Primitive::Triangle:
				Rasterizer::DrawTriangle(context, m_vertices[i], m_vertices[i + 1], m_vertices[i + 2]);
				i += 3;
				break;
			}
		}
	}
}

//Model Code

VertexArray Model::LoadOBJ(std::string path) {
	VertexArray va;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str());

	if (!err.empty()) { // `err` may contain warning message.
		std::cout << err << std::endl;
	}

	for (int s = 0; s < shapes.size(); s++) {
		int index_offset = 0;

		for (int f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			for (size_t v = 0; v < fv; v++) {
				Vertex newVertex;

				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				float vx = attrib.vertices[3 * idx.vertex_index + 0];
				float vy = attrib.vertices[3 * idx.vertex_index + 1];
				float vz = attrib.vertices[3 * idx.vertex_index + 2];
				newVertex.m_position = { vx,vy,vz };

				if (attrib.colors.size() != 0) {
					float red = attrib.colors[3 * idx.vertex_index + 0];
					float green = attrib.colors[3 * idx.vertex_index + 1];
					float blue = attrib.colors[3 * idx.vertex_index + 2];
					newVertex.m_colour = { red, green, blue, 1.0f };
				}

				if (attrib.texcoords.size() != 0) {
					float tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					float ty = attrib.texcoords[2 * idx.texcoord_index + 1];
					newVertex.m_textureCoords = { tx,ty };
				}

				va.appendVertex(newVertex);
			}

			index_offset += fv;
		}
	}

	return va;
}

VertexArray Model::CreateBox(Texture* texture) {
	VertexArray va = VertexArray(Primitive::Triangle, texture);

	va.appendVertices({
		{ -0.5f, -0.5f, -0.5f, 0.0f, 0.0f },
		{ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f },
		{ 0.5f, 0.5f, -0.5f, 1.0f, 1.0f },
		{ 0.5f, 0.5f, -0.5f, 1.0f, 1.0f },
		{ -0.5f, 0.5f, -0.5f, 0.0f, 1.0f },
		{ -0.5f, -0.5f, -0.5f, 0.0f, 0.0f },

		{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f },
		{ 0.5f, -0.5f, 0.5f, 1.0f, 0.0f },
		{ 0.5f, 0.5f, 0.5f, 1.0f, 1.0f },
		{ 0.5f, 0.5f, 0.5f, 1.0f, 1.0f },
		{ -0.5f, 0.5f, 0.5f, 0.0f, 1.0f },
		{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f },

		{ -0.5f, 0.5f, 0.5f, 1.0f, 0.0f },
		{ -0.5f, 0.5f, -0.5f, 1.0f, 1.0f },
		{ -0.5f, -0.5f, -0.5f, 0.0f, 1.0f },
		{ -0.5f, -0.5f, -0.5f, 0.0f, 1.0f },
		{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f },
		{ -0.5f, 0.5f, 0.5f, 1.0f, 0.0f },

		{ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f },
		{ 0.5f, 0.5f, -0.5f, 1.0f, 1.0f },
		{ 0.5f, -0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.5f, 0.0f, 0.0f },
		{ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f },

		{ -0.5f, -0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, -0.5f, 1.0f, 1.0f },
		{ 0.5f, -0.5f, 0.5f, 1.0f, 0.0f },
		{ 0.5f, -0.5f, 0.5f, 1.0f, 0.0f },
		{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f },
		{ -0.5f, -0.5f, -0.5f, 0.0f, 1.0f },

		{ -0.5f, 0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f, 0.5f, -0.5f, 1.0f, 1.0f },
		{ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f },
		{ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f },
		{ -0.5f, 0.5f, 0.5f, 0.0f, 0.0f },
		{ -0.5f, 0.5f, -0.5f, 0.0f, 1.0f }
	});

	return va;
}
