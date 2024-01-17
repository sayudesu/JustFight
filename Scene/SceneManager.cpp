#include <DxLib.h>
#include <cassert>

#include "SceneManager.h"
#include "SceneDebug.h"
#include "SceneBase.h"
#include "SceneTitle.h"
#include "SceneMain.h"
#include "SceneResult.h"

#include "../Util/Pad.h"
#include "../Util/Game.h"

SceneManager::SceneManager():
	m_pScene()
{
}
SceneManager::~SceneManager()
{

}

void SceneManager::Init()
{
//	m_pScene.reset(new SceneDebug);
	m_pScene.reset(new SceneTitle);
	m_pScene->Init();
}

void SceneManager::End()
{
	assert(m_pScene);
	if (!m_pScene)	return;

	m_pScene->End();
}

void SceneManager::Update()
{
	assert(m_pScene);
	if (!m_pScene)return;
#if _DEBUG
	LONGLONG start = GetNowHiPerformanceCount();
#endif
	Pad::Update();

	SceneBase* pScene = m_pScene->Update();

	if (pScene != m_pScene.get())
	{
		// 前のシーンの終了処理
		m_pScene->End();

		m_pScene.reset(pScene);
		m_pScene->Init();
	}
#if _DEBUG
	m_updateTime = GetNowHiPerformanceCount() - start;
#endif
}

void SceneManager::Draw()
{
	assert(m_pScene);
	if (!m_pScene)	return;

#if _DEBUG
	LONGLONG start = GetNowHiPerformanceCount();
#endif

	m_pScene->Draw();

#if _DEBUG
	m_drawTime = GetNowHiPerformanceCount() - start;
	
	DrawString(0, Game::kScreenHeight - 48, "処理", 0xffffff, 0x000000);
	DrawBox(32 + 2, Game::kScreenHeight - 48 + 2, 48 + 16 - 2, Game::kScreenHeight - 32 - 2, 0x0000ff, true);

	DrawString(0, Game::kScreenHeight - 32, "描画", 0xffffff, 0x000000);
	DrawBox(32 + 2, Game::kScreenHeight - 32 + 2, 48 + 16 - 2, Game::kScreenHeight - 16 - 2, 0xff0000, true);

	float rate = static_cast<float>(m_updateTime + m_drawTime) / 16666.6f;
	int width = static_cast<int>(Game::kScreenWidth * rate);
	DrawBox(0, Game::kScreenHeight - 16, width, Game::kScreenHeight, 0x0000ff,true);

	rate = static_cast<float>(m_updateTime) / 16666.6f;
	width = static_cast<int>(Game::kScreenWidth * rate);
	DrawBox(0, Game::kScreenHeight - 16, width, Game::kScreenHeight, 0xff0000, true);
#endif
}

