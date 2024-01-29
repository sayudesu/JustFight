#include <DxLib.h>

#include "BlurScreen.h"

#include "Util/Game.h"

EffectScreen* EffectScreen::m_pInstance = nullptr;

void EffectScreen::Load()
{
#if true		

	// �U�����o�p
	m_quakeX = 0.0f;
	m_quakeFrame = 0;

	// ���H�p�̉�ʃn���h����ۑ�
	int sw = 0, sh = 0, bit = 0;      // ��ʕ�����ʍ����r�b�g��
	GetScreenState(&sw, &sh, &bit);   // ���ƍ������擾���Ă���

	for (int i = 0; i < static_cast<int>(DamageEffectNo::MAX); i++)
	{
		m_damageScreen[i] = MakeScreen(sw, sh);// ���H�p��ʂ�p�ӂ���
	}
#endif
	// ���[�V�����u���[�p
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
//		GraphFilter(m_screen[static_cast<int>(EffectNo::QUAKE)], DX_GRAPH_FILTER_INVERT);         // �F�𔽓]������
//		GraphFilter(m_screen[static_cast<int>(EffectNo::QUAKE)], DX_GRAPH_FILTER_MONO, 128, 0);   // �e�s�N�Z���̐F���q�f�a�`������YCbCr�`���ɕϊ��������� Cb Cr �̒l��u����������A�Ăтq�f�a�`���ɖ߂�
//      GraphFilter(m_screen[static_cast<int>(EffectNo::QUAKE)], DX_GRAPH_FILTER_GAUSS, 16, 1400);// �K�E�X�łڂ���������
		SetDrawBlendMode(DX_BLENDMODE_ADD, 128);                                                  // ���Z��������
		DrawGraph(m_quakeX, m_quakeX, m_damageScreen[static_cast<int>(DamageEffectNo::QUAKE)], false);        // ��ʂ�`��
		GraphFilter(m_damageScreen[static_cast<int>(DamageEffectNo::QUAKE)], DX_GRAPH_FILTER_GAUSS, 32, 1400);// �K�E�X�łڂ���������
		DrawGraph(m_quakeX, m_quakeX, m_damageScreen[static_cast<int>(DamageEffectNo::QUAKE)], false);        // ��ʂ�`��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);                                              // �u�����h���[�h��������Ԃɖ߂�
	}
	else
	{
		DrawGraph(0, 0, m_damageScreen[static_cast<int>(DamageEffectNo::QUAKE)], false);                      // �ʏ��ʕ`��
	}
#endif
}
