#include "Rasterizer.h"
#include <GLM/gtc/matrix_transform.hpp>
#include "MCG_GFX_Lib.h"
#include "stb_image.h"
#include "Surface.h"
#include "Texture.h"
#include "Lighting.h"

#include <vector>
#include <algorithm>
#include <iostream>


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
		if (x < 0 || x >= context.m_surface->getWidth() || y < 0 || y >= context.m_surface->getHeight())
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

	for (float y = v0.m_position.y; y <= v1.m_position.y; y++) {
		if (x < 0 || x >= context.m_surface->getWidth() || y < 0 || y >= context.m_surface->getHeight())
			continue;

		context.m_surface->setColourAt(x, y, v0.m_colour);

		if (d > 0.0f) {
			x = x + xi;
			d = d - 2 * dy;
		}

		d = d + 2 * dx;
	}
}

void Rasterizer::DrawPoint(const Context& context, const Vertex& a) {
	Vertex v0 = a;

	if (Transform(context, v0)) {
		glm::ivec2 pixel = { round(v0.m_position.x), round(v0.m_position.y) };
		float d = context.m_surface->getDepthAt(pixel.x, pixel.y);

		if (v0.m_position.z <= d) {
			context.m_surface->setDepthAt(pixel.x, pixel.y, v0.m_position.z);
			context.m_surface->setColourAt(pixel.x, pixel.y, a.m_colour);
		}
	}
}

void Rasterizer::DrawLine(const Context& context, const Vertex& a, const Vertex& b) {
	Vertex v0 = a, v1 = b;

	if (Transform(context, v0, v1)) {

		//v1.m_position.x += 1.0f;
		//v1.m_position.y += 1.0f;

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

void DrawWeightedPixel(const Context& context, Vertex& a, Vertex& b, Vertex& c, glm::ivec2 pixel, glm::vec3 surfaceNormal) {
	float w0, w1, w2;
	CalculateWeights((glm::vec2)pixel + glm::vec2(0.5f, 0.5f), a.m_position, b.m_position, c.m_position, w0, w1, w2);

	glm::vec4 newPosition = w0 * a.m_posExt + w1 * b.m_posExt + w2 * c.m_posExt;
	glm::vec4 newColour = w0 * a.m_colour + w1 * b.m_colour + w2 * c.m_colour;
	glm::vec2 newTexCoords = (w0 * a.m_textureCoords) + (w1 * b.m_textureCoords) + (w2 * c.m_textureCoords);
	glm::vec4 texColour = glm::vec4(1.0f);
	glm::vec4 finalColour = glm::vec4(1.0f);

	if (context.m_texture != NULL)
		texColour = context.m_texture->sample(newTexCoords);

	finalColour = texColour * newColour;

	if (context.m_lighting)
		Lighting::Directional(context.m_cameraPosition, glm::normalize(glm::vec3(1.0f, 0.5f, 1.0f)), surfaceNormal, finalColour);

	float d = context.m_surface->getDepthAt(pixel.x, pixel.y);

	if (newPosition.z <= d) {
		context.m_surface->setDepthAt(pixel.x, pixel.y, newPosition.z);
		context.m_surface->setColourAt(pixel.x, pixel.y, finalColour);
	}
}

void FlatBottom(const Context& context, Vertex a, Vertex b, Vertex c, glm::vec3 surfaceNormal) {
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
			DrawWeightedPixel(context, a, b, c, { x, y }, surfaceNormal);
		}
	}
}

void FlatTop(const Context& context, Vertex a, Vertex b, Vertex c, glm::vec3 surfaceNormal) {
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
			DrawWeightedPixel(context, a, b, c, { x, y }, surfaceNormal);
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
	//glm::vec3 surfaceNormal = glm::normalize(glm::cross(b.m_position - a.m_position, c.m_position - a.m_position));
	//glm::vec3 surfaceNormal = glm::mat3(glm::inverse(glm::transpose(context.m_world * context.m_model))) * glm::normalize(glm::cross(b.m_position - a.m_position, c.m_position - a.m_position));
	glm::vec3 surfaceNormal = glm::mat3(glm::transpose(context.m_world * context.m_model)) * glm::cross(glm::normalize(b.m_position - a.m_position), glm::normalize(c.m_position - a.m_position));

	//DrawLine(context, { a.m_position.x, a.m_position.y , a.m_position.z }, { a.m_position.x + surfaceNormal.x, a.m_position.y + surfaceNormal.y , a.m_position.z + surfaceNormal.z });
	Vertex v0 = a, v1 = b, v2 = c;

	if (Transform(context, v0, v1, v2)) {
		//Sort the vertices from highest to lowest in the y axis.
		SortY(v0, v1);
		SortY(v1, v2);
		SortY(v0, v1);

		if (v0.m_position.y == v1.m_position.y) {
			if (v1.m_position.x < v0.m_position.x) {
				FlatTop(context, v1, v0, v2, surfaceNormal);
			}
			else {
				FlatTop(context, v0, v1, v2, surfaceNormal);
			}
		}
		else if (v1.m_position.y == v2.m_position.y) {
			if (v2.m_position.x < v1.m_position.x) {
				FlatBottom(context, v0, v2, v1, surfaceNormal);
			}
			else {
				FlatBottom(context, v0, v1, v2, surfaceNormal);
			}
		}
		else {
			float alphaSplit = (v1.m_position.y - v0.m_position.y) / (v2.m_position.y - v0.m_position.y);

			Vertex v3;
			v3.m_posExt = v0.m_posExt + (v2.m_posExt - v0.m_posExt) * alphaSplit;
			v3.m_textureCoords = v0.m_textureCoords + (v2.m_textureCoords - v0.m_textureCoords) * alphaSplit;
			v3.m_colour = v0.m_colour + (v2.m_colour - v0.m_colour) * alphaSplit;

			if (v1.m_position.x < v3.m_position.x) {
				FlatBottom(context, v0, v1, v3, surfaceNormal);
				FlatTop(context, v1, v3, v2, surfaceNormal);
			}
			else {
				FlatBottom(context, v0, v3, v1, surfaceNormal);
				FlatTop(context, v3, v1, v2, surfaceNormal);
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

void Rasterizer::DrawImage(Context& context, Texture* texture, float x, float y) {
	DrawImage(context, texture, x, y, texture->getWidth(), texture->getHeight());
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