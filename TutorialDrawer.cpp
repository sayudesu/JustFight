#include <DxLib.h>

#include "TutorialDrawer.h"

#include "CSVData/FontManager.h"

#include "Util/FontSize.h"
namespace
{
	// 説明用文字
	const char* const kTipsOne = "_説明①_";
	const char* const kLeftStick = " 左のスティックで移動できる！\n 敵に近づこう！";
	const char* const kEnemy = " 敵に近づこう！";

	const char* const kTipsTwo = "_説明②_";
	const char* const kAttack = " RB で 攻撃しよう！";

	const char* const kTipsThree = "_説明③_";
	const char* const kGuard = " LB で 防御しよう！";

	const char* const kTipsEnd = "_おつかれさま！_";
	const char* const kEnd = " これでチュートリアルは終了！\n 敵を倒せ！";
}

TutorialDrawer::TutorialDrawer()
{

}

TutorialDrawer::~TutorialDrawer()
{

}

void TutorialDrawer::Init()
{

	for (int i = 0; i < static_cast<int>(Tips::MAX); i++)
	{
		m_tips[i] = false;
	}

	m_step = kTipsOne;
	m_text = kLeftStick;

	m_endFrameCount = 0;
}

void TutorialDrawer::End()
{

}

void TutorialDrawer::Update()
{

}

void TutorialDrawer::Draw()
{
	// 枠組み
	DrawBox(50, 50, 1260, 300, 0xffffff, true);
	DrawBox(50 + 1, 50 + 1, 1260 + 1, 300 + 1, 0xff0000, false);

	// 説明を描画
	FontManager::GetInstance().DrawString(70, 60, m_step, 0x000000, FontSize::GENEITERAMIN_SMALL);
	FontManager::GetInstance().DrawString(50, 100, m_text, 0x000000, FontSize::GENEITERAMIN_MEDIUM);
}

void TutorialDrawer::SetTips(Tips tips)
{

	for (int i = 0; i < static_cast<int>(Tips::MAX); i++)
	{
		m_tips[i] = false;
	}

	m_tips[static_cast<int>(tips)] = true;

	if (m_tips[static_cast<int>(Tips::MOVE)])
	{
		m_step = kTipsOne;
		m_text = kLeftStick;
	}
	else if (m_tips[static_cast<int>(Tips::ATTACK)])
	{
		m_step = kTipsTwo;
		m_text = kAttack;
	}
	else if (m_tips[static_cast<int>(Tips::GUARD)])
	{
		m_step = kTipsThree;
		m_text = kGuard;

		m_endFrameCount++;
	}

	if (m_endFrameCount > 60 * 3)
	{
		m_step = kTipsEnd;
		m_text = kEnd;
	}
}
