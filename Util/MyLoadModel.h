#pragma once

namespace MyModel3D
{
	/// <summary>
	/// ���f���̓ǂݍ���
	/// </summary>
	/// <param name="filePathName">���f���̃p�X</param>
	/// <returns></returns>
	int Load(const char* const filePathName);
	/// <summary>
	/// ���f���̉��
	/// </summary>
	/// <param name="handle">���f���̃n���h���f�[�^</param>
	/// <returns></returns>
	int End(int handle);
}
