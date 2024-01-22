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

#include <string>

namespace
{
	constexpr int kLoadObjectNum = 250;
	constexpr int kLoadObjectSpeed = 10;
}

SceneManager::SceneManager():
	m_pScene()
{
	std::string file = "Data/Image/Fade/";
	std::string v = ".png";
	
	for (int i = 0; i < 26; i++)
	{
		std::string str = std::to_string(i + 1);
		std::string a = file + str + v;
		m_hFade[i] = LoadGraph(a.c_str());
	}

	m_handle[1] = LoadGraph("Data/Image/UI/馬.png");
	m_handle[0] = LoadGraph("Data/Image/UI/馬黒.png");

	m_isReverce[1] = true;
	m_isReverce[0] = false;
	std::vector<float> test;
	for (int i = 0; i < kLoadObjectNum; i++)
	{
		m_x[0].push_back(GetRand(Game::kScreenWidth * 2) + Game::kScreenWidth);
		m_y[0].push_back(GetRand(Game::kScreenHeight));

		m_x[1].push_back(GetRand(Game::kScreenWidth * 2) + Game::kScreenWidth);
		m_y[1].push_back(GetRand(Game::kScreenHeight));

		m_x[1].push_back(GetRand(Game::kScreenWidth * 2));

		m_rota.push_back((GetRand(70) - 30) * DX_PI_F / 180.0f);

		m_isRota.push_back(false);
	}

	m_isLoading = true;
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
	//if (m_isLoading)
	//{
	//	if (!LoadUpdate())
	//	{
	//		m_isLoading = false;
	//		printfDx("LoadEnd\n");
	//	}
	//	return;
	//}

	if (pScene != m_pScene.get())
	{
		// 前のシーンの終了処理
		m_pScene->End();

		m_pScene.reset(pScene);
		m_pScene->Init();

		m_isLoading = true;
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

	//if (m_isLoading)
	//{
	//	LoadDraw();
	//}

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

bool SceneManager::LoadUpdate()
{
	int count = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < kLoadObjectNum; j++)
		{

			if (m_rota[j] > 70 * DX_PI_F / 180.0f)
			{
				m_isRota[j] = true;
			}
			if (m_rota[j] < -70 * DX_PI_F / 180.0f)
			{
				m_isRota[j] = false;
			}

			if (m_isRota[j])
			{
				m_rota[j] -= 0.10f;
			}
			else
			{
				m_rota[j] += 0.10f;
			}

			m_x[i][j] -= kLoadObjectSpeed;

			if (m_x[i][j] < 0.0f)
			{
				count++;
			}
		}
	}

	if (count == kLoadObjectNum)
	{
		return false;
	}

	return true;
}

void SceneManager::LoadDraw()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < kLoadObjectNum; j++)
		{
			DrawRotaGraph(m_x[i][j], m_y[i][j], 1, m_rota[j], m_handle[i], true, m_isReverce[i]);
		}
	}

	int a = LoadGraph("Data/Image/UI/8.png");
	DrawGraph(100, 100, a, true);
}

