#include <DxLib.h>

#include "SceneResult.h"
#include "SceneDebug.h"
#include "SceneTitle.h"

#include "../Util/Game.h"
#include "../Util/Pad.h"

#include "../CSVData/SoundManager.h"
#include "../Util/SoundName.h"

#include "../CSVData/FontManager.h"
#include "../Util/FontSize.h"

SceneResult::SceneResult(GameResultData data):
	m_imageAngle(0.0f)
{
	// リザルトデータを取得する
	m_resultData = data;
}

SceneResult::~SceneResult()
{
}

void SceneResult::Init()
{
	m_hImageResultBg = LoadGraph("Data/Image/UI/ゲーム難易度選択ベース2.png");
	if (m_resultData == GameResultData::CREAR)
	{
		m_hImageResult = LoadGraph("Data/Image/UI/Win.png");

		for (int i = 0; i < 100; i++)
		{
			m_posX[0].push_back(GetRand(Game::kScreenWidth));
			m_posY[0].push_back(GetRand(Game::kScreenHeight) + Game::kScreenHeight);

			m_vec[0].push_back(1);
		}
	}
	else if (m_resultData == GameResultData::OVER)
	{
		m_hImageResult = LoadGraph("Data/Image/UI/Lose.png");

		for (int i = 0; i < 100; i++)
		{
			m_posX[1].push_back(GetRand(Game::kScreenWidth));
			m_posY[1].push_back(0 - GetRand(Game::kScreenHeight));

			m_vec[1].push_back(1);
		}
	}
}

void SceneResult::End()
{
	DeleteGraph(m_hImageResult);
}

SceneBase* SceneResult::Update()
{
	// 敗北した場合は画像を傾ける
	if (m_resultData == GameResultData::OVER)
	{
		SoundManager::GetInstance().Play(SoundName::LOSE,true);

		if (m_imageAngle < 0.1f)
		{
			m_imageAngle += 0.001f;
		}

		for (int i = 0; i < 100; i++)
		{
			m_vec[1][i] = GetRand(5) + 1;
			m_posY[1][i] += m_vec[1][i];
		}
	}

	if (m_resultData == GameResultData::CREAR)
	{
		SoundManager::GetInstance().Play(SoundName::WIN, true);

		for (int i = 0; i < 100; i++)
		{
			m_vec[0][i] = GetRand(5) + 1;
			m_posY[0][i] -= m_vec[0][i];
		}
	}

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		SoundManager::GetInstance().Stop(SoundName::WIN);
		SoundManager::GetInstance().Stop(SoundName::LOSE);
		SoundManager::GetInstance().Play(SoundName::SELECT);

		return new SceneTitle();
	}
	
	return this;
}

void SceneResult::Draw()
{
#if _DEBUG
	DrawString(0, 0, "SceneResult", 0xffffff);
#endif
	// 背景を描画
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 1, 0.0f, m_hImageResultBg, true);
	if (m_resultData == GameResultData::CREAR)
	{
		for (int i = 0; i < 100; i++)
		{
			DrawRotaGraph(m_posX[0][i], m_posY[0][i], 0.5f, 0.0f, m_hImageResult, true);
		}
	}
	else if (m_resultData == GameResultData::OVER)
	{
		for (int i = 0; i < 100; i++)
		{
			DrawRotaGraph(m_posX[1][i], m_posY[1][i], 0.5f, 0.0f, m_hImageResult, true);
		}
	}
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 1, m_imageAngle, m_hImageResult, true);

	FontManager::GetInstance().DrawString(Game::kScreenWidth / 2 - 470.0f, Game::kScreenHeight / 2 + 200, "A ボタンでタイトルに戻る", 0xffffff, FontSize::GENEITERAMIN_MEDIUM);
}