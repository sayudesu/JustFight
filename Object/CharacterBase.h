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
	virtual void Input() = 0;
protected:
	// �A�C�h�����
	virtual void Idle();
	// �U��
	virtual void Attack();
	// �h��
	virtual void Guard();
	// �W���X�g�K�[�h
	virtual void JustGuard();
	// �W���X�g�K�[�h�����ꂽ�ꍇ
	virtual void JustGuardBreak();
protected:
	virtual void SetAngle(float angle);
	virtual void SetRotMtx(MATRIX rotMtx);
public:
	// �ʒu
	VECTOR GetPos()const;
	VECTOR GetWeaponPos()const;
	VECTOR GetSieldPos()const;

	MATRIX GetRot()const;

	float GetWeaponAttackRadius()const;
	VECTOR GetWeaponAttackRelative()const;

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
	
	// �W���X�g�K�[�h���ꂽ���ǂ���
	bool IsJustGuard()const;

	// ����̊p�x
	void SetRota(MATRIX rot);

	// �U�����󂯂����ǂ���
	void SetDamage(bool isDamage);
	// �K�[�h�������������ǂ���
	void SetGuard(bool isGuard);
	// �W���X�g�K�[�h�������������ǂ���
	void SetJustGuard(bool isJustGuard);
	// �W���X�g�K�[�h�����ꂩ���ǂ���
	void SetJustGuardBreak(bool isJustGuardBreak);

	// �X�^�~�i�̊Ǘ�
	void SetStamina(float addStamina, float subStamina);
private:
	// 3D���f���n���h��
	int m_weaponHnadle;
	int m_shieldHnadle;

	// �p�x
	MATRIX m_rotMtx;
	MATRIX m_enemyRotMtx;

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
	int m_justGuardBreakFrame;

	int m_knockBackFrame;

	// �G����̍U�����K�[�h�ł������ǂ���
	bool m_isResultGuard;

protected:
	// �����o�֐��|�C���^
	void(CharacterBase::*m_pFunc)();

	// �ʒu
	VECTOR m_pos;
	VECTOR m_vec;
	VECTOR m_targetPos;

	// �p�x
	float m_angle;

	// ���̏��
	bool m_isAttack;
	bool m_isGuard;
	bool m_isJustGuard;
	bool m_isJustGuardBreak;
};

