#include "BulletBoss1.h"
#include "ObjectManager.h"

BulletBoss1::~BulletBoss1()
{
	delete _anim;
}

void BulletBoss1::Init(D3DXVECTOR2 pos, int type, int kind)
{
	OEnemy::Init(pos, type, kind);
	this->SetBound(8, 8);
	maxBullet = 1;
	HP = 10;
	Damage = 10;
	delete _anim;
	_anim = OItem::GetAnimation();
	ObjectManager::GetInstance()->AddObjectMap(this);
}

void BulletBoss1::Controller()
{
	OEnemy::Controller();
	SetBound(8, 8);
}

D3DXVECTOR2 BulletBoss1::OnCollision(Object* obj, D3DXVECTOR2 side)
{
	switch (obj->Tag)
	{
	case Object::Wall:
		if (abs(positionStart.y - position.y) > 5)
		{
			HP = -1;
		}
		return side;
	default:
		return D3DXVECTOR2(Collision::NONE, Collision::NONE);
	}
}

void BulletBoss1::Update(float gameTime, Keyboard* key)
{
	if (HP < 0 && State != Object::Dying)
	{
		State = Dying;
		StartExplode();
		explode->positionStart = position;
		HP = 1;
	}

	if (isExplode)
	{
		explode->UpdateAnimation(gameTime);
		explode->_anim->SetPosition(explode->positionStart);
		timeExplode += gameTime;
		if (timeExplode < 0.5)
		{
			explode->AllowDraw = true;
		}
		else explode->AllowDraw = false;
	}
	_anim->NewAnimationByIndex(_enemyType + Jumping);
	_anim->SetPosition(position);
	_anim->Update(gameTime);

	Object::Update(gameTime, key);
}