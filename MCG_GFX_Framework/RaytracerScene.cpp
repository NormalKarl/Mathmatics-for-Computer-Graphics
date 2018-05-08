#include "RaytracerScene.h"
#include "Raytracer.h"

RaytracerScene::RaytracerScene()
{
	setTitle("Raytracer");
	m_tracer = new Raytracer(getContext().m_surface);

	sphere.m_position = { -0.5f, 0.0f, 0.0f };
	sphere.m_radius = 0.20f;
	m_tracer->addGeom(&sphere);

	tri.m_cull = true;
	tri.m_v0 = { 0.0f, -0.25f, 1.0f };
	tri.m_v1 = { 0.5f, 0.25f, 0.5f };
	tri.m_v2 = { 1.0f, -0.25f, 0.0f };
	m_tracer->addGeom(&tri);
}

void RaytracerScene::draw()
{
	m_tracer->trace();
}