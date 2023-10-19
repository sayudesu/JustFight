#include "Camera.h"
#include <math.h>
#include "../../Util/Pad.h"

namespace
{
	// 視野角
	constexpr float kFov = 60.0f * DX_PI_F / 180.0f;
}

Camera::Camera():
	m_pos(VGet(0,300,-1000)),
	m_posTarget(VGet(0,0,0)),
	m_angle(VGet(0, 0, 0))
{
}

Camera::~Camera()
{
}

void Camera::Init()
{
	// どこから、どこまで見えるか
	SetCameraNearFar(100.0f, 4000.0f);
	// どこを居てどこをみるか
	SetCameraPositionAndTarget_UpVecY(m_pos, m_posTarget);
	// 遠近法のセットアップ( ラジアン値に変換しています )
	SetupCamera_Perspective(kFov);
}

void Camera::Update()
{
	DINPUT_JOYSTATE input;
	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	// カメラの回転角度を調整
	if (input.Rx > 30)
	{
		m_angle.y -= 3.0f;
	}
	if (input.Rx < -30)
	{
		m_angle.y += 3.0f;
	}

	// カメラの位置を更新
	// カメラの高さを300.0fに固定
	m_pos.x = m_posTarget.x + 500.0f * sinf(m_angle.y * DX_PI / 180);
	m_pos.y = m_posTarget.y + 300.0f; // カメラの高さを固定
	m_pos.z = m_posTarget.z - 500.0f * cosf(m_angle.y * DX_PI / 180);

	// カメラ位置の設定
	SetCameraPositionAndTarget_UpVecY(m_pos, m_posTarget);
}

void Camera::SetTargetPos(VECTOR pos)
{
	m_posTarget = pos;
}

void Camera::SetLockonPos(VECTOR pos)
{
	m_posLockon = pos;
}
