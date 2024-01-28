#include <DxLib.h>

#include "TutorialDrawer.h"

#include "CSVData/FontManager.h"

#include "Util/FontSize.h"
namespace
{
	// �����p����
	const char* const kTipsOne = "_�����@_";
	const char* const kLeftStick = " ���̃X�e�B�b�N�ňړ��ł���I\n �G�ɋ߂Â����I";
	const char* const kEnemy = " �G�ɋ߂Â����I";

	const char* const kTipsTwo = "_�����A_";
	const char* const kAttack = " RB �� �U�����悤�I";

	const char* const kTipsThree = "_�����B_";
	const char* const kGuard = " LB �� �h�䂵�悤�I";

	const char* const kTipsEnd = "_�����ꂳ�܁I_";
	const char* const kEnd = " ����Ń`���[�g���A���͏I���I\n �G��|���I";
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
	// �g�g��
	DrawBox(50, 50, 1260, 300, 0xffffff, true);
	DrawBox(50 + 1, 50 + 1, 1260 + 1, 300 + 1, 0xff0000, false);

	// ������`��
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
