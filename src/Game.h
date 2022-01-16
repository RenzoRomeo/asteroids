#pragma once

#include "Asteroid.h"
#include "Player.h"
#include "Projectile.h"

#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>

class Game
{
public:
	Game(int screen_width, int screen_height);
	void run_loop();
	int s_width, s_height;

private:
	bool running;
	
	sf::RenderWindow window;
	std::vector<Asteroid> asteroids;
	Player player = Player::Player(glm::vec2(s_width / 2, s_height / 2), 15);
	std::deque<Projectile> projectiles;

	void move_asteroids();
	void move_player();

	void draw_asteroids();
	void draw_player();

	void handle_events();

	void check_player_collision();

	void check_asteroid_destruction();
	void split_asteroid(unsigned int id);
	void delete_projectile(unsigned int id);

	void shoot_projectile();
	void move_projectiles();
	void draw_projectiles();
};