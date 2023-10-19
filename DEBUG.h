#pragma once

namespace DEBUG
{
	/// <summary>
	/// フレームの確認
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="posX">位置</param>
	/// <param name="posY">位置</param>
	/// <param name="frameMaxNum">最大のフレーム数</param>
	/// <param name="frameNow">現在のフレーム数</param>
	/// <param name="meterSize">メーター同士の間</param>
	/// <param name="color">メーターの色</param>
	void FrameMeter(const char* const name,int posX, int posY, int frameMaxNum, int frameNow, int meterSize,int color);

	// 地面を描画
	void Field();
}

