#include <corecrt_math.h>

#include "Player.h"

#include "../../Util/Pad.h"

#include "../../CSVData/SoundManager.h"
#include "../../Util/SoundName.h"
#include "../../Util/HitPos.h"


namespace
{
	// プレイヤーの移動量
	constexpr VECTOR kVecZ{ 0.0f,0.0f,-10.0f };
	constexpr VECTOR kVecZP{ 0.0f,0.0f,10.0f };
	constexpr VECTOR kVecX{ -10.0f,0.0f,0.0f };
	constexpr VECTOR kVecXP{ 10.0f,0.0f,0.0f };

	// 回避
	constexpr VECTOR kVecAwayZ{ 0.0f,0.0f,-25.0f };

	// 移動速度
	constexpr float kMoveSpeed = 10.0f;
}

Player::Player(DifficultyData data,VECTOR pos):
	CharacterBase(data,pos),
	m_awayVec(VGet(0,0,0)),
	m_awayRelativePos(VGet(0,0,0)),
	m_isUp(false),
	m_isDown(false),
	m_isLeft(false),
	m_isRight(false),
	m_isCameraLockon(false),
	m_isMove(false)
{
	// 初期待機状態で停止
	m_pFunc = &Player::Idle;

	m_isAttack = false;
	m_isGuard  = false;
	m_isJustGuard = false;

	m_isAway = false;

	m_angle = 0.0f;

	// 自身がプレイヤーであると決める
	m_myId = CharacterName::PLAYER;

	for (int i = 0; i < static_cast<int>(Tips::MAX); i++)
	{
		m_isTipsMove[i] = false;
	}
	m_isTipsMove[static_cast<int>(Tips::MOVE)] = true;

	m_parameter.fileName = "Data/Model/Knight_W.mv1";
	// パラメーター調整
	m_parameter.attackFrameMax = 10;
	m_parameter.attackFrameGapMax = 20;
	m_parameter.attackTotalFrame = m_parameter.attackFrameMax + m_parameter.attackFrameGapMax;

	m_parameter.attackAfterStopFrameMax = 60;

	m_parameter.strongAttackFrameMax = 60;
	m_parameter.strongAttackFrameGapMax = 25;
	m_parameter.strongAttackTotalFrame = m_parameter.strongAttackFrameMax + m_parameter.strongAttackFrameGapMax;

	m_parameter.guardFrameMax = 5;
	m_parameter.justGuardFrameMax = 5;

	m_parameter.stunFrameMax = 60 * 3;

	m_parameter.hpMax = 6;
	m_parameter.fightingMeterMax = 100;

	m_parameter.weaponRelativePos = { -80.0f, 100.0f, 0.0f };
	m_parameter.shieldRelativePos = { 100.0f, 100.0f, 10.0f };

	m_parameter.weaponAttackRadius = 100.0f;
	m_parameter.shieldRadius = 50.0f;
	m_parameter.modelRadius = 180.0f;

	m_parameter.weaponAttackPos = { 0.0f, 0.0f, -210.0f };
	m_parameter.knockBackPos = { 0.0f,0.0f ,-20.0f };

	m_parameter.weaponBackSpeed = 30.0f;
	m_parameter.shieldBackSpeed = 30.0f;
}

Player::~Player()
{
}

void Player::Input()
{
	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &m_padInput);

	// 向きを指定
	Direction();

	// スタンしていなかったら
	if (!IsStun())
	{
		// 移動
		InputMove();
		// 攻撃
		InputAttack();
		// 防御
		InputGuard();
	}

#if _DEBUG
	{
		// カメラの操作変更
		static int frameCount = 0;
		if (m_padInput.Buttons[9] == 128)
		{
			frameCount++;
			if (frameCount == 1)
			{
				m_isCameraLockon = (!m_isCameraLockon);
			}
		}
		else
		{
			frameCount = 0;
		}
	}
#endif
}

void Player::InputTutorial()
{
	// 向きを指定
	Direction();

	// 動いた場合攻撃を有効にする
	if (m_isMove)
	{
		m_isTipsMove[static_cast<int>(Tips::ATTACK)] = true;
	}

	// 動けるかどうか
	if (m_isTipsMove[static_cast<int>(Tips::MOVE)])
	{
		InputMove();
	}

	// 攻撃ができるかどうか
	if (m_isTipsMove[static_cast<int>(Tips::ATTACK)])
	{
		InputAttack();
		if (m_isAttack)
		{
			m_isTipsMove[static_cast<int>(Tips::GUARD)] = true;
		}
	}

	// ガードできるかどうか
	if (m_isTipsMove[static_cast<int>(Tips::GUARD)])
	{
		InputGuard();
	}

}

void Player::Direction()
{
	if (m_isCameraLockon)
	{
		// カメラの回転角度を調整
		if (m_padInput.Rx > 30)
		{
			m_angle += 0.05f;
		}
		if (m_padInput.Rx < -30)
		{
			m_angle -= 0.05f;
		}
	}
	else
	{
		const VECTOR direction = VSub(m_targetPos, m_pos);
		m_angle = atan2f(-direction.x, -direction.z);
	}
	// angleを基底クラスに渡す
	SetAngle(m_angle);
	// プレイヤーの進行方向
	m_platerRotMtx = MGetRotY(m_angle);
	// 回転行列を基底クラスに渡す
	SetRotMtx(m_platerRotMtx);
}

void Player::InputMove()
{
	// 移動or回避
	if (m_isAway)
	{
		static VECTOR away = kVecAwayZ;
		static int frameCount = 0;
		int frameCountMax = 10;

		if (frameCount < frameCountMax)
		{
			float t = static_cast<float>(frameCount) / frameCountMax;
			m_awayVec.x = m_awayRelativePos.x * t;
			m_awayVec.z = m_awayRelativePos.z * t;

			// 移動ベクトルを正規化
			VECTOR moveVector = VTransform(kVecX, m_platerRotMtx);  // 例としてX方向に移動する場合
			moveVector = VNorm(moveVector);

			// 速度係数
			const float speedFactor = 1.0f;  // 適切な値を設定してください

			// 速度ベクトルに速度係数を掛ける
			VECTOR velocity = VAdd(moveVector, VGet(speedFactor, speedFactor, speedFactor));

			// キャラクターの位置に速度を加える
			m_pos = VAdd(m_pos, velocity);
		}
		else
		{
			m_isAway = false;
			frameCount = 0;
		}
	}
	else
	{
		m_isUp = false;
		m_isDown = false;
		m_isRight = false;
		m_isLeft = false;

		if (Pad::IsPress(PAD_INPUT_UP))
		{
			m_isUp = true;
			MoveCharacter(VTransform(kVecZ, m_platerRotMtx));
			MoveAway(0.0f, -60.0f, m_platerRotMtx);
		}
		else if (Pad::IsPress(PAD_INPUT_DOWN))
		{
			m_isDown = true;
			MoveCharacter(VTransform(kVecZP, m_platerRotMtx));
			MoveAway(0.0f, 60.0f, m_platerRotMtx);
		}
		if (Pad::IsPress(PAD_INPUT_RIGHT))
		{
			m_isRight = true;
			MoveCharacter(VTransform(kVecX, m_platerRotMtx));
			MoveAway(-60.0f, 0.0f, m_platerRotMtx);
		}
		else if (Pad::IsPress(PAD_INPUT_LEFT))
		{
			m_isLeft = true;
			MoveCharacter(VTransform(kVecXP, m_platerRotMtx));
			MoveAway(60.0f, 0.0f, m_platerRotMtx);
		}

		if (!(m_isUp) && !(m_isDown) && !(m_isLeft) && !(m_isRight))
		{
			MoveAway(0.0f, 60.0f, m_platerRotMtx);
		}
	}
}

void Player::InputAttack()
{
	// 通常攻撃
	if (Pad::IsTrigger(PAD_INPUT_6) &&
		!m_isAttack &&
		!m_isStrongAttack &&
		!m_isGuard &&
		!m_isJustGuard)
	{
		m_isAttack = true;
		m_comboAttack++;
		if (m_comboAttack == 1)
		{
			m_pFunc = &Player::Attack;
		}
	}

	if (m_difficultyData != DifficultyData::NOIVE)
	{
		// 強攻撃
		if (m_padInput.Z < -100 &&
			!m_isAttack &&
			!m_isStrongAttack &&
			!m_isGuard &&
			!m_isJustGuard &&
			(GetStrongPower() >= 100.0f))
		{
			m_isStrongAttack = true;
			m_pFunc = &Player::StrongAttack;
		}
	}
}

void Player::InputGuard()
{
	// 防御
	if (Pad::IsPress(PAD_INPUT_5))
	{
		if (!m_isAttack && !m_isStrongAttack && !m_isJustGuard && !m_isJustGuard)
		{
			// 
			//SetStrongPowerReset();
			m_isGuard = true;
			m_pFunc = &Player::Guard;
		}
	}
	else
	{
		m_isGuard = false;
	}
}
void Player::MoveCharacter(VECTOR moveVector)
{
	if (!m_isMove)
	{
		m_isMove = true;
	}

	// 正規化
	moveVector = VNorm(moveVector);

	// 速度係数
	float speedFactor = kMoveSpeed;
	if (m_isGuard)
	{
		// ガード中は速度を半分に
		speedFactor = speedFactor / 2.0f;
	}

	if (GetCheckHitWall())
	{		
		if (HitPos::ZP == m_hitPos)
		{
			if(moveVector.z < 0.0f)
			{
				moveVector.z = 0.0f;
			}
		}
		if (HitPos::XP == m_hitPos)
		{
			if (moveVector.x < 0.0f)
			{
				moveVector.x = 0.0f;
			}
		}
	}

	// 速度ベクトルに速度係数を掛ける
	VECTOR velocity = VScale(moveVector, speedFactor);

	// キャラクターの位置に速度を加える
	m_pos = VAdd(m_pos, velocity);
}

VECTOR Player::AddMoving(const VECTOR RelativePos, const MATRIX rotMtx, const VECTOR pos)
{
	VECTOR move = VTransform(RelativePos, rotMtx);
	move = VAdd(pos, move);
	return move;
}

VECTOR Player::SubMoving(const VECTOR RelativePos, const MATRIX rotMtx, const VECTOR pos)
{
	VECTOR move = VTransform(RelativePos, rotMtx);
	move = VSub(m_pos, move);
	return move;
}

void Player::MoveAway(float x, float z, MATRIX rotMtx)
{
	// 回避の仮実装
	{
		if (Pad::IsTrigger(PAD_INPUT_3))
		{
			SoundManager::GetInstance().Play(SoundName::SPEEDMOVE);
			m_isAway = true;
			m_awayRelativePos.x = x;
			m_awayRelativePos.z = z;

			// 速度ベクトルを設定
			m_awayVec.x = x;
			m_awayVec.y = 0.0f;
			m_awayVec.z = z;
		}
	}
}
