#pragma once

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Game.h"

#include <stdlib.h>
#include <cmath>
#include <ctime>
#include <random>

Game::Game(int screen_width, int screen_height) : s_width(screen_width), s_height(screen_height)
{
	window.create(sf::VideoMode(s_width, s_height), "Asteroids");
	window.setFramerateLimit(60);
	running = true;
	std::random_device rd;
	std::default_random_engine randengine{ rd() };
	std::uniform_real_distribution<float> x_dist{ 0.0f, (float)s_width };
	std::uniform_real_distribution<float> y_dist{ 0.0f, (float)s_height };
	std::uniform_real_distribution<float> r_dist{ 20.0f, 30.0f };

	for (int i = 0; i < 15; i++) {
		float x = x_dist(randengine);
		float y = y_dist(randengine);
		float r = r_dist(randengine);
		if (x * x - y * y <= r * r) continue;
		asteroids.push_back(Asteroid::Asteroid(glm::vec2(x, y), r));
	}
}

void Game::shoot_projectile()
{
	projectiles.push_front(Projectile::Projectile(player.pos, player.dir));
}

void Game::move_projectiles()
{
	if (projectiles.size() <= 0) return;

	for (Projectile& p : projectiles) { p.pos += p.vel; }
	int w = s_width, h = s_height;

	projectiles.erase(std::remove_if(projectiles.begin(),
		projectiles.end(), [&, w, h](Projectile& p) { return p.pos.x <= 0 || p.pos.x > w || p.pos.y <= 0 || p.pos.y > h; }),
		projectiles.end());
}

void Game::draw_projectiles()
{
	for (Projectile& p : projectiles)
	{
		sf::CircleShape proj(5, 4);
		proj.setFillColor(sf::Color::Red);
		proj.setPosition(sf::Vector2f(p.pos.x, p.pos.y));
		window.draw(proj);
	}
}

void Game::move_asteroids()
{
	for (Asteroid& a : asteroids)
	{
		a.pos += a.vel;

		if (a.pos.y >= s_height)
			a.pos.y = 0;
		if (a.pos.y < 0)
			a.pos.y = s_height;

		if (a.pos.x >= s_width)
			a.pos.x = 0;
		if (a.pos.x < 0)
			a.pos.x = s_width;
	}
}

void Game::draw_asteroids()
{
	for (Asteroid const& a : asteroids)
	{
		sf::CircleShape circle(a.radius);
		circle.setOrigin(a.radius, a.radius);
		circle.setPosition(sf::Vector2f(a.pos.x, a.pos.y));
		circle.setOutlineThickness(5);
		circle.setOutlineColor(sf::Color::White);
		circle.setFillColor(sf::Color::Black);
		window.draw(circle);
	}
}

void Game::move_player()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		player.vel += player.acc * player.dir;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		player.dir = glm::rotate(player.dir, (float)glm::radians(-5.0));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		player.dir = glm::rotate(player.dir, (float)glm::radians(5.0));
	}

	if (glm::length(player.vel) > 0) { player.vel -= glm::normalize(player.vel) * player.frict; }
	if (glm::length(player.vel) < 0) { player.vel = glm::vec2(0, 0); }

	player.pos += player.vel;

	if (player.pos.x <= 0) { player.pos.x = s_width; }

	if (player.pos.x > s_width) { player.pos.x = 0; }

	if (player.pos.y <= 0) { player.pos.y = s_height; }

	if (player.pos.y > s_height) { player.pos.y = 0; }
}

void Game::draw_player()
{
	float angle = glm::degrees(glm::angle(glm::vec2(0.0, -1.0), player.dir));

	if (player.dir.x < 0) { angle = 360.0 - angle; }

	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(player.pos.x, player.pos.y)),
		sf::Vertex(sf::Vector2f(player.pos.x + player.dir.x * 30.0f, player.pos.y + player.dir.y * 30.0f))
	};

	window.draw(line, 2, sf::Lines);

	sf::CircleShape triangle(player.radius, 3);
	triangle.setOrigin(triangle.getRadius(), triangle.getRadius());
	triangle.setPosition(player.pos.x, player.pos.y);
	triangle.setRotation(angle);
	triangle.setFillColor(sf::Color::Green);
	triangle.setOutlineThickness(1);
	triangle.setOutlineColor(sf::Color::Red);
	window.draw(triangle);
}

void Game::check_player_collision()
{
	for (Asteroid const& a : asteroids)
	{
		if ((player.pos.x - a.pos.x) * (player.pos.x - a.pos.x) +
			(player.pos.y - a.pos.y) * (player.pos.y - a.pos.y) < (a.radius + player.radius) * (a.radius + player.radius))
		{
			running = false;
			break;
		}
	}
}

void Game::check_asteroid_destruction()
{
	for (Projectile& p : projectiles)
	{
		for (Asteroid& a : asteroids)
		{
			if ((p.pos.x - a.pos.x) * (p.pos.x - a.pos.x)
				+ (p.pos.y - a.pos.y) * (p.pos.y - a.pos.y) <= a.radius * a.radius)
			{
				split_asteroid(a.id);
				delete_projectile(p.id);
			}
		}
	}
}

void Game::split_asteroid(unsigned int id)
{
	for (int i = 0; i < asteroids.size(); i++)
	{
		Asteroid& a = asteroids[i];
		if (a.id == id)
		{
			if (a.times_broken + 1 != 3)
			{
				asteroids.push_back(Asteroid::Asteroid(a.pos, a.radius - 5.0, a.times_broken + 1));
				asteroids.push_back(Asteroid::Asteroid(a.pos, a.radius - 5.0, a.times_broken + 1));
			}
			asteroids.erase(asteroids.begin() + i);
			break;
		}
	}
}

void Game::delete_projectile(unsigned int id)
{
	for (int i = 0; i < projectiles.size(); i++)
	{
		Projectile& p = projectiles[i];
		if (p.id == id)
		{
			projectiles.erase(projectiles.begin() + i);
			break;
		}
	}
}

void Game::handle_events()
{
	sf::Event e;
	while (window.pollEvent(e))
		switch (e.type)
		{
		case sf::Event::Closed:
			running = false;
			window.close();
			break;
		case sf::Event::KeyPressed:
			if (e.key.code == sf::Keyboard::Space) { shoot_projectile(); }
			break;
		}
}

void Game::run_loop()
{
	while (running)
	{
		window.clear();

		handle_events();

		move_asteroids();
		draw_asteroids();

		move_player();
		draw_player();

		move_projectiles();
		draw_projectiles();

		check_asteroid_destruction();

		check_player_collision();

		window.display();
	}
}