#pragma once
#include "Cannon.h"

class GunBoss : public OEnemy
{
public:
	Object* bullet = NULL;
public:
	GunBoss() : OEnemy() {};
	~GunBoss();

	void Init(D3DXVECTOR2 pos, int type = Enemytype::Gunboss, int kind = 0);
	void Update(float gameTime, Keyboard* key);
	void Controller();
};