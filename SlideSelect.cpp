#include <DxLib.h>
#include "SlideSelect.h"
#include "Util/Pad.h"
#include "Util/Game.h"

namespace 
{
	// �I���ł���ő吔
	constexpr int kSelectMaxNum = 10;
	constexpr int kSelectFrameMax = 30;
	constexpr int kSelectFrameMin = 1;
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
		m_selectFrameMax[i] = kSelectFrameMax;
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
			m_selectUpSpeedFramePressCount[SelectButton::UP] > m_selectFrameMax[SelectButton::UP])
		{
			// �I����ύX����
			m_selectNo--;

			// �I�������ꍇ�t���[��������������
			m_selectUpSpeedFramePressCount[SelectButton::UP] = 0;

			if (m_selectFrameMax[SelectButton::UP] > kSelectFrameMin)
			{
				m_selectFrameMax[SelectButton::UP]--;
			}

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

		m_selectFrameMax[SelectButton::UP] = kSelectFrameMax;
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
			m_selectUpSpeedFramePressCount[SelectButton::DOWN] > m_selectFrameMax[SelectButton::DOWN])
		{
			// �I����ύX����
			m_selectNo++;

			// �I�������ꍇ�t���[��������������
			m_selectUpSpeedFramePressCount[SelectButton::DOWN] = 0;

			if (m_selectFrameMax[SelectButton::DOWN] > kSelectFrameMin)
			{
				m_selectFrameMax[SelectButton::DOWN]--;
			}

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

		m_selectFrameMax[SelectButton::DOWN] = kSelectFrameMax;
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
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(m_selectNo * 10, m_selectNo * 10, m_selectNo * 10), true);
	if (m_isImageDraw)
	{
	}
}

int SlideSelect::GetResult()
{
	return m_selectNoResult;
}
