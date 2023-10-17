#include "Camera.h"

namespace
{
	// ����p
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
	// �ǂ�����A�ǂ��܂Ō����邩
	SetCameraNearFar(100.0f, 4000.0f);
	// �ǂ������Ăǂ����݂邩
	SetCameraPositionAndTarget_UpVecY(m_pos, m_posTarget);
	// ���ߖ@�̃Z�b�g�A�b�v( ���W�A���l�ɕϊ����Ă��܂� )
	SetupCamera_Perspective(kFov);
}

void Camera::Update()
{
}
