#include <DxLib.h>

#include "PoseScreen.h"

#include "Game.h"
#include "SlideSelect.h"
#include "Pad.h"

#include "../CSVData/FontManager.h"

namespace
{
	// スライドクラスの数
	constexpr int kSlideNum = 3;
	// 左右スライドの数
	constexpr int kLeftRightSelectsNum = 1;
	// 上下スライドの数
	constexpr int kUpDwonSelectsNum = 2;
	constexpr int kUpDwonSelectsNum2 = 3;

	constexpr int kSlideSpeed = 100;

	// 背景位置
	constexpr int kPoseBgX = 200;
	constexpr int kPoseBgY = 100;
	constexpr int kPoseBgX1 = Game::kScreenWidth - 200;
	constexpr int kPoseBgY1 = Game::kScreenHeight - 100;
}

PoseScreen* PoseScreen::m_pInstance = nullptr;

void PoseScreen::Load()
{
	// インスタンス生成
	for (int i = 0; i < kSlideNum; i++)
	{
		m_pSlide[i] = new SlideSelect();
	}

	// 左右にスライド
	m_pSlide[0]->Init(kLeftRightSelectsNum, false);
	// 上下にスライド
	m_pSlide[1]->Init(kUpDwonSelectsNum - 1, true);
	// 上下にスライド
	m_pSlide[2]->Init(kUpDwonSelectsNum2 - 1, true);

	const char* text[5]{};
	text[0] = "BGM";
	text[1] = "SE";

	for (int i = 0; i < kUpDwonSelectsNum; i++)
	{
		m_soundSelectBox.push_back
		(
			{
				kPoseBgX + 50,
				(kPoseBgY + 200) + (i * 100) + 50,
				kPoseBgX + 50 + 500,
				(kPoseBgY + 200) + (i * 100) + 130,
				0xaaaaaaa,
				true,
				text[i],
				FontSize::GENEITERAMIN_MEDIUM,
				0x0000ff
			}
		);
	}

	text[0] = "やり直す";
	text[1] = "操作説明";
	text[2] = "選択画面に戻る";

	for (int i = 0; i < kUpDwonSelectsNum2; i++)
	{
		m_utilSelectBox.push_back
		(
			{
				kPoseBgX + 50,
				(kPoseBgY + 200) + (i * 100) + 50,
				kPoseBgX + 50 + 500,
				(kPoseBgY + 200) + (i * 100) + 130,
				0xaaaaaaa,
				true,
				text[i],
				FontSize::GENEITERAMIN_MEDIUM,
				0x0000ff
			}
		);
	}

	for (int i = 0; i < 2; i++)
	{
		m_blendModeRate[i] = 0;
	}

	m_isPose = false;

	m_slidePosY = -Game::kScreenHeight;
}

void PoseScreen::Unload()
{
	// メモリの解放
	for (int i = 0; i < kSlideNum; i++)
	{
		delete m_pSlide[i];
		m_pSlide[i] = nullptr;
	}
}

void PoseScreen::Update()
{
	// 左右の選択の更新処理
	m_pSlide[0]->Update();
	// 選択をしている上下の更新処理
	if (m_pSlide[0]->GetSelect() == 0)
	{
		m_blendModeRate[0] = 255;
		m_blendModeRate[1] = 255 / 2;

		m_pSlide[1]->Update();

		m_pSlide[2]->Reset();
	}
	else if (m_pSlide[0]->GetSelect() == 1)
	{
		m_blendModeRate[1] = 255;
		m_blendModeRate[0] = 255 / 2;

		m_pSlide[2]->Update();

		m_pSlide[1]->Reset();
	}

	// ポーズボタンを押した場合
	if (m_isPose)
	{
		if (m_slidePosY < 0)
		{
			m_slidePosY += kSlideSpeed;
		}
		else
		{
			m_slidePosY = 0;
		}
	}
	else
	{
		if (m_slidePosY > -Game::kScreenHeight)
		{
			m_slidePosY -= kSlideSpeed;
		}
		else
		{
			m_slidePosY = -Game::kScreenHeight;
		}
	}
}

void PoseScreen::Draw()
{
	// 背景
	DrawBox(kPoseBgX, kPoseBgY + m_slidePosY, kPoseBgX1, kPoseBgY1 + m_slidePosY, 0xffffff, true);

	// 左右の選択
	DrawBox(200, 100 + m_slidePosY, Game::kScreenWidth / 2, 200 + m_slidePosY, 0x000000, false);
	DrawBox(Game::kScreenWidth / 2, 100 + m_slidePosY, Game::kScreenWidth - 200, 200 + m_slidePosY, 0x000000, false);

	// 描画ブレンドモードをアルファブレンド
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blendModeRate[0]);
	DrawBox(200 - 1, 100 - 1 + m_slidePosY, Game::kScreenWidth / 2 - 1, 200 - 1 + m_slidePosY, 0xaaaaaa, true);
	
	// 描画ブレンドモードをアルファブレンド
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blendModeRate[1]);
	DrawBox(Game::kScreenWidth / 2 - 1, 100 - 1 + m_slidePosY, Game::kScreenWidth - 200 - 1, 200 - 1 + m_slidePosY, 0xaaaaaa, true);

	// 描画ブレンドモードをノーブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	FontManager::GetInstance().DrawString(200, 100 + m_slidePosY, "サウンド", 0xffffff, FontSize::GENEITERAMIN_MEDIUM);
	FontManager::GetInstance().DrawString(Game::kScreenWidth / 2, 100 + m_slidePosY,"その他", 0xffffff, FontSize::GENEITERAMIN_MEDIUM);

	// サウンドの設定の場合		
	if (m_pSlide[0]->GetSelect() == 0)
	{
		// 上下の選択しを描画
		for (int i = 0; i < kUpDwonSelectsNum; i++)
		{
			// フォーカスを合わせた項目のアルファ値を変更する
			if (m_pSlide[1]->GetSelect() == i)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 126);
			}		
			else
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
			}
			// 枠を描画
			DrawBox(m_soundSelectBox[i].x, m_soundSelectBox[i].y + m_slidePosY, m_soundSelectBox[i].x2, m_soundSelectBox[i].y2 + m_slidePosY, m_soundSelectBox[i].color, m_soundSelectBox[i].fill);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			// 設定用の名前を描画
			FontManager::GetInstance().DrawString(m_soundSelectBox[i].x, m_soundSelectBox[i].y + m_slidePosY, m_soundSelectBox[i].text, m_soundSelectBox[i].textColor, m_soundSelectBox[i].size);
		}
	}

	if (m_pSlide[0]->GetSelect() == 1)
	{
		// 上下の選択しを描画
		for (int i = 0; i < kUpDwonSelectsNum2; i++)
		{
			// フォーカスを合わせた項目のアルファ値を変更する
			if (m_pSlide[2]->GetSelect() == i)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 126);
			}
			else
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
			}
			// 枠を描画
			DrawBox(m_utilSelectBox[i].x, m_utilSelectBox[i].y + m_slidePosY, m_utilSelectBox[i].x2, m_utilSelectBox[i].y2 + m_slidePosY, m_utilSelectBox[i].color, m_utilSelectBox[i].fill);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			// 設定用の名前を描画
			FontManager::GetInstance().DrawString(m_utilSelectBox[i].x, m_utilSelectBox[i].y + m_slidePosY, m_utilSelectBox[i].text, m_utilSelectBox[i].textColor, m_utilSelectBox[i].size);
		}
	}
}

void PoseScreen::PoseSelect()
{
	// スタートボタンを押した場合
	if (Pad::IsTrigger(PAD_INPUT_8))
	{
		m_isPose = !m_isPose;
	}

	if (m_isPose)
	{		

	}
}
