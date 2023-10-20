#pragma once
#include <DxLib.h>
#include <array>

// �L�����N�^�[��p���N���X
class CharacterBase
{
public:
	CharacterBase(VECTOR pos);
	virtual ~CharacterBase();

	virtual void Init();
	virtual void End();
	virtual void Update();
	virtual void Draw();

	// ����
	virtual void Input();
protected:
	// �A�C�h�����
	virtual void Idle();
	// �U��
	virtual void Attack();
	// �h��
	virtual void Guard();
protected:
	void SetPlayerOnlyAngle(float angle);
	void SetRotMtx(MATRIX rotMtx);
public:
	// �ʒu
	VECTOR GetPos();

	// ���݂̍U���t���[��
	int GetAttackFrame()const;
	// ���݂̖h��t���[��
	int GetGuardFrame()const;
	// ���݂̃W���X�g�K�[�h�t���[��
	int GetJustGuardFrame()const;
	// �ő�U���t���[��
	int GetAttackFrameMax()const;
	// �ő�h��t���[��
	int GetGuardFrameMax()const;
	// �ő�W���X�g�K�[�h�t���[��
	int GetJustGuardFrameMax()const;

	// ���݂̃q�b�g�|�C���g
	int GetHp()const;
	// ���݂̃X�^�~�i
	int GetStamina()const;
	
	// �U�����󂯂����ǂ���
	void SetDamage(bool isDamage);
	// �X�^�~�i�̊Ǘ�
	void SetStamina(float addStamina, float subStamina);

	void SetAngle(VECTOR angle);
protected:
	// �����o�֐��|�C���^
	void(CharacterBase::*m_pFunc)();

	// 3D���f���n���h��
	int m_lanceHnadle;
	int m_shieldHnadle;

	// �ʒu
	// �ړ���
	// �p�x
	VECTOR m_pos;
	VECTOR m_vec;
	VECTOR m_angle;
	MATRIX m_rotMtx;

	// �����ʒu
	VECTOR m_posWeapon;
	VECTOR m_posSield;

	// �̗�
	// �X�^�~�i
	int m_hp;
	float m_stamina;

	// ���̏��
	bool m_isAttack;
	bool m_isGuard;

	// �t���[���֘A
	int m_attackFrame;
	int m_guardFrame;
	int m_justGuardFrame;

	float tempPlayerAngle;
};

