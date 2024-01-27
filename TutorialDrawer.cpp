#include <DxLib.h>

#include "TutorialDrawer.h"

#include "CSVData/FontManager.h"

#include "Util/FontSize.h"
namespace
{
	// ê‡ñæópï∂éö
	const char* const kTipsOne = "_ê‡ñæá@_";
	const char* const kLeftStick = " ç∂ÇÃÉXÉeÉBÉbÉNÇ≈à⁄ìÆÇ≈Ç´ÇÈÅI";
	const char* const kEnemy = " ìGÇ…ãﬂÇ√Ç±Ç§ÅI";

	const char* const kTipsTwo = "_ê‡ñæáA_";
	const char* const kAttack = " RB Ç≈ çUåÇÇµÇÊÇ§ÅI";

	const char* const kTipsThree = "_ê‡ñæáB_";
	const char* const kGuard = " LB Ç≈ ñhå‰ÇµÇÊÇ§ÅI";
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
}

void TutorialDrawer::End()
{

}

void TutorialDrawer::Update()
{

}

void TutorialDrawer::Draw()
{
	// ògëgÇ›
	DrawBox(50, 50, 1260, 300, 0xffffff, true);
	DrawBox(50 + 1, 50 + 1, 1260 + 1, 300 + 1, 0xff0000, false);

	// ê‡ñæÇï`âÊ
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
	}
}
