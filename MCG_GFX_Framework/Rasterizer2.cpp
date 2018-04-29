#include "Rasterizer2.h"
#include "Geometry.h"

#include "Texture.h"
#include "Surface.h"

#include <GLM\glm.hpp>
#include <iostream>

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

	glm::vec3 zValues = { 1.0f / a.m_posExt.w, 1.0f / b.m_posExt.w , 1.0f / c.m_posExt.w };

	float persp = w0 * (1.0f / a.m_posExt.w) + w1 * (1.0f / a.m_posExt.w) + w2 * (1.0f / a.m_posExt.w);

	glm::vec3 newPosition = w0 * a.m_position + w1 * b.m_position + w2 * c.m_position;
	glm::vec4 newColour = w0 * a.m_colour + w1 * b.m_colour + w2 * c.m_colour;
	glm::vec2 newTexCoords = (w0 * (a.m_textureCoords / a.m_posExt.w)) + (w1 * (b.m_textureCoords / b.m_posExt.w)) + (w2 * (c.m_textureCoords / c.m_posExt.w));

	//newTexCoords /= persp;

	if (context.m_texture != NULL) {
		newColour = context.m_texture->sample(newTexCoords);
	}

	if (newPosition.z < context.m_surface->getDepthAt(pixel.x, pixel.y)) {
		context.m_surface->setDepthAt(pixel.x, pixel.y, newPosition.z);
		context.m_surface->setColourAt(pixel.x, pixel.y, newColour);
	}
}

/*float lerp(float value, float a, float b) {
	return a + (value * (b - a));
}

Vertex lerp(float value, const Vertex& a, const Vertex& b) {
	Vertex v;
	v.m_posExt.x = lerp(value, a.m_posExt.x, b.m_posExt.x);
	v.m_posExt.y = lerp(value, a.m_posExt.y, b.m_posExt.y);
	v.m_posExt.z = lerp(value, a.m_posExt.z, b.m_posExt.z);
	v.m_posExt.w = lerp(value, a.m_posExt.w, b.m_posExt.w);
	v.m_colour.x = lerp(value, a.m_colour.x, b.m_colour.x);
	v.m_colour.y = lerp(value, a.m_colour.y, b.m_colour.y);
	v.m_colour.z = lerp(value, a.m_colour.z, b.m_colour.z);
	v.m_colour.w = lerp(value, a.m_colour.w, b.m_colour.w);
	v.m_textureCoords.x = lerp(value, a.m_textureCoords.x, b.m_textureCoords.x);
	v.m_textureCoords.y = lerp(value, a.m_textureCoords.y, b.m_textureCoords.y);
	return v;
}

void DrawLine(const Context& context, Vertex a, Vertex b) {
	for (int x = a.m_posExt.x; x < b.m_posExt.x; x++) {
		context.m_surface->setColourAt(x, b.m_posExt.y, {1.0f, 1.0f, 1.0f, 1.0f});
	}
}*/

void FlatBottom(const Context& context, Vertex a, Vertex b, Vertex c) {
	float invSlope1 = (b.m_position.x - a.m_position.x) / (b.m_position.y - a.m_position.y);
	float invSlope2 = (c.m_position.x - a.m_position.x) / (c.m_position.y - a.m_position.y);

	int yStart = (int)glm::ceil(a.m_position.y - 0.5f);
	int yEnd = (int)glm::ceil(c.m_position.y - 0.5f);

	Viewport& viewport = context.m_surface->getViewport();
	yStart = glm::clamp(yStart, viewport.y, viewport.height);
	yEnd = glm::clamp(yEnd, viewport.y, viewport.height);

	for (int y = yStart; y < yEnd; y++)
	{
		float px0 = invSlope1 * ((float)y + 0.5f - a.m_position.y) + a.m_position.x;
		float px1 = invSlope2 * ((float)y + 0.5f - a.m_position.y) + a.m_position.x;

		int xStart = (int)glm::ceil(px0 - 0.5f);
		int xEnd = (int)glm::ceil(px1 - 0.5f);
		xStart = glm::clamp(xStart, viewport.x, viewport.width);
		xEnd = glm::clamp(xEnd, viewport.x, viewport.width);

		/*float interpA = glm::dot(glm::normalize(glm::vec2(xStart + 0.5f, y + 0.5f) - glm::vec2(a.m_position)), glm::normalize(glm::vec2(b.m_position - a.m_position)));
		float interpB = glm::dot(glm::normalize(glm::vec2(xEnd + 0.5f, y + 0.5f) - glm::vec2(a.m_position)), glm::normalize(glm::vec2(c.m_position - a.m_position)));

		Vertex newA = lerp(interpA, b, a);
		Vertex newB = lerp(interpB, c, a);*/

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

	for (int y = yStart; y < yEnd; y++)
	{
		float px0 = invSlope1 * ((float)y + 0.5f - a.m_position.y) + a.m_position.x;
		float px1 = invSlope2 * ((float)y + 0.5f - b.m_position.y) + b.m_position.x;

		int xStart = (int)glm::ceil(px0 - 0.5f);
		int xEnd = (int)glm::ceil(px1 - 0.5f);
		xStart = glm::clamp(xStart, viewport.x, viewport.width);
		xEnd = glm::clamp(xEnd, viewport.x, viewport.width);

		for (int x = xStart; x < xEnd; x++) {
			DrawWeightedPixel(context, a, b, c, { x, y });
		}
	}
}

bool Transform(const Context& context, Vertex& v0, Vertex& v1, Vertex& v2) {
	float width = context.m_surface->getWidth();
	float height = context.m_surface->getHeight();

	auto transform = [&context, &width, &height](Vertex& v) {
		glm::vec4 t = (context.m_projection * context.m_view * context.m_world * context.m_model) * glm::vec4(v.m_position, 1.0f);

		if (t.w <= 0.0f)
			return false;

		float tempW = t.w;

		t /= t.w;
		v.m_posExt = { ((t.x + 1.0f) * 0.5f) * width, ((t.y + 1.0f) * 0.5f) * height, (t.z + 1.0f) * 0.5f, tempW };

		return true;
	};

	return transform(v0) && transform(v1) && transform(v2);
}

void SortY(Vertex& a, Vertex& b) {
	if (b.m_position.y < a.m_position.y) {
		Vertex copy = a;
		a = b;
		b = copy;
	}
}

void Render::DrawTriangle(const Context& context, const Vertex& a, const Vertex& b, const Vertex& c) {
	Vertex v0 = a, v1 = b, v2 = c;

	if (Transform(context, v0, v1, v2)) {
		SortY(v0, v1);
		SortY(v1, v2);
		SortY(v0, v1);

		if (v0.m_position.y == v1.m_position.y) {
			if (v1.m_position.x < v0.m_position.x) {
				FlatTop(context, v1, v0, v2);
			} else {
				FlatTop(context, v0, v1, v2);
			}
		}
		else if (v1.m_position.y == v2.m_position.y) {
			if (v2.m_position.x < v1.m_position.x) {
				FlatBottom(context, v0, v2, v1);
			} else {
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
			} else {
				FlatBottom(context, v0, v3, v1);
				FlatTop(context, v3, v1, v2);
			}
		}
	}
}
