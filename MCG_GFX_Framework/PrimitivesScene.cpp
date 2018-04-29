#define PI 3.14159265359
#include <cmath>
#include "PrimitivesScene.h"

PrimitivesScene::PrimitivesScene() : Scene()
{
	setTitle("Primitives");
	m_renderer = Rasterizer(getSurface());
	m_renderer.ortho(0.0f, (float)getSurface()->getWidth(), (float)getSurface()->getHeight(), 0.0f, -1.0f, 1.0f);

	m_triangle = VertexArray(Primitive::Triangle);
	m_square = VertexArray(Primitive::Triangle);
	m_circle = VertexArray(Primitive::Triangle);
	m_polygon = VertexArray(Primitive::Triangle);
	m_lines = VertexArray(Primitive::Line);

	//Triangle
	m_triangle.appendVertices({ { 125.0f, 100.0f },{ 175.0f, 175.0f },{ 75.0f, 175.0f } });
	m_triangle.appendIndices({ 0, 1, 2 });

	//Rectangle
	m_square.appendVertices({ { 275.0f, 100.0f },{ 350.0f, 100.0f },{ 350.0f, 175.0f },{ 275.0f, 175.0f } });
	m_square.appendIndices({ 0,1,2,2,3,0 });

	float angleStep = (PI * 2) / 25;
	float angle = 0;
	float radius = 85.0f / 2;

	//Circle
	{
		int vertexCount = 1;
		m_circle.appendVertex({ 500.0f, 140.0f });

		while (angle < PI * 2) {
			m_circle.appendVertex({ 500.0f + (cos(angle) * radius), 140.0f + (sin(angle) * radius) });
			angle += angleStep;
			vertexCount++;
		}

		m_circle.appendVertex({ 500.0f + (cos(0.0f) * radius), 140.0f + (sin(0.0f) * radius) });
		vertexCount++;

		for (int i = 2; i < vertexCount; i++) {
			m_circle.appendIndices({ 0, ((unsigned int)i - 1), ((unsigned int)i) });
		}
	}

	//Create polygon
	{
		m_polygon.appendVertices({ { 175.0f, 300.0f },{ 225.0f, 350.0f },{ 275.0f, 300.0f },{ 275.0f, 400.0f },{ 175.0f, 400.0f } });
		m_polygon.appendIndices({ 0, 1, 4, 1, 2, 3, 1, 3, 4 });
	}

	//Create lines
	{
		m_lines.appendVertices({ {400.0f, 300.0f},{ 450.0f, 350.0f } });
	}

}

PrimitivesScene::~PrimitivesScene()
{

}

void PrimitivesScene::draw() {
	/*m_triangle.render(&m_renderer);
	m_square.render(&m_renderer);
	m_circle.render(&m_renderer);
	m_polygon.render(&m_renderer);
	m_lines.render(&m_renderer);*/
}