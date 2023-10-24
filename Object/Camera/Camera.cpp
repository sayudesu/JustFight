#include "Camera.h"
#include <math.h>
#include "../../Util/Pad.h"

namespace
{
	// ����p
	constexpr float kFov = 60.0f * DX_PI_F / 180.0f;

	// �J�����̏����ʒu
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
	// �ǂ�����A�ǂ��܂Ō����邩
	SetCameraNearFar(100.0f, 4000.0f);
	// �ǂ������Ăǂ����݂邩
	SetCameraPositionAndTarget_UpVecY(m_pos, m_posTarget);
	// FOV
	SetupCamera_Perspective(kFov);
}

void Camera::Update()
{
	m_angle = (m_angle * 0.8f) + (m_playerAngle * 0.2f);
	const MATRIX cameraRotMtx = MGetRotY(m_angle);

	// �W�����v���͒P���Ƀv���C���[�ɕ��]����̂ł͂Ȃ�
	//�v���C���[�̗����Ă����ʒu������悤�ɂ���
	VECTOR cameraTrans = m_posTarget;
	cameraTrans.y = m_posTarget.y * 0.0f;
	const MATRIX playerTransMtx = MGetTranslate(cameraTrans);

	// �v���C���[�̉�]�ɍ��킹�ăJ�����ʒu�A�����_����]������
	// �v���C���[�̉�]���ƕ��s�ړ���������
	const MATRIX cameraMtx = MMult(cameraRotMtx, playerTransMtx);

	const VECTOR cameraPos = VTransform(kCameraPos, cameraMtx);
	const VECTOR cameraTarget = VTransform(kCameraTarget, cameraMtx);

	// �J�����̈ʒu�A�ǂ������Ă��邩��ݒ肷��
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
