#pragma once
#include "OEnemy.h"
class Cannon : public OEnemy
{
public:
	Cannon() : OEnemy() {};
	~Cannon();

	void Init(D3DXVECTOR2 pos, int type = Enemytype::Cannon, int kind = 0);
};

