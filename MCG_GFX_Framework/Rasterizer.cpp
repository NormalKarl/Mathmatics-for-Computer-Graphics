#include "Rasterizer.h"
#include <GLM/gtc/matrix_transform.hpp>
#include "MCG_GFX_Lib.h"
#include "stb_image.h"

#include <vector>
#include <algorithm>

const std::vector<glm::vec2> Rasterizer::CLIP_COORDS = { { -1.0f, -1.0f },{ -1.0f, 1.0f },{ 1.0f, 1.0f },{ 1.0f, -1.0f } };

Texture::Texture(const char* filename)
{
	data = stbi_load(filename, &width, &height, &bytesPerPixel, STBI_rgb_alpha);
	filter = Filter::Point;
}

glm::vec4 Texture::getPixelApprox(glm::vec2 uv)
{
	float x = glm::clamp(uv.x, 0.0f, 1.0f);
	float y = glm::clamp(uv.y, 0.0f, 1.0f);

	float approxPosX = uv.x * float(width);
	float approxPosY = uv.y * float(height);

	int pixelX = static_cast<int>(glm::floor(approxPosX));
	int pixelY = static_cast<int>(glm::floor(approxPosY));
	//printf("Pixel W, %i, H %i, X %i, Y %i\n, U %f, V %f", width, height, pixelX, pixelY, uv.x, uv.y);

	glm::vec4 pixel = getPixelAt(pixelX, pixelY);

	if(filter == Filter::Bilinear)
	{
		float unitPosX = (approxPosX - (float)pixelX);
		float unitPosY = (approxPosY - (float)pixelY);
		float unitPosXInv = 1.0f - unitPosX;
		float unitPosYInv = 1.0f - unitPosY;

		glm::vec4 right = getPixelAt(pixelX + 1, pixelY);
		glm::vec4 down = getPixelAt(pixelX, pixelY + 1);
		glm::vec4 rightDown = getPixelAt(pixelX + 1, pixelY + 1);

		pixel = (pixel * unitPosXInv + right * unitPosX) * unitPosYInv + (down * unitPosXInv + rightDown * unitPosX) * unitPosY;
	}

	return pixel;
}

glm::vec4 Texture::getPixelAt(int x, int y)
{
	x = glm::clamp(x, 0, width - 1);
	y = glm::clamp(y, 0, height - 1);
	//Problem is here somewhere
	unsigned char dat[4] = { 0 };
	memcpy_s(dat, 4, &data[((y * width) + x) * bytesPerPixel], 4);

	return glm::vec4((float)dat[0] / 255.0f, (float)dat[1] / 255.0f, (float)dat[2] / 255.0f, (float)dat[3] / 255.0f);
}

//Basic Shader

/*glm::vec4 IShader::sample() const
{
	glm::vec3 w = getWeights();
	float persp = w.x * (1.0f / getFrag().w) + w.y * (1.0f / getFrag().w) + w.z * (1.0f / getFrag().w);
	glm::vec2 texUV = (w.x * (a.m_textureCoords / getFrag().w) + w.y * (b.m_textureCoords / getFrag().w) + w.z * (c.m_textureCoords / getFrag().w)) / persp;
}
 

glm::vec4 BasicShader::vertex() {
	return getModelViewProjection() * getVertex();
}

glm::vec4 BasicShader::frag() {

}*/


//Raster Code

Rasterizer::Rasterizer(Surface* _surface)
{
	m_surface = _surface;
	m_model = glm::mat4();
	m_world = glm::mat4();
	m_view = glm::mat4();
	m_projection = glm::mat4();

	m_culling = Rasterizer::Culling::Backface;
	m_windingOrder = Rasterizer::WindingOrder::Clockwise;
}

Rasterizer::~Rasterizer()
{
}

bool Rasterizer::transform(const Vertex & a, const Vertex & b, const Vertex & c, glm::vec4 & target_a, glm::vec4 & target_b, glm::vec4 & target_c)
{
	//Convert the vertex position to clip space
	glm::vec4 viewA = m_view * m_world * m_model * glm::vec4(a.m_position, 1.0f);
	glm::vec4 viewB = m_view * m_world * m_model * glm::vec4(b.m_position, 1.0f);
	glm::vec4 viewC = m_view * m_world * m_model * glm::vec4(c.m_position, 1.0f);

	target_a = transform(a.m_position);
	target_b = transform(b.m_position);
	target_c = transform(c.m_position);

	switch (m_culling)
	{
		case Culling::None: return true;
		case Culling::Backface:
		{
			if (m_windingOrder == WindingOrder::Clockwise)
			{
				return glm::dot(glm::cross((glm::vec3(viewC) - glm::vec3(viewA)), (glm::vec3(viewB) - glm::vec3(viewA))), glm::vec3(viewA)) >= 0.0f;
			}
			else
			{
				return glm::dot(glm::cross((glm::vec3(viewB) - glm::vec3(viewA)), (glm::vec3(viewC) - glm::vec3(viewA))), glm::vec3(viewA)) >= 0.0f;
			}
		}
	}
	//backface culling
}


void Rasterizer::drawLineLow(float x0, float y0, float x1, float y1)
{
	float dx = x1 - x0;
	float dy = y1 - y0;
	int yi = 1;
	if (dy < 0.0f)
	{
		yi = -1;
		dy = -dy;
	}

	float d = 2 * dy - dx;
	int y = y0;

	for (int x = x0; x <= x1; x++)
	{
		//frameBuffer.Set(x, y, { 1.0f, 1.0f, 1.0f, 1.0f });
		//MCG::DrawPixel({ x, m_surface->getViewport().height - y }, { 255, 255, 255 });
		m_surface->setColourAt(x, y, { 0.0f, 0.0f, 0.0f, 1.0f });

		if (d > 0.0f)
		{
			y = y + yi;
			d = d - 2 * dx;
		}
		d = d + 2 * dy;
	}
}

void Rasterizer::drawLineHigh(float x0, float y0, float x1, float y1)
{
	float dx = x1 - x0;
	float dy = y1 - y0;

	int xi = 1;

	if (dx < 0.0f)
	{
		xi = -1;
		dx = -dx;
	}

	float d = 2 * dx - dy;
	float x = x0;

	for (int y = y0; y <= y1; y++)
	{
		//frameBuffer.Set(x, y, { 1.0f, 1.0f, 1.0f, 1.0f });
		//MCG::DrawPixel({ x, m_surface->getViewport().height - y }, { 255, 255, 255 });
		m_surface->setColourAt(x, y, { 0.0f, 0.0f, 0.0f, 1.0f });

		if (d > 0.0f)
		{
			x = x + xi;
			d = d - 2 * dy;
		}

		d = d + 2 * dx;
	}
}

void Rasterizer::drawLine(float x0, float y0, float x1, float y1)
{
	if (glm::abs(y1 - y0) < glm::abs(x1 - x0))
	{
		if (x0 > x1)
		{
			drawLineLow(x1, y1, x0, y0);
		}
		else
		{
			drawLineLow(x0, y0, x1, y1);
		}
	}
	else
	{
		if (y0 > y1)
		{
			drawLineHigh(x1, y1, x0, y0);
		}
		else
		{
			drawLineHigh(x0, y0, x1, y1);
		}
	}
}

void Rasterizer::drawLine(Vertex& a, Vertex& b)
{
	glm::vec4 tA = transform(a.m_position);
	glm::vec4 tB = transform(b.m_position);


}

void Barycentric(glm::dvec4 p, glm::dvec4 a, glm::dvec4 b, glm::dvec4 c, float &u, float &v, float &w)
{
	glm::dvec4 v0 = b - a, v1 = c - a, v2 = p - a;
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

glm::vec4 blend(glm::vec4 colourA, glm::vec4 colourB)
{
	float alpha = colourA.a;
	colourA *= alpha;
	colourB *= colourB.a;
	colourB *= colourB * (1.0f - alpha);
	colourA += colourB;
	return colourA;
}


bool sortY(const glm::vec4& a, const glm::vec4& b)
{
	return a.y < b.y;
}

void findPointsTop(Viewport viewport, std::vector<glm::vec2>& points, glm::vec2 a, glm::vec2 b, glm::vec2 c)
{
	float invslope1 = (b.x - a.x) / (b.y - a.y);
	float invslope2 = (c.x - a.x) / (c.y - a.y);

	int yStart = (int)glm::ceil(a.y - 0.5f);
	int yEnd = (int)glm::ceil(c.y - 0.5f);
	yStart = glm::clamp(yStart, viewport.y, viewport.height);
	yEnd = glm::clamp(yEnd, viewport.y, viewport.height);

	for (int y = yStart; y < yEnd; y++)
	{
		float px0 = invslope1 * ((float)y + 0.5f - a.y) + a.x;
		float px1 = invslope2 * ((float)y + 0.5f - a.y) + a.x;

		int xStart = (int)glm::ceil(px0 - 0.5f);
		int xEnd = (int)glm::ceil(px1 - 0.5f);
		xStart = glm::clamp(xStart, viewport.x, viewport.width);
		xEnd = glm::clamp(xEnd, viewport.x, viewport.width);

		for (int x = xStart; x < xEnd; x++)
		{
			points.push_back({ x, y });
		}
	}
}

void findPointsBottom(Viewport viewport, std::vector<glm::vec2>& points, glm::vec2 a, glm::vec2 b, glm::vec2 c)
{
	float invslope1 = (c.x - a.x) / (c.y - a.y);
	float invslope2 = (c.x - b.x) / (c.y - b.y);

	int yStart = (int)glm::ceil(a.y - 0.5f);
	int yEnd = (int)glm::ceil(c.y - 0.5f);

	yStart = glm::clamp(yStart, viewport.y, viewport.height);
	yEnd = glm::clamp(yEnd, viewport.y, viewport.height);

	for (int y = yStart; y < yEnd; y++)
	{
		float px0 = invslope1 * ((float)y + 0.5f - a.y) + a.x;
		float px1 = invslope2 * ((float)y + 0.5f - b.y) + b.x;

		int xStart = (int)glm::ceil(px0 - 0.5f);
		int xEnd = (int)glm::ceil(px1 - 0.5f);
		xStart = glm::clamp(xStart, viewport.x, viewport.width);
		xEnd = glm::clamp(xEnd, viewport.x, viewport.width);

		for (int x = xStart; x < xEnd; x++)
		{
			points.push_back({ x, y });
		}
	}
}

std::vector<glm::vec2> findPoints(Viewport viewport, glm::vec4 a, glm::vec4 b, glm::vec4 c)
{
	std::vector<glm::vec2> pixels;
	std::vector<glm::vec4> points = { a, b, c };
	std::sort(points.begin(), points.end(), sortY);

	a = points[0];
	b = points[1];
	c = points[2];

	if (a.y == b.y)
	{
		if (b.x < a.x)
		{
			findPointsBottom(viewport, pixels, b, a, c);
		}
		else
		{
			findPointsBottom(viewport, pixels, a, b, c);
		}
	}
	else if (b.y == c.y)
	{
		if (c.x < b.x)
		{
			findPointsTop(viewport, pixels, a, c, b);
		}
		else
		{
			findPointsTop(viewport, pixels, a, b, c);
		}
	}
	else
	{
		float alphaSplit = (b.y - a.y) / (c.y - a.y);

		glm::vec4 v4 = a + (c - a) * alphaSplit;

		if (b.x < v4.x)
		{
			findPointsTop(viewport, pixels, a, b, v4);
			findPointsBottom(viewport, pixels, b, v4, c);
		}
		else
		{
			findPointsTop(viewport, pixels, a, v4, b);
			findPointsBottom(viewport, pixels, v4, b, c);
		}

	}

	return pixels;
}

inline int getSide(glm::vec2 p, glm::vec2 line0, glm::vec2 line1)
{
	return glm::sign((p.x - line0.x) * (line1.y - line0.y) - (p.y - line0.y) * (line1.x - line0.x));
}

inline glm::vec4 intersect(glm::vec4 v0, glm::vec4 v1, glm::vec2 c0, glm::vec2 c1)
{
	float x = (c0.x * c1.y - c0.y * c1.x) * (v0.x - v1.x) - (c0.x - c1.x) * (v0.x * v1.y - v0.y * v1.x);
	float y = (c0.x * c1.y - c0.y * c1.x) * (v0.y - v1.y) - (c0.y - c1.y) * (v0.x * v1.y - v0.y * v1.x);

	float div = (c0.x - c1.x) * (v0.y - v1.y) - (c0.y - c1.y) * (v0.x - v1.x);

	x /= div;
	y /= div;

	return glm::vec4(x, y, v0.z, v0.w);
}


//Sutherland-Hodgeman Clipping Algorithm
void edgeClip(std::vector<glm::vec4>& vertices, std::vector<glm::vec2> clips)
{
	//Define 2 vectors for both the current iteration and the new vertices.
	std::vector<glm::vec4> iteration = vertices;
	std::vector<glm::vec4> newVertices;

	for (int c = 0; c < clips.size(); c++)
	{
		glm::vec2 c1 = clips[c];
		glm::vec2 c2 = clips[(c + 1) % clips.size()];

		for (int v = 0; v < iteration.size(); v++)
		{
			glm::vec4 v1 = iteration[v];
			glm::vec4 v2 = iteration[(v + 1) % iteration.size()];

			//Get the sign's of the side in which the point resides.
			int sign1 = getSide(v1, c1, c2);
			int sign2 = getSide(v2, c1, c2);

			//First point is inside the clipping plane.
			if (sign1 >= 0)
			{
				//Add current point.
				newVertices.push_back(v1);

				//Second point is outside the clipping plane.
				if (sign2 < 0)
				{
					//Add intersected point.
					newVertices.push_back(intersect(v1, v2, c1, c2));
				}
			}
			//First point is outside the clipping plane and second point is inside.
			else if (sign2 >= 0)
			{
				//Add intersected point.
				newVertices.push_back(intersect(v1, v2, c1, c2));
			}
		}

		//Set new iteration to clipped vertices and clear the new vertex point.
		iteration = newVertices;
		newVertices.clear();
	}

	//Allocated the new clipped vertices to the referenced input.
	vertices = iteration;
}

//Reorder this so it can execute without creating duplicates.
void clipEdge(glm::vec4 v1, glm::vec4 v2, std::vector<glm::vec4>& output) {
	glm::vec4 new1 = v1;
	glm::vec4 new2 = v2;

	bool b1 = v1.w > 0.0f && v1.z > -v1.w;
	bool b2 = v2.w > 0.0f && v2.z > -v2.w;

	if (b1 && b2) {
	}
	else if (b1 || b2) {
		float d1 = v1.z + v1.w;
		float d2 = v2.z + v2.w;

		float factor = 1.0f / (d2 - d1);

		glm::vec4 newVertex = v2 + (factor * (d2 * v1 - d1 * v2));

		//float diff = 1.0f / (v2.z - v1.z);
		//glm::vec4 newVertex = v1 * (diff * (v2 - v1));

		if (b1) {
			new2 = newVertex;
		}
		else {
			new1 = newVertex;
		}
	}
	else {
		return;
	}

	if (output.size() == 0 || output[output.size() - 1] != new1) {
		output.push_back(new1);
	}

	output.push_back(new2);
}


//This quick clip algorithm is used for convex polygons only.
//Since I am using simple equalaterial clipping the clipping algotithm will produce only convex polygons.
void quickClip(std::vector<glm::vec4>& vertices)
{
	//If the vertex count is only 3 then there is no need to trianglulate the polygon.
	if (vertices.size() >= 4)
	{
		std::vector<glm::vec4> clippedVertices;

		for (int i = 2; i < vertices.size(); i++)
		{
			clippedVertices.push_back(vertices[0]);
			clippedVertices.push_back(vertices[i - 1]);
			clippedVertices.push_back(vertices[i]);
		}

		vertices = clippedVertices;
	}
}

void nearFarClip(std::vector<glm::vec4>& vertices) {
	std::vector<glm::vec4> newVertices;

	for (int i = 0; i < vertices.size(); i += 3) {
		glm::vec4 v1 = vertices[i];
		glm::vec4 v2 = vertices[i + 1];
		glm::vec4 v3 = vertices[i + 2];

		//Outside the camera.
		if (v1.w <= 0.0f || v2.w <= 0.0f || v3.w <= 0.0f) {
			continue;
		}

		//Inside the camera
		else if (glm::abs(v1.z) < v1.w && glm::abs(v1.z) < v1.w && glm::abs(v1.z) < v1.w) {
			newVertices.push_back(v1);
			newVertices.push_back(v2);
			newVertices.push_back(v3);
		}

		else {

			std::vector<glm::vec4> vs;

			clipEdge(v1, v2, vs);
			clipEdge(v2, v3, vs);
			clipEdge(v3, v1, vs);

			if (vs.size() < 3) {
				continue;
			}

			if (*(vs.end()) == *(vs.begin())) {
				vs.pop_back();
			}

			quickClip(vs);

			newVertices.insert(newVertices.end(), vs.begin(), vs.end());
		}
	}

	vertices = newVertices;
}

std::vector<glm::vec4> Rasterizer::transform(std::vector<Vertex>& vertices)
{
	std::vector<Vertex> positions = vertices;
	std::vector<glm::vec4> parseVectors;

	//First transform the positions to camera space for culling.
	for (int i = 0; i < vertices.size(); i++)
	{
		parseVectors.push_back(m_view * m_world * m_model * glm::vec4(vertices[i].m_position, 1.0f));
	}

	bool cull = false;
	//Perform face culling.
	switch (m_culling)
	{

		case Culling::None:
			cull = false;
			break;
		case Culling::Backface:
		{
			if (m_windingOrder == WindingOrder::Clockwise)
			{
				cull = glm::dot(glm::cross((glm::vec3(parseVectors[2]) - glm::vec3(parseVectors[0])), (glm::vec3(parseVectors[1]) - glm::vec3(parseVectors[0]))), glm::vec3(parseVectors[0])) >= 0.0f;
			}
			else
			{
				cull = glm::dot(glm::cross((glm::vec3(parseVectors[1]) - glm::vec3(parseVectors[0])), (glm::vec3(parseVectors[2]) - glm::vec3(parseVectors[0]))), glm::vec3(parseVectors[0])) >= 0.0f;
			}
		}
	}

	if (cull)
		return std::vector<glm::vec4>();

	//Convert to projection space for clipping.
	for (int i = 0; i < parseVectors.size(); i++)
	{
		parseVectors[i] = m_projection * parseVectors[i];
	}

	//edgeClip(parseVectors, CLIP_COORDS);

	nearFarClip(parseVectors);
	//edgeClip(parseVectors, CLIP_COORDS);
	quickClip(parseVectors);

	//Apply perspective divide.
	for (int i = 0; i < parseVectors.size(); i++)
	{
		glm::vec4 clip = parseVectors[i];
		float w = clip.w;

		if (w != 0.0f || w != 1.0f)
			clip /= w;

		parseVectors[i] = { (clip.x + 1.0f) * 0.5f * m_surface->getViewport().width, (clip.y + 1.0f) * 0.5f * m_surface->getViewport().height, (clip.z + 1.0f) * 0.5f, w };
	}

	return parseVectors;
}

void Rasterizer::drawTriangle(Vertex& a, Vertex& b, Vertex &c)
{
	std::vector<Vertex> vertices = { a,b,c };
	std::vector<glm::vec4> transformedPoints = transform(vertices);
	
	//Todo change this algorithm so it produces better triangles.
	//Use edge detection!
	for(int i = 0; i < transformedPoints.size(); i += 3)
	{
		/*int minX = (int)glm::floor(glm::clamp(glm::min(tA.x, glm::min(tB.x, tC.x)), 0.0f, (float)m_surface->getViewport().width));
		int minY = (int)glm::floor(glm::clamp(glm::min(tA.y, glm::min(tB.y, tC.y)), 0.0f, (float)m_surface->getViewport().height));
		int maxX = (int)glm::ceil(glm::clamp(glm::max(tA.x, glm::max(tB.x, tC.x)), 0.0f, (float)m_surface->getViewport().width));
		int maxY = (int)glm::ceil(glm::clamp(glm::max(tA.y, glm::max(tB.y, tC.y)), 0.0f, (float)m_surface->getViewport().height));*/

		glm::vec4 tA = transformedPoints[i];
		glm::vec4 tB = transformedPoints[i + 1];
		glm::vec4 tC = transformedPoints[i + 2];

		std::vector<glm::vec2> points = findPoints(m_surface->getViewport(), tA, tB, tC);

		for(glm::vec2 point : points) {
			if (point.x < 0 || point.y < 0 || point.x >= m_surface->getWidth() || point.y >= m_surface->getHeight())
			{
				continue;
			}

			glm::vec4 pixel = { (float)point.x + 0.5f, (float)point.y + 0.5f, 0.0f, 0.0f };

			float w0, w1, w2;
			Barycentric(pixel, tA, tB, tC, w0, w1, w2);

			//Make sure all of the weights are above or equal to 0
			//ensuring that the point is indeed inside the triangle.
			//if (w0 >= 0 && w1 >= 0 && w2 >= 0)
			//{
			float persp = w0 * (1.0f / tA.w) + w1 * (1.0f / tB.w) + w2 * (1.0f / tC.w);
			glm::vec2 texUV = (w0 * (a.m_textureCoords / tA.w) + w1 * (b.m_textureCoords / tB.w) + w2 * (c.m_textureCoords / tC.w)) / persp;

			glm::vec4 pixelLoc = w0 * tA + w1 * tB + w2 * tC;

			if (pixelLoc.z <= m_surface->getDepthAt(point.x, point.y))
			{
				m_surface->setDepthAt(point.x, point.y, pixelLoc.z);

				//TODO perspective correct color
				glm::vec4 pixelCol = (w0 * (a.m_colour / tA.w) + w1 * (b.m_colour / tB.w) + w2 * (c.m_colour / tC.w)) / persp;
				glm::vec4 texColor = { 1.0f, 1.0f, 1.0f, 1.0f };

				if (m_textures[0] != NULL)
				{
					glm::vec2 texUV = (w0 * (a.m_textureCoords / tA.w) + w1 * (b.m_textureCoords / tB.w) + w2 * (c.m_textureCoords / tC.w)) / persp;
					texColor = m_textures[0]->getPixelApprox(texUV);
				}

				//2 Pixel combination
				glm::vec4 finalCol = texColor * pixelCol;


				//finalCol = blend(texColor, glm::vec4(m_surface->getClearColour(), 1.0f));
				//float a = finalCol.a;
				//finalCol *= a;
				//glm::vec4 other = m_surface->getClearColour() * (1.0f - a));
				//finalCol += other;

				//MCG::DrawPixel({ (int)x, (int)m_viewport.height - y }, finalCol * 255.0f);
				//frameBuffer.Set(x, y, finalCol);

				m_surface->setColourAt(point.x, point.y, finalCol);
			}
		}

		/*tA = glm::clamp(tA, { 0.0f, 0.0f, -1.0f, 0.0f, }, {m_surface->getViewport().width - 1.0f, m_surface->getViewport().height - 1.0f, -1.0f, 0.0f });
		tB = glm::clamp(tB, { 0.0f, 0.0f, -1.0f, 0.0f, }, { m_surface->getViewport().width - 1.0f, m_surface->getViewport().height - 1.0f, -1.0f, 0.0f });
		tC = glm::clamp(tC, { 0.0f, 0.0f, -1.0f, 0.0f, }, { m_surface->getViewport().width - 1.0f, m_surface->getViewport().height - 1.0f, -1.0f, 0.0f });

		drawLine(tA.x, tA.y, tB.x, tB.y);
		drawLine(tB.x, tB.y, tC.x, tC.y);
		drawLine(tA.x, tA.y, tC.x, tC.y);*/
	}
}

void transform(std::vector<glm::vec4> points)
{

}

glm::vec4 Rasterizer::transform(glm::vec3 _position)
{
	//Convert the vertex position to clip space
	glm::vec4 clip = m_projection * m_view * m_world * m_model * glm::vec4(_position, 1.0f);

	//Divide by w to apply the perspective
	float w = clip.w;

	if (w != 0.0f || w != 1.0f)
		clip /= w;

	//Convert the NDC to screen space using the viewport.
	return { (clip.x + 1.0f) * 0.5f * m_surface->getViewport().width, (clip.y + 1.0f) * 0.5f * m_surface->getViewport().height, (clip.z + 1.0f) * 0.5f, w };
}


void Rasterizer::lookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
{
	m_view = glm::lookAt(glm::vec3(eyeX, eyeY, eyeZ), glm::vec3(centerX, centerY, centerZ), glm::vec3(upX, upY, upZ));
}

void Rasterizer::ortho(float left, float right, float bottom, float top, float near, float far)
{
	m_projection = glm::ortho(left, right, bottom, top, near, far);
}

void Rasterizer::perspective(float fovy, float aspect, float near, float far)
{
	m_projection = glm::perspective(fovy, aspect, near, far);
}

glm::mat4 Rasterizer::getMVP()
{
	return m_projection * m_view * m_world * m_model;
}

void Rasterizer::bindTexture(Texture* texture, int i)
{
	if (i >= 0 && i < 16)
		m_textures[i] = texture;
}