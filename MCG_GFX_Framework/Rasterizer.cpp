#include "Rasterizer.h"
#include <GLM/gtc/matrix_transform.hpp>
#include "MCG_GFX_Lib.h"
#include "stb_image.h"
#include "tiny_obj_loader.h"
#include "Surface.h"
#include "Texture.h"

#include <vector>
#include <algorithm>
#include <iostream>

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

	for (tinyobj::material_t mat : materials) {
		arrays.push_back(VertexArray(Primitive::Triangle, new Texture(("assets/WolfModel/" + mat.diffuse_texname).c_str())));
	}


	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = shapes[s].mesh.num_face_vertices[f];

			std::vector<glm::vec3> vectors;

			float gray = (100 + (rand() % 100)) / 255.0f;
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex


				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];

				if (attrib.normals.size() != 0) {
					tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];


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

				arrays[shapes[s].mesh.material_ids[f]].appendVertex({ vx,vy,vz });

				//array.appendVertex({ vx, vy, vz, val, 0.2f, 0.2f, 1.0f });
				//array.appendVertex({ vx, vy, vz, gray, gray, gray, 1.0f });
			}

			//glm::vec3 normal = glm::cross(vectors[1] - vectors[0], vectors[2] - vectors[0]);

			//float val = glm::dot(glm::normalize(normal), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));

			//val = glm::clamp(val, 0.0f, 1.0f);
			
			//for (glm::vec3 v : vectors) {
				//array.appendVertex({ v.x, v.y, v.z, val, val, val, 1.0f });
			//}

			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}
}

void Model::draw(const Context& context) {
	//for (Texture* texture : arrays.size()) {
	//}
}

//Rasterizer

bool Transform(const Context& context, Vertex& vertex) {
	float width = context.m_surface->getWidth();
	float height = context.m_surface->getHeight();

	glm::vec4 t = context.m_projection * ((context.m_view * context.m_world * context.m_model) * glm::vec4(vertex.m_position, 1.0f));

	if (t.w <= 0.0f)
		return false;

	float tempW = t.w;

	t /= t.w;
	vertex.m_posExt = { ((t.x + 1.0f) * 0.5f) * width, ((t.y + 1.0f) * 0.5f) * height, (t.z + 1.0f) * 0.5f, tempW };

	return true;
}

bool Transform(const Context& context, Vertex& v0, Vertex& v1) {
	return Transform(context, v0) && Transform(context, v1);
}

bool Transform(const Context& context, Vertex& v0, Vertex& v1, Vertex& v2) {
	return Transform(context, v0) && Transform(context, v1) && Transform(context, v2);
}

void LineLow(const Context& context, const Vertex& v0, const Vertex& v1) {
	float dx = v1.m_position.x - v0.m_position.x;
	float dy = v1.m_position.y - v0.m_position.y;
	int yi = 1;

	if (dy < 0.0f) {
		yi = -1;
		dy = -dy;
	}

	float d = 2 * dy - dx;
	float y = v0.m_position.y;

	for (float x = v0.m_position.x; x <= v1.m_position.x; x++) {
		if (x < 0 || x > context.m_surface->getWidth() || y < 0 || y > context.m_surface->getHeight())
			continue;

		context.m_surface->setColourAt(x, y, v0.m_colour);

		if (d > 0.0f) {
			y = y + yi;
			d = d - 2 * dx;
		}
		d = d + 2 * dy;
	}
}

void LineHigh(const Context& context, const Vertex& v0, const Vertex& v1) {
	float dx = v0.m_position.x - v1.m_position.x;
	float dy = v1.m_position.y - v0.m_position.y;

	int xi = 1;

	if (dx < 0.0f) {
		xi = -1;
		dx = -dx;
	}

	float d = 2 * dx - dy;
	float x = v0.m_position.x;

	for (float y = v0.m_position.y; y <= v1.m_position.x; y++) {
		if (x < 0 || x > context.m_surface->getWidth() || y < 0 || y > context.m_surface->getHeight())
			continue;

		context.m_surface->setColourAt(x, y, v0.m_colour);

		if (d > 0.0f) {
			x = x + xi;
			d = d - 2 * dy;
		}

		d = d + 2 * dx;
	}
}

void Rasterizer::DrawLine(const Context& context, const Vertex& a, const Vertex& b) {
	Vertex v0 = a, v1 = b;

	if (Transform(context, v0, v1)) {
		if (glm::abs(v1.m_position.y - v0.m_position.y) < glm::abs(v1.m_position.x - v0.m_position.x)) {
			if (v0.m_position.x > v1.m_position.x) {
				LineLow(context, v1, v0);
			} else {
				LineLow(context, v0, v1);
			}
		} else {
			if (v0.m_position.y > v1.m_position.y) {
				LineHigh(context, v1, v0);
			} else {
				LineHigh(context, v0, v1);
			}
		}
	}
}

void CalculateWeights(glm::dvec2 p, glm::dvec2 a, glm::dvec2 b, glm::dvec2 c, float &u, float &v, float &w)
{
	glm::dvec2 v0 = b - a, v1 = c - a, v2 = p - a;
	double d00 = glm::dot(v0, v0);
	double d01 = glm::dot(v0, v1);
	double d11 = glm::dot(v1, v1);
	double d20 = glm::dot(v2, v0);
	double d21 = glm::dot(v2, v1);
	double denom = d00 * d11 - d01 * d01;
	v = (d11 * d20 - d01 * d21) / denom;
	w = (d00 * d21 - d01 * d20) / denom;
	u = 1.0f - v - w;
}

void DrawWeightedPixel(const Context& context, Vertex& a, Vertex& b, Vertex& c, glm::ivec2 pixel) {
	float w0, w1, w2;
	CalculateWeights((glm::vec2)pixel + glm::vec2(0.5f, 0.5f), a.m_position, b.m_position, c.m_position, w0, w1, w2);

	glm::vec4 newPosition = w0 * a.m_posExt + w1 * b.m_posExt + w2 * c.m_posExt;
	glm::vec4 newColour = w0 * a.m_colour + w1 * b.m_colour + w2 * c.m_colour;
	glm::vec2 newTexCoords = (w0 * a.m_textureCoords) + (w1 * b.m_textureCoords) + (w2 * c.m_textureCoords);

	//float diff = newPosition.w;
	//newColour = glm::vec4(diff, diff, diff, 1.0f);

	//printf("%f\n",newPosition.z);

	if (context.m_texture != NULL) {
		newColour = context.m_texture->sample(newTexCoords);
	}

	float d = context.m_surface->getDepthAt(pixel.x, pixel.y);

	if (newPosition.z <= d) {
		context.m_surface->setDepthAt(pixel.x, pixel.y, newPosition.z);
		context.m_surface->setColourAt(pixel.x, pixel.y, newColour);
	}
}

void FlatBottom(const Context& context, Vertex a, Vertex b, Vertex c) {
	float invSlope1 = (b.m_position.x - a.m_position.x) / (b.m_position.y - a.m_position.y);
	float invSlope2 = (c.m_position.x - a.m_position.x) / (c.m_position.y - a.m_position.y);

	int yStart = (int)glm::ceil(a.m_position.y - 0.5f);
	int yEnd = (int)glm::ceil(c.m_position.y - 0.5f);

	Viewport& viewport = context.m_surface->getViewport();
	yStart = glm::clamp(yStart, viewport.y, viewport.height);
	yEnd = glm::clamp(yEnd, viewport.y, viewport.height);

#pragma omp parallel
#pragma omp for
	for (int y = yStart; y < yEnd; y++)
	{
		float px0 = invSlope1 * ((float)y + 0.5f - a.m_position.y) + a.m_position.x;
		float px1 = invSlope2 * ((float)y + 0.5f - a.m_position.y) + a.m_position.x;

		int xStart = (int)glm::ceil(px0 - 0.5f);
		int xEnd = (int)glm::ceil(px1 - 0.5f);
		xStart = glm::clamp(xStart, viewport.x, viewport.width);
		xEnd = glm::clamp(xEnd, viewport.x, viewport.width);

#pragma omp parallel
#pragma omp for
		for (int x = xStart; x < xEnd; x++) {
			DrawWeightedPixel(context, a, b, c, { x, y });
		}
	}
}

void FlatTop(const Context& context, Vertex a, Vertex b, Vertex c) {
	float invSlope1 = (c.m_position.x - a.m_position.x) / (c.m_position.y - a.m_position.y);
	float invSlope2 = (c.m_position.x - b.m_position.x) / (c.m_position.y - b.m_position.y);

	int yStart = (int)glm::ceil(a.m_position.y - 0.5f);
	int yEnd = (int)glm::ceil(c.m_position.y - 0.5f);

	Viewport& viewport = context.m_surface->getViewport();
	yStart = glm::clamp(yStart, viewport.y, viewport.height);
	yEnd = glm::clamp(yEnd, viewport.y, viewport.height);

#pragma omp parallel
#pragma omp for
	for (int y = yStart; y < yEnd; y++)
	{
		float px0 = invSlope1 * ((float)y + 0.5f - a.m_position.y) + a.m_position.x;
		float px1 = invSlope2 * ((float)y + 0.5f - b.m_position.y) + b.m_position.x;

		int xStart = (int)glm::ceil(px0 - 0.5f);
		int xEnd = (int)glm::ceil(px1 - 0.5f);
		xStart = glm::clamp(xStart, viewport.x, viewport.width);
		xEnd = glm::clamp(xEnd, viewport.x, viewport.width);

#pragma omp parallel
#pragma omp for
		for (int x = xStart; x < xEnd; x++) {
			DrawWeightedPixel(context, a, b, c, { x, y });
		}
	}
}

void SortY(Vertex& a, Vertex& b) {
	if (b.m_position.y < a.m_position.y) {
		Vertex copy = a;
		a = b;
		b = copy;
	}
}

void Rasterizer::DrawTriangle(const Context& context, const Vertex& a, const Vertex& b, const Vertex& c) {
	Vertex v0 = a, v1 = b, v2 = c;

	if (Transform(context, v0, v1, v2)) {
		SortY(v0, v1);
		SortY(v1, v2);
		SortY(v0, v1);

		if (v0.m_position.y == v1.m_position.y) {
			if (v1.m_position.x < v0.m_position.x) {
				FlatTop(context, v1, v0, v2);
			}
			else {
				FlatTop(context, v0, v1, v2);
			}
		}
		else if (v1.m_position.y == v2.m_position.y) {
			if (v2.m_position.x < v1.m_position.x) {
				FlatBottom(context, v0, v2, v1);
			}
			else {
				FlatBottom(context, v0, v1, v2);
			}
		}
		else {
			float alphaSplit = (v1.m_position.y - v0.m_position.y) / (v2.m_position.y - v0.m_position.y);

			Vertex v3;
			v3.m_posExt = v0.m_posExt + (v2.m_posExt - v0.m_posExt) * alphaSplit;
			v3.m_textureCoords = v0.m_textureCoords + (v2.m_textureCoords - v0.m_textureCoords) * alphaSplit;
			v3.m_colour = v0.m_colour + (v2.m_colour - v0.m_colour) * alphaSplit;

			if (v1.m_position.x < v3.m_position.x) {
				FlatBottom(context, v0, v1, v3);
				FlatTop(context, v1, v3, v2);
			}
			else {
				FlatBottom(context, v0, v3, v1);
				FlatTop(context, v3, v1, v2);
			}
		}
	}
}

void Rasterizer::DrawQuad(const Context& context, const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d) {
	DrawTriangle(context, a, b, d);
	DrawTriangle(context, b, c, d);
}

void Rasterizer::FillRect(Context& context, float x, float y, float width, float height, const glm::uvec4& colour) {
	glm::vec4 calculatedColour = (glm::vec4)colour / glm::vec4(255.0f, 255.0f, 255.0f, 255.0f);
	FillRect(context, x, y, width, height, calculatedColour);
}

void Rasterizer::FillRect(Context& context, float x, float y, float width, float height, const glm::vec4& colour) {
	context.m_texture = NULL;
	DrawQuad(context,
		{ x, y, 0.0f, colour.r, colour.g, colour.b, colour.a },
		{ x + width, y, 0.0f, colour.r, colour.g, colour.b, colour.a },
		{ x + width, y + height, 0.0f, colour.r, colour.g, colour.b, colour.a },
		{ x, y + height, 0.0f, colour.r, colour.g, colour.b, colour.a });
}

void Rasterizer::DrawImage(Context& context, Texture* texture, float x, float y, float width, float height, float tx, float ty, float tw, float th) {
	Texture* temp = context.m_texture;
	context.m_texture = texture;

	DrawQuad(context,
		{ x, y, 0.0f, tx, ty },
		{ x + width, y, 0.0f, tx + tw, ty },
		{ x + width, y + height, 0.0f, tx + tw, ty + th },
		{ x, y + height, 0.0f, tx, ty + th });

	context.m_texture = temp;
}