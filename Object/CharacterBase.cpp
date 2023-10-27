#include "CharacterBase.h"
#include "../Util/MyLoadModel.h"
#include <cmath>

namespace
{
	// フレーム管理
	// 攻撃最大フレーム
	constexpr int kAttackFrameMax = 5;
	constexpr int kAttackGapFrameMax = 20;
	constexpr int kAttackTotalFrame = kAttackFrameMax + kAttackGapFrameMax;

	// ガード最大フレーム
	constexpr int kGuardFrameMax  = 12;
	// ジャストガード最大フレーム
	constexpr int kJustGuardFrameMax = 7;

	// 最大体力
	constexpr int kHpMax = 6;
	// 最大スタミナ
	constexpr float kStaminaMax = 100.0f;

	// 装備の相対位置
	const VECTOR kWeaponPos = { -80.0f, 100.0f, 0.0f };
	const VECTOR kSieldPos  = { 100.0f, 100.0f, -50.0f };

	// モデルの当たり判定用範囲
	constexpr float kModelRadus = 180.0f;

	// 攻撃時の当たり判定
	// 武器の半径
	constexpr float kWeaponAttackRadius = 60.0f;
	// 武器の攻撃時の判定の相対位置
	const VECTOR kWeaponAttackPos = { 0.0f, 0.0f, -300.0f };
	// 盾の半径
	constexpr float kSieldRadius = 50.0f;

	// ガードしている時に攻撃を受けた場合のノックバックで移動する相対位置
	const VECTOR kKnockBackPos = { 0.0f,0.0f ,-20.0f };

	// 剣が元の位置に戻る速度
	constexpr float kWeaponBackSpeed = 30.0f;
	// 盾が元の位置に戻る速度
	constexpr float kSieldBackSpeed  = 30.0f;
}

CharacterBase::CharacterBase(VECTOR pos):
	m_pFunc(nullptr),
	m_weaponHandle(-1),
	m_shieldHandle(-1),
	m_rotMtx({}),
	m_enemyRotMtx({}),
	m_pos(pos),
	m_vec(kKnockBackPos),
	m_targetPos(VGet(0,0,0)),
	m_vecWeapon(VGet(0, 0, 0)),
	m_vecSield(VGet(0, 0, 0)),
	m_tempWeaponPos(VGet(0, 0, 0)),
	m_hp(kHpMax),
	m_stamina(kStaminaMax),
	m_angle(0.0f),
	m_isAttack(false),
	m_isGuard(false),
	m_isJustGuard(false),
	m_isJustGuardBreak(false),
	m_isAway(false),
	m_isResultGuard(false),
	m_isResultDamage(false),
	m_isStun(false),
	m_isChanceAway(false),
	m_isSlow(false),
	m_slowCount(0),
	m_attackFrame(0),
	m_attackGapFrame(0),
	m_guardFrame(0),
	m_justGuardFrame(0),
	m_justGuardBreakFrame(0)
{
	m_pFunc = &CharacterBase::Idle;
}

CharacterBase::~CharacterBase()
{
}

void CharacterBase::Init()
{
	m_weaponHandle  = MyModel3D::Load("Data/Model/Lance2.mv1");
	m_shieldHandle  = MyModel3D::Load("Data/Model/Shield.mv1");

	m_vecWeapon = kWeaponPos;
	m_vecSield = kSieldPos;

	// 位置情報の更新
	UpdatePos();

	MV1SetScale(m_shieldHandle, VGet(3.0f, 3.0f, 3.0f));
}

void CharacterBase::End()
{
	MyModel3D::End(m_weaponHandle);
	MyModel3D::End(m_shieldHandle);
}

void CharacterBase::Update()
{
	(this->*m_pFunc)();
}

void CharacterBase::Draw()
{
	// 体
	DrawCapsule3D(m_pos, VGet(m_pos.x, m_pos.y + 200.0f, m_pos.z), 40.0f, 8, 0x00ff00, 0xffffff, true);
	// 武器
	MV1DrawModel(m_weaponHandle);
	// 盾
	MV1DrawModel(m_shieldHandle);
}

void CharacterBase::TargetMove()
{
	// 向きを算出
	VECTOR m_dir = VSub(m_targetPos, m_pos);

	// プレイヤーからエネミーまでの角度を求める
	const float angle = atan2(m_dir.y, m_dir.x);

	// 現在敵が向いている方向のベクトルを生成する
	const MATRIX enemyRotMtx = MGetRotY(angle);

	// 斜めになったとき((1, 1, 0)など)にいったん長さ１に戻す(正規化)
	if (VSquareSize(m_dir) > 0)
	{
		m_dir = VNorm(m_dir);
	}

	// 速度を求める
	const VECTOR velecity = VScale(m_dir, 5.0f);

	// 位置を変える
	m_pos = VAdd(m_pos, velecity);

	// 距離を測る
	m_targetRange.x = static_cast<float>(sqrt(pow(m_pos.x - m_targetPos.x, 2) + pow(m_pos.x - m_targetPos.x, 2)));
	m_targetRange.z = static_cast<float>(sqrt(pow(m_pos.z - m_targetPos.z, 2) + pow(m_pos.z - m_targetPos.z, 2)));
}

void CharacterBase::Idle()
{
	SetStamina(0.2f, 0.0f);

	// 武器を元の位置に戻す
	{
		bool isEndX = false;
		bool isEndZ = false;
		if (m_vecWeapon.x > kWeaponPos.x)
		{
			m_vecWeapon.x -= 10.0f;
		}
		else
		{
			isEndX = true;
			m_vecWeapon.x = kWeaponPos.x;
		}

		if (m_vecWeapon.z < kWeaponPos.z)
		{
			m_vecWeapon.z += 30.0f;
		}
		else
		{
			isEndZ = true;
			m_vecWeapon.z = kWeaponPos.z;
		}
		if (isEndX && isEndZ)
		{
			m_isAttack = false;
		}

		if (m_vecWeapon.y < kWeaponPos.y)
		{
			m_vecWeapon.y -= 10.0f;
		}
		else
		{
			m_vecWeapon.y = kWeaponPos.y;
		}
	}
	// 盾を元の位置に戻す
	{
		if (m_vecSield.x < kSieldPos.x)
		{
			m_vecSield.x += 30.0f;
		}
		else
		{
			m_vecSield.x = kSieldPos.x;
		}

		if (m_vecSield.y < kSieldPos.y)
		{
			m_vecSield.y -= 30.0f;
		}
		else
		{
			m_vecSield.y = kSieldPos.y;
		}
	}

	if (m_hp == 0)
	{
		m_pFunc = &CharacterBase::Losers;
	}

	if (m_isResultDamage)
	{
		m_isResultDamage = false;
		m_hp--;
	}

	if (m_isChanceAway)
	{
		m_isChanceAway = false;
		if (m_isAway)
		{
			m_isSlow = true;
		}
	}

	if (m_isSlow)
	{
		m_slowCount++;
		if (m_slowCount == 60 * 3)
		{
			m_slowCount = 0;
			m_isSlow = false;
		}
	}

	// 位置情報の更新
	UpdatePos();
}

void CharacterBase::Attack()
{
	SetStamina(0.0f, 1.0f);

	// 最大フレーム内に目標地点まで移動する
	// 始めに隙ようの後ろに動かす動作
	if (m_attackGapFrame < kAttackGapFrameMax)
	{
		m_vecWeapon.z = kWeaponPos.z + (100.0f - kWeaponPos.z) * (m_attackGapFrame) / kAttackGapFrameMax;
		m_attackGapFrame++;
		m_tempWeaponPos = m_vecWeapon;
	}
	// 攻撃をする動作
	if (m_attackGapFrame == kAttackGapFrameMax)
	{
		if (m_attackFrame < kAttackFrameMax)
		{
			m_vecWeapon.z = m_tempWeaponPos.z + (-200.0f - m_tempWeaponPos.z) * (m_attackFrame) / kAttackFrameMax;
			m_vecWeapon.x = kWeaponPos.x + (0.0f - kWeaponPos.x) * (m_attackFrame) / kAttackFrameMax;
			m_attackFrame++;
		}
	}

	if (m_attackFrame + m_attackGapFrame == kAttackTotalFrame)
	{
		m_attackFrame = 0;
		m_attackGapFrame = 0;
		m_pFunc = &CharacterBase::Idle;
	}

	if (m_isJustGuardBreak)
	{
		// スタン状態に上書き
		m_isStun = true;
		m_pFunc = &CharacterBase::JustGuardBreak;
		printfDx("スタン中\n");
	}


	// 位置情報の更新
	UpdatePos();
}

void CharacterBase::Guard()
{
	SetStamina(0.05f, 0.0f);

	// 最大フレーム内に目標地点まで移動する
	if (m_guardFrame < kGuardFrameMax)
	{
		m_vecSield.x = kSieldPos.x + (0.0f - kSieldPos.x) * (float(m_guardFrame) / kGuardFrameMax);
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
		m_vecSield.x = 0.0f;
	}

	// ガードが成功したら
	// 後ろにノックバック
	if (m_isResultGuard)
	{
		if (m_vec.z < 0.0f)
		{			
			m_vec.z += 1.0f;
			VECTOR move = VTransform(m_vec, m_enemyRotMtx);
			m_pos = VAdd(m_pos, move);
		}
		else
		{
			m_vec = kKnockBackPos;
			m_isResultGuard = false;
		}
	}

	// ガードをやめた場合
	if (!m_isGuard)
	{
		m_guardFrame = 0;
		m_justGuardFrame = 0;
		m_pFunc = &CharacterBase::Idle;
	}

	if (m_isJustGuardBreak)
	{
		// スタン状態に上書き
		m_isStun = true;
		m_pFunc = &CharacterBase::JustGuardBreak;
		printfDx("スタン中\n");
	}


	// 位置情報の更新
	UpdatePos();
}

void CharacterBase::JustGuard()
{
//	m_isJustGuard = false;
	m_vecWeapon = kWeaponPos;
	m_vecSield = kSieldPos;
	m_pFunc = &CharacterBase::Idle;

	// 位置情報の更新
	UpdatePos();
}

void CharacterBase::JustGuardBreak()
{
	m_justGuardBreakFrame++;

	if (m_vecWeapon.y < kWeaponPos.y + 60.0f)
	{
		m_vecWeapon.y += 10.0f;
	}
	if (m_vecSield.y < kSieldPos.y + 60.0f)
	{
		m_vecSield.y += 10.0f;
	}

	// フレームのリセット
	m_attackFrame = 0;
	m_attackGapFrame = 0;
	m_guardFrame = 0;
	m_justGuardFrame = 0;

	const int total = (((kAttackFrameMax + kAttackGapFrameMax) * 2) * 2);

	if ((total) < m_justGuardBreakFrame)
	{
		printfDx("StunEnd\n");
		m_justGuardBreakFrame = 0;
		m_isJustGuardBreak = false;
		m_isStun = false;
		m_pFunc = &CharacterBase::Idle;
	}

	// 位置情報の更新
	UpdatePos();
}

void CharacterBase::Winner()
{

}

void CharacterBase::Losers()
{

	if (m_vecWeapon.y > 0.0f)
	{
		m_vecWeapon.y -= 3.0f;
	}
	else
	{
		m_vecWeapon.y = 0;
	}
	if (m_vecSield.y > 0.0f)
	{
		m_vecSield.y -= 3.0f;
	}
	else
	{
		m_vecSield.y = 0;
	}
	
	// 位置情報の更新
	UpdatePos();
}

void CharacterBase::UpdatePos()
{
	{
		VECTOR move = VTransform(m_vecWeapon, m_rotMtx);
		move = VAdd(m_pos, move);
		MV1SetPosition(m_weaponHandle, move);
		MV1SetRotationXYZ(m_weaponHandle, VGet(0, m_angle, 0));
	}
	{
		VECTOR move = VTransform(m_vecSield, m_rotMtx);
		move = VAdd(m_pos, move);
		MV1SetPosition(m_shieldHandle, move);
		MV1SetRotationXYZ(m_shieldHandle, VGet(0, m_angle, 0));
	}
}

void CharacterBase::SetAngle(float angle)
{
	m_angle = angle;
}

void CharacterBase::SetRotMtx(MATRIX rotMtx)
{
	m_rotMtx = rotMtx;
}

VECTOR CharacterBase::GetPos()const
{
	return m_pos;
}

VECTOR CharacterBase::GetWeaponPos() const
{
	VECTOR move = VTransform(m_vecWeapon, m_rotMtx);
	move = VAdd(m_pos, move);
	return move;
}

VECTOR CharacterBase::GetSieldPos() const
{
	VECTOR move = VTransform(m_vecSield, m_rotMtx);
	move = VAdd(m_pos, move);
	return move;
}

MATRIX CharacterBase::GetRot()const
{
	return m_rotMtx;
}

float CharacterBase::GetAngle() const
{
	return m_angle;
}

float CharacterBase::GetModelRadius() const
{
	return kModelRadus;
}

float CharacterBase::GetWeaponAttackRadius() const
{
	return kWeaponAttackRadius;
}

VECTOR CharacterBase::GetWeaponAttackRelative() const
{
	return kWeaponAttackPos;
}

float CharacterBase::GetSieldRadius() const
{
	return kSieldRadius;
}

VECTOR CharacterBase::GetSieldRelative() const
{
	return kSieldPos;
}

void CharacterBase::SetTargetPos(VECTOR pos)
{
	m_targetPos = pos;
}

int CharacterBase::GetAttackFrame()const
{
	const int totalFrame = m_attackFrame + m_attackGapFrame;
	return totalFrame;
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
	return kAttackTotalFrame;
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

bool CharacterBase::IsJustGuard() const
{
	return m_isJustGuard;
}

bool CharacterBase::IsStun() const
{
	return m_isStun;
}

bool CharacterBase::IsSlowMode() const
{
	return m_isSlow;
}

bool CharacterBase::IsAttackRange() const
{
	return m_isAttackRange;
}

void CharacterBase::SetRota(MATRIX rot)
{
	m_enemyRotMtx = rot;
}

void CharacterBase::SetAttackRange(const bool isRange)
{
	m_isAttackRange = isRange;
}

void CharacterBase::SetDamage(bool isDamage)
{
	m_isResultDamage = isDamage;
}

void CharacterBase::SetGuard(bool isGuard)
{
	m_isResultGuard = isGuard;
}

void CharacterBase::SetJustGuard(bool isJustGuard)
{
	m_isJustGuard = isJustGuard;
}

void CharacterBase::SetJustGuardBreak(bool isJustGuardBreak)
{
	m_isJustGuardBreak = isJustGuardBreak;
}

void CharacterBase::SetChanceAway(const bool isChance)
{
	m_isChanceAway = isChance;
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
