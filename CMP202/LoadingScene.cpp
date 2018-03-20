#include "LoadingScene.h"


LoadingScene::LoadingScene()
{

	// loading flag 
	loading = true;

	// Create new objects for storing data
	loadingTexture = new sf::Texture();
	loadingSprite = new sf::Sprite();
	loadingWheelTexture = new sf::Texture();
	loadingWheelSprite = new sf::Sprite();

}


LoadingScene::~LoadingScene()
{

	// Loading threads need to cleanup themselves
	CleanUp();

}


bool LoadingScene::Init()
{

	// Load the texture
	if (!loadingTexture->loadFromFile("Textures/LoadingScreen.png")	||
		!loadingWheelTexture->loadFromFile("Textures/LoadingWheel.png"))
	{
		return false;
	}

	// get the current desktop
	sf::VideoMode currentResolution = sf::VideoMode::getDesktopMode();

	// Create the sprite
	loadingSprite->setTexture(*loadingTexture);
	loadingWheelSprite->setTexture(*loadingWheelTexture);

	// scale it to the screen resolution converting the ints to floats for accurate scaling (Distortion expected as aspect not maintained)
	float width = (float)currentResolution.width / (float)loadingTexture->getSize().x;
	float height = (float)currentResolution.height / (float)loadingTexture->getSize().y;
	loadingSprite->setScale(sf::Vector2f(width, height));
	
	// set the origin to the middle
	float whellsizex = (float)loadingWheelTexture->getSize().x;
	float whellsizey = (float)loadingWheelTexture->getSize().y;
	loadingWheelSprite->setOrigin(whellsizex / 2.0f, whellsizey / 2.0f);

	// Place the wheel middle and 3/4 down to the bottom
	loadingWheelSprite->setPosition((float)currentResolution.width / 2.0f, (float)currentResolution.height * 3.0f / 4.0f);

	// Double the size
	loadingWheelSprite->setScale(sf::Vector2f(3.0 * width, 3.0 * height));

	// Return is successfull
	return true;

}


void LoadingScene::CleanUp()
{

	delete loadingTexture;
	loadingTexture = nullptr;

	delete loadingSprite;
	loadingSprite = nullptr;

	newScene_ = nullptr;
	oldScene_ = nullptr;

	delete loadThread;
	loadThread = nullptr;

	delete unloadThread;
	unloadThread = nullptr;

}


void LoadingScene::SceneToLoad(Scene * newScene, Scene* oldScene)
{

	// set a reference to the scene to be loaded
	newScene_ = newScene;
	oldScene_ = oldScene;

}

Scene * LoadingScene::transition()
{

	// Return the scene once loaded
	if (newScene_->Loaded())
	{
		if (oldScene_)
		{
			if (oldScene_->UnLoaded())
			{
				delete oldScene_;
				return newScene_;
			}
		}
		else
		{
			return newScene_;
		}

	}

	// Not transition
	return nullptr;

}


void LoadingScene::HandleInput()
{

	// Not Required

}


void LoadingScene::Update(float delta_time)
{

	// Create a thread to load the next Scene 
	if (!unloadThread && oldScene_ && !oldScene_->UnLoaded())
	{
		unloadThread = new std::thread(&Scene::CleanUp, oldScene_);
		unloadThread->detach();
	}
	else if (!loadThread && !newScene_->Loaded())
	{
		loadThread = new std::thread(&Scene::Init, newScene_);
		loadThread->detach();
	}

	// Rotate the loading wheel sprite at 30 degrees per second
	loadingWheelSprite->rotate(30.0f * delta_time);

}


void LoadingScene::Render(sf::RenderWindow & window)
{

	// Render the Loading Screen
	window.draw(*loadingSprite);
	window.draw(*loadingWheelSprite);

}


void LoadingScene::RenderUI(sf::RenderWindow & window)
{
	
	// Not Required

}

