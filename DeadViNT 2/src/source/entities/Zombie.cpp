#include "../../headers/entities/Zombie.hpp"
#include "../../headers/entities/Human.hpp"
#include "../../headers/util/SteeringBehaviours.hpp"
#include "../../headers/util/Utility.hpp"

Zombie::Zombie(const TextureHolder& textures) :
Entity(20),
mSprite(textures.get(Resources::Textures::Zombie)),
mTarget(nullptr)
{
	rotate(90);
	centerOrigin(mSprite);


}

void Zombie::updateCurrent(sf::Time dt, CommandQueue& commands){
	seekTarget();
	float angle = toDegree( std::atan2(getVelocity().x , -getVelocity().y));
	//printf("%f, %f\n", angle, getRotation());

	setRotation(angle);

Entity::updateCurrent(dt,commands);
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

void Zombie::setTarget(Human* human){
	mTarget = human;
}

Human* Zombie::getTarget(){
	return mTarget;
}

void Zombie::seekTarget(){
	mVelocity += seek(*this,*mTarget,50);
}