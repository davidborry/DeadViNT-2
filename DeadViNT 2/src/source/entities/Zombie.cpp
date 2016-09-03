#include "../../headers/entities/Zombie.hpp"

Zombie::Zombie(const TextureHolder& textures) :
Entity(20),
mSprite(textures.get(Resources::Textures::Zombie))
{

}

void Zombie::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(mSprite, states);
}

sf::FloatRect Zombie::getBoundingRect() const{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

unsigned int Zombie::getCategory() const{
	return Category::Zombie;
}