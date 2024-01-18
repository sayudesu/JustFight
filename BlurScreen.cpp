#include <DxLib.h>

#include "BlurScreen.h"

#include "Util/Game.h"

EffectScreen* EffectScreen::m_pInstanceSound = nullptr;

void EffectScreen::Load()
{
#if true		
	m_quakeX = 0.0f;
	m_quakeTimer = 0;
	m_screen[0] = 0;
	m_screen[1] = 0;
	m_current = 0;
	m_alpha = 0;
	m_notBlendDraw = 0;
	// 加工用の画面ハンドルを保存
	int sw = 0, sh = 0, bit = 0;      // 画面幅＆画面高＆ビット数
	GetScreenState(&sw, &sh, &bit);   // 幅と高さを取得しておく

	for (int i = 0; i < static_cast<int>(EffectNo::MAX); i++)
	{
		m_screen[i] = MakeScreen(sw, sh);// 加工用画面を用意する
	}
#endif
}

void EffectScreen::Unload()
{
#if true	
	for (int i = 0; i < 3; ++i)
	{
		if (m_screen[i] != -1) DeleteGraph(m_screen[i]);
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

//void EffectScreen::BlurInit(int alpha)
//{
//	//for (int i = 0; i < 2; ++i)
//	//{
//	//	m_screen[i] = -1;
//	//	m_screen[i] = MakeScreen(m_screenWidth, m_screenHeight);
//	//}
//	//m_current = 0;
//	//m_alpha = alpha;
//
//	//m_notBlendDraw = 0;
//}

void EffectScreen::BlurIReplayInit(int alpha)
{
#if true	
	/*for (int i = 0; i < 3; ++i)
	{
		if (m_screen[i] != -1) DeleteGraph(m_screen[i]);
		m_screen[i] = MakeScreen(1920, 1080);
	}*/
	for (int i = static_cast<int>(EffectNo::BLUR_0); i <static_cast<int>(EffectNo::BLUR_3); ++i)
	{
		if (m_screen[i] != -1) DeleteGraph(m_screen[i]);
		m_screen[i] = MakeScreen(Game::kScreenWidth, Game::kScreenHeight);
	}

	m_current = 0;
	m_notBlendDraw = 0;
	m_alpha = alpha;
#endif
}

void EffectScreen::BlurRelease()
{
	//for (int i = 0; i < 3; ++i)
	//{
	//	if (m_screen[i] != -1) DeleteGraph(m_screen[i]);
	//}
}

void EffectScreen::BlurPreRenderBlurScreen()
{
#if true	
	SetDrawScreen(m_screen[m_current]);
#endif
}

void EffectScreen::BlurPostRenderBlurScreen()
{
#if true	
	int drawMode = GetDrawMode();
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	int blendMode, param;
	GetDrawBlendMode(&blendMode, &param);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);

	if (m_notBlendDraw++ > 2)
	{
		DrawExtendGraph(0, 0, 1920, 1080, m_screen[static_cast<int>(EffectNo::BLUR_0) + 1 - m_current], false);
	}

	SetDrawBlendMode(blendMode, param);
	SetDrawMode(drawMode);

	SetDrawScreen(DX_SCREEN_BACK);

	DrawGraph(0, 0, m_screen[static_cast<int>(EffectNo::BLUR_0) + 1 - m_current], false);

	m_current = 1 - m_current;
#endif
}

void EffectScreen::QuakeReplayInit()
{
#if true	
	m_quakeX     = 40.0f;
	m_quakeTimer = 30;
#endif
}

void EffectScreen::QuakeUpdate()
{
#if true	
	if (--m_quakeTimer > 0) 
	{
		m_quakeX = -m_quakeX;
		m_quakeX *= 0.95f;
		m_quakeTimer--;
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
	DeleteGraph(m_screen[static_cast<int>(EffectNo::QUAKE)]);
#endif
}

void EffectScreen::QuakePreRenderBlurScreen()
{
#if true
	SetDrawScreen(m_screen[static_cast<int>(EffectNo::QUAKE)]);
#endif
}

void EffectScreen::QuakePostRenderBlurScreen()
{
#if true
	if (m_quakeTimer > 0)
	{
		GraphFilter(m_screen[static_cast<int>(EffectNo::QUAKE)], DX_GRAPH_FILTER_INVERT);         // 色を反転させる
//		GraphFilter(m_screen[static_cast<int>(EffectNo::QUAKE)], DX_GRAPH_FILTER_MONO, 128, 0);   // 各ピクセルの色をＲＧＢ形式からYCbCr形式に変換し引数の Cb Cr の値を置き換えた後、再びＲＧＢ形式に戻す
//      GraphFilter(m_screen[static_cast<int>(EffectNo::QUAKE)], DX_GRAPH_FILTER_GAUSS, 16, 1400);// ガウスでぼかしを入れる
		SetDrawBlendMode(DX_BLENDMODE_ADD, 128);                                                  // 加算合成する
		DrawGraph(m_quakeX, m_quakeX, m_screen[static_cast<int>(EffectNo::QUAKE)], false);        // 画面を描画
		GraphFilter(m_screen[static_cast<int>(EffectNo::QUAKE)], DX_GRAPH_FILTER_GAUSS, 32, 1400);// ガウスでぼかしを入れる
		DrawGraph(m_quakeX, m_quakeX, m_screen[static_cast<int>(EffectNo::QUAKE)], false);        // 画面を描画
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);                                              // ブレンドモードを初期状態に戻す
	}
	else
	{
		DrawGraph(0, 0, m_screen[static_cast<int>(EffectNo::QUAKE)], false);                      // 通常画面描画
	}
#endif
}
