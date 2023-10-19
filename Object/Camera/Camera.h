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
public:
	void SetTargetPos(VECTOR pos);
	void SetLockonPos(VECTOR pos);
private:
	// �ʒu
	VECTOR m_pos;
	// �^�[�Q�b�g�ʒu
	VECTOR m_posTarget;
	VECTOR m_posLockon;
	// �p�x
	VECTOR m_angle;
};

