#include "OBullet.h"
#include "Player.h"

Animation* OBullet::GetAnimationBullet()
{
	//Tạo Animation
	Animation::DataAnimMap data;
	// Soldier
	data[OBullet::NormalBullet + Object::Running] = { 14 , 14 };
	data[OBullet::NormalBullet + Object::Dying] = { 15 , 16 };


	Animation* _anim = new Animation(ItemXML, ItemPNG);
	_anim->SetDataAnimation(data);
	return _anim;
}

OBullet::OBullet()
{
	_anim = GetAnimationBullet();
	Tag = Object::Bullet;
}

OBullet::~OBullet()
{
	delete _anim;
}

void OBullet::Init(int angle, int acceleration, D3DXVECTOR2 pos, int _type, int kind)
{
	Init(angle, acceleration, _type, kind);
	Fire(pos);
}

void OBullet::Init(int angle, int acceleration, int _type, int kind)
{
	Reset();
	_bulletType = (Bullettype)_type;
	_kind = kind;
	velocity = D3DXVECTOR2(BulletSpeed * acceleration, 0);
	type = _type;
	Angle = acceleration * ((kind * 15) + angle) + 0.01;
}

void OBullet::Reset()
{
	IsFire = false;
	AllowDraw = false;
	SetState(Object::Dying);
	this->SetBound(0, 0);
	timeDead = 0;
	HP = 1;
	Damage = 0;
}

void OBullet::Fire(D3DXVECTOR2 pos)
{
	IsFire = true;
	AllowDraw = true;
	SetState(Object::Running);
	position = pos;
	SetPositionStart(pos);

	this->SetBound(8, 8);
	HP = 1;
	Damage = 1;
}

void OBullet::Controller()
{
	velocity.y = 0;
}

D3DXVECTOR2 OBullet::OnCollision(Object* obj, D3DXVECTOR2 side)
{
	switch (obj->Tag)
	{
	case Object::Player:
	case Object::Enemy:
		OnCollision(obj);
		return D3DXVECTOR2(Collision::NONE, Collision::NONE);
	default:
		return D3DXVECTOR2(Collision::NONE, Collision::NONE);
	}
}

void OBullet::OnCollision(Object* obj)
{
	switch (obj->Tag)
	{
	case Object::Enemy:
		if (_bulletType == EnemyBullet)
		{
			break;
		}
		velocity.x = 0;
		this->State = Object::Dying;
		obj->SetHP(obj->GetHP() - this->Damage);
		obj->SetVelocityX(velocity.x);
		break;
	case Object::Player:
		if (_bulletType != EnemyBullet)
		{
			break;
		}
		velocity.x = 0;
		this->State = Object::Dying;
		obj->SetHP(obj->GetHP() - this->Damage);
		break;
	default:
		break;
	}
}

void OBullet::BeforeUpdate(float gameTime, Keyboard* key)
{
	if (!IsFire) return;

	this->SetBound(8, 8);
	this->Controller();
}

void OBullet::Update(float gameTime, Keyboard* key)
{
	if (!IsFire) return;

	//Update Animation
	if (State == Object::Dying)
	{
		this->SetBound(0, 0);
		this->SetVelocity(0, 0);
		timeDead += gameTime;
		if (timeDead > 0.2)
			Reset();
	}

	UpdateAnimation(gameTime);

	//Object::Update(gameTime, key);
	position.x += cos(Angle / 180 * 3.14159265358979323846)* velocity.x * gameTime;
	position.y = (tan(Angle / 180 * 3.14159265358979323846 ))* (position.x - positionStart.x) + positionStart.y;
}

void OBullet::UpdateAnimation(float gameTime)
{
	_anim->NewAnimationByIndex(_bulletType + this->State);
	_anim->SetPosition(position);
	//_anim->SetFlipFlag(velocity.x > 0);
	_anim->Update(gameTime);
}

void OBullet::SetBound(float width, float height)
{
	InfoSprite::Infoframe info = _anim->GetCurrentFrameInfo();
	Width = info.w;
	Height = info.h;

	float w = width >= 0 ? width : Width;
	float h = height >= 0 ? height : Height;
	bound.left = position.x - w / 2;
	bound.right = bound.left + w;
	bound.top = position.y + h/2;
	bound.bottom = bound.top - h;
}

void OBullet::Render(Viewport* viewport)
{
	//Vẽ OBullet
	if (AllowDraw)
	{
		_anim->Render(viewport);
	}
}