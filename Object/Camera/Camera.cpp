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
	// �ǂ�����A�ǂ��܂Ō����邩
	SetCameraNearFar(100.0f, 4000.0f);
	// �ǂ������Ăǂ����݂邩
	SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);
	// FOV
	SetupCamera_Perspective(kFov);
}

void Camera::Update()
{

	m_deqTargetPos.push_front(m_targetPos);
	m_deqAnglePos.push_front(m_playerAngle);
	if (m_deqTargetPos.size() > 12 &&
		m_deqAnglePos.size()  > 12)
	{
		m_deqTargetPos.pop_back();
		m_deqAnglePos.pop_back();
	}

	m_angle = (m_angle * 0.0f) + (m_deqAnglePos.back() * 1.0f);
	const MATRIX cameraRotMtx = MGetRotY(m_angle);

	// �W�����v���͒P���Ƀv���C���[�ɕ��]����̂ł͂Ȃ�
	//�v���C���[�̗����Ă����ʒu������悤�ɂ���
	VECTOR cameraTrans = m_deqTargetPos.back();
	cameraTrans.y = m_deqTargetPos.back().y * 0.0f;
	const MATRIX playerTransMtx = MGetTranslate(cameraTrans);

	// �v���C���[�̉�]�ɍ��킹�ăJ�����ʒu�A�����_����]������
	// �v���C���[�̉�]���ƕ��s�ړ���������
	const MATRIX cameraMtx = MMult(cameraRotMtx, playerTransMtx);

	const VECTOR cameraPos = VTransform(kCameraPos, cameraMtx);
	const VECTOR cameraTarget = VTransform(kCameraTarget, cameraMtx);

	//m_deqTargetPos.push_front(m_playerAngle);
	//if (m_deqTargetPos.size() > 8)
	//{
	//	m_deqTargetPos.pop_back();
	//}

	//m_angle = (m_angle * 0.0f) + (m_deqTargetPos.back() * 1.0f);
	//const MATRIX cameraRotMtx = MGetRotY(m_angle);

	// �W�����v���͒P���Ƀv���C���[�ɕ��]����̂ł͂Ȃ�
	//�v���C���[�̗����Ă����ʒu������悤�ɂ���
	//VECTOR cameraTrans = m_targetPos;
	//cameraTrans.y = m_targetPos.y * 0.0f;
	//const MATRIX playerTransMtx = MGetTranslate(cameraTrans);

	// �v���C���[�̉�]�ɍ��킹�ăJ�����ʒu�A�����_����]������
	// �v���C���[�̉�]���ƕ��s�ړ���������
	//const MATRIX cameraMtx = MMult(cameraRotMtx, playerTransMtx);

	//const VECTOR cameraPos = VTransform(kCameraPos, cameraMtx);
	//const VECTOR cameraTarget = VTransform(kCameraTarget, cameraMtx);


	// �J�����̈ʒu�A�ǂ������Ă��邩��ݒ肷��
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
