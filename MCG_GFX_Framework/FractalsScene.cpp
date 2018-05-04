#include "FractalsScene.h"

#include "Scene.h"
#include "Surface.h"

#define MAX_ITERATIONS 1000

void GenerateMandlebrot(Texture* texture) {
	glm::vec2 mX = { -2.5, 1.0 };
	glm::vec2 mY = { -1.0, 1.0 };

	for (int px = 0; px < texture->getWidth(); px++)
	{
		for (int py = 0; py < texture->getHeight(); py++)
		{
			double x0 = mX[0] + (((double)px / (double)texture->getWidth()) * (mX[1] - mX[0]));
			double y0 = mY[0] + (((double)py / (double)texture->getHeight()) * (mY[1] - mY[0]));
			double x = 0.0f;
			double y = 0.0f;

			int iteration = 0;

			while (x * x + y * y < 2 * 2 && iteration < MAX_ITERATIONS)
			{
				double tempX = x * x - y * y + x0;
				y = 2 * x * y + y0;
				x = tempX;
				iteration++;
			}

			unsigned char col = (unsigned char)((float)iteration / 1000.0f) * 255;
			texture->setPixelAt(px, py, col, col, col, 255);
		}
	}
}

void GenerateSeripinski(Texture* texture) {

}


FractalsScene::FractalsScene() {
	setTitle("Fractals");
	context = &getContext();
	int width = context->m_surface->getWidth();
	int height = context->m_surface->getHeight() - getStartY();
	GenerateMandlebrot(mandlebrot = new Texture(width, height));
	GenerateSeripinski(seripinski = new Texture(width, height));
}


FractalsScene::~FractalsScene() {
	delete mandlebrot;
	delete seripinski;
}

void FractalsScene::draw() {
	Rasterizer::DrawImage(*context, mandlebrot, 0, getStartY(), mandlebrot->getWidth(), mandlebrot->getHeight());
}