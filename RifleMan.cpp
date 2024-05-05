#include "RifleMan.h"

void RifleMan::Init(D3DXVECTOR2 pos, int type, int kind)
{
	OEnemy::Init(pos, type, kind);
	velocity = D3DXVECTOR2(0, Gravity);
	SetState(Object::Standing);
	this->SetBound(15, 32);
}
