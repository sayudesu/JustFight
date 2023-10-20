#pragma once
#include <DxLib.h>

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
	void SetLockonPos(VECTOR pos);

	void SetPlayerAngle(float angle);
private:
	// 位置
	VECTOR m_pos;
	// ターゲット位置
	VECTOR m_posTarget;
	VECTOR m_posLockon;
	// 角度
	float m_angle;
	float m_playerAngle;
};

