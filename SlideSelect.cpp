#include <DxLib.h>
#include "SlideSelect.h"
#include "Util/Pad.h"
#include "Util/Game.h"

namespace 
{
	// �I���ł���ő吔
	constexpr int kSelectMaxNum = 1;
	constexpr int kSelectFrameMax = 30;
}

SlideSelect::SlideSelect():
	m_selectNo(0),
	m_selectNoResult(-1),
	m_isImageDraw(false)
{
	for (int i = 0; i < SelectButton::BUTTONMAX; i++)
	{
		m_selectFramesPressCount[i] = 0;
		m_selectUpSpeedFramePressCount[i] = 0;
	}
}

SlideSelect::~SlideSelect()
{
}

void SlideSelect::Init()
{
}

void SlideSelect::End()
{
}

void SlideSelect::Update()
{
	// ��ɃX���C�h
	if (Pad::IsPress(PAD_INPUT_UP))
	{
		// �{�^���������Ă���ԃt���[���P�ʂŃJ�E���g����
		m_selectFramesPressCount[SelectButton::UP]++;
		m_selectUpSpeedFramePressCount[SelectButton::UP]++;
		
		// �I���{�^�����������u�Ԃ�
		// �I���{�^�������������Ă���ۂɑI����ύX����
		if (m_selectFramesPressCount[SelectButton::UP] == 1 ||
			m_selectUpSpeedFramePressCount[SelectButton::UP] > kSelectFrameMax)
		{
			// �I����ύX����
			m_selectNo--;

			// �I�������ꍇ�t���[��������������
			m_selectUpSpeedFramePressCount[SelectButton::UP] = 0;

			// �ŏ����ɂȂ�ƍő吔�ɕύX����
			if (m_selectNo <= -1)
			{
				m_selectNo = kSelectMaxNum;
			}
		}
	}
	else
	{
		// �t���[���J�E���g������������
		m_selectFramesPressCount[SelectButton::UP] = 0;
		m_selectUpSpeedFramePressCount[SelectButton::UP] = 0;
	}

	// ���ɃX���C�h
	if (Pad::IsPress(PAD_INPUT_DOWN))
	{
		// �{�^���������Ă���ԃt���[���P�ʂŃJ�E���g����
		m_selectFramesPressCount[SelectButton::DOWN]++;
		m_selectUpSpeedFramePressCount[SelectButton::DOWN]++;

		// �I���{�^�����������u�Ԃ�
		// �I���{�^�������������Ă���ۂɑI����ύX����
		if (m_selectFramesPressCount[SelectButton::DOWN] == 1 ||
			m_selectUpSpeedFramePressCount[SelectButton::DOWN] > kSelectFrameMax)
		{
			// �I����ύX����
			m_selectNo++;

			// �I�������ꍇ�t���[��������������
			m_selectUpSpeedFramePressCount[SelectButton::DOWN] = 0;

			// �ő吔�ɂȂ�ƍŏ����ɂ���
			if (m_selectNo > kSelectMaxNum)
			{
				m_selectNo = 0;
			}
		}
	}
	else
	{
		// �t���[���J�E���g������������
		m_selectFramesPressCount[SelectButton::DOWN] = 0;
		m_selectUpSpeedFramePressCount[SelectButton::DOWN] = 0;
	}

	// �I��
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		m_selectNoResult = m_selectNo;
	}

	if (m_selectNo == 0)
	{
		m_isImageDraw = true;
	}
	if (m_selectNo == 1)
	{
		m_isImageDraw = false;
	}

	printfDx("%d\n", m_selectNo);
}

void SlideSelect::Draw()
{
	if (m_isImageDraw)
	{
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xffffff,true);
	}
}

int SlideSelect::GetResult()
{
	return m_selectNoResult;
}
