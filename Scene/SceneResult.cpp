#include <DxLib.h>

#include "SceneResult.h"
#include "SceneDebug.h"

#include "../Util/Game.h"

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
	}
	else if (m_resultData == GameResultData::OVER)
	{
		m_hImageResult = LoadGraph("Data/Image/UI/Lose.png");
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
		if (m_imageAngle < 0.1f)
		{
			m_imageAngle += 0.001f;
		}

	}

	if (DxLib::CheckHitKey(KEY_INPUT_Z))
	{
		return new SceneDebug();
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
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 1, m_imageAngle, m_hImageResult, true);
}