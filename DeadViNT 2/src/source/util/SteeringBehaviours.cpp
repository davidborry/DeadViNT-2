#include "../../headers/util/SteeringBehaviours.hpp"
#include "../../headers/entities/Entity.hpp"
#include "../../headers/util/Utility.hpp"

sf::Vector2f seek(Entity& seeker, sf::Vector2f target, float speed){

		//printf("%f,%f\n", seeker.getVelocity().x, seeker.getVelocity().y);

	sf::Vector2f desiredVelocity = unitVector(target - seeker.getWorldPosition()) * speed;
	sf::Vector2f steering = desiredVelocity - seeker.getVelocity();

	return steering;
}

float distance2(sf::Vector2f p1, sf::Vector2f p2){
	return std::sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}