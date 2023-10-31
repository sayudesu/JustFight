#include "CharacterBase.h"
#include "../Util/MyLoadModel.h"
#include <cmath>

namespace
{
	// �t���[���Ǘ�
	// �U���ő�t���[��
	constexpr int kAttackFrameMax = 5;
	constexpr int kAttackGapFrameMax = 10;
	constexpr int kAttackTotalFrame = kAttackFrameMax + kAttackGapFrameMax;

	// ���U��
	constexpr int kStrongAttackFrameMax = 5;
	constexpr int kStrongAttackGapFrameMax = 60;
	constexpr int kStrongAttackTotalFrame = kStrongAttackFrameMax + kStrongAttackGapFrameMax;

	// �K�[�h�ő�t���[��
	constexpr int kGuardFrameMax  = 12;
	// �W���X�g�K�[�h�ő�t���[��
	constexpr int kJustGuardFrameMax = 7;

	// �ő�̗�
	constexpr int kHpMax = 6;
	// �ő�X�^�~�i
	constexpr float kStaminaMax = 100.0f;

	// �����̑��Έʒu
	const VECTOR kWeaponPos = { -80.0f, 100.0f, 0.0f };
	const VECTOR kSieldPos  = { 100.0f, 100.0f, -50.0f };

	// ���f���̓����蔻��p�͈�
	constexpr float kModelRadus = 180.0f;

	// �U�����̓����蔻��
	// ����̔��a
	constexpr float kWeaponAttackRadius = 100.0f;
	// ����̍U�����̔���̑��Έʒu
	const VECTOR kWeaponAttackPos = { 0.0f, 0.0f, -210.0f };
	// ���̔��a
	constexpr float kSieldRadius = 50.0f;

	// �K�[�h���Ă��鎞�ɍU�����󂯂��ꍇ�̃m�b�N�o�b�N�ňړ����鑊�Έʒu
	const VECTOR kKnockBackPos = { 0.0f,0.0f ,-20.0f };

	// �������̈ʒu�ɖ߂鑬�x
	constexpr float kWeaponBackSpeed = 30.0f;
	// �������̈ʒu�ɖ߂鑬�x
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
	m_isStrongAttack(false),
	m_isGuard(false),
	m_isJustGuard(false),
	m_isAway(false),
	m_isResultGuard(false),
	m_isResultDamage(false),
	m_isStun(false),
	m_attackFrame(0),
	m_attackGapFrame(0),
	m_guardFrame(0),
	m_justGuardFrame(0),
	m_stunFrame(0)
{
	// �����o�֐��̏���
	m_pFunc = &CharacterBase::Idle;

	// ���g�̔��ʗpID��������
	m_myId = CharacterName::NONE;
	// �U�����ʂ�������
	m_attackId = AttackData::NONE;

}

CharacterBase::~CharacterBase()
{
}

void CharacterBase::Init()
{
	// ���f���̃��[�h
	m_weaponHandle  = MyModel3D::Load("Data/Model/Lance2.mv1");
	m_shieldHandle  = MyModel3D::Load("Data/Model/Shield.mv1");

	// ���f���̑��Έʒu
	m_vecWeapon = kWeaponPos;
	m_vecSield = kSieldPos;

	// �ʒu���̍X�V
	UpdatePos();

	// ���f���̃T�C�Y����
	MV1SetScale(m_shieldHandle, VGet(3.0f, 3.0f, 3.0f));
}

void CharacterBase::End()
{
	// �������
	MyModel3D::End(m_weaponHandle);
	MyModel3D::End(m_shieldHandle);
}

void CharacterBase::Update()
{
	(this->*m_pFunc)();
}

void CharacterBase::Draw()
{
	// ��
	DrawCapsule3D(m_pos, VGet(m_pos.x, m_pos.y + 200.0f, m_pos.z), 40.0f, 8, 0x00ff00, 0xffffff, true);
	// ����
	MV1DrawModel(m_weaponHandle);
	// ��
	MV1DrawModel(m_shieldHandle);
}

void CharacterBase::TargetMove()
{
	// �������Z�o
	VECTOR m_dir = VSub(m_targetPos, m_pos);

	// �v���C���[����G�l�~�[�܂ł̊p�x�����߂�
	const float angle = atan2(m_dir.y, m_dir.x);

	// ���ݓG�������Ă�������̃x�N�g���𐶐�����
	const MATRIX enemyRotMtx = MGetRotY(angle);

	// �΂߂ɂȂ����Ƃ�((1, 1, 0)�Ȃ�)�ɂ������񒷂��P�ɖ߂�(���K��)
	if (VSquareSize(m_dir) > 0)
	{
		m_dir = VNorm(m_dir);
	}

	// ���x�����߂�
	const VECTOR velecity = VScale(m_dir, 5.0f);

	// �ʒu��ς���
	m_pos = VAdd(m_pos, velecity);

	// �����𑪂�
	m_targetRange.x = sqrt(pow(m_pos.x - m_targetPos.x, 2.0f) + pow(m_pos.x - m_targetPos.x, 2.0f));
	m_targetRange.z = sqrt(pow(m_pos.z - m_targetPos.z, 2.0f) + pow(m_pos.z - m_targetPos.z, 2.0f));
}

CharacterName CharacterBase::GetMyId()
{
	return m_myId;
}

AttackData CharacterBase::GetMyAttackId()
{
	return m_attackId;
}

void CharacterBase::Idle()
{
	SetAddStamina(0.2f);

	m_attackId = AttackData::NONE;

	// ��������̈ʒu�ɖ߂�
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
			m_isStrongAttack = false;
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
	// �������̈ʒu�ɖ߂�
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


	// �ʒu���̍X�V
	UpdatePos();
}

void CharacterBase::Attack()
{
	SetSubStamina(1.0f);

	// �ő�t���[�����ɖڕW�n�_�܂ňړ�����
	// �n�߂Ɍ��悤�̌��ɓ���������
	if (m_attackGapFrame < kAttackGapFrameMax)
	{
		m_vecWeapon.z = kWeaponPos.z + (100.0f - kWeaponPos.z) * (m_attackGapFrame) / kAttackGapFrameMax;
		m_attackGapFrame++;
		m_tempWeaponPos = m_vecWeapon;
	}
	// �U�������铮��
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

	if (m_isStun)
	{
		m_pFunc = &CharacterBase::Stun;
	}


	// �ʒu���̍X�V
	UpdatePos();
}

void CharacterBase::StrongAttack()
{
	SetSubStamina(0.4f);
	int slideX = 0;
	int slideY = 0;
	int slideZ = 0;
	// �ő�t���[�����ɖڕW�n�_�܂ňړ�����
	// �n�߂Ɍ��悤�̌��ɓ���������
	if (m_attackGapFrame < kStrongAttackGapFrameMax)
	{
		m_vecWeapon.z = kWeaponPos.z + (100.0f - kWeaponPos.z) * (m_attackGapFrame) / kStrongAttackGapFrameMax;
		m_attackGapFrame++;
		m_tempWeaponPos = m_vecWeapon;
		slideX = GetRand(6);
		slideY = GetRand(6);
		slideZ = GetRand(6);
	}
	// �U�������铮��
	if (m_attackGapFrame == kStrongAttackGapFrameMax)
	{
		slideX = 0;
		slideY = 0;
		slideZ = 0;
		if (m_attackFrame < kAttackFrameMax)
		{
			m_vecWeapon.z = m_tempWeaponPos.z + (-200.0f - m_tempWeaponPos.z) * (m_attackFrame) / kStrongAttackFrameMax;
			m_vecWeapon.x = kWeaponPos.x + (0.0f - kWeaponPos.x) * (m_attackFrame) / kStrongAttackFrameMax;
			m_attackFrame++;
		}
	}

	if (m_attackFrame + m_attackGapFrame == kStrongAttackTotalFrame)
	{
		m_attackFrame = 0;
		m_attackGapFrame = 0;
		m_pFunc = &CharacterBase::Idle;
	}

	if (m_isStun)
	{
		m_pFunc = &CharacterBase::Stun;
	}


	// �ʒu���̍X�V
	UpdatePos(slideX, slideY, slideZ);
}

void CharacterBase::Guard()
{
	SetAddStamina(0.05f);

	// �ő�t���[�����ɖڕW�n�_�܂ňړ�����
	if (m_guardFrame < kGuardFrameMax)
	{
		m_vecSield.x = kSieldPos.x + (0.0f - kSieldPos.x) * (float(m_guardFrame) / kGuardFrameMax);
		m_guardFrame++;

		// �W���X�g�K�[�h�̃t���[��
		// + 1 �̓W���X�g�K�[�h�͈̔͊O������
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

	// �K�[�h������������
	// ���Ƀm�b�N�o�b�N
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

	// �K�[�h����߂��ꍇ
	if (!m_isGuard)
	{
		m_guardFrame = 0;
		m_justGuardFrame = 0;
		m_pFunc = &CharacterBase::Idle;
	}

	if (m_isStun)
	{
		m_pFunc = &CharacterBase::Stun;
	}

	// �ʒu���̍X�V
	UpdatePos();
}

void CharacterBase::JustGuard()
{
//	m_isJustGuard = false;
	m_vecWeapon = kWeaponPos;
	m_vecSield = kSieldPos;
	m_pFunc = &CharacterBase::Idle;

	// �ʒu���̍X�V
	UpdatePos();
}

void CharacterBase::Stun()
{
	m_stunFrame++;

	if (m_vecWeapon.y < kWeaponPos.y + 60.0f)
	{
		m_vecWeapon.y += 10.0f;
	}
	if (m_vecSield.y < kSieldPos.y + 60.0f)
	{
		m_vecSield.y += 10.0f;
	}

	// �t���[���̃��Z�b�g
	m_attackFrame = 0;
	m_attackGapFrame = 0;
	m_guardFrame = 0;
	m_justGuardFrame = 0;

	if (60 * 3 < m_stunFrame)
	{
		m_stunFrame = 0;
		m_isStun = false;
		m_pFunc = &CharacterBase::Idle;
	}

	// �ʒu���̍X�V
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
	
	// �ʒu���̍X�V
	UpdatePos();
}

void CharacterBase::UpdatePos(int shiftX, int shiftY, int shiftZ)
{
	{
		VECTOR move = VTransform(m_vecWeapon, m_rotMtx);
		move = VAdd(m_pos, move);
		MV1SetPosition(m_weaponHandle, VGet(move.x + shiftX, move.y + shiftY, move.z + shiftZ));
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
	if (m_isAttack)
	{
		return kAttackTotalFrame;
	}

	if (m_isStrongAttack)
	{
		return kStrongAttackTotalFrame;
	}

	return -1;
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

bool CharacterBase::IsAttackRange() const
{
	return m_isAttackRange;
}

void CharacterBase::SetTargetRota(MATRIX rot)
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

void CharacterBase::SetStun(bool isStun)
{
	m_isStun = isStun;
}

void CharacterBase::SetAddStamina(const float addStamina)
{
	// �ő�l�ƍŏ��l�𒴂��Ȃ��悤�ɐ���
	if (m_stamina < kStaminaMax)
	{
		m_stamina += addStamina;
	}
	else
	{
		m_stamina = kStaminaMax;
	}
}

void CharacterBase::SetSubStamina(const float subStamina)
{
	if (m_stamina >= 0.0f)
	{
		m_stamina -= subStamina;
	}
	else
	{
		m_stamina = 0.0f;
	}
}
