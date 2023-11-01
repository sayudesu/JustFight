#pragma once
#include "../CharacterBase.h"
#include <DxLib.h>
#include <deque>

class Enemy : public CharacterBase
{
public:
	Enemy(VECTOR pos);
	virtual ~Enemy();
	void Input()override;
private:
	void BattleType();
private:
	std::deque<float> m_delayFrameAngle;

	// ñhå‰
	bool m_isCheckGuard;
	bool m_isResultGuard;
	// í èÌçUåÇ
	bool m_isCheckAttack;
	bool m_isAttackResult;
	// ã≠çUåÇ
	bool m_isCheckStrongAttack;
	bool m_isStrongAttackResult;
};

