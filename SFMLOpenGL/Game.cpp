#include <Game.h>

bool flip = false;
int current = 1;

Game::Game() :
	window(VideoMode(800, 600), "OpenGL Cube"),
	m_rotations{ 0.0f,0.0f,0.0f },
	m_translations{ 0.0f,0.0f,0.0f },
	m_scale{ 100.0f }
{

}

Game::~Game() {}

// Vertices for one Triangle
float baseVertices[] = { -1.0f, -1.0f, 1.0f,
					1.0f, -1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,
					-1.0f, 1.0f, 1.0f,
					-1.0f, -1.0f, -1.0f,
					1.0f, -1.0f, -1.0f,
					1.0f, 1.0f, -1.0f,
					-1.0f, 1.0f, -1.0f };

float vertices[24];

// Colors for those vertices
float colors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 0.0f
};

// Index to be drawn
unsigned int vertex_index[] = { 1, 5, 6,
								6, 2, 1,
								4, 0, 3,
								3, 7, 4,
								3, 2, 6, 
								6, 7, 3,
								0, 4, 5,
								5, 1, 0,
								0, 1, 2,
								2, 3, 0,
								4, 7, 6,
								6, 5, 4 };


void Game::run()
{

	initialize();

	Event event;

	while (isRunning)
	{
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				isRunning = false;
			}
		}

		update();
		render();
	}

}

void Game::initialize()
{
	isRunning = true;

	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, window.getSize().x / window.getSize().y, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0.0f, 0.0f, -8.0f);
	glEnable(GL_CULL_FACE);

	for (int i = 0; i < 24; i++)
	{
		vertices[i] = baseVertices[i];
	}
}

void Game::update()
{
	elapsed = clock.getElapsedTime();

	if (elapsed.asSeconds() >= 1.0f / 60.0f)
	{
		clock.restart();

		// Get the key input
		// Reset transformations
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			m_rotations = { 0.0f, 0.0f, 0.0f };
			m_translations = { 0.0f, 0.0f, 0.0f };
			m_scale = 100.0f;
		}

		// Scale
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
			m_scale -= 10.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			m_scale += 10.0f;

		// Rotate
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
			m_rotations.x += 0.8f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
			m_rotations.y += 0.8f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			m_rotations.z += 0.8f;

		// Translation
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			m_translations.x += 0.1f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			m_translations.x -= 0.1f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			m_translations.y += 0.1f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			m_translations.y -= 0.1f;


		// Create an identity matrix
		cube::Matrix3f transformationMatrix{ 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };

		transformationMatrix = transformationMatrix * cube::Matrix3f::Scale(m_scale, m_scale);

		transformationMatrix = transformationMatrix * cube::Matrix3f::RotationX(m_rotations.x);
		transformationMatrix = transformationMatrix * cube::Matrix3f::RotationY(m_rotations.y);
		transformationMatrix = transformationMatrix * cube::Matrix3f::RotationZ(m_rotations.z);

		// Apply the transformations
		for (int i = 0; i < 24; i += 3)
		{
			// Rotation and scale
			cube::Vector3f vector{ baseVertices[i], baseVertices[i + 1], baseVertices[i + 2] };
			vector = transformationMatrix * vector;
			vertices[i + 2] = vector.z;

			// Apply the translations
			vector.z = 1.0f;
			vector = cube::Matrix3f::Translate(m_translations.x, m_translations.y) * vector;
			vertices[i] = vector.x;
			vertices[i + 1] = vector.y;
		}
	}
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);


	glVertexPointer(3, GL_FLOAT, 0, &vertices);
	glColorPointer(3, GL_FLOAT, 0, &colors);

	//glDrawArrays(GL_TRIANGLES, 0, 3);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, &vertex_index);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	window.display();

}

void Game::unload()
{
	cout << "Cleaning up" << endl;
}

