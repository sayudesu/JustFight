#include <DxLib.h>
#include "SlideSelect.h"
#include "Util/Pad.h"
#include "Util/Game.h"

namespace 
{
	// 選択できる最大数
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
	// 上にスライド
	if (Pad::IsPress(PAD_INPUT_UP))
	{
		// ボタンを押している間フレーム単位でカウントする
		m_selectFramesPressCount[SelectButton::UP]++;
		m_selectUpSpeedFramePressCount[SelectButton::UP]++;
		
		// 選択ボタンを押した瞬間か
		// 選択ボタンを押し続けている際に選択を変更する
		if (m_selectFramesPressCount[SelectButton::UP] == 1 ||
			m_selectUpSpeedFramePressCount[SelectButton::UP] > m_selectFrameMax[SelectButton::UP])
		{
			// 選択を変更する
			m_selectNo--;

			// 選択した場合フレームを初期化する
			m_selectUpSpeedFramePressCount[SelectButton::UP] = 0;

			if (m_selectFrameMax[SelectButton::UP] > kSelectFrameMin)
			{
				m_selectFrameMax[SelectButton::UP]--;
			}

			// 最小数になると最大数に変更する
			if (m_selectNo <= -1)
			{
				m_selectNo = kSelectMaxNum;
			}
		}
	}
	else
	{
		// フレームカウントを初期化する
		m_selectFramesPressCount[SelectButton::UP] = 0;
		m_selectUpSpeedFramePressCount[SelectButton::UP] = 0;

		m_selectFrameMax[SelectButton::UP] = kSelectFrameMax;
	}

	// 下にスライド
	if (Pad::IsPress(PAD_INPUT_DOWN))
	{
		// ボタンを押している間フレーム単位でカウントする
		m_selectFramesPressCount[SelectButton::DOWN]++;
		m_selectUpSpeedFramePressCount[SelectButton::DOWN]++;

		// 選択ボタンを押した瞬間か
		// 選択ボタンを押し続けている際に選択を変更する
		if (m_selectFramesPressCount[SelectButton::DOWN] == 1 ||
			m_selectUpSpeedFramePressCount[SelectButton::DOWN] > m_selectFrameMax[SelectButton::DOWN])
		{
			// 選択を変更する
			m_selectNo++;

			// 選択した場合フレームを初期化する
			m_selectUpSpeedFramePressCount[SelectButton::DOWN] = 0;

			if (m_selectFrameMax[SelectButton::DOWN] > kSelectFrameMin)
			{
				m_selectFrameMax[SelectButton::DOWN]--;
			}

			// 最大数になると最小数にする
			if (m_selectNo > kSelectMaxNum)
			{
				m_selectNo = 0;
			}
		}
	}
	else
	{
		// フレームカウントを初期化する
		m_selectFramesPressCount[SelectButton::DOWN] = 0;
		m_selectUpSpeedFramePressCount[SelectButton::DOWN] = 0;

		m_selectFrameMax[SelectButton::DOWN] = kSelectFrameMax;
	}

	// 選択
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
