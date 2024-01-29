#include <DxLib.h>

#include "BlurScreen.h"

#include "Util/Game.h"

EffectScreen* EffectScreen::m_pInstance = nullptr;

void EffectScreen::Load()
{
#if true		

	// 攻撃演出用
	m_quakeX = 0.0f;
	m_quakeFrame = 0;

	// 加工用の画面ハンドルを保存
	int sw = 0, sh = 0, bit = 0;      // 画面幅＆画面高＆ビット数
	GetScreenState(&sw, &sh, &bit);   // 幅と高さを取得しておく

	for (int i = 0; i < static_cast<int>(DamageEffectNo::MAX); i++)
	{
		m_damageScreen[i] = MakeScreen(sw, sh);// 加工用画面を用意する
	}
#endif
	// モーションブラー用
	m_notBlendDraw = 0;
	m_current = 0;
	m_alpha = 150;

	for (int i = 0; i < static_cast<int>(BlurEffectNo::MAX); ++i)
	{
		m_blurScreen[i] = MakeScreen(Game::kScreenWidth, Game::kScreenHeight);
	}
}

void EffectScreen::Unload()
{
#if true	
	for (int i = 0; i < 3; ++i)
	{
		if (m_damageScreen[i] != -1)
		{
			DeleteGraph(m_damageScreen[i]);
		}
	}
#endif
}

void EffectScreen::ClearScreen()
{
#if true	
	ClearDrawScreen();
#endif
}

void EffectScreen::ScreenBack()
{
#if true	
	SetDrawScreen(DX_SCREEN_BACK);
#endif
}

void EffectScreen::BlurIReplayInit()
{
#if true	
	for (int i = 0; i < static_cast<int>(BlurEffectNo::MAX); ++i)
	{
		if (m_blurScreen[i] != -1)
		{
			DeleteGraph(m_blurScreen[i]);
		}

		m_blurScreen[i] = MakeScreen(Game::kScreenWidth, Game::kScreenHeight);
	}
	m_current = 0;
	m_notBlendDraw = 0;
#endif
}

void EffectScreen::BlurRelease()
{
	for (int i = 0; i < static_cast<int>(BlurEffectNo::MAX); ++i)
	{
		if (m_blurScreen[i] != -1)
		{
			DeleteGraph(m_blurScreen[i]);
		}
	}
}

void EffectScreen::BlurPreRenderBlurScreen()
{
#if true	
	SetDrawScreen(m_blurScreen[m_current]);
	ClearDrawScreen();
#endif
}

void EffectScreen::BlurPostRenderBlurScreen()
{
#if true	
//	int drawMode = GetDrawMode();
//	SetDrawMode(DX_DRAWMODE_BILINEAR);

	//int blendMode, param;
	//GetDrawBlendMode(&blendMode, &param);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);

	m_notBlendDraw++;
	if (m_notBlendDraw > static_cast<int>(BlurEffectNo::MAX))
	{
		DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_blurScreen[1 - m_current], false);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
//	SetDrawMode(DX_DRAWMODE_BILINEAR);

	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, m_blurScreen[m_current], false);
	m_current = 1 - m_current;
#endif
}

void EffectScreen::QuakeReplayInit()
{
#if true	
	m_quakeX     = 10.0f;
	m_quakeFrame = 30;
#endif
}

void EffectScreen::QuakeUpdate()
{
#if true	
	if (--m_quakeFrame > 0) 
	{
		m_quakeX = -m_quakeX;
		m_quakeX *= 0.95f;
		m_quakeFrame--;
	}
	else
	{
		m_quakeX = 0.0f;
	}
#endif
}

void EffectScreen::QuakeRelease()
{
#if true
	DeleteGraph(m_damageScreen[static_cast<int>(DamageEffectNo::QUAKE)]);
#endif
}

void EffectScreen::QuakePreRenderBlurScreen()
{
#if true
	SetDrawScreen(m_damageScreen[static_cast<int>(DamageEffectNo::QUAKE)]);
#endif
}

void EffectScreen::QuakePostRenderBlurScreen()
{
#if true
	if (m_quakeFrame > 0)
	{
//		GraphFilter(m_screen[static_cast<int>(EffectNo::QUAKE)], DX_GRAPH_FILTER_INVERT);         // 色を反転させる
//		GraphFilter(m_screen[static_cast<int>(EffectNo::QUAKE)], DX_GRAPH_FILTER_MONO, 128, 0);   // 各ピクセルの色をＲＧＢ形式からYCbCr形式に変換し引数の Cb Cr の値を置き換えた後、再びＲＧＢ形式に戻す
//      GraphFilter(m_screen[static_cast<int>(EffectNo::QUAKE)], DX_GRAPH_FILTER_GAUSS, 16, 1400);// ガウスでぼかしを入れる
		SetDrawBlendMode(DX_BLENDMODE_ADD, 128);                                                  // 加算合成する
		DrawGraph(m_quakeX, m_quakeX, m_damageScreen[static_cast<int>(DamageEffectNo::QUAKE)], false);        // 画面を描画
		GraphFilter(m_damageScreen[static_cast<int>(DamageEffectNo::QUAKE)], DX_GRAPH_FILTER_GAUSS, 32, 1400);// ガウスでぼかしを入れる
		DrawGraph(m_quakeX, m_quakeX, m_damageScreen[static_cast<int>(DamageEffectNo::QUAKE)], false);        // 画面を描画
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);                                              // ブレンドモードを初期状態に戻す
	}
	else
	{
		DrawGraph(0, 0, m_damageScreen[static_cast<int>(DamageEffectNo::QUAKE)], false);                      // 通常画面描画
	}
#endif
}
