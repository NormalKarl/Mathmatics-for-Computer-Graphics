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

Model::Model(std::string name) {
	std::string inputfile = name;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());

	if (!err.empty()) { // `err` may contain warning message.
		std::cout << err << std::endl;
	}

	if (!ret) {
		//exit(1);
	}

	if (materials.size() != 0) {
		for (tinyobj::material_t mat : materials) {
			Texture* texture = NULL;

			if (!mat.diffuse_texname.empty()) {
				texture = new Texture(("assets/WolfModel/" + mat.diffuse_texname).c_str());
			}

			arrays.push_back(VertexArray(Primitive::Triangle, texture));
		}
	}
	else {
		arrays.push_back(VertexArray(Primitive::Triangle, NULL));
	}

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = shapes[s].mesh.num_face_vertices[f];

			std::vector<Vertex> vectors;

			float gray = (100 + (rand() % 100)) / 255.0f;
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				Vertex newVertex;

				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
				newVertex.m_position = { vx,vy,vz };
				//newVertex.m_normal = glm::normalize(newVertex.m_position);

				if (attrib.normals.size() != 0) {
					tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];

				}

				if (attrib.texcoords.size() != 0) {
					tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
					newVertex.m_textureCoords = { tx,ty };
				}
				//tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
				//tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
				//tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
				//tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				//tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				// Optional: vertex colors
				// tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
				// tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
				// tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];

				//float val = glm::dot(glm::normalize(glm::vec3(vx, vy, vz)), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
				//val = glm::clamp(val, 0.0f, 1.0f);

				//arrays[shapes[s].mesh.material_ids[f]].appendVertex(newVertex);

				vectors.push_back(newVertex);

				//array.appendVertex({ vx, vy, vz, val, 0.2f, 0.2f, 1.0f });
				//array.appendVertex({ vx, vy, vz, gray, gray, gray, 1.0f });
			}

			glm::vec3 normal = glm::cross(vectors[1].m_position - vectors[0].m_position, vectors[2].m_position - vectors[0].m_position);

			for (Vertex& newVertex : vectors) {
				newVertex.m_normal = glm::normalize(normal);

				if (shapes[s].mesh.material_ids.size() != 0) {
					int index = shapes[s].mesh.material_ids[f];
					arrays[index == -1 ? 0 : index].appendVertex(newVertex);
				}
			}

			//float val = glm::dot(glm::normalize(normal), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));

			//val = glm::clamp(val, 0.0f, 1.0f);

			//for (glm::vec3 v : vectors) {
			//array.appendVertex({ v.x, v.y, v.z, val, val, val, 1.0f });
			//}

			index_offset += fv;

			// per-face material
			/*if (shapes[s].mesh.material_ids.size() != 0) {
			shapes[s].mesh.material_ids[f];
			}
			else {
			shapes[s].pus
			}*/
		}
	}
}

void Model::draw(Context& context) {
	for (VertexArray& array : arrays) {
		array.render(context);
	}
}