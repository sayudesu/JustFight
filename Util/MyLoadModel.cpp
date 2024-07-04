#include "MyLoadModel.h"
#include <DxLib.h>
#include <cassert>

// 3Dモデルのロード
namespace MyModel3D
{
	// パスがあってるかassertを使用する
	int MyModel3D::Load(const char* const filePathName)
	{
		int handle = MV1LoadModel(filePathName);
		assert(handle != -1);
		return handle;
	}

	// 解放処理
	int MyModel3D::End(int handle)
	{
		return MV1DeleteModel(handle);
	}
}
