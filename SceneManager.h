#pragma once
#include "Graphic.h"
#include "Keyboard.h"
#include "ObjectManager.h"
#include "GameDefine.h"

using namespace Define;

class SceneManager
{
static SceneManager* _sceneManager;

public:
	bool isEnd;
	int level = 1;
	float endTime;
	float timedelay;
	Sprite* sprite = new Sprite("./Resource Files/intro.png");
	GSound* gSound;

	enum SceneType
	{
		Intro = 0,
		Wait = 1,
		End = 2,
		Play = 3,
	} sceneType = SceneType::Intro;
public:
	SceneManager();
	~SceneManager();

	//Load Data Game
	void InitDT();

	//Update Game sau khoảng thời gian gameTime
	void Update(float gameTime, Keyboard* key);
	void StartEnd(float time = timeEnd);
	//Vẽ Object lên màn hình
	void Render();
	void Map(int level = 1);
	static SceneManager* GetInstance();
};

