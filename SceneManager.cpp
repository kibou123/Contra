#include "SceneManager.h"

SceneManager* SceneManager::_sceneManager;
SceneManager* SceneManager::GetInstance()
{
	if (nullptr == _sceneManager) {
		_sceneManager = new SceneManager();
	}
	return _sceneManager;
}

SceneManager::SceneManager()
{
	isEnd = false;
	endTime = 0;
}
SceneManager::~SceneManager()
{
	if (gSound != NULL)
	{
		Object::StopSound(gSound);
		delete gSound;
		gSound = NULL;
	}
	delete sprite;
}

//Load Data Game
void SceneManager::InitDT()
{
	sceneType = Intro;
	if (gSound != NULL)
	{
		Object::StopSound(gSound);
		delete gSound;
		gSound = NULL;
	}
	gSound = Object::PlaySoundA("./Resource Files/Sound/Intro.wav");
	//ObjectManager::GetInstance()->InitDT();
}
void SceneManager::Map(int level)
{
	isEnd = true;
	sceneType = Wait;
	timedelay = 0;
	this->level = level;
	if (gSound != NULL)
	{
		Object::StopSound(gSound);
		delete gSound;
		gSound = NULL;
	}
}
//Update các scene game Update lớn nhất
void SceneManager::Update(float gameTime, Keyboard* key)
{
	if (key->IsKeyDown(DIK_1))
	{
		Map();
	}

	RECT rect;
	rect.left = sceneType * GameWidth;
	rect.top = 0;
	rect.right = rect.left + GameWidth;
	rect.bottom = GameHeight;
	sprite->SetRect(rect);
	sprite->SetPosition(D3DXVECTOR2(0, 0));

	timedelay += gameTime;
	float posx = GameWidth;
	switch (sceneType)
	{
	case SceneManager::Intro:
		isEnd = false;
		//Reset
		if (timedelay > 5 && timedelay < 10)
		{
			timedelay = 0.0f;
		}
		//chạy từ phải qua
		if (key->IsKeyDown(Dik_START) && timedelay > 0.2 && timedelay < 2.1)
		{
			timedelay = 2.1f;
		}

		posx -= timedelay*(GameWidth/2);
		posx = posx <= 0 ? 0 : posx;
		sprite->SetPosition(D3DXVECTOR2(posx, 0));
		//
		if (key->IsKeyDown(Dik_START) && timedelay > 2.2)
		{
			timedelay = 10.0f;
		}

		if (timedelay > 11)
		{
			Map();
		}

		if (timedelay > 10 && timedelay < 11 && (timedelay/2) && (((int)(timedelay / 0.05)) % 2 == 1))
		{
			rect.top = GameHeight;
			rect.bottom = GameHeight*2;
			sprite->SetRect(rect);
		}
		break;
	case SceneManager::Wait:
		if (isEnd)
		{
			isEnd = false;
			ObjectManager::GetInstance()->InitDT(level);
		}
		if (((int)(timedelay / 0.2)) % 2 == 1)
		{
			rect.top = GameHeight;
			rect.bottom = GameHeight * 2;
			sprite->SetRect(rect);
		}
		if (timedelay > 3)
		{
			if (gSound != NULL)
			{
				Object::StopSound(gSound);
				delete gSound;
				gSound = NULL;
			}
			if (level == 1)
				gSound = Object::PlaySoundA("./Resource Files/Sound/BG_Map1.wav", true);
			else
				gSound = Object::PlaySoundA("./Resource Files/Sound/BG_Map2.wav", true);

			sceneType = Play;
			Player::GetInstance()->_life = StartLive;
			//Player::GetInstance()->Init();
		}
		break;
	case SceneManager::Play:
		ObjectManager::GetInstance()->Update(gameTime, key);
		if (Player::GetInstance()->_life <= 0 && timedelay > 3)
		{
			timedelay = 0.0f;
		}
		else
		{
			rect.right = rect.left + 8 + Player::GetInstance()->_life * 13;
			rect.bottom = 25;
			sprite->SetRect(rect);
		}
		if (timedelay > 2 && timedelay < 3)
		{
			if (gSound != NULL)
			{
				Object::StopSound(gSound);
				delete gSound;
				gSound = NULL;
			}
			gSound = Object::PlaySoundA("./Resource Files/Sound/Game_Over.wav");
			sceneType = End;
		}
		break;
	case SceneManager::End:
		if (((int)(timedelay / 0.2)) % 2 == 1)
		{
			rect.top = GameHeight;
			rect.bottom = GameHeight * 2;
			sprite->SetRect(rect);
		}
		if (key->IsKeyDown(Dik_START) && timedelay > 1)
		{
			sceneType = Intro;
			if (gSound != NULL)
			{
				Object::StopSound(gSound);
				delete gSound;
				gSound = NULL;
			}
			gSound = Object::PlaySoundA("./Resource Files/Sound/Intro.wav");
			timedelay = 0.0f;
		}
		break;
	default:
		break;
	}
}

void SceneManager::StartEnd(float time)
{
	ObjectManager::GetInstance()->StartPause(time);
	isEnd = true;
	endTime = time;
}

//Vẽ Object lên màn hình
void SceneManager::Render()
{
	if (sceneType == Play)
	{
		ObjectManager::GetInstance()->Render();
	}
	sprite->Render();
}