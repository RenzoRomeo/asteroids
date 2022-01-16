#include <glm/ext/vector_float2.hpp>

struct Projectile
{
	Projectile(glm::vec2 pos, glm::vec2 dir);
	Projectile(const Projectile&);
	Projectile& operator=(Projectile& p) noexcept;
	Projectile& operator=(Projectile&& p) noexcept;
	glm::vec2 pos, vel;
	unsigned int id;

private:
	static unsigned int current_id;
	const float speed = 0.5f;
};