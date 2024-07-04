#pragma once

namespace MyModel3D
{
	/// <summary>
	/// モデルの読み込み
	/// </summary>
	/// <param name="filePathName">モデルのパス</param>
	/// <returns></returns>
	int Load(const char* const filePathName);
	/// <summary>
	/// モデルの解放
	/// </summary>
	/// <param name="handle">モデルのハンドルデータ</param>
	/// <returns></returns>
	int End(int handle);
}
