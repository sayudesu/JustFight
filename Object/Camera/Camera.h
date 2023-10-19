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
private:
	// 位置
	VECTOR m_pos;
	// ターゲット位置
	VECTOR m_posTarget;
	VECTOR m_posLockon;
	// 角度
	VECTOR m_angle;
};

