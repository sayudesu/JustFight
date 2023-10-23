#include "Enemy.h"
#include "../../Util/Pad.h"

Enemy::Enemy(VECTOR pos) :
	CharacterBase(pos)
{
	m_pFunc = &Enemy::Idle;

	m_isAttack = false;
	m_isGuard = false;
//	m_isJustGuard = false;
	m_isJustGuardBreak = false;
}

Enemy::~Enemy()
{
}

void Enemy::Input()
{
	DINPUT_JOYSTATE input;
	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	const VECTOR direction = VSub(m_targetPos, m_pos);
	const float angle = atan2f(-direction.x, -direction.z);

	SetAngle(angle);

	// プレイヤーの進行方向
	MATRIX rotMtx = MGetRotY(angle);

	SetRotMtx(rotMtx);

	if (!m_isJustGuardBreak)
	{
		if (GetStamina() > 20.0f)
		{
			if (GetRand(100) == 0 && !m_isGuard)
			{
				m_isAttack = true;
				m_pFunc = &Enemy::Attack;
			}
		}
		//if (GetRand(10)  == 0 && !m_isAttack)
		//{
		//	m_isGuard = true;
		//	m_pFunc = &Enemy::Guard;
		//}
		//else
		//{
		//	m_isGuard = false;
		//}
	}
}

void Enemy::SetTargetPos(VECTOR pos)
{
	m_targetPos = pos;
}
