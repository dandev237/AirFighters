#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "SceneNode.h"

class SpriteNode : public SceneNode
{
public:
	explicit SpriteNode(const sf::Texture& texture);
	SpriteNode(const sf::Texture& texture, const sf::IntRect& rectangle);

private:
	/**
		Draws the sprite node to the game window.
	*/
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Sprite mSprite;
};
