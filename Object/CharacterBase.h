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

private:
	virtual void UpdateInput();
	virtual void Attack();
	virtual void Guard();

private:
	// �����o�֐��|�C���^
	void(CharacterBase::*m_pFunc)();
	// �ʒu
	VECTOR m_pos;
	// �ړ���
	VECTOR m_vec;
};

