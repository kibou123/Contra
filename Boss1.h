#pragma once
#include "Gunboss.h"

class Boss1 : public OEnemy
{
public:
	GunBoss * gun1;
	GunBoss* gun2;
	Boss1() : OEnemy() {};
	~Boss1();

	void Init(D3DXVECTOR2 pos, int type = Enemytype::Boss, int kind = 0);
	void Controller();
	void BeforeUpdate(float gameTime, Keyboard* key);
	void Update(float gameTime, Keyboard* key);
	void Render(Viewport* viewport);
	void SetBound(float width, float height);

};