// CMP 208 Data and Algorithms Coursework
// Written by James Mcadam (0702207)
// Main - Handles game loop and Window

// Includes for SFML
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

// Application includes
#include "BattleScene.h"

int main()
{

	// get the current desktop
	sf::VideoMode currentResolution = sf::VideoMode::getDesktopMode();

	// Create the main game window with HD resolution
	sf::RenderWindow window(sf::VideoMode(currentResolution.width, currentResolution.height), "CMP202 Coursework", sf::Style::Fullscreen);

	// Render a loading screen **********

	// Load the texture
	sf::Texture loadingTexture;
	if (!loadingTexture.loadFromFile("Textures/LoadingScreen.png"))
	{
		return EXIT_FAILURE;
	}

	// Create the sprite and scale it to the screen resolution 
	sf::Sprite loadingSprite;
	loadingSprite.setTexture(loadingTexture);
	loadingSprite.setScale(sf::Vector2f(currentResolution.width / loadingTexture.getSize().x, currentResolution.height / loadingTexture.getSize().y));

	// Place this on the scrren
	window.clear();
	window.draw(loadingSprite);
	window.display();

	// End Render a loading screen **********

	// Application has one scene
	BattleScene scene;

	// Load all the information to the scene
	if (!scene.Init())
	{
		return EXIT_FAILURE;
	}

	// This clock object is required to determine the delta time
	sf::Clock clock;
	float deltaTime;

	// Event Handling while the window is open
	while (window.isOpen())
	{

		// Event type
		sf::Event event;

		// While there are unprocessed events 
		while (window.pollEvent(event))
		{

			switch (event.type)
			{
			case sf::Event::KeyPressed:

				// Check if escape is pressed to close the window
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
				break;

			case sf::Event::Closed:

				// Check if and external event caused it to exit
				window.close();
				break;

			}

		}

		// Calculate the delta time (time to render each frame)
		deltaTime = clock.restart().asSeconds();

		// Update wrap the scene
		scene.HandleInput();
		scene.Update(deltaTime);

		// Clear the current window
		window.clear();

		// Pass the scene access to the screen
		scene.Render(window);
		scene.RenderUI(window);
		
		// redering the scene 
		window.display();

	}

	// Cleanup the application
	scene.CleanUp();

	// Return success
	return EXIT_SUCCESS;

}

