#include "Gunboss.h"
#include "BulletBoss1.h"


GunBoss::~GunBoss()
{
	delete _anim;
	delete explode;
}

void GunBoss::Init(D3DXVECTOR2 pos, int type, int kind)
{
	OEnemy::Init(pos, type, kind);
	SetVelocity(0, 0);
	HP = 10;
	Damage = 0;

	position.y = pos.y + 40;
	position.x = pos.x - 16;
	if (kind == 1)
	{
		position.x = pos.x + 8;
	}
	timeAttack = 0;
	reload = 0.5;

	if (bullet == NULL)
	{
		bullet = new BulletBoss1();
		bullet->Init(position, BulletBoss);
		bullet->State = Dying;
	}
}

void GunBoss::Controller()
{
	this->SetBound(16, 8);
	SetVelocity(0, 0);

	if (bullet->State == Dying && timeAttack > reload)
	{
		timeAttack = 0;
		bullet->AllowDraw = true;
		int speed = 10 + (std::rand() % (28 - 10 + 1));
		bullet->velocity = D3DXVECTOR2(-(float)speed / 10 * EnemySpeed, Gravity);
		bullet->position = position;
		bullet->positionStart = position;
		bullet->StartJump(0, 0, Gravity);
	}
	//bullet->Controller();
}

void GunBoss::Update(float gameTime, Keyboard* key)
{
	timeAttack += gameTime;

	if (HP < 0 && State != Object::Dying)
	{
		bullet->AllowDraw = false;
		State = Dying;
		StartExplode();
	}
	if (isExplode)
	{
		explode->UpdateAnimation(gameTime);
		explode->_anim->SetPosition(position);
		timeExplode += gameTime;
		if (timeExplode < 0.5)
		{
			explode->AllowDraw = true;
		}
		else explode->AllowDraw = false;
	}
	UpdateAnimation(gameTime);
}
