#include "FractalsScene.h"

#include "Scene.h"
#include "Surface.h"

//Define FractalEntry
//Struct for storing different fractal information for use by FractalsScene
struct FractalEntry {
	std::string m_name;
	Texture* m_texture;

	inline FractalEntry(std::string name, int width, int height) : m_name(name) {
		m_texture = new Texture(width, height);
	}

	inline ~FractalEntry() {
		delete m_texture;
	}
};

#define MANDLEBROT_MAX_ITERATIONS 1000
#define SERIPINSKI_MAX_DEPTH 5

void GenerateMandlebrot(FractalEntry* entry) {
	glm::vec2 mX = { -2.5, 1.0 };
	glm::vec2 mY = { -1.0, 1.0 };

	int width = entry->m_texture->getWidth();
	int height = entry->m_texture->getHeight();

	for (int px = 0; px < width; px++)
	{
		for (int py = 0; py < height; py++)
		{
			double x0 = mX[0] + (((double)px / (double)width) * (mX[1] - mX[0]));
			double y0 = mY[0] + (((double)py / (double)height) * (mY[1] - mY[0]));
			double x = 0.0f;
			double y = 0.0f;

			int iteration = 0;

			while (x * x + y * y < 2 * 2 && iteration < MANDLEBROT_MAX_ITERATIONS)
			{
				double tempX = x * x - y * y + x0;
				y = 2 * x * y + y0;
				x = tempX;
				iteration++;
			}

			unsigned char col = 255 - ((unsigned char)((float)iteration / 1000.0f) * 255);
			entry->m_texture->setPixelAt(px, py, col, col, col, 255);
		}
	}
}

void SeripinskiHalf(Context& context, glm::vec2 a, glm::vec2 b, glm::vec2 c, int depth) {
	//Once the max depth has been reached, return.
	if (depth >= SERIPINSKI_MAX_DEPTH)
		return;

	//Find the half points for the current depth.
	glm::vec2 halfA = a + ((b - a) * 0.5f);
	glm::vec2 halfB = a + ((c - a) * 0.5f);
	glm::vec2 halfC = b + ((c - b) * 0.5f);

	//Draw the triangles and fill in the center so it leaves what would look like a blank space.
	Rasterizer::DrawTriangle(context, { a.x, a.y, 0, 0, 0, 0, 1 }, {halfA.x, halfA.y, 0, 0, 0, 0, 1 }, {halfB.x, halfB.y, 0, 0, 0, 0, 1 });
	Rasterizer::DrawTriangle(context, { halfA.x, halfA.y, 0, 0, 0, 0, 1 }, { halfC.x, halfC.y, 0, 0, 0, 0, 1 }, { b.x, b.y, 0, 0, 0, 0, 1 });
	Rasterizer::DrawTriangle(context, { halfB.x, halfB.y, 0, 0, 0, 0, 1 }, { halfC.x, halfC.y, 0, 0, 0, 0, 1 }, { c.x, c.y, 0, 0, 0, 0, 1 });
	Rasterizer::DrawTriangle(context, { halfA.x, halfA.y, 0, 1, 1, 1, 1 }, { halfB.x, halfB.y, 0, 1, 1, 1, 1 }, { halfC.x, halfC.y, 0, 1, 1, 1, 1 });

	depth++;

	//Recursive Part.
	//Here I take the half points and generate the next level down in each direction.
	SeripinskiHalf(context, a, halfA, halfB, depth);
	SeripinskiHalf(context, c, halfC, halfB, depth);
	SeripinskiHalf(context, halfA, halfC, b, depth);
}

void GenerateSeripinski(FractalEntry* fractal) {
	Surface surf = Surface(Viewport(0, 0, fractal->m_texture->width, fractal->m_texture->height));
	Context cont = Context(&surf);

	SeripinskiHalf(cont, { (float)fractal->m_texture->width * 0.5f, 0 }, {0,(float)fractal->m_texture->height}, { (float)fractal->m_texture->width,(float)fractal->m_texture->height }, 1);

	surf.writeToTexture(fractal->m_texture);
}

FractalsScene::FractalsScene() : m_context(getContext()) {
	setTitle("Fractals");

	//Store the potential width and height for each fractal.
	int fractalWidth = m_context.m_surface->getWidth() * 0.75;
	int fractalHeight = (m_context.m_surface->getHeight() - getStartY()) * 0.75;

	//Create fractal entries.
	FractalEntry* mandlebrot = new FractalEntry("Mandlebrot", fractalWidth, fractalHeight);
	FractalEntry* seripinski = new FractalEntry("Seripinski", fractalHeight, fractalHeight);

	//Generate the fractal for each type.
	GenerateMandlebrot(mandlebrot);
	GenerateSeripinski(seripinski);

	//Add to fractal list and set index to 0
	m_fractals.push_back(mandlebrot);
	m_fractals.push_back(seripinski);
	m_fractalIndex = 0;

	//This is just some arbritary placement for the buttons.
	float h = m_context.getHeight() - getSharedAssets().openSansFont->getLineHeight() * 2;

	//Arbritary Positioning
	m_prev = Button(getSharedAssets().backIcon, (int)(m_context.getWidth() * 0.30f), h, 0);
	m_next = Button(getSharedAssets().forwardIcon, (int)(m_context.getWidth() * 0.675f), h, 0);
	//Inital position of the text
	m_fractalTitlePos = { (m_context.getWidth() - getSharedAssets().openSansFont->getWidth(m_fractals[m_fractalIndex]->m_name)) / 2
		, m_context.getHeight() - getSharedAssets().openSansFont->getLineHeight() * 2 };
}


FractalsScene::~FractalsScene() {
	for (FractalEntry* fractal : m_fractals)
		delete fractal;
}

void FractalsScene::update() {
	m_prev.update();
	m_next.update();

	if (m_prev.isClicked() || m_next.isClicked()) {
		//Change the current fractal index based on the buttons clicked
		//and check for the bounds.
		if (m_prev.isClicked())
			m_fractalIndex--;

		if (m_next.isClicked())
			m_fractalIndex++;

		if (m_fractalIndex < 0)
			m_fractalIndex = m_fractals.size();

		if (m_fractalIndex >= m_fractals.size())
			m_fractalIndex = 0;

		//Update text so it is always in the center.
		m_fractalTitlePos = { (m_context.getWidth() - getSharedAssets().openSansFont->getWidth(m_fractals[m_fractalIndex]->m_name)) / 2
			, m_context.getHeight() - getSharedAssets().openSansFont->getLineHeight() * 2 };
	}
}

void FractalsScene::draw() {
	//Get the current FractalEntry.
	FractalEntry* entry = m_fractals[m_fractalIndex];

	//Draw fractal.
	Rasterizer::DrawImage(m_context, entry->m_texture
		, (m_context.getWidth() - entry->m_texture->getWidth()) / 2
		, ((m_context.getHeight() - getStartY()) - entry->m_texture->getHeight()) / 2);

	//Draw the fractal name.
	getSharedAssets().openSansFont->drawText(m_context, entry->m_name
		, m_fractalTitlePos.x, m_fractalTitlePos.y
		, 1.0f, Filter::Point, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	//Draw buttons
	m_prev.draw(getContext());
	m_next.draw(getContext());
}