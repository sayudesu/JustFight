#include <DxLib.h>

#include "PoseScreen.h"

#include "Game.h"
#include "SlideSelect.h"
#include "Pad.h"

#include "../CSVData/FontManager.h"

namespace
{
	// �X���C�h�N���X�̐�
	constexpr int kSlideNum = 3;
	// ���E�X���C�h�̐�
	constexpr int kLeftRightSelectsNum = 1;
	// �㉺�X���C�h�̐�
	constexpr int kUpDwonSelectsNum = 2;
	constexpr int kUpDwonSelectsNum2 = 3;

	constexpr int kSlideSpeed = 100;

	// �w�i�ʒu
	constexpr int kPoseBgX = 200;
	constexpr int kPoseBgY = 100;
	constexpr int kPoseBgX1 = Game::kScreenWidth - 200;
	constexpr int kPoseBgY1 = Game::kScreenHeight - 100;
}

PoseScreen* PoseScreen::m_pInstance = nullptr;

void PoseScreen::Load()
{
	// �C���X�^���X����
	for (int i = 0; i < kSlideNum; i++)
	{
		m_pSlide[i] = new SlideSelect();
	}

	// ���E�ɃX���C�h
	m_pSlide[0]->Init(kLeftRightSelectsNum, false);
	// �㉺�ɃX���C�h
	m_pSlide[1]->Init(kUpDwonSelectsNum - 1, true);
	// �㉺�ɃX���C�h
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

	text[0] = "��蒼��";
	text[1] = "�������";
	text[2] = "�I����ʂɖ߂�";

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
	// �������̉��
	for (int i = 0; i < kSlideNum; i++)
	{
		delete m_pSlide[i];
		m_pSlide[i] = nullptr;
	}
}

void PoseScreen::Update()
{
	// ���E�̑I���̍X�V����
	m_pSlide[0]->Update();
	// �I�������Ă���㉺�̍X�V����
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

	// �|�[�Y�{�^�����������ꍇ
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
	// �w�i
	DrawBox(kPoseBgX, kPoseBgY + m_slidePosY, kPoseBgX1, kPoseBgY1 + m_slidePosY, 0xffffff, true);

	// ���E�̑I��
	DrawBox(200, 100 + m_slidePosY, Game::kScreenWidth / 2, 200 + m_slidePosY, 0x000000, false);
	DrawBox(Game::kScreenWidth / 2, 100 + m_slidePosY, Game::kScreenWidth - 200, 200 + m_slidePosY, 0x000000, false);

	// �`��u�����h���[�h���A���t�@�u�����h
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blendModeRate[0]);
	DrawBox(200 - 1, 100 - 1 + m_slidePosY, Game::kScreenWidth / 2 - 1, 200 - 1 + m_slidePosY, 0xaaaaaa, true);
	
	// �`��u�����h���[�h���A���t�@�u�����h
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blendModeRate[1]);
	DrawBox(Game::kScreenWidth / 2 - 1, 100 - 1 + m_slidePosY, Game::kScreenWidth - 200 - 1, 200 - 1 + m_slidePosY, 0xaaaaaa, true);

	// �`��u�����h���[�h���m�[�u�����h�ɂ���
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	FontManager::GetInstance().DrawString(200, 100 + m_slidePosY, "�T�E���h", 0xffffff, FontSize::GENEITERAMIN_MEDIUM);
	FontManager::GetInstance().DrawString(Game::kScreenWidth / 2, 100 + m_slidePosY,"���̑�", 0xffffff, FontSize::GENEITERAMIN_MEDIUM);

	// �T�E���h�̐ݒ�̏ꍇ		
	if (m_pSlide[0]->GetSelect() == 0)
	{
		// �㉺�̑I������`��
		for (int i = 0; i < kUpDwonSelectsNum; i++)
		{
			// �t�H�[�J�X�����킹�����ڂ̃A���t�@�l��ύX����
			if (m_pSlide[1]->GetSelect() == i)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 126);
			}		
			else
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
			}
			// �g��`��
			DrawBox(m_soundSelectBox[i].x, m_soundSelectBox[i].y + m_slidePosY, m_soundSelectBox[i].x2, m_soundSelectBox[i].y2 + m_slidePosY, m_soundSelectBox[i].color, m_soundSelectBox[i].fill);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			// �ݒ�p�̖��O��`��
			FontManager::GetInstance().DrawString(m_soundSelectBox[i].x, m_soundSelectBox[i].y + m_slidePosY, m_soundSelectBox[i].text, m_soundSelectBox[i].textColor, m_soundSelectBox[i].size);
		}
	}

	if (m_pSlide[0]->GetSelect() == 1)
	{
		// �㉺�̑I������`��
		for (int i = 0; i < kUpDwonSelectsNum2; i++)
		{
			// �t�H�[�J�X�����킹�����ڂ̃A���t�@�l��ύX����
			if (m_pSlide[2]->GetSelect() == i)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 126);
			}
			else
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
			}
			// �g��`��
			DrawBox(m_utilSelectBox[i].x, m_utilSelectBox[i].y + m_slidePosY, m_utilSelectBox[i].x2, m_utilSelectBox[i].y2 + m_slidePosY, m_utilSelectBox[i].color, m_utilSelectBox[i].fill);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			// �ݒ�p�̖��O��`��
			FontManager::GetInstance().DrawString(m_utilSelectBox[i].x, m_utilSelectBox[i].y + m_slidePosY, m_utilSelectBox[i].text, m_utilSelectBox[i].textColor, m_utilSelectBox[i].size);
		}
	}
}

void PoseScreen::PoseSelect()
{
	// �X�^�[�g�{�^�����������ꍇ
	if (Pad::IsTrigger(PAD_INPUT_8))
	{
		m_isPose = !m_isPose;
	}

	if (m_isPose)
	{		

	}
}
