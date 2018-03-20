#include "Scene.h"

Scene::Scene():
	loaded(false),
	unloaded(false)
{
}


Scene::~Scene()
{
}


bool Scene::Loaded()
{

	return loaded;

}


bool Scene::UnLoaded()
{

	return unloaded;

}
