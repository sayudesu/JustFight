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
	// �W���X�g�K�[�h
	virtual void JustGuard();
protected:
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
	void SetJustGuard(bool isJustGuard);

	// �X�^�~�i�̊Ǘ�
	void SetStamina(float addStamina, float subStamina);

	void SetAngle(float angle);
private:
	// 3D���f���n���h��
	int m_lanceHnadle;
	int m_shieldHnadle;

	// �ړ���
	// �p�x
	VECTOR m_vec;
	MATRIX m_rotMtx;

	// �����ʒu
	VECTOR m_vecWeapon;
	VECTOR m_vecSield;

	// �̗�
	// �X�^�~�i
	int m_hp;
	float m_stamina;

	// �t���[���֘A
	int m_attackFrame;
	int m_guardFrame;
	int m_justGuardFrame;

protected:
	// �����o�֐��|�C���^
	void(CharacterBase::*m_pFunc)();

	// �ʒu
	VECTOR m_pos;
	VECTOR m_targetPos;

	// �p�x
	float m_angle;

	// ���̏��
	bool m_isAttack;
	bool m_isGuard;
	bool m_isJustGuard;
};

