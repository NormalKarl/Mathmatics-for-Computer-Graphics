#define PI 3.14159265359
#include <cmath>
#include "PrimitivesScene.h"

PrimitivesScene::PrimitivesScene() : Scene()
{
	setTitle("Primitives");
	m_renderer = Rasterizer(getSurface());
	m_renderer.ortho(0.0f, (float)getSurface()->getWidth(), (float)getSurface()->getHeight(), 0.0f, -1.0f, 1.0f);
	m_polygonArray = VertexArray(Primitive::Triangle);

	//Add Triangle
	m_polygonArray.appendVertices({ { 125.0f, 100.0f },{ 175.0f, 175.0f },{ 75.0f, 175.0f } });
	m_polygonArray.appendIndices({ 0, 1, 2 });

	//Add Rectangle Vertices
	m_polygonArray.appendVertices({ { 275.0f, 100.0f },{ 350.0f, 100.0f },{ 350.0f, 175.0f },{ 275.0f, 175.0f } });
	m_polygonArray.appendIndices({ 3,4,5,5,6,3 });

	float angleStep = (PI * 2) / 100;
	float angle = 0;
	float radius = 100.0f / 2;


	int vertexCount = 1;
	m_polygonArray.appendVertex({ 500.0f, 140.0f });

	while (angle < PI * 2) {
		m_polygonArray.appendVertex({ 500.0f + (cos(angle) * radius), 140.0f + (sin(angle) * radius) });
		angle += angleStep;
		vertexCount++;
	}

	m_polygonArray.appendVertex({ 500.0f + (cos(0.0f) * radius), 140.0f + (sin(0.0f) * radius) });
	vertexCount++;

	for (int i = 2; i < vertexCount; i++) {
		m_polygonArray.appendIndices({ 7, 7 + ((unsigned int)i - 1), 7 + ((unsigned int)i) });
	}

}

PrimitivesScene::~PrimitivesScene()
{

}

void PrimitivesScene::draw() {
	m_polygonArray.render(&m_renderer);
}
