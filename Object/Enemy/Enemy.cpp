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

				// 乱数で攻撃するかを決める
				if (!isAttack)
				{
					if (GetRand(60) == 0)
					{
						isAttack = true;
						isAttackResult = true;
					}
				}

				//// 乱数で防御するかを決める
				//if (!isGuard)
				//{
				//	if (GetRand(60) == 0)
				//	{
				//		isGuard = true;
				//		isGuardResult = true;
				//	}
				//}

				// 攻撃かどうか
				if (isAttackResult && !m_isGuard)
				{
					isAttackResult = false;
					m_isAttack = true;
					m_pFunc = &Enemy::Attack;
				}
				// 攻撃をしていない場合
				if (!m_isAttack)
				{
					isAttack = false;
				}


				//if (guardFrameCount == guardFrame)
				//{
				//	guardFrameCount = 0;
				//}
				//if (isGuardResult && !m_isAttack)
				//{
				//	isGuardResult = false;
				//	m_isGuard = true;
				//	guardFrameCount++;
				//	m_pFunc = &Enemy::Guard;
				//}
				//else
				//{
				//	isGuard = false;
				//	m_isGuard = false;
				//}




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

