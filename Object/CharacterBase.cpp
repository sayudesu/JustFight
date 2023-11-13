#include "CharacterBase.h"
#include "../Util/MyLoadModel.h"
#include <cmath>
#include "../Util/EffekseerDrawer.h"

CharacterBase::CharacterBase(VECTOR pos):
	m_pFunc(nullptr),
	m_weaponHandle(-1),
	m_shieldHandle(-1),
	m_effectHandle(-1),
	m_rotMtx({}),
	m_targetRotMtx({}),
	m_targetAngle(0.0f),
	m_pos(pos),
	m_targetPos    (VGet(0,0,0)),
	m_vecWeapon    (VGet(0, 0, 0)),
	m_vecSield     (VGet(0, 0, 0)),
	m_tempWeaponPos(VGet(0, 0, 0)),
	m_tempFightingMeter(0),
	m_angle         (0.0f),
	m_isAttack      (false),
	m_isStrongAttack(false),
	m_isGuard       (false),
	m_isJustGuard   (false),
	m_isAway	    (false),
	m_isResultGuard (false),
	m_isResultDamage(false),
	m_isWeaponAttacksShield(false),
	m_isStun        (false),
	m_isAttackRange (false),
	m_targetHp(0),
	m_attackFrame   (0),
	m_attackGapFrame(0),
	m_guardFrame    (0),
	m_justGuardFrame(0),
	m_stunFrame     (0),
	m_recoilFrame   (0)
{
	// �����o�֐��̏���
	m_pFunc = &CharacterBase::Idle;

	// ���g�̔��ʗpID��������
	m_myId = CharacterName::NONE;

	// ���݂̍s�����L�^
	m_battleState       = BattleState::NONE;// ���g
	m_targetBattleState = BattleState::NONE;// �^�[�Q�b�g


	m_parameter = {};
}

CharacterBase::~CharacterBase()
{
}

void CharacterBase::Init()
{
	// ���f���̃��[�h
//	m_weaponHandle  = MyModel3D::Load("Data/Model/Lance2.mv1");
	m_weaponHandle  = MyModel3D::Load("Data/Model/Sword.mv1");
	m_shieldHandle  = MyModel3D::Load("Data/Model/Shield.mv1");

	// ���f���̑��Έʒu
	m_vecWeapon = m_parameter.weaponRelativePos;
	m_vecSield = m_parameter.sieldRelativePos;

	// �ʒu���̍X�V
	UpdatePos();

	// ���f���̃T�C�Y����
	MV1SetScale(m_weaponHandle, VGet(3.0f, 3.0f, 3.0f));
	MV1SetScale(m_shieldHandle, VGet(3.0f, 3.0f, 3.0f));

	// ���f���̊p�x

	testV1.z = 90 * DX_PI_F / 180.0f;

	MV1SetRotationXYZ(m_weaponHandle, VGet(testV1.x, m_angle, testV1.z));

	m_hp = m_parameter.hpMax;
	m_vec = m_parameter.knockBackPos;
	m_fightingMeter = m_parameter.fightingMeterMax;
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

BattleState CharacterBase::GetBattleState()
{
	return m_battleState;
}

void CharacterBase::Idle()
{
	// ���݂̍s�����L�^
	m_battleState = BattleState::IDLE;

	SetFightingMeter(0.03f);

	// ��������̈ʒu�ɖ߂�
	{
		bool isEndX = false;
		bool isEndZ = false;
		if (m_vecWeapon.x > m_parameter.weaponRelativePos.x)
		{
			m_vecWeapon.x -= 10.0f;
		}
		else
		{
			isEndX = true;
			m_vecWeapon.x = m_parameter.weaponRelativePos.x;
		}

		if (m_vecWeapon.z < m_parameter.weaponRelativePos.z)
		{
			m_vecWeapon.z += 30.0f;
		}
		else
		{
			isEndZ = true;
			m_vecWeapon.z = m_parameter.weaponRelativePos.z;
		}

		if (isEndX && isEndZ)
		{
			m_isAttack = false;
			m_isStrongAttack = false;
		}

		if (m_vecWeapon.y < m_parameter.weaponRelativePos.y)
		{
			m_vecWeapon.y -= 10.0f;
		}
		else
		{
			m_vecWeapon.y = m_parameter.weaponRelativePos.y;
		}
	}
	// �������̈ʒu�ɖ߂�
	{
		if (m_vecSield.x < m_parameter.sieldRelativePos.x)
		{
			m_vecSield.x += 30.0f;
		}
		else
		{
			m_vecSield.x = m_parameter.sieldRelativePos.x;
		}

		if (m_vecSield.y < m_parameter.sieldRelativePos.y)
		{
			m_vecSield.y -= 30.0f;
		}
		else
		{
			m_vecSield.y = m_parameter.sieldRelativePos.y;
		}
	}

	if (m_hp == 0)
	{
		m_pFunc = &CharacterBase::Losers;
		m_vecWeapon.y = 100.0f;
	}

	if (m_targetHp == 0)
	{
		m_pFunc = &CharacterBase::Winner;
	}


	if (m_isResultDamage)
	{
		m_isResultDamage = false;
		m_hp--;
	}

	if (m_isStun)
	{
		m_pFunc = &CharacterBase::Stun;
	}

	
	//{
	//	VECTOR move = VTransform(m_vecWeapon, m_rotMtx);
	//	move = VAdd(m_pos, move);
	//	MV1SetPosition(m_weaponHandle, VGet(move.x, move.y, move.z));
	//	MV1SetRotationXYZ(m_weaponHandle, VGet(testV1.x, m_angle, testV1.z));
	//}
	//{
	//	VECTOR move = VTransform(m_vecSield, m_rotMtx);
	//	move = VAdd(m_pos, move);
	//	MV1SetPosition(m_shieldHandle, move);
	//	MV1SetRotationXYZ(m_shieldHandle, VGet(0, m_angle, 0));
	//}


	// �ʒu���̍X�V
	UpdatePos();
}

void CharacterBase::Attack()
{
	// ���݂̍s�����L�^
	m_battleState = BattleState::ATTACK;

	//// �ő�t���[�����ɖڕW�n�_�܂ňړ�����
	//// �n�߂Ɍ��悤�̌��ɓ���������
	//if (m_attackGapFrame < m_parameter.attackFrameGapMax)
	//{
	//	m_vecWeapon.z = MoveByFrame(m_parameter.weaponRelativePos.z, 150.0f, m_attackGapFrame, m_parameter.attackFrameGapMax);
	//	m_attackGapFrame++;
	//	m_tempWeaponPos = m_vecWeapon;
	//}
	//// �U�������铮��
	//if (m_attackGapFrame == m_parameter.attackFrameGapMax)
	//{
	//	if (m_attackFrame < m_parameter.attackFrameMax)
	//	{
	//		m_vecWeapon.z = MoveByFrame(m_tempWeaponPos.z, -200.0f, m_attackFrame, m_parameter.attackFrameMax);
	//		m_vecWeapon.x = MoveByFrame(m_parameter.weaponRelativePos.x, 0.0f, m_attackFrame, m_parameter.attackFrameMax);
	//		m_attackFrame++;
	//	}
	//}

	//if (m_attackFrame + m_attackGapFrame == m_parameter.attackRotalFrame)
	//{
	//	m_pFunc = &CharacterBase::Idle;
	//	m_attackFrame = 0;
	//	m_attackGapFrame = 0;

	//	if (m_isWeaponAttacksShield)
	//	{
	//		m_isWeaponAttacksShield = false;
	//		m_pFunc = &CharacterBase::WeaponAttacksShield;
	//	}
	//}

	//if (m_isStun)
	//{
	//	m_pFunc = &CharacterBase::Stun;
	//}


	test1 += 0.01;
	// Y����]�s����쐬
	MATRIX rotZ= MGetRotZ(test1);
	MATRIX rotY = MGetRotY(m_angle);
//	MATRIX rot = MGetRotZ(test1);

	VECTOR move = VTransform(m_vecWeapon, m_rotMtx);
	move = VAdd(m_pos, move);
	MV1SetPosition(m_weaponHandle, VGet(move.x, move.y, move.z));
//	MV1SetRotationXYZ(m_weaponHandle, VGet(testV1.x, m_angle, testV1.z));
//	MV1SetRotationMatrix(m_weaponHandle, rotZ);
	

	{
		//VECTOR move = VTransform(m_vecWeapon, m_rotMtx);
		//move = VAdd(m_pos, move);
		//MV1SetPosition(m_weaponHandle, VGet(move.x, move.y, move.z));
		//MV1SetRotationXYZ(m_weaponHandle, VGet(testV1.x, m_angle, testV1.z));
	}
	
	{
		VECTOR move = VTransform(m_vecSield, m_rotMtx);
		move = VAdd(m_pos, move);
		MV1SetPosition(m_shieldHandle, move);
		MV1SetRotationXYZ(m_shieldHandle, VGet(0, m_angle, 0));
	}

	// �ʒu���̍X�V
//	UpdatePos();

}

void CharacterBase::StrongAttack()
{
	// ���݂̍s�����L�^
	m_battleState = BattleState::STRONGATTACK;

	int slideX = 0;
	int slideY = 0;
	int slideZ = 0;
	// �ő�t���[�����ɖڕW�n�_�܂ňړ�����
	// �n�߂Ɍ��悤�̌��ɓ���������
	if (m_attackGapFrame < m_parameter.strongAttackFrameGapMax  )
	{
		m_vecWeapon.z = MoveByFrame(m_parameter.weaponRelativePos.z, 100.0f, m_attackGapFrame, m_parameter.strongAttackFrameGapMax  );
		m_attackGapFrame++;
		m_tempWeaponPos = m_vecWeapon;
		slideX = GetRand(6);
		slideY = GetRand(6);
		slideZ = GetRand(6);
	}
	// �U�������铮��
	if (m_attackGapFrame == m_parameter.strongAttackFrameGapMax  )
	{
		slideX = 0;
		slideY = 0;
		slideZ = 0;
		if (m_attackFrame < m_parameter.attackFrameMax)
		{
			m_vecWeapon.z = MoveByFrame(m_tempWeaponPos.z, -200.0f, m_attackFrame, m_parameter.strongAttackFrameMax);
			m_vecWeapon.x = MoveByFrame(m_parameter.weaponRelativePos.x, 0.0f, m_attackFrame, m_parameter.strongAttackFrameMax);
			m_attackFrame++;
		}
	}

	if (m_attackFrame + m_attackGapFrame == m_parameter.strongAttackRotalFrame )
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
	// ���݂̍s�����L�^
	m_battleState = BattleState::GUARD;

	// �ő�t���[�����ɖڕW�n�_�܂ňړ�����
	if (m_guardFrame < m_parameter.guardFrameMax )
	{
		m_vecSield.x = MoveByFrame(m_parameter.sieldRelativePos.x, 0.0f, m_guardFrame, m_parameter.guardFrameMax );
		m_guardFrame++;

		// �W���X�g�K�[�h�̃t���[��
		// + 1 �̓W���X�g�K�[�h�͈̔͊O������
		if (m_justGuardFrame < m_parameter.justGuardFrameMax + 1)
		{
			m_justGuardFrame++;
		}
		
	}
	else
	{
		m_guardFrame = m_parameter.guardFrameMax ;
		m_vecSield.x = 0.0f;
	}

	// �K�[�h������������
	// ���Ƀm�b�N�o�b�N
	if (m_isResultGuard)
	{
		if (m_vec.z < 0.0f)
		{			
			m_vec.z += 1.0f;
			VECTOR move = VTransform(m_vec, m_targetRotMtx);
			m_pos = VAdd(m_pos, move);
		}
		else
		{
			m_vec = m_parameter.knockBackPos;
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
	m_vecWeapon = m_parameter.weaponRelativePos;
	m_vecSield = m_parameter.sieldRelativePos;
	m_pFunc = &CharacterBase::Idle;

	// �ʒu���̍X�V
	UpdatePos();
}

void CharacterBase::Stun()
{
	// ���݂̍s�����L�^
	m_battleState = BattleState::STUN;
	// �X�^����Ԃ̃t���[�����J�E���g
	m_stunFrame++;

	if (m_vecWeapon.y < m_parameter.weaponRelativePos.y + 60.0f)
	{
		m_vecWeapon.y += 10.0f;
	}
	if (m_vecSield.y < m_parameter.sieldRelativePos.y + 60.0f)
	{
		m_vecSield.y += 10.0f;
	}

	// �t���[���̃��Z�b�g
	m_attackFrame    = 0;
	m_attackGapFrame = 0;
	m_guardFrame     = 0;
	m_justGuardFrame = 0;

	if (m_parameter.stunFrameMax < m_stunFrame)
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
	// �ʒu���̍X�V
	UpdatePos();

	MV1SetRotationXYZ(m_weaponHandle, VGet(90 * DX_PI_F / 180.0f, m_angle, 0));
	MV1SetRotationXYZ(m_shieldHandle, VGet(90 * DX_PI_F / 180.0f, m_angle, 0));
}

void CharacterBase::Losers()
{
	static float m_jumpAcc = 60.0f;

	{
		VECTOR move = VTransform(m_vecWeapon, m_rotMtx);
		move = VAdd(m_pos, move);
		static VECTOR Wpos = move;
		// �d��
		m_jumpAcc += -2.0f;
		if (Wpos.y < 300.0f)
		{
			m_jumpAcc = 0.0f;
		}
		Wpos.y += m_jumpAcc;

		MV1SetPosition(m_weaponHandle, Wpos);
	}

	{
		VECTOR move = VTransform(m_vecSield, m_rotMtx);
		move = VAdd(m_pos, move);
		static VECTOR Wpos = move;

		// �d��
		m_jumpAcc += -2.0f;
		if (Wpos.y < 300.0f)
		{
			m_jumpAcc = 0.0f;
		}
		Wpos.y += m_jumpAcc;

		MV1SetPosition(m_shieldHandle, Wpos);
		MV1SetRotationXYZ(m_shieldHandle, VGet(90 * DX_PI_F / 180.0f, m_angle, 0));
	}
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

void CharacterBase::WeaponAttacksShield()
{
	// �ʒu  =  ���Έʒu�@+ �������������W - ���Έʒu�@*�@�@���݂̃t���[���@/�@�ő�t���[���@�@
//	m_vecSield.x = m_parameter.sieldRelativePos.x + (0.0f - m_parameter.sieldRelativePos.x) * (float(m_guardFrame) / m_parameter.guardFrameMax );
	m_recoilFrame++;

	if (m_recoilFrame < 30)
	{
		m_vecWeapon.z = MoveByFrame(m_parameter.weaponRelativePos.z,100.0f, m_recoilFrame, 30);
		m_vecWeapon.x = MoveByFrame(m_parameter.weaponRelativePos.z,-100.0f, m_recoilFrame, 30);
	}


	if (m_recoilFrame > 30)
	{
		m_recoilFrame = 0;
		m_pFunc = &CharacterBase::Idle;
	}

	// �ʒu���̍X�V
	UpdatePos();
}

float CharacterBase::MoveByFrame(const float relativePos, const float EndPos, const int nowFrame, const int maxFrame)
{
	// �ʒu = ���Έʒu + �������������W - ���Έʒu * ���݂̃t���[�� / �ő�t���[���@�@
	const float resultPos = relativePos + (EndPos - relativePos) * (float(nowFrame) / maxFrame);
	return resultPos;
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
	return m_parameter.modelRadius ;
}

float CharacterBase::GetWeaponAttackRadius() const
{
	return m_parameter.weaponAttackRadius;
}

VECTOR CharacterBase::GetWeaponAttackRelative() const
{
	return m_parameter.weaponAttackPos;
}

float CharacterBase::GetSieldRadius() const
{
	return m_parameter.sieldRadius;
}

VECTOR CharacterBase::GetSieldRelative() const
{
	return m_parameter.sieldRelativePos;
}

void CharacterBase::SetTargetPos(VECTOR pos)
{
	m_targetPos = pos;
}

void CharacterBase::SetTargetHp(const int hp)
{
	m_targetHp = hp;
}

void CharacterBase::SetCollGuardEffect()
{
	// �K�[�h�ۂ̃G�t�F�N�g�Đ�
	EffekseerDrawer::GetInstance().Play(
		m_effectHandle, Id::Guard,
		EffectPlayType::NORMAL,
		GetSieldPos(),
		VGet(1, 1, 1),
		VGet(0, m_angle, 0));
}

void CharacterBase::SetCollJustGuardEffect()
{
	// �W���X�g�K�[�h�����ۂ̃G�t�F�N�g�Đ�
	EffekseerDrawer::GetInstance().Play(
		m_effectHandle, Id::JustGuard,
		EffectPlayType::NORMAL,
		VGet(GetSieldPos().x, GetSieldPos().y + 100.0f, GetSieldPos().z),
		VGet(1, 1, 1),
		VGet(0, 0, 0));
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
		return m_parameter.attackRotalFrame;
	}

	if (m_isStrongAttack)
	{
		return m_parameter.strongAttackRotalFrame ;
	}

	return -1;
}

int CharacterBase::GetGuardFrameMax()const
{
	return m_parameter.guardFrameMax ;
}

int CharacterBase::GetJustGuardFrameMax()const
{
	return m_parameter.justGuardFrameMax;
}

int CharacterBase::GetStunFrameMax() const
{
	return m_parameter.stunFrameMax;
}

int CharacterBase::GetHp()const
{
	return m_hp;
}

float CharacterBase::GetFightingMeter()const
{
	return m_fightingMeter;
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

void CharacterBase::SetWeaponAttacksShield(const bool isShieldHit)
{
	m_isWeaponAttacksShield = isShieldHit;
}

void CharacterBase::SetBattleState(BattleState state)
{	
	m_targetBattleState = state;
}

void CharacterBase::SetTargetMtxRota(MATRIX rot)
{
	m_targetRotMtx = rot;
}

void CharacterBase::SetTargetRota(const float rot)
{
	m_targetAngle = rot;
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

void CharacterBase::SetFightingMeter(const float fightingMeter)
{
	m_tempFightingMeter = fightingMeter;

	FightingMeter();
}

void CharacterBase::FightingMeter()
{
	m_fightingMeter += m_tempFightingMeter;

	// �킢�ɕK�v�ȃ��[�^�[���ő�l�ɂȂ�ƍő�l�ŌŒ�
	if (m_fightingMeter > m_parameter.fightingMeterMax)
	{
		m_fightingMeter = m_parameter.fightingMeterMax;
	}
	// �킢�ɕK�v�ȃ��[�^�[���ŏ��l�ɂȂ�ƍŏ��l�ŌŒ�
	if (m_fightingMeter < 0.0f)
	{
		m_isStun = true;
		m_fightingMeter = 0.0f;
	}
}
