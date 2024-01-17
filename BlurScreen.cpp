#include <DxLib.h>

#include "BlurScreen.h"

#include "Util/Game.h"

EffectScreen* EffectScreen::m_pInstanceSound = nullptr;

void EffectScreen::Load()
{
	m_quakeX = 0.0f;
	m_quakeTimer = 0;
	m_screen[0] = 0;
	m_screen[1] = 0;
	m_current = 0;
	m_alpha = 0;
	m_notBlendDraw = 0;
	// ���H�p�̉�ʃn���h����ۑ�
	int sw = 0, sh = 0, bit = 0;      // ��ʕ�����ʍ����r�b�g��
	GetScreenState(&sw, &sh, &bit);   // ���ƍ������擾���Ă���

	for (int i = 0; i < static_cast<int>(EffectNo::MAX); i++)
	{
		m_screen[i] = MakeScreen(sw, sh);// ���H�p��ʂ�p�ӂ���
	}
}

void EffectScreen::Unload()
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_screen[i] != -1) DeleteGraph(m_screen[i]);
	}
}

void EffectScreen::ClearScreen()
{
	ClearDrawScreen();
}

void EffectScreen::ScreenBack()
{
	SetDrawScreen(DX_SCREEN_BACK);
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
	SetDrawScreen(m_screen[m_current]);
}

void EffectScreen::BlurPostRenderBlurScreen()
{
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
}

void EffectScreen::QuakeReplayInit()
{
	m_quakeX     = 40.0f;
	m_quakeTimer = 30;
}

void EffectScreen::QuakeUpdate()
{
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
}

void EffectScreen::QuakeRelease()
{
	DeleteGraph(m_screen[static_cast<int>(EffectNo::QUAKE)]);
}

void EffectScreen::QuakePreRenderBlurScreen()
{
	SetDrawScreen(m_screen[static_cast<int>(EffectNo::QUAKE)]);
}

void EffectScreen::QuakePostRenderBlurScreen()
{
	if (m_quakeTimer > 0)
	{
		GraphFilter(m_screen[static_cast<int>(EffectNo::QUAKE)], DX_GRAPH_FILTER_INVERT);         // �F�𔽓]������
//		GraphFilter(m_screen[static_cast<int>(EffectNo::QUAKE)], DX_GRAPH_FILTER_MONO, 128, 0);   // �e�s�N�Z���̐F���q�f�a�`������YCbCr�`���ɕϊ��������� Cb Cr �̒l��u����������A�Ăтq�f�a�`���ɖ߂�
//      GraphFilter(m_screen[static_cast<int>(EffectNo::QUAKE)], DX_GRAPH_FILTER_GAUSS, 16, 1400);// �K�E�X�łڂ���������
		SetDrawBlendMode(DX_BLENDMODE_ADD, 128);                                                  // ���Z��������
		DrawGraph(m_quakeX, m_quakeX, m_screen[static_cast<int>(EffectNo::QUAKE)], false);        // ��ʂ�`��
		GraphFilter(m_screen[static_cast<int>(EffectNo::QUAKE)], DX_GRAPH_FILTER_GAUSS, 32, 1400);// �K�E�X�łڂ���������
		DrawGraph(m_quakeX, m_quakeX, m_screen[static_cast<int>(EffectNo::QUAKE)], false);        // ��ʂ�`��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);                                              // �u�����h���[�h��������Ԃɖ߂�
	}
	else
	{
		DrawGraph(0, 0, m_screen[static_cast<int>(EffectNo::QUAKE)], false);                      // �ʏ��ʕ`��
	}
}
