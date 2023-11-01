#include "Enemy.h"
#include "../../Util/Pad.h"

namespace
{
	// 角度情報をフレーム単位で遅らせる
	constexpr int kDelayFrameAngle = 18;
}

Enemy::Enemy(VECTOR pos) :
	CharacterBase(pos),
	m_isCheckGuard(false),
	m_isResultGuard(false),
	m_isCheckAttack(false),
	m_isAttackResult(false),
	m_isCheckStrongAttack(false),
	m_isStrongAttackResult(false)
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
	printfDx("%d\n", m_targetBattleState);
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

		if (IsAttackRange())
		{
			static int guardFrameCount = 0;
			const int guardFrame = 60;

			//// 乱数で強攻撃するかを決める
			//if (!m_isCheckStrongAttack &&
			//	!m_isAttack &&
			//	!m_isCheckStrongAttack &&
			//	!m_isGuard)
			//{
			//	if (GetRand(90) == 0)
			//	{
			//		m_isCheckStrongAttack = true;
			//		m_isStrongAttackResult = true;
			//	}
			//}

			//// 乱数で防御するかを決める
			//if (!m_isCheckGuard &&
			//	!m_isAttack &&
			//	!m_isCheckStrongAttack)
			//{
			//	if (GetRand(30) == 0)
			//	{
			//		m_isCheckGuard = true;
			//		m_isResultGuard = true;
			//	}
			//}

			//// 乱数で攻撃するかを決める
			//if (!m_isCheckAttack &&
			//	!m_isAttack &&
			//	!m_isCheckStrongAttack &&
			//	!m_isGuard)
			//{
			//	if (GetRand(10) == 0)
			//	{
			//		m_isCheckAttack = true;
			//		m_isAttackResult = true;
			//	}
			//}

			if (!m_isAttack && !m_isGuard && !m_isStrongAttack)
			{
				// 攻撃or防御を決める
				BattleType();
			}


			// 攻撃かどうか
			if (m_isAttackResult)
			{
				m_isAttackResult = false;
				m_isAttack = true;
				m_attackId = AttackData::NORMAL;
				m_pFunc = &Enemy::Attack;
			}
			// 攻撃をしていない場合
			if (!m_isAttack)
			{
				m_isAttack = false;
			}

			// 強攻撃かどうか
			if (m_isStrongAttackResult && !m_isGuard)
			{
				m_isStrongAttackResult = false;
				m_isCheckStrongAttack = true;
				m_attackId = AttackData::STRONG;
				m_pFunc = &Enemy::StrongAttack;
			}
			// 強攻撃をしていない場合
			if (!m_isCheckStrongAttack)
			{
				m_isCheckStrongAttack = false;
			}


			if (guardFrameCount == guardFrame)
			{
				guardFrameCount = 0;
				m_isCheckGuard = false;
				m_isGuard = false;
				m_isResultGuard = false;
			}
			if (m_isResultGuard && !m_isAttack)
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
		printfDx("aaaaaaaaaaaaa");
	}
}

void Enemy::BattleType()
{
	// ターゲットの状態で動作を変更する

	// ターゲットが攻撃している場合
	if (m_targetBattleState == BattleState::ATTACK)
	{
		if (GetRand(2) == 0)
		{
			m_isCheckGuard = true;
			m_isResultGuard = true;
		}
	}

	// ターゲットがスタンしている場合
	if (m_targetBattleState == BattleState::STUN)
	{
		m_isCheckStrongAttack = true;
		m_isStrongAttackResult = true;
	}

	// ターゲットが防御している場合
	if (m_targetBattleState == BattleState::GUARD)
	{
	//	printfDx("ガードだよ\n");
	}
}

