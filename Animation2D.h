#pragma once
#include <DxLib.h>

class Animation2D
{
public:
	Animation2D();
	virtual~Animation2D();

	virtual void Init(const char* fileName, int chipX, int chipY);
	virtual void End();
	virtual void Update();
	virtual void Draw();

public:
	virtual void SetAnim(bool isAnim);
	// ���[���h���W���X�N���[�����W�ɕϊ����Ă�����W���󂯎��
	virtual void SetPos3D(VECTOR pos);
private:
	// �`�悷��摜�p�n���h��
	int m_handle;

	// �`����W
	VECTOR m_pos;

	// �`�b�v�ԍ�
	int m_chipX;
	int m_chipY;

	// �摜����A�j���[�V�������������ꕔ���̍��W
	int m_amimX;
	int m_amimY;

	// �A�j���[�V�������x
	int m_animFrame;

	// �A�j���[�V�������Đ����邩�ǂ���
	bool m_isAnimStart;
};

