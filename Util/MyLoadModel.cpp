#include "MyLoadModel.h"
#include <DxLib.h>
#include <cassert>

// 3D���f���̃��[�h
namespace MyModel3D
{
	// �p�X�������Ă邩assert���g�p����
	int MyModel3D::Load(const char* const filePathName)
	{
		int handle = MV1LoadModel(filePathName);
		assert(handle != -1);
		return handle;
	}

	// �������
	int MyModel3D::End(int handle)
	{
		return MV1DeleteModel(handle);
	}
}
