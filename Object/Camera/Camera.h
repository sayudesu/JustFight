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
private:
	// 位置
	VECTOR m_pos;
	// ターゲット位置
	VECTOR m_posTarget;
};

