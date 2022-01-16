#include "Asteroid.h"

#include <random>
#include <cmath>

unsigned int Asteroid::current_id = 0;

Asteroid::Asteroid(glm::vec2 pos, float r, unsigned int times_broken) : radius(r), id(current_id++), times_broken(times_broken), pos(pos)
{
	std::random_device rd;
	std::default_random_engine randengine{ rd() };
	std::uniform_real_distribution<float> mag_dist{ 0.25f, 1.0f };
	std::uniform_real_distribution<float> angle_dist{ 0.0f, 6.28f };

	float v_mag = mag_dist(randengine);
	float v_angle = angle_dist(randengine);
	vel = glm::vec2(v_mag * cos(v_angle), v_mag * sin(v_angle));
}