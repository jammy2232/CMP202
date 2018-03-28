// CMP 208 Data and Algorithms Coursework
// Written by James Mcadam (0702207)
// Main - Handles game loop and Window

// Includes for SFML
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

// Application includes
#include "LoadingScene.h"
#include "BattleScene.h"
#include "Moving.h"
#include "LookAround.h"
#include "SearchAndDestoy.h"
#include "Fighting.h"


int main()
{

	// Seed the random number generator
	srand(time(NULL));

	// get the current desktop
	sf::VideoMode currentResolution = sf::VideoMode::getDesktopMode();

	// Create the main game window with HD resolution
	sf::RenderWindow window(sf::VideoMode(currentResolution.width, currentResolution.height), "CMP202 Coursework", sf::Style::Fullscreen);

	// Application has start up
	Scene* current;
	LoadingScene* loading = new LoadingScene();
	BattleScene* battle = new BattleScene();

	// Load all the information to the scene
	if (!loading->Init())
	{
		return EXIT_FAILURE;
	}

	// Start loading the first scene
	loading->SceneToLoad(battle, nullptr);

	// Assign the loading scene as the current scene 
	current = (Scene*)loading;

	// Reference to the Ai manager system
	AiState::SetSceneToControl(battle);

	// Create copies of all the state objects
	Moving::stateInstance = new Moving();
	LookAround::stateInstance = new LookAround();
	SearchAndDestoy::stateInstance = new SearchAndDestoy();
	Fighting::stateInstance = new Fighting();

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

		// Check if a scene transition has been requested
		if (current->transition())
		{

			// Get the next scene
			Scene* nextScene = current->transition();

			// if the current scene is a loading scene (Loading scenes will handle deleting the old scene)
			if (current->IsLoadingScene())
			{
				delete current;
				current = nullptr;
			}

			// update the scene pointer
			current = nextScene;

		}

		// Update wrap the scene
		current->HandleInput(deltaTime);
		current->Update(deltaTime);

		// Clear the current window
		window.clear(sf::Color::Black);

		// Pass the scene access to the screen
		current->Render(window);
		current->RenderUI(window);
		
		// redering the scene 
		window.display();

	}

	// Cleanup the application
	current->CleanUp();

	// Return success
	return EXIT_SUCCESS;

}

