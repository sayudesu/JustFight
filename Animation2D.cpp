#include "Animation2D.h"

namespace
{
	// �A�j���[�V�������X�V����t���[��
	constexpr int kAnimFrameMax = 8;
}

Animation2D::Animation2D():
	m_handle(-1),
	m_pos(VGet(0,0,0)),
	m_chipX(0),
	m_chipY(0),
	m_amimX(0),
	m_amimY(0),
	m_animFrame(0),
	m_isAnimStart(false)
{
}

Animation2D::~Animation2D()
{
}

void Animation2D::Init(const char* fileName,int chipX, int chipY)
{
	m_handle = LoadGraph("Data/Image/Sstun.png");
	m_chipX = chipX;
	m_chipY = chipY;
}

void Animation2D::End()
{
	DeleteGraph(m_handle);
}

void Animation2D::Update()
{
	if (m_isAnimStart)
	{
		m_animFrame++;

		// �A�j���[�V�����̍X�V����
		if (m_animFrame == kAnimFrameMax)
		{
			m_animFrame = 0;

			// �A�j���[�V�����摜�̍ő吔��������
			if (m_amimX > 2)
			{
				m_amimX = 0;
			}
			else
			{
				m_amimX++;
			}
		}
	}


}

void Animation2D::Draw()
{
	if (m_isAnimStart)
	{
		DrawRectRotaGraph(
			m_pos.x, m_pos.y,
			32 * (m_chipX + m_amimX), 32 * (m_chipY + m_amimY),
			32, 32,
			4,
			DX_PI_F / 180.0f,
			m_handle,
			true);
	}
}

void Animation2D::SetPlay(bool isAnim)
{
	m_isAnimStart = isAnim;
}

void Animation2D::SetPos3D(VECTOR pos)
{
	VECTOR tempPos = VGet(pos.x, pos.y + 300, pos.z);
	// ���[���h���W���X�N���[�����W�ɕϊ����� 
	m_pos = ConvWorldPosToScreenPos(tempPos);
}
