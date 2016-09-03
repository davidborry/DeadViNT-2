#ifndef STEERINGBEHAVIOURS_HPP
#define STEERINGBEHAVIOURS_HPP

#include "SFML\System\Vector2.hpp"

class Entity;



sf::Vector2f seek(Entity& seeker, Entity& target, float speed = 100);

#endif