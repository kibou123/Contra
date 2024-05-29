#pragma once
#include "RifleMan.h"
class BulletBoss1 : public OEnemy
{
public:
	BulletBoss1() : OEnemy() {};
	~BulletBoss1();

	void Init(D3DXVECTOR2 pos, int type = Enemytype::BulletBoss, int kind = 0);
	void Controller();
	D3DXVECTOR2 OnCollision(Object* obj, D3DXVECTOR2 side);
	void Update(float gameTime, Keyboard* key);
};

