#include "Enemy.h"
#include "../../Util/Pad.h"

namespace
{
	// 角度情報をフレーム単位で遅らせる
	constexpr int kDelayFrameAngle = 18;
}

Enemy::Enemy(VECTOR pos) :
	CharacterBase(pos)
{
	m_pFunc = &Enemy::Idle;

	m_isAttack = false;
	m_isGuard = false;

	// 自身がエネミーであると決める
	m_myId = CharacterName::ENEMY;
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

	m_delayFrameAngle.push_front(angle);
	if (m_delayFrameAngle.size() > kDelayFrameAngle)
	{
		m_delayFrameAngle.pop_back();
	}

	SetAngle(m_delayFrameAngle.back());

	// プレイヤーの進行方向
	MATRIX rotMtx = MGetRotY(m_delayFrameAngle.back());

	SetRotMtx(rotMtx);

	if (!IsStun())
	{
		// 一定距離近づくとランダムで左右移動を始める
		if (m_targetRange.x + m_targetRange.z < 1000.0f)
		{
			int moveType = 0;
			static bool isLeft = false;
			static bool isRight = false;
			if (GetRand(100) == 0)
			{
				isRight = true;
				isLeft = false;
			}
			if (GetRand(100) == 1)
			{
				isLeft = true;
				isRight = false;
			}
			if (GetRand(100) == 2)
			{
				isLeft = false;
				isRight = false;
			}
			if (isLeft)
			{
				moveType = 1;
			}
			if (isRight)
			{
				moveType = -1;
			}
			const VECTOR move = VTransform(VGet(moveType * 10, 0, 0), rotMtx);
			m_pos = VAdd(m_pos, move);
		}

		if (GetStamina() > 20.0f)
		{
			if (IsAttackRange())
			{
				static bool isAttack = false;
				static bool isAttackResult = false;

				static bool isStrongAttack = false;
				static bool isStrongAttackResult = false;

				static bool isGuard  = false;
				static bool isGuardResult = false;

				static int guardFrameCount = 0;
				const int guardFrame = 60;

				// 乱数で攻撃するかを決める
				if (!isAttack &&
					!m_isAttack &&
					!m_isStrongAttack &&
					!m_isGuard)
				{
					if (GetRand(30) == 0)
					{
						isAttack = true;
						isAttackResult = true;
					}
				}

				// 乱数で強攻撃するかを決める
				if (!isStrongAttack &&
					!m_isAttack &&
					!m_isStrongAttack &&
					!m_isGuard)
				{
					if (GetRand(90) == 0)
					{
						isStrongAttack = true;
						isStrongAttackResult = true;
					}
				}

				// 乱数で防御するかを決める
				if (!isGuard &&
					!m_isAttack &&
					!m_isStrongAttack)
				{
					if (GetRand(30) == 0)
					{
						isGuard = true;
						isGuardResult = true;
					}
				}

				// 攻撃かどうか
				if (isAttackResult)
				{
					isAttackResult = false;
					m_isAttack = true;
					m_attackId = AttackData::NORMAL;
					m_pFunc = &Enemy::Attack;
				}
				// 攻撃をしていない場合
				if (!m_isAttack)
				{
					isAttack = false;
				}

				// 強攻撃かどうか
				if (isStrongAttackResult && !m_isGuard)
				{
					isStrongAttackResult = false;
					m_isStrongAttack = true;
					m_attackId = AttackData::STRONG;
					m_pFunc = &Enemy::StrongAttack;
				}
				// 強攻撃をしていない場合
				if (!m_isStrongAttack)
				{
					isStrongAttack = false;
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
			}
			else
			{
				TargetMove();
			}
		}
		else
		{
			m_isGuard = false;
		}

	}
}

