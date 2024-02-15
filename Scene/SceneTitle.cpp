#include <DxLib.h>
#include "SceneTitle.h"
#include "SceneLevelSelect.h"

#include "../Util/Game.h"
#include "../Util/Pad.h"
#include "../CSVData/FontManager.h"
#include "../Util/FontSize.h"


SceneTitle::SceneTitle():
	m_hTitle(-1),
	m_hBg(-1),
	m_blendAlpha(0)
{
	m_hTitle = LoadGraph("Data/Image/Title.png");
	m_hBg = LoadGraph("Data/Image/UI/ゲーム難易度選択ベース2.png");
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{

}

void SceneTitle::End()
{
	
}

SceneBase* SceneTitle::Update()
{	
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		return new SceneLevelSelect();
	}

	static int timer = 0;
	m_blendAlpha = cos(timer * 0.06f) * 255.0f;
	timer++;

	return this;
}

void SceneTitle::Draw()
{
	// 背景
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_hBg, true);

	// タイトル描画
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 1, 0.0f, m_hTitle, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blendAlpha);
	FontManager::GetInstance().DrawString(Game::kScreenWidth/2 - 272 + 12 + 5, Game::kScreenHeight/2 + 256 + 5, "Press any key", 0x111111, FontSize::GENEITERAMIN_MEDIUM);
	FontManager::GetInstance().DrawString(Game::kScreenWidth/2 - 272 + 12, Game::kScreenHeight/2 + 256, "Press any key", 0xffffff, FontSize::GENEITERAMIN_MEDIUM);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}