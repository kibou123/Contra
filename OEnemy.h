#include "Object.h"
#include "GameDefine.h"
#include "Collision.h"

using namespace Define;

class OEnemy : public Object
{
public:
	float timeAttack, reload;
	bool isExplode;
	float timeExplode;
	Object* explode;

	enum Enemytype
	{
		Soldier = 100,
		Rifleman =  200,
		Tank = 300,
		Cannon = 400,
		Boss = 500,
		Gunboss = 600,
		BulletBoss = 700,
	} _enemyType = Enemytype::Soldier;

	OEnemy();
	~OEnemy();

	void Init(D3DXVECTOR2 pos, int type = Enemytype::Soldier, int kind = 0);
	void BeforeUpdate(float gameTime, Keyboard* key);
	void Update(float gameTime, Keyboard* key);
	void UpdateAnimation(float gameTime);
	void Controller();
	D3DXVECTOR2 OnCollision(Object* obj, D3DXVECTOR2 side);
	void OnCollision(Object* obj);
	void SetBound(float width, float height);
	void Render(Viewport* viewport);
	void StartExplode();
	static Animation* GetAnimationEnemy();

	int GetIndexGun()
	{
		return Object::GetArrowIndexByAngle(AngleGun);
	}
};
