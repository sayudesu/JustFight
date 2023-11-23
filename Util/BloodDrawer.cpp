#include "BloodDrawer.h"
#include "game.h"

namespace
{
	// �d��
	constexpr float kGravity = 0.4f;
}

BloodDrawer::BloodDrawer(VECTOR pos)
{
	// �p�[�e�B�N���̏����ʒu
	m_pos = pos;
	m_vec = VGet(0, 0, 0);
	m_pFunc = &BloodDrawer::FirstMove;

	m_isErase = false;
}

BloodDrawer::~BloodDrawer()
{
}

void BloodDrawer::Init(int no)
{		
	// �ړ���
	if (no % 2 == 0)
	{
		m_vec.x = (-GetRand(10));
		m_vec.z = (-GetRand(10));
	}
	else
	{
		m_vec.x = (GetRand(10));
		m_vec.z = (GetRand(10));
	}
	m_vec.y = static_cast<float>(GetRand(12) + 1);
}

void BloodDrawer::Update()
{
	(this->*m_pFunc)();
}

void BloodDrawer::Draw()
{
	// �R�c��ԏ�ɋ���`�悷��
	DrawSphere3D(m_pos, 2, 3, 0xff0000, 0xff0000, true);

}

bool BloodDrawer::IsGetErase()
{
	return m_isErase;
}

void BloodDrawer::FirstMove()
{
	// �d�͂�^����
	m_vec.y -= kGravity;
	// �x�N�g���̉��Z
	m_pos = VAdd(m_pos, m_vec);

	if (m_pos.y < -100.0f)
	{
		m_isErase = true;
	}
}
