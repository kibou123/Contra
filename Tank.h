#pragma once
#include "OEnemy.h"
class Tank : public OEnemy
{
public:
	Tank() : OEnemy() {};
	~Tank();

	void Init(D3DXVECTOR2 pos, int type = Enemytype::Tank, int kind = 0);
};

