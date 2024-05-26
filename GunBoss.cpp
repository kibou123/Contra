#include "Gunboss.h"


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
}

void GunBoss::Controller()
{
	this->SetBound(16, 8);
	SetVelocity(0, 0);
}

void GunBoss::Update(float gameTime, Keyboard* key)
{
	if (HP < 0 && State != Object::Dying)
	{
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
