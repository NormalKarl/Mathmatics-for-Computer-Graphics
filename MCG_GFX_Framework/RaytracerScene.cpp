#include "RaytracerScene.h"
#include "Raytracer.h"

RaytracerScene::RaytracerScene()
{
	setTitle("Raytracer");
	m_tracer = new Raytracer(getSurface());
}


RaytracerScene::~RaytracerScene()
{
}

void RaytracerScene::draw()
{
	m_tracer->trace();
}