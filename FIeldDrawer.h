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
	// マップ用のハンドル
	int m_handle;
	// 位置
	VECTOR m_pos;	
};

