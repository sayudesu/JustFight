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

	void SetPlayerAngle(float angle);
private:
	// �ʒu
	VECTOR m_pos;
	// �^�[�Q�b�g�ʒu
	VECTOR m_posTarget;
	VECTOR m_posLockon;
	// �p�x
	float m_angle;
	float m_playerAngle;
};

