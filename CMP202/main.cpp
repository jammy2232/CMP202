// CMP 208 Data and Algorithms Coursework
// Written by James Mcadam (0702207)
// Main - Handles game loop and Window

// Includes for SFML
#include <SFML\Window.hpp>

int main()
{

	// get the current desktop
	sf::VideoMode currentResolution = sf::VideoMode::getDesktopMode();

	// Create the main game window with HD resolution
	sf::Window window(sf::VideoMode(currentResolution.width, currentResolution.height), "CMP202 Coursework", sf::Style::Fullscreen);

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

	}


}

