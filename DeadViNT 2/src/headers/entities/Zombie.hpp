#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include "Entity.hpp"
#include "../scene/SpriteNode.hpp"

class Human;



class Zombie : public Entity{
public:
	Zombie(const TextureHolder& textures);

	void setTarget(Human* human);
	Human* getTarget();

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;

	void seekTarget();

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	sf::Sprite mSprite;
	Human* mTarget;

};



#endif