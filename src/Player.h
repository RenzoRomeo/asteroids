#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/geometric.hpp>

struct Player
{
	Player(glm::vec2 pos, float r);

	glm::vec2 pos, vel, dir;

	float acc;
	float frict;
	float radius;
};