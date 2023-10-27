#include "Enemy.h"
#include "../../Util/Pad.h"

namespace
{
	// Šp“xî•ñ‚ğƒtƒŒ[ƒ€’PˆÊ‚Å’x‚ç‚¹‚é
	constexpr int kDelayFrameAngle = 18;
}

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
	// “ü—Íó‘Ô‚ğæ“¾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	const VECTOR direction = VSub(m_targetPos, m_pos);


	const float angle = atan2f(-direction.x, -direction.z);

	m_delayFrameAngle.push_front(angle);

	if (m_delayFrameAngle.size() > kDelayFrameAngle)
	{
		m_delayFrameAngle.pop_back();
	}

	SetAngle(m_delayFrameAngle.back());

	// ƒvƒŒƒCƒ„[‚Ìis•ûŒü
	MATRIX rotMtx = MGetRotY(m_delayFrameAngle.back());

	SetRotMtx(rotMtx);

	if (!m_isJustGuardBreak)
	{
		if (IsAttackRange())
		{
			if (GetStamina() > 20.0f)
			{
				static bool isAttack = false;
				static bool isAttackResult = false;
				static bool isGuard  = false;
				static bool isGuardResult = false;
				static int guardFrameCount = 0;
				const int guardFrame = 60;

				// —”‚ÅUŒ‚‚·‚é‚©‚ğŒˆ‚ß‚é
				if (!isAttack)
				{
					if (GetRand(30) == 0)
					{
						isAttack = true;
						isAttackResult = true;
					}
				}

				// —”‚Å–hŒä‚·‚é‚©‚ğŒˆ‚ß‚é
				if (!isGuard)
				{
					if (GetRand(30) == 0)
					{
						isGuard = true;
						isGuardResult = true;
					}
				}

				// UŒ‚‚©‚Ç‚¤‚©
				if (isAttackResult && !m_isGuard)
				{
					isAttackResult = false;
					m_isAttack = true;
					m_pFunc = &Enemy::Attack;
				}
				// UŒ‚‚ğ‚µ‚Ä‚¢‚È‚¢ê‡
				if (!m_isAttack)
				{
					isAttack = false;
				}


				if (guardFrameCount == guardFrame)
				{
					guardFrameCount = 0;
					isGuard = false;
					m_isGuard = false;
					isGuardResult = false;
				}
				if (isGuardResult && !m_isAttack)
				{
					m_isGuard = true;
					guardFrameCount++;
					m_pFunc = &Enemy::Guard;
				}




				/*if (GetRand(2) == 0 && !m_isAttack)
				{
					m_isGuard = true;
					m_pFunc = &Enemy::Guard;
				}
				else
				{
					m_isGuard = false;
				}*/
			}
		}
		else
		{
			TargetMove();
		}
	}
}

