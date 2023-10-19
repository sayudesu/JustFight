#include "Camera.h"
#include <math.h>
#include "../../Util/Pad.h"

namespace
{
	// ����p
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
	// �ǂ�����A�ǂ��܂Ō����邩
	SetCameraNearFar(100.0f, 4000.0f);
	// �ǂ������Ăǂ����݂邩
	SetCameraPositionAndTarget_UpVecY(m_pos, m_posTarget);
	// ���ߖ@�̃Z�b�g�A�b�v( ���W�A���l�ɕϊ����Ă��܂� )
	SetupCamera_Perspective(kFov);
}

void Camera::Update()
{
	DINPUT_JOYSTATE input;
	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	// �J�����̉�]�p�x�𒲐�
	if (input.Rx > 30)
	{
		m_angle.y -= 3.0f;
	}
	if (input.Rx < -30)
	{
		m_angle.y += 3.0f;
	}

	// �J�����̈ʒu���X�V
	// �J�����̍�����300.0f�ɌŒ�
	m_pos.x = m_posTarget.x + 500.0f * sinf(m_angle.y * DX_PI / 180);
	m_pos.y = m_posTarget.y + 300.0f; // �J�����̍������Œ�
	m_pos.z = m_posTarget.z - 500.0f * cosf(m_angle.y * DX_PI / 180);

	// �J�����ʒu�̐ݒ�
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
