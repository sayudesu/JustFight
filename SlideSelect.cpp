#include <DxLib.h>
#include "SlideSelect.h"
#include "Util/Pad.h"
#include "Util/Game.h"

namespace 
{
	constexpr int kSelectFrameMax = 20;
	constexpr int kSelectFrameMin = 1;
}

SlideSelect::SlideSelect():
	m_selectNo(0),
	m_selectNoResult(-1),
	m_selectMaxNum(0),
	m_isSelect(false),
	m_isUpBotton(false),
	m_isDownBotton(false)
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

void SlideSelect::Init(int selectMaxNum)
{
	m_selectMaxNum = selectMaxNum;
}

void SlideSelect::Update()
{
	m_isUpBotton = false;
	m_isDownBotton = false;
	// ��ɃX���C�h
	if (Pad::IsPress(PAD_INPUT_UP))
	{
		m_isUpBotton = true;
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
				m_selectNo = m_selectMaxNum;
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
		m_isDownBotton = true;
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
			if (m_selectNo > m_selectMaxNum)
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
}

int SlideSelect::GetResult()
{
	return m_selectNoResult;
}

int SlideSelect::GetSelect()
{
	return m_selectNo;
}

bool SlideSelect::IsUpBotton()
{
	return m_isUpBotton;
}

bool SlideSelect::IsDownBotton()
{
	return m_isDownBotton;
}
