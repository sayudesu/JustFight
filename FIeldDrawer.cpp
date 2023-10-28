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
}
