#include "CharacterBase.h"
#include "../Util/MyLoadModel.h"
#include <cmath>

namespace
{
	// フレーム管理
	// 攻撃最大フレーム
	constexpr int kAttackFrameMax = 40;
	// ガード最大フレーム
	constexpr int kGuardFrameMax  = 15;
	// ジャストガード最大フレーム
	constexpr int kJustGuardFrameMax = 12;

	// 最大体力
	constexpr int kHpMax = 6;
	// 最大スタミナ
	constexpr float kStaminaMax = 100.0f;

	// 装備の相対位置
	const VECTOR kWeaponPos = { -80.0f, 100.0f, 0.0f };
	const VECTOR kSieldPos = { 100.0f, 100.0f, -50.0f };
}

CharacterBase::CharacterBase(VECTOR pos):
	m_pFunc(nullptr),
	m_lanceHnadle(-1),
	m_shieldHnadle(-1),
	m_pos(pos),
	m_vec(VGet(0,0,0)),
	m_vecWeapon(VGet(0, 0, 0)),
	m_vecSield(VGet(0, 0, 0)),
	m_hp(kHpMax),
	m_stamina(kStaminaMax),
	m_isAttack(false),
	m_isGuard(false),
	m_isJustGuard(false),
	m_attackFrame(0),
	m_guardFrame(0),
	m_justGuardFrame(0)
{
	m_pFunc = &CharacterBase::Idle;
}

CharacterBase::~CharacterBase()
{
}

void CharacterBase::Init()
{
	m_lanceHnadle  = MyModel3D::Load("Data/Model/Lance2.mv1");
	m_shieldHnadle = MyModel3D::Load("Data/Model/Shield.mv1");

	{
		m_vecWeapon = kWeaponPos;
		VECTOR move = VTransform(m_vecWeapon, m_rotMtx);
		move = VAdd(m_pos, move);
		MV1SetPosition(m_lanceHnadle, move);
		MV1SetRotationXYZ(m_lanceHnadle,  VGet(0.0f, 0.0f,0.0f));
	}
	{
		m_vecSield = kSieldPos;
		VECTOR move = VTransform(m_vecSield, m_rotMtx);
		move = VAdd(m_pos, move);
		MV1SetPosition(m_shieldHnadle, move);
		MV1SetRotationXYZ(m_shieldHnadle, VGet(0, -90 * DX_PI_F / 180.0f, 0));
		MV1SetScale(m_shieldHnadle, VGet(3, 3, 3));
	}

}

void CharacterBase::End()
{
	MyModel3D::End(m_lanceHnadle);
	MyModel3D::End(m_shieldHnadle);
}

void CharacterBase::Update()
{
	(this->*m_pFunc)();
}

void CharacterBase::Input()
{
}

void CharacterBase::Idle()
{
	SetStamina(0.2f, 0.0f);

	{
		VECTOR move = VTransform(m_vecWeapon, m_rotMtx);
		move = VAdd(m_pos, move);
		MV1SetPosition(m_lanceHnadle, move);
		MV1SetRotationXYZ(m_lanceHnadle, VGet(0.0f, m_angle,0.0f));
	}
	{
		VECTOR move = VTransform(m_vecSield, m_rotMtx);
		move = VAdd(m_pos, move);
		MV1SetPosition(m_shieldHnadle, move);
		MV1SetRotationXYZ(m_shieldHnadle, VGet(0.0f, m_angle, 0.0f));
	}
}

void CharacterBase::Attack()
{
	SetStamina(0.0f, 1.0f);
	// 攻撃フレームの制御
	if (m_attackFrame < kAttackFrameMax)
	{
		m_attackFrame++;
	}
	else
	{
		m_attackFrame = 0;
		m_isAttack = false;
		m_vecWeapon.z = 0.0f;
		m_vecWeapon.x = -80.0f;
		m_pFunc = &CharacterBase::Idle;
	}

	// 攻撃フレーム後半から攻撃開始
	if (m_attackFrame > kAttackFrameMax / 2)
	{
		// 武器モデルの移動
		if(m_vecWeapon.z > -200.0f)
		{
			m_vecWeapon.z -= 50.0f;
		}
		if (m_vecWeapon.x < -40.0f)
		{
			m_vecWeapon.x += 10.0f;
		}
	}
	else
	{
		m_vecWeapon.z += 1.5f;
	}

	{
		VECTOR move = VTransform(m_vecWeapon, m_rotMtx);
		move = VAdd(m_pos, move);
		MV1SetPosition(m_lanceHnadle, move);
		MV1SetRotationXYZ(m_lanceHnadle, VGet(0.0f, m_angle,0.0f));
	}
	{
		VECTOR move = VTransform(m_vecSield, m_rotMtx);
		move = VAdd(m_pos, move);
		MV1SetPosition(m_shieldHnadle, move);
		MV1SetRotationXYZ(m_shieldHnadle, VGet(0.0f, m_angle, 0.0f));
	}
}

void CharacterBase::Guard()
{
	SetStamina(0.0f, 0.0f);

	if (m_vecSield.x > 0.0f)
	{
		m_vecSield.x -= 15.0f;
	}
	else
	{
		m_vecSield.x = 0.0f;
	}

	if (m_guardFrame < kGuardFrameMax)
	{
		m_guardFrame++;

		// ジャストガードのフレーム
		// + 1 はジャストガードの範囲外を示す
		if (m_justGuardFrame < kJustGuardFrameMax + 1)
		{
			m_justGuardFrame++;
		}
	}
	else
	{
		m_guardFrame = kGuardFrameMax;
	}
	
	if (!m_isGuard)
	{
		m_guardFrame = 0;
		m_justGuardFrame = 0;
		m_vecSield.x = 100.0f;
		m_pFunc = &CharacterBase::Idle;
	}

	{
		VECTOR move = VTransform(m_vecWeapon, m_rotMtx);
		move = VAdd(m_pos, move);
		MV1SetPosition(m_lanceHnadle, move);
		MV1SetRotationXYZ(m_lanceHnadle, VGet(0.0f, m_angle, 0.0f));
	}
	{
		VECTOR move = VTransform(m_vecSield, m_rotMtx);
		move = VAdd(m_pos, move);
		MV1SetPosition(m_shieldHnadle, move);
		MV1SetRotationXYZ(m_shieldHnadle, VGet(0.0f, m_angle, 0.0f));
	}
}

void CharacterBase::JustGuard()
{
	m_isJustGuard = false;
	m_vecWeapon = kWeaponPos;
	m_vecSield = kSieldPos;
	m_pFunc = &CharacterBase::Idle;
	{
		VECTOR move = VTransform(m_vecWeapon, m_rotMtx);
		move = VAdd(m_pos, move);
		MV1SetPosition(m_lanceHnadle, move);
		MV1SetRotationXYZ(m_lanceHnadle, VGet(0.0f, m_angle, 0.0f));
	}
	{
		VECTOR move = VTransform(m_vecSield, m_rotMtx);
		move = VAdd(m_pos, move);
		MV1SetPosition(m_shieldHnadle, move);
		MV1SetRotationXYZ(m_shieldHnadle, VGet(0.0f, m_angle, 0.0f));
	}
}


void CharacterBase::Draw()
{
	// 体
	DrawCapsule3D(m_pos, VGet(m_pos.x, m_pos.y + 200.0f, m_pos.z), 40.0f, 8, 0x00ff00, 0xffffff, true);
	// 武器
	MV1DrawModel(m_lanceHnadle);
	// 盾
	MV1DrawModel(m_shieldHnadle);
}

VECTOR CharacterBase::GetPos()
{
	return m_pos;
}

void CharacterBase::SetAngle(float angle)
{
	m_angle = angle;
}

void CharacterBase::SetRotMtx(MATRIX rotMtx)
{
	m_rotMtx = rotMtx;
}

int CharacterBase::GetAttackFrame()const
{
	return m_attackFrame;
}

int CharacterBase::GetGuardFrame()const
{
	return m_guardFrame;
}

int CharacterBase::GetJustGuardFrame()const
{
	return m_justGuardFrame;
}

int CharacterBase::GetAttackFrameMax()const
{
	return kAttackFrameMax;
}

int CharacterBase::GetGuardFrameMax()const
{
	return kGuardFrameMax;
}

int CharacterBase::GetJustGuardFrameMax()const
{
	return kJustGuardFrameMax;
}

int CharacterBase::GetHp()const
{
	return m_hp;
}

int CharacterBase::GetStamina()const
{
	return m_stamina;
}

void CharacterBase::SetDamage(bool isDamage)
{
	if (isDamage)
	{
		m_hp--;
	}
}

void CharacterBase::SetJustGuard(bool isJustGuard)
{
	if (isJustGuard)
	{
		m_isJustGuard = isJustGuard;
		m_pFunc = &CharacterBase::JustGuard;
	}
}

void CharacterBase::SetStamina(float addStamina,float subStamina)
{
	// 最大値と最小値を超えないように制御
	if (m_stamina < kStaminaMax)
	{
		m_stamina += addStamina;
	}
	else
	{
		m_stamina = kStaminaMax;
	}
	if (m_stamina > 0.0f)
	{
		m_stamina -= subStamina;
	}
	else
	{
		m_stamina = 0.0f;
	}
}
