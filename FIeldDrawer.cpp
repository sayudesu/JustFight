#include "FIeldDrawer.h"

FIeldDrawer::FIeldDrawer():
	m_handle(-1),
	m_pos(VGet(0,0,0))
{
}

FIeldDrawer::~FIeldDrawer()
{
}

void FIeldDrawer::Init()
{
	// ���f���̓ǂݍ���
	m_handle = MV1LoadModel("Data/Model/Arena.mv1");

	// �ʒu��ݒ�
	MV1SetPosition(m_handle, m_pos);
	// �T�C�Y�̕ύX
	MV1SetScale(m_handle, VGet(2.0f, 2.0f, 2.0f));
}

void FIeldDrawer::End()
{
	// ���
	MV1DeleteModel(m_handle);
}

void FIeldDrawer::Draw()
{
	// �`��
	MV1DrawModel(m_handle);

#if _DEBUG
	VECTOR start = VGet(1200, 300, 1200);
	VECTOR end = VGet(-1200, 300, 1200);
	DrawLine3D(start, end, 0xffffff);

	start = VGet(-1200, 300, -1200);
	end = VGet(1200, 300, -1200);
	DrawLine3D(start, end, 0xffffff);

	start = VGet(1200, 300, -1200);
	end = VGet(1200, 300, 1200);
	DrawLine3D(start, end, 0xffffff);

	start = VGet(-1200, 300, 1200);
	end = VGet(-1200, 300, -1200);
	DrawLine3D(start, end, 0xffffff);
	// ��
	VECTOR Xp = ConvWorldPosToScreenPos(VGet(1000, 300, 0));
	VECTOR Xm = ConvWorldPosToScreenPos(VGet(-1000, 300, 0));

	VECTOR Zp = ConvWorldPosToScreenPos(VGet(0, 300, 1000));
	VECTOR Zm = ConvWorldPosToScreenPos(VGet(0, 300, -1000));

	DrawString(Xp.x, Xp.y, "X+", 0xffffff);
	DrawString(Xm.x, Xm.y, "X-", 0xffffff);
	DrawString(Zp.x, Zp.y, "Z+", 0xffffff);
	DrawString(Zm.x, Zm.y, "Z-", 0xffffff);
#endif
}
