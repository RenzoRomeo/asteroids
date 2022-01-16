#pragma once

#include <glm/ext/vector_float2.hpp>

struct Asteroid
{
	Asteroid(glm::vec2 pos, float r, unsigned int times_broken = 0);
	glm::vec2 pos, vel;
	float radius;
	unsigned int times_broken;
	unsigned int id;

private:
	static unsigned int current_id;
};