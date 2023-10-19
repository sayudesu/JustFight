#include "MyLoadModel.h"
#include <DxLib.h>
#include <cassert>

namespace MyModel3D
{
	int MyModel3D::Load(const char* const filePathName)
	{
		int handle = MV1LoadModel(filePathName);
		assert(handle != -1);
		return handle;
	}

	int MyModel3D::End(int handle)
	{
		return MV1DeleteModel(handle);
	}
}
