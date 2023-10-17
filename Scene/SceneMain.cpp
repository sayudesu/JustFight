#include "SceneMain.h"
#include <DxLib.h>
#include "../Object/Camera/Camera.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/Enemy.h"

SceneMain::SceneMain():
	m_pCamera(nullptr),
	m_pPlayer(nullptr),
	m_pEnemy(nullptr)
{
	
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{	
	m_pCamera = std::make_unique<Camera>();
	m_pPlayer = std::make_unique<Player>(VGet(-300, 0, 0));
	m_pEnemy  = std::make_unique<Enemy> (VGet( 300, 0, 0));

	m_pCamera->Init();
	m_pPlayer->Init();
	m_pEnemy->Init();
}

void SceneMain::End()
{
	m_pPlayer->End();
	m_pEnemy->End();
}

SceneBase* SceneMain::Update()
{
	m_pPlayer->Update();
	m_pEnemy->Update();
	return this;
}

void SceneMain::Draw()
{
	m_pPlayer->Draw();
	m_pEnemy->Draw();

	for (int i = 0; i < 30; i++)
	{
		const VECTOR sPos = VGet(-1000 + (70 * i), 0, 1000);
		const VECTOR ePos = VGet(-1000 + (70 * i), 0, -1000);
		DrawLine3D(sPos, ePos, 0xff0000);
	}
	for (int i = 0; i < 30; i++)
	{
		const VECTOR sPos = VGet(1000, 0, -1000 + (70 * i));
		const VECTOR ePos = VGet(-1000, 0, -1000 + (70 * i));
		DrawLine3D(sPos, ePos, 0xffff00);
	}

	DrawString(0, 0, "SceneMain", 0xffffff);
}

