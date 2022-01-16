#include "Projectile.h"

unsigned int Projectile::current_id = 0;

Projectile::Projectile(glm::vec2 pos, glm::vec2 dir) : pos(pos), vel(dir * 5.0f), id(current_id++)
{

}

Projectile& Projectile::operator=(Projectile&& p) noexcept
{
	return *this;
}

Projectile& Projectile::operator=(Projectile& p) noexcept
{
	return *this;
}

Projectile::Projectile(const Projectile&p) : pos(p.pos), vel(p.vel), id(p.id)
{

};