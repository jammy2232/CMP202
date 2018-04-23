#include "GameObject.h"


GameObject::GameObject()
{

	active_ = true;

}


GameObject::~GameObject()
{
}


void GameObject::SetScreenPosition(sf::Vector2f position)
{

	// Lock the access to be updated
	sprite_.screen_position = position;

}


void GameObject::SetSpriteId(int id)
{

	// Lock the access to be updated
	sprite_.id = id;

}
