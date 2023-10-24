#include "Camera.h"
#include <math.h>
#include "../../Util/Pad.h"

namespace
{
	// 視野角
	constexpr float kFov = 60.0f * DX_PI_F / 180.0f;

	// カメラの初期位置
	constexpr VECTOR kCameraPos{ 0.0f, 520.0f, 520.0f };
	constexpr VECTOR kCameraTarget{ 0.0f, 200.0f, -120.0f };
}

Camera::Camera():
	m_pos(VGet(0,300,-1000)),
	m_posTarget(VGet(0,0,0)),
	m_posLockon(VGet(0,0,0)),
	m_angle(0.0f),
	m_playerAngle(0.0f)
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
	// FOV
	SetupCamera_Perspective(kFov);
}

void Camera::Update()
{
	m_angle = (m_angle * 0.8f) + (m_playerAngle * 0.2f);
	const MATRIX cameraRotMtx = MGetRotY(m_angle);

	// ジャンプ時は単純にプレイヤーに服従するのではなく
	//プレイヤーの立っていた位置を見るようにする
	VECTOR cameraTrans = m_posTarget;
	cameraTrans.y = m_posTarget.y * 0.0f;
	const MATRIX playerTransMtx = MGetTranslate(cameraTrans);

	// プレイヤーの回転に合わせてカメラ位置、注視点を回転させる
	// プレイヤーの回転情報と平行移動情報を合成
	const MATRIX cameraMtx = MMult(cameraRotMtx, playerTransMtx);

	const VECTOR cameraPos = VTransform(kCameraPos, cameraMtx);
	const VECTOR cameraTarget = VTransform(kCameraTarget, cameraMtx);

	// カメラの位置、どこを見ているかを設定する
	SetCameraPositionAndTarget_UpVecY(cameraPos, cameraTarget);
//	SetCameraPositionAndTarget_UpVecY(cameraPos, m_posLockon);
}

void Camera::SetTargetPos(VECTOR pos)
{
	m_posTarget = pos;
}

void Camera::SetLockonPos(VECTOR pos)
{
	m_posLockon = pos;
}

void Camera::SetPlayerAngle(float angle)
{
	m_playerAngle = angle;
}
