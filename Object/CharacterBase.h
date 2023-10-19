#pragma once
#include <DxLib.h>

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
	// �A�C�h�����
	virtual void Idle();
	// �U��
	virtual void Attack();
	// �h��
	virtual void Guard();
public:
	// ���݂̍U���t���[��
	int GetAttackFrame();
	// ���݂̖h��t���[��
	int GetGuardFrame();
	// ���݂̃W���X�g�K�[�h�t���[��
	int GetJustGuardFrame();
	// �ő�U���t���[��
	int GetAttackFrameMax();
	// �ő�h��t���[��
	int GetGuardFrameMax();
	// �ő�W���X�g�K�[�h�t���[��
	int GetJustGuardFrameMax();

	// ���݂̃q�b�g�|�C���g
	int GetHp();
	// ���݂̃X�^�~�i
	int GetStamina();
	
	// �U�����󂯂����ǂ���
	void SetDamage(bool isDamage);
	// �X�^�~�i�̊Ǘ�
	void SetStamina(int addStamina, int subStamina);
protected:
	// �����o�֐��|�C���^
	void(CharacterBase::*m_pFunc)();

	// 3D���f���n���h��
	int m_lanceHnadle;
	int m_shieldHnadle;

	// �ʒu
	// �ړ���
	VECTOR m_pos;
	VECTOR m_vec;

	// �����ʒu
	VECTOR m_posWeapon;
	VECTOR m_posSield;

	// �̗�
	// �X�^�~�i
	int m_hp;
	int m_stamina;

	// ���̏��
	bool m_isAttack;
	bool m_isGuard;

	// �t���[���֘A
	int m_attackFrame;
	int m_guardFrame;
	int m_justGuardFrame;
};

