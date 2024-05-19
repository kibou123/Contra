#include "bridge.h"
#include "Player.h"
#include "ObjectManager.h"
#include "OWall.h"

Animation* Bridge::GetAnimationEnemy()
{
	Animation::DataAnimMap data;
	// Soldier
	data[OWall::Brigde+ Object::Standing] = { 92 , 94 };



	Animation* _anim = new Animation(EnemyXML, EnemyPNG);
	_anim->SetDataAnimation(data);
	return _anim;
}




void Bridge::Init(D3DXVECTOR2 pos, int _type, int kind)
{
	_walltype = (Walltype)_type;
	_kind = kind;
	velocity = D3DXVECTOR2(0, 0);
	type = _type;
	ObjectManager::GetInstance()->AddObjectMap(this);
}

void Bridge::SetBound(float width, float height)
{

}

void Bridge::BeforeUpdate(float gameTime, Keyboard* key)
{
}

D3DXVECTOR2 Bridge::OnCollision(Object* obj, D3DXVECTOR2 side)
{
	return D3DXVECTOR2();
}

void Bridge::Update(float gameTime, Keyboard* key)
{
	UpdateAnimation(gameTime);
}

void Bridge::OnCollision(Object* obj)
{
	switch (obj->Tag)
	{
	case Object::Player:

	}
}

void Bridge::Render(Viewport* viewport)
{
	_anim->Render(viewport);
}

void Bridge::Controller()
{
}


void Bridge::UpdateAnimation(float gameTime)
{
	_anim->NewAnimationByIndex(_walltype + this->State);
	_anim->SetPosition(position);
	_anim->Update(gameTime);
}

Bridge::~Bridge()
{
}