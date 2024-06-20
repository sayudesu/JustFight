#include <DxLib.h>
#include <cassert>
#include <string>
#include <algorithm>

#include "SceneManager.h"
#include "SceneBase.h"
#include "SceneTitle.h"
#include "SceneLevelSelect.h"
#include "SceneMain.h"
#include "SceneResult.h"

#include "../Util/Pad.h"
#include "../Util/Game.h"


namespace
{
	constexpr int kLoadObjectNum = 12;
	constexpr int kLoadObjectSpeed = 16;

	constexpr int kFadeSpeedRate = 15;
}

SceneManager::SceneManager():
	m_pScene(nullptr),
	m_pTempScene(nullptr),
	m_blendRate(0)
{	
	m_isLoading = false;
	m_isFade = false;
}
SceneManager::~SceneManager()
{

}

void SceneManager::Init()
{
	m_pScene.reset(new SceneTitle);
//	m_pScene.reset(new SceneMain(DifficultyData::INTERMEDIATE));

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

	// コントローラーの更新処理
	Pad::Update();

	if (m_isFade)
	{
		// ロードを初期化する
		InitFade();
		// 更新処理
		UpdateFadeOut();

		m_isInit = true;

		return;
	}
	else if (m_isInit)
	{
		// 前のシーンの終了処理
		m_pScene->End();

		// シーンを変更する
		m_pScene.reset(m_pTempScene);

		// 初期化する
		m_pScene->Init();

		m_isInit = false;
	}

	UpdateFadeIn();

	m_pTempScene = m_pScene->Update();

	if (m_pTempScene != m_pScene.get())
	{
		// フェイドを開始する
		StartFade();
	}
}

void SceneManager::Draw()
{
	assert(m_pScene);
	if (!m_pScene)	return;

#if _DEBUG
	LONGLONG start = GetNowHiPerformanceCount();
#endif

	// 描画
	m_pScene->Draw();

	// フェイド画面の描画
	if (m_isLoading)
	{
		DrawFade();
	}
}

void SceneManager::StartFade()
{
	m_isFade = true;
}

void SceneManager::InitFade()
{
	if (!m_isLoading)
	{
		m_fadeOut = true;
		if (m_fadeOut)
		{
			m_isLoading = true;
		}
	}
}

void SceneManager::UpdateFadeIn()
{
	m_blendRate = (std::max)(m_blendRate - kFadeSpeedRate, 0);
	// フェイドインの更新処理
	if (m_fadeIn)
	{
		if(m_blendRate <= 0)
		{
			m_blendRate = 0;

			m_fadeIn = false;

			

			m_isLoading = false;
		}
	}
}

void SceneManager::UpdateFadeOut()
{

	// フェイドアウト処理
	m_blendRate = (std::min)(m_blendRate + kFadeSpeedRate, 255);

	if (m_blendRate >= 255)
	{
		m_fadeOut = false;
		m_fadeIn = true;
		m_isFade = false;
	}
}

void SceneManager::DrawFade()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blendRate);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

