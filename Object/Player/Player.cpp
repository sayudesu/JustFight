#include "Player.h"
#include "../../Util/Pad.h"

namespace
{
	// プレイヤーの移動量
	constexpr VECTOR kVecZ{ 0.0f,0.0f,-10.0f };
	constexpr VECTOR kVecX{ -10.0f,0.0f,0.0f };

	// 回避
	constexpr VECTOR kVecAwayZ{ 0.0f,0.0f,-25.0f };
}

Player::Player(VECTOR pos):
	CharacterBase(pos)
{
	m_pFunc = &Player::Idle;

	m_isAttack = false;
	m_isGuard  = false;
	m_isJustGuard = false;
	m_isJustGuardBreak = false;

	m_angle = 0.0f;
}

Player::~Player()
{
}

void Player::Input()
{
	DINPUT_JOYSTATE input;
	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	// カメラの回転角度を調整
	if (input.Rx > 30)
	{
		m_angle += 0.05f;
	}
	if (input.Rx < -30)
	{
		m_angle -= 0.05f;
	}

	SetAngle(m_angle);

	// プレイヤーの進行方向
	MATRIX rotMtx = MGetRotY(m_angle);

	if (Pad::isPress(PAD_INPUT_UP))
	{
		const VECTOR move = VTransform(kVecZ, rotMtx);
		m_pos = VAdd(m_pos, move);
	}
	else if (Pad::isPress(PAD_INPUT_DOWN))
	{
		const VECTOR move = VTransform(kVecZ, rotMtx);
		m_pos = VSub(m_pos, move);
	}
	if (Pad::isPress(PAD_INPUT_RIGHT))
	{
		const VECTOR move = VTransform(kVecX, rotMtx);
		m_pos = VAdd(m_pos, move);
	}
	else if (Pad::isPress(PAD_INPUT_LEFT))
	{
		const VECTOR move = VTransform(kVecX, rotMtx);
		m_pos = VSub(m_pos, move);
	}
	SetRotMtx(rotMtx);

	if (!m_isJustGuardBreak)
	{
		//if (GetStamina() > 20.0f)
		{
			if (Pad::isTrigger(PAD_INPUT_6) &&
				!m_isAttack &&
				!m_isGuard)
			{
				m_isAttack = true;
				m_pFunc = &Player::Attack;
			}

			if (Pad::isPress(PAD_INPUT_5) && !m_isAttack)
			{
				m_isGuard = true;
				m_pFunc = &Player::Guard;
			}
			else
			{
				m_isGuard = false;
			}
		}
		//else
		//{
		//	m_isGuard = false;
		//}
	}

	// 回避の仮実装
	{
		static VECTOR away = kVecAwayZ;
		static float isAway = false;

		if (Pad::isTrigger(PAD_INPUT_3))
		{
			isAway = true;
		
		}

		if (isAway)
		{
			if (away.z < 0.0f)
			{
				away.z += 1.0f;
			}
			else
			{
				away = kVecAwayZ;
				isAway = false;
			}
			VECTOR move = VTransform(away, rotMtx);
			m_pos = VSub(m_pos, move);
		}
	}
}

float Player::GetAngle()const
{
	return m_angle;
}

void Player::MoveAway()
{
}
