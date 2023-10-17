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

	virtual void UpdateInput();
	virtual void Attack();
	virtual void Guard();

protected:
	// �����o�֐��|�C���^
	void(CharacterBase::*m_pFunc)();
	// 3D���f���n���h��
	int m_lanceHnadle;
	int m_shieldHnadle;
	// �ʒu
	VECTOR m_pos;
	// �ړ���
	VECTOR m_vec;
};

