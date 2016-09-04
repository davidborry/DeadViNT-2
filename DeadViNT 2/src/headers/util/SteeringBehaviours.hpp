#ifndef STEERINGBEHAVIOURS_HPP
#define STEERINGBEHAVIOURS_HPP

#include "SFML\System\Vector2.hpp"
#include "PathFindingGrid.hpp"

class Entity;



sf::Vector2f seek(Entity& seeker, sf::Vector2f target, float speed = 100);
float distance2(sf::Vector2f p1, sf::Vector2f p2);

#endif