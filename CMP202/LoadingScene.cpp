#include "LoadingScene.h"


LoadingScene::LoadingScene()
{

	// Create new objects for storing data
	loadingBackgroundTexture_ = new sf::Texture();
	loadingBackgroundSprite_ = new sf::Sprite();
	loadingIconTexture_ = new sf::Texture();
	loadingIconSprite_ = new sf::Sprite();

}


LoadingScene::~LoadingScene()
{

	// Loading threads need to cleanup themselves
	CleanUp();

}


bool LoadingScene::Init()
{

	// Load in the textures
	if  (
		!loadingBackgroundTexture_->loadFromFile("Textures/LoadingIcon.png") ||
		!loadingIconTexture_->loadFromFile("Textures/LoadingBackground.png")
		)
	{
		// flag as a failed to load
		return false;
	}

	// get the current desktop
	sf::VideoMode currentResolution = sf::VideoMode::getDesktopMode();

	// Create the sprite
	loadingBackgroundSprite_->setTexture(*loadingBackgroundTexture_);
	loadingIconSprite_->setTexture(*loadingIconTexture_);

	// scale it to the screen resolution converting the ints to floats for accurate scaling (Distortion expected as aspect not maintained)
	float width = (float)currentResolution.width / (float)loadingBackgroundTexture_->getSize().x;
	float height = (float)currentResolution.height / (float)loadingBackgroundTexture_->getSize().y;
	loadingBackgroundSprite_->setScale(sf::Vector2f(width, height));
	loadingIconSprite_->setScale(sf::Vector2f(width, height));

	// Return is successfull
	return true;

}


void LoadingScene::CleanUp()
{

	// Delete all the textures and sprites
	delete loadingBackgroundTexture_;
	delete loadingBackgroundSprite_;
	delete loadingIconTexture_;
	delete loadingIconSprite_;
	loadingBackgroundTexture_ = nullptr;
	loadingBackgroundSprite_ = nullptr;
	loadingIconTexture_ = nullptr;
	loadingIconSprite_ = nullptr;

}


void LoadingScene::HandleInput(float delta_time)
{

	// Not Required

}


void LoadingScene::Update(float delta_time)
{

	// update the internal timer
	timer += delta_time;

	// modify the alpha with a fixed wavelength
	float frequency = 1.0f; // 2Hz
	int alphaMod = (int)std::abs(255 * std::cos(1.0f * 2.0f * 3.14f * timer));

	// Lock the mutex for editing the sprite
	std::unique_lock<std::mutex> lock(LoadingIconMutex);

	// Rotate the loading wheel sprite at 30 degrees per second
	loadingIconSprite_->setColor(sf::Color(255, 255, 255, alphaMod));

}


void LoadingScene::Render(sf::RenderWindow & window)
{

	// ** Must be thread safe

	// Render the Loading Screen
	window.draw(*loadingBackgroundSprite_);

	// Lock the mutex for editing the sprite
	std::unique_lock<std::mutex> lock(LoadingIconMutex);
	window.draw(*loadingIconSprite_);

}


void LoadingScene::RenderUI(sf::RenderWindow & window)
{
	
	// Not Required

}

