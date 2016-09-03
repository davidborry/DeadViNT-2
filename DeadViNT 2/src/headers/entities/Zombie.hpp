#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include "Entity.hpp"

class Zombie : public Entity{
public:
	Zombie(const TextureHolder& textures);

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;

private:
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Sprite mSprite;
};

#endif