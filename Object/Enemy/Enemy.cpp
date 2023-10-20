#include "Enemy.h"
#include "../../Util/Pad.h"

Enemy::Enemy(VECTOR pos) :
	CharacterBase(pos)
{

}

Enemy::~Enemy()
{
}

void Enemy::Input()
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

	SetPlayerOnlyAngle(m_angle);

	// プレイヤーの進行方向
	MATRIX rotMtx = MGetRotY(m_angle);

	SetRotMtx(rotMtx);

	if (GetRand(100) == 0 && !m_isGuard)
	{
		m_isAttack = true;
		m_pFunc = &Enemy::Attack;
	}

	if (GetRand(10) == 0 && !m_isAttack)
	{
		m_isGuard = true;
		m_pFunc = &Enemy::Guard;
	}
	else
	{
		m_isGuard = false;
	}
}

void Enemy::SetTargetPos(VECTOR pos)
{
	m_targetPos = pos;
}
