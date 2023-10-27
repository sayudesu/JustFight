#pragma once
#include <DxLib.h>
#include <deque>

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

	void SetPlayerAngle(float angle);
private:
	// �ʒu
	VECTOR m_pos;
	// �^�[�Q�b�g�ʒu
	VECTOR m_targetPos;
	// �p�x
	float m_angle;
	float m_playerAngle;

	std::deque<VECTOR> m_delayFrameTargetPos;
	std::deque<float> m_delayFrameAngle;
};

