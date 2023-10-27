#include "Camera.h"
#include <math.h>
#include "../../Util/Pad.h"

namespace
{
	// 視野角
	constexpr float kFov = 90.0f * DX_PI_F / 180.0f;

	// カメラの初期位置
	constexpr VECTOR kCameraPos{ -50.0f, 370.0f, 190.0f };
	constexpr VECTOR kCameraTarget{ 0.0f, 200.0f, -240.0f };

	// カメラの追跡処理にディレイを入れるフレーム数
	constexpr int kCameraDelayFrame = 8;
}

Camera::Camera():
	m_pos(VGet(0,300,-1000)),
	m_targetPos(VGet(0,0,0)),
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
	SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);
	// FOV
	SetupCamera_Perspective(kFov);
}

void Camera::Update()
{
	// カメラの追跡処理にディレイを入れる
	m_delayFrameTargetPos.push_front(m_targetPos);
	m_delayFrameAngle.push_front(m_playerAngle);
	if (m_delayFrameTargetPos.size() > kCameraDelayFrame &&
		m_delayFrameAngle.size()  > kCameraDelayFrame)
	{
		m_delayFrameTargetPos.pop_back();
		m_delayFrameAngle.pop_back();
	}

	m_angle = (m_angle * 0.0f) + (m_delayFrameAngle.back() * 1.0f);
	const MATRIX cameraRotMtx = MGetRotY(m_angle);

	// ジャンプ時は単純にプレイヤーに服従するのではなく
	//プレイヤーの立っていた位置を見るようにする
	VECTOR cameraTrans = m_delayFrameTargetPos.back();
	cameraTrans.y = m_delayFrameTargetPos.back().y * 0.0f;
	const MATRIX playerTransMtx = MGetTranslate(cameraTrans);

	// プレイヤーの回転に合わせてカメラ位置、注視点を回転させる
	// プレイヤーの回転情報と平行移動情報を合成
	const MATRIX cameraMtx = MMult(cameraRotMtx, playerTransMtx);

	const VECTOR cameraPos = VTransform(kCameraPos, cameraMtx);
	const VECTOR cameraTarget = VTransform(kCameraTarget, cameraMtx);

	// カメラの位置、どこを見ているかを設定する
	SetCameraPositionAndTarget_UpVecY(cameraPos, cameraTarget);
}

void Camera::SetTargetPos(VECTOR pos)
{
	m_targetPos = pos;
}
void Camera::SetPlayerAngle(float angle)
{
	m_playerAngle = angle;
}
