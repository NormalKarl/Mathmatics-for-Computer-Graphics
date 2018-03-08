#include "MandlebrotScene.h"



MandlebrotScene::MandlebrotScene()
{
}


MandlebrotScene::~MandlebrotScene()
{
}

#define MAX_ITERATIONS 1000

void MandlebrotScene::draw()
{
	//Mandlebrot range.
	glm::vec2 mX = { -2.5, 1.0 };
	glm::vec2 mY = { -1.0, 1.0 };

	for (int px = 0; px < getSurface()->getWidth(); px++)
	{
		for (int py = 0; py < getSurface()->getHeight(); py++)
		{
			double x0 = mX[0] + (((double)px / (double)getSurface()->getWidth()) * (mX[0] - mX[1]));
			double y0 = mY[0] + (((double)py / (double)getSurface()->getHeight()) * (mY[0] - mY[1]));
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

			float col = (float)iteration / 1000.0f;
			getSurface()->setColourAt(px, py, { col, col, col, 1.0f });
		}
	}
}