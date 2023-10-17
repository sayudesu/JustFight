#include "Camera.h"

namespace
{
	// 視野角
	constexpr float kFov = 60.0f * DX_PI_F / 180.0f;
}

Camera::Camera():
	m_pos(VGet(0,300,-1000)),
	m_posTarget(VGet(0,0,0))
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
}
