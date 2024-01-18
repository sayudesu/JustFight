#include "Enemy.h"

#include "../../Util/Pad.h"

#include "../../CharactorDataManager.h"
#include "../../ParameterData.h"

#include "../../ModelManager.h"
#include "../../ModelName.h"

namespace
{
	// 角度情報をフレーム単位で遅らせる
	constexpr int kDelayFrameAngle = 18;
	constexpr int kGuardFrameCountMax = 60 * 2;
}

namespace
{
	// プレイヤーの移動量
	constexpr VECTOR kVecZ{ 0.0f,0.0f,-10.0f };
	constexpr VECTOR kVecX{ -10.0f,0.0f,0.0f };

	// 回避
	constexpr VECTOR kVecAwayZ{ 0.0f,0.0f,-25.0f };
}


Enemy::Enemy(DifficultyData data,VECTOR pos) :
	CharacterBase(data,pos),
	m_isCheckGuard(false),
	m_isResultGuard(false),
	m_isCheckAttack(false),
	m_isAttackResult(false),
	m_isCheckStrongAttack(false),
	m_isStrongAttackResult(false),
	m_isMoveLeft(false),
	m_isMoveRight(false)
{
	m_pFunc = &Enemy::Idle;

	m_isAttack = false;
	m_isGuard = false;

	m_attackContinueDelayFrameCount[0] = 0;
	m_attackContinueDelayFrameCount[1] = 0;
	m_isAttackContinueDelay[0] = false;
	m_isAttackContinueDelay[1] = false;

	// 自身がエネミーであると決める
	m_myId = CharacterName::ENEMYNORMAL;

	// 選択した難易度によってモデルを変更
	if (data == DifficultyData::NOIVE)
	{		
		m_parameter.fileName = ModelManager::GetInstance().ModelType(ModelName::Pawn_B);
		InputParamType(0);
	}
	else if (data == DifficultyData::INTERMEDIATE)
	{
		m_parameter.fileName = ModelManager::GetInstance().ModelType(ModelName::Knight_B);
		InputParamType(1);
	}
	else if (data == DifficultyData::EXPERT)
	{
		m_parameter.fileName = ModelManager::GetInstance().ModelType(ModelName::Queen_B);
		InputParamType(2);
	}
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
#if true	
	if (!IsStun())
	{
		// 一定距離近づくとランダムで左右移動を始める
		if (m_targetRange.x + m_targetRange.z < 1000.0f)
		{
			MoveLeftAndRight(rotMtx);
			//m_isGuard = false;
		}

		// 攻撃可能範囲に入っているかどうか
		if (IsAttackRange())
		{
			static int guardFrameCount = 0;		

			// 攻撃or防御していなかったら次の行動を決める
			if (!m_isAttack && !m_isGuard && !m_isStrongAttack)
			{
				// ターゲットの状態で動作を変更する
				BattleType();
			}

			// 攻撃かどうか
			if (m_isAttackResult)
			{
				m_isAttackResult = false;
				m_isAttack = true;
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
				m_pFunc = &Enemy::StrongAttack;
			}
			// 強攻撃をしていない場合
			if (!m_isCheckStrongAttack)
			{
				m_isCheckStrongAttack = false;
			}

			if (m_isResultGuard && !m_isAttack)
			{
				m_isGuard = true;
				guardFrameCount++;
				m_pFunc = &Enemy::Guard;		
			}
			if (guardFrameCount == kGuardFrameCountMax)
			{
				guardFrameCount = 0;
				m_isCheckGuard = false;
				m_isGuard = false;
				m_isResultGuard = false;
			}

		}
		else
		{
			TargetMove();

			m_isGuard = false;
			m_isCheckGuard = false;
			m_isResultGuard = false;

			m_isAttack = false;
			m_isCheckAttack = false;
			m_isAttackResult = false;
		}
	}
	else
	{
		m_isGuard = false;
		m_isCheckGuard = false;
		m_isResultGuard = false;

		m_isAttack = false;
		m_isCheckAttack = false;
		m_isAttackResult = false;
	}
#endif
}

void Enemy::MoveLeftAndRight(MATRIX mtxRot)
{
	if (GetRand(100) == 0)
	{
		m_isMoveRight = true;
		m_isMoveLeft = false;
	}
	if (GetRand(100) == 1)
	{
		m_isMoveLeft = true;
		m_isMoveRight = false;
	}
	if (GetRand(100) == 2)
	{
		m_isMoveLeft = false;
		m_isMoveRight = false;
	}
	int moveType = 0;
	if (m_isMoveLeft)
	{
		moveType = 1;
	}
	if (m_isMoveRight)
	{
		moveType = -1;
	}

	const VECTOR move = VTransform(VGet(moveType * 10, 0, 0), mtxRot);
	m_pos = VAdd(m_pos, move);
}

void Enemy::BattleType()
{
	// ターゲットの状態で動作を変更する
	// ターゲットが攻撃している場合
	if (m_targetBattleState == BattleState::ATTACK)
	{
		if (GetRand(5) == 0)
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
		if (GetRand(30) == 0 && (!m_isAttackContinueDelay[0]))
		{
			m_isCheckAttack = true;
			m_isAttackResult = true;

			m_isAttackContinueDelay[0] = true;
		}
	}
	// アイドル状態の場合
	if (m_targetBattleState == BattleState::IDLE)
	{
		if (!m_isAttackContinueDelay[1])
		{
			m_isCheckAttack = true;
			m_isAttackResult = true;

			m_isAttackContinueDelay[1] = true;
		}
	}

	// 攻撃を1フレーム以内に出さない様に修正
	// これではいけない...
	if (m_isAttackContinueDelay[0] || m_isAttackContinueDelay[1])
	{
		m_isAttackContinueDelay[0] = true;
		m_isAttackContinueDelay[1] = true;
	}

	for (int i = 0; i < 2; i++)
	{
		if (m_isAttackContinueDelay[i])
		{
			m_attackContinueDelayFrameCount[i]++;
			if (m_attackContinueDelayFrameCount[i] == 2)
			{
				m_attackContinueDelayFrameCount[i] = 0;
				m_isAttackContinueDelay[i] = false;
			}
		}		
	}
}

