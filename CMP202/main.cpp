// CMP 208 Data and Algorithms Coursework
// Written by James Mcadam (0702207)
// Main - Handles game loop and Window

// Includes for SFML
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

// Application includes
#include "GameManager.h"

// Std thread includes
#include <thread>
#include <memory>

// data logger 
#include "DataLogger.h"


// Main Thread handling game system updates
int main()
{

	// Create a new system for data logging
	DataLogger logger("GameData.csv");

	// Seed the random number generator
	srand(time(NULL));

	// get the current desktop
	sf::VideoMode currentResolution = sf::VideoMode::getDesktopMode();

	// Create the main game window with HD resolution
	sf::RenderWindow window(sf::VideoMode(currentResolution.width, currentResolution.height), "CMP202 Coursework", sf::Style::Fullscreen);

	// Set the opengl instance to false to allow multithreaded rendering
	window.setActive(false);

	// Initisaise the game application
	GameManager* gameApplication = GameManager::Create();;

	// Start the rendering thread to render the game manager
  	std::thread rendering(&GameManager::Render, gameApplication, &window);
 	rendering.detach();

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

		// Update the game application and check of exit begin triggered
		if (!gameApplication->Update(deltaTime))
		{

			// Cleanup the application
			gameApplication->CleanUp();

			// Return success if the game is exited
			return EXIT_SUCCESS;

		}

	}

	// Cleanup the application
	gameApplication->CleanUp();

	// Return success if the wondow is closed
	return EXIT_SUCCESS;

}

