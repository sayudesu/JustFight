#pragma once
#include <DxLib.h>
#include "../CharacterName.h"
#include "../AttackData.h"

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
	// ���U��
	virtual void StrongAttack();
	// �h��
	virtual void Guard();
	// �W���X�g�K�[�h
	virtual void JustGuard();
	// �W���X�g�K�[�h�����ꂽ�ꍇ
	virtual void Stun();
	// ���������ꍇ
	virtual void Winner();
	// �s�k�����ꍇ
	virtual void Losers();
private:
	// �ʒu���̍X�V
	virtual void UpdatePos(int shiftX = 0, int shiftY = 0, int shiftZ = 0);
protected:
	// �p�x���擾
	virtual void SetAngle(float angle);
	virtual void SetRotMtx(MATRIX rotMtx);

	// �^�[�Q�b�g�ʒu�ǐՈړ�
	virtual void TargetMove();
public:
	// ���g���N����Ԃ�
	CharacterName GetMyId();
	AttackData GetMyAttackId();

	// �ʒu
	VECTOR GetPos()const;
	VECTOR GetWeaponPos()const;
	VECTOR GetSieldPos()const;

	// �p�x
	MATRIX GetRot()const;
	float GetAngle()const;

	// ���f���̓����蔻��p���a
	float GetModelRadius()const;
	
	// �����̔���p���a
	float GetWeaponAttackRadius()const;
	float GetSieldRadius()const;

	// �����̑��Έʒu
	VECTOR GetWeaponAttackRelative()const;
	VECTOR GetSieldRelative()const;

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
	
	// �W���X�g�K�[�h�ł������ǂ���
	bool IsJustGuard()const;

	// �X�^����Ԃ��ǂ���
	bool IsStun()const;

	// �U���͈͂ɓ����Ă��邩�ǂ���
	bool IsAttackRange()const;

	// ����̊p�x
	void SetTargetRota(const MATRIX rot);

	// �U���͈͂ɓ����Ă��邩�ǂ���
	void SetAttackRange(const bool isRange);

	// �U�����󂯂����ǂ���
	void SetDamage(const bool isDamage);
	// �K�[�h�������������ǂ���
	void SetGuard(const bool isGuard);
	// �W���X�g�K�[�h�������������ǂ���
	void SetJustGuard(const bool isJustGuard);
	// �X�^�������邩�ǂ���
	void SetStun(const bool isStun);

	// �X�^�~�i�̊Ǘ�
	void SetAddStamina(const float addStamina);
	void SetSubStamina(const float subStamina);

	// �^�[�Q�b�g�̈ʒu���擾����
	void SetTargetPos(const VECTOR pos);
private:
	// 3D���f���n���h��
	int m_weaponHandle;
	int m_shieldHandle;

	// �p�x
	MATRIX m_rotMtx;
	MATRIX m_enemyRotMtx;

	// ����
	VECTOR m_vecWeapon;
	VECTOR m_vecSield;

	// �ꎞ�I�ȕ���̈ʒu
	VECTOR m_tempWeaponPos;

	// �̗�
	// �X�^�~�i
	int m_hp;
	float m_stamina;

	// �t���[���֘A
	int m_attackGapFrame;
	int m_attackFrame;
	int m_guardFrame;
	int m_justGuardFrame;
	int m_stunFrame;

	// �G����̍U�����K�[�h�ł������ǂ���
	bool m_isResultGuard;
	bool m_isResultDamage;

	// �X�^����Ԃ��ǂ���
	bool m_isStun;

	// �U���͈͂ɂ��邩�ǂ���
	bool m_isAttackRange;
protected:
	// �����o�֐��|�C���^
	void(CharacterBase::*m_pFunc)();

	// �ʒu
	VECTOR m_pos;
	VECTOR m_vec;
	VECTOR m_targetPos;

	// �p�x
	float m_angle;

	// ���
	bool m_isAttack;
	bool m_isStrongAttack;
	bool m_isGuard;
	bool m_isJustGuard;

	// ���
	bool m_isAway;

	// �^�[�Q�b�g�̋���
	VECTOR m_targetRange;

	CharacterName m_myId;
	AttackData m_attackId;
};

