#include "Player.h"

Player::Player(glm::vec2 pos, float r) : acc(0.1), frict(0.01f), radius(r), pos(pos)
{
	vel = glm::vec2(0, 0);
	dir = glm::normalize(glm::vec2(1, 0));
}