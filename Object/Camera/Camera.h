#pragma once
#include <DxLib.h>

// �J�����Ǘ��N���X
class Camera
{
public:
	Camera();
	virtual ~Camera();
	void Init();

	void Update();
private:
	// �ʒu
	VECTOR m_pos;
	// �^�[�Q�b�g�ʒu
	VECTOR m_posTarget;
};

