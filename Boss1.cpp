#include "Boss1.h"
#include "SceneManager.h"

Boss1::~Boss1()
{
	delete _anim;
	delete gun1;
	delete gun2;
	delete explode;
}

void Boss1::Init(D3DXVECTOR2 pos, int type, int kind)
{
	OEnemy::Init(pos, type, kind);
	this->SetBound(32, 32);
	HP = 20;
	Damage = 0;
	reload = 0;
}

void Boss1::Controller()
{
	if (gun1 == NULL)
	{
		gun1 = new GunBoss();
		gun1->Init(position, Gunboss, 0);
		gun2 = new GunBoss();
		gun2->Init(position, Gunboss, 1);
		ObjectManager::GetInstance()->AddObjectMap(gun1);
		ObjectManager::GetInstance()->AddObjectMap(gun2);
	}
	SetVelocity(0, 0);
}

void Boss1::BeforeUpdate(float gameTime, Keyboard* key)
{
	if (State == Object::Dying)
	{
		SetVelocity(0, 0);
		this->SetBound(0, 0);
		return;
	}
	this->SetBound(16, 20);
	this->Controller();
}

void Boss1::SetBound(float width, float height)
{
	InfoSprite::Infoframe info = _anim->GetCurrentFrameInfo();
	Width = width;
	Height = height;

	bound.left = position.x - width / 2;
	bound.right = bound.left + Width;
	bound.top = position.y + height/2;
	bound.bottom = bound.top - height;
}

void Boss1::Update(float gameTime, Keyboard* key)
{
	_anim->NewAnimationByIndex(_enemyType + this->State);
	int w = _anim->GetCurrentFrameInfo().w;
	_anim->SetPosition(D3DXVECTOR2(position.x - 16 + w/2, position.y));
	_anim->Update(gameTime);

	if (HP < 0 && State != Object::Dying)
	{
		State = Dying;
		StartExplode();
		timeAttack = 0;
		reload = 3;

		if (gSound != NULL)
		{
			Object::StopSound(gSound);
			delete gSound;
			gSound = NULL;
		}
		gSound = Object::PlaySoundA("./Resource Files/Sound/Destroy_Boss.wav");
	}
	if (reload == 3)
	{
		timeAttack += gameTime;
		if (timeAttack > 3)
		SceneManager::GetInstance()->Map(3);
	}

	if (isExplode)
	{
		explode->UpdateAnimation(gameTime);
		float posx  = position.x + timeExplode * 30;
		explode->_anim->SetPosition(D3DXVECTOR2(posx, position.y));
		timeExplode += gameTime;
		if (timeExplode < 1.5)
		{
			explode->AllowDraw = true;
		} else explode->AllowDraw = false;
	}
}

void Boss1::Render(Viewport* viewport)
{
	OEnemy::Render(viewport);
}