#pragma once
#include <DxLib.h>

class FIeldDrawer
{
public:
	FIeldDrawer();
	~FIeldDrawer();

	void Init();
	void End();
	void Draw();
private:
	// �}�b�v�p�̃n���h��
	int m_handle;
	// �ʒu
	VECTOR m_pos;	
};

