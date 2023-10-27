#pragma once
#include <DxLib.h>
#include <deque>

// カメラ管理クラス
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
	// 位置
	VECTOR m_pos;
	// ターゲット位置
	VECTOR m_targetPos;
	// 角度
	float m_angle;
	float m_playerAngle;

	std::deque<VECTOR> m_delayFrameTargetPos;
	std::deque<float> m_delayFrameAngle;
};

