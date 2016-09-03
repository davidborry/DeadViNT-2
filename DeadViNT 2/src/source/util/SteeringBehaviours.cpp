#include "../../headers/util/SteeringBehaviours.hpp"
#include "../../headers/entities/Entity.hpp"
#include "../../headers/util/Utility.hpp"

sf::Vector2f seek(Entity& seeker, Entity& target, float speed){
	sf::Vector2f desiredVelocity = unitVector(target.getWorldPosition() - seeker.getWorldPosition()) * speed;
	sf::Vector2f steering = desiredVelocity - seeker.getVelocity();

	return steering;
}