#include "Cannon.h"

void Cannon::Init(D3DXVECTOR2 pos, int type, int kind)
{
	OEnemy::Init(pos, type, kind);
	velocity = D3DXVECTOR2(0, Gravity);
	SetState(Object::Standing);
	this->SetBound(32, 32);
}
