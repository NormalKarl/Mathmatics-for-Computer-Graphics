#define PI 3.14159265359
#include <cmath>
#include "PrimitivesScene.h"
#include "Surface.h"
#include "Defs.h"

PrimitivesScene::PrimitivesScene() : Scene()
{
	/* The position on this may seem a bit random but initally I drew them directly to a 600x480 screen 
	however now its changed to 480p which is different and so I just use a matrix in the render function
	to align all these values to the center. */

	setTitle("Primitives");

	//Create triangle
	{
		m_triangle = VertexArray(Primitive::Triangle);
		m_triangle.appendVertices({ { 125.0f, 100.0f, 0, 0,0,0,1 },{ 175.0f, 175.0f, 0, 0,0,0,1 },{ 75.0f, 175.0f, 0, 0,0,0,1 } });
		m_triangle.appendIndices({ 0, 1, 2 });
	}

	//Create square
	{
		m_square = VertexArray(Primitive::Triangle);
		m_square.appendVertices({ { 275.0f, 100.0f , 0, 0,0,0,1 },{ 350.0f, 100.0f , 0, 0,0,0,1 },{ 350.0f, 175.0f , 0, 0,0,0,1 },{ 275.0f, 175.0f , 0, 0,0,0,1 } });
		m_square.appendIndices({ 0,1,2,2,3,0 });
	}

	//Create circle

	{
		m_circle = VertexArray(Primitive::Triangle);
		float angleStep = (float)((PI * 2) / 25);
		float angle = 0;
		float radius = 85.0f / 2;

		int vertexCount = 1;
		m_circle.appendVertex({ 500.0f, 140.0f, 0, 0,0,0,1 });

		while (angle < PI * 2) {
			m_circle.appendVertex({ 500.0f + (cos(angle) * radius), 140.0f + (sin(angle) * radius), 0, 0,0,0,1 });
			angle += angleStep;
			vertexCount++;
		}

		m_circle.appendVertex({ 500.0f + (cos(0.0f) * radius), 140.0f + (sin(0.0f) * radius), 0, 0,0,0,1 });
		vertexCount++;

		for (int i = 2; i < vertexCount; i++) {
			m_circle.appendIndices({ 0, ((unsigned int)i - 1), ((unsigned int)i) });
		}
	}

	//Create polygon
	{
		m_polygon = VertexArray(Primitive::Triangle);
		m_polygon.appendVertices({ { 175.0f, 300.0f, 0, 0,0,0,1 },{ 225.0f, 350.0f, 0, 0,0,0,1 },{ 275.0f, 300.0f, 0, 0,0,0,1 },{ 275.0f, 400.0f, 0, 0,0,0,1 },{ 175.0f, 400.0f, 0, 0,0,0,1 } });
		m_polygon.appendIndices({ 0, 1, 4, 1, 2, 3, 1, 3, 4 });
	}

	//Create lines
	{
		m_lines = VertexArray(Primitive::Line);

		for (int x = 0; x <= 100; x += 20) {
			m_lines.appendVertices({ { 375.0f + x, 300.0f, 0, 0,0,0,1 },{ 375.0f + x, 400.0f, 0, 0, 0, 0, 1 } });
			m_lines.appendVertices({ { 375.0f, 300.0f + x, 0, 0,0,0,1 },{ 475.0f, 300.0f + x, 0, 0, 0, 0, 1 } });
		}
	}

}

void PrimitivesScene::draw() {
	getContext().m_model = glm::translate(glm::mat4(), glm::vec3((float)(SCREEN_WIDTH - 650) / 2, (float)(SCREEN_HEIGHT - 480) / 2, 0.0f));
	m_triangle.render(getContext());
	m_square.render(getContext());
	m_circle.render(getContext());
	m_polygon.render(getContext());
	m_lines.render(getContext());
}
