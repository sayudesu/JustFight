#include "DEBUG.h"
#include <DxLib.h>

namespace DEBUG
{
	void DEBUG::FrameMeter(const char* const name,int posX, int posY,int frameMaxNum,int frameNow,int meterSize,int color)
	{
		DrawString(posX, posY - 15, name, 0xffffff);

		// 攻撃フレームメーター
		for (int i = 0; i <= frameMaxNum; i++)
		{
			int x = (meterSize * i) + posX;
			int y = posY;

			int x2 = (meterSize * i) + posX;
			int y2 = y + 50;

			DrawFormatString(x, y, 0x000000, "%d", i);
			DrawLine(
				x, y,
				x2, y2,
				color);
		}

		{
			int x = posX;
			int y = posY;

			int x2 = x;
			int y2 = y + 50;

			DrawLine(
				x + (meterSize * frameNow), y,
				x2 + (meterSize * frameNow), y2,
				0x0000ff);
		}
	}
	void DEBUG::Field()
	{
		for (int i = 0; i < 30; i++)
		{
			const VECTOR sPos = VGet(-1000 + (70 * i), 0, 1000);
			const VECTOR ePos = VGet(-1000 + (70 * i), 0, -1000);
			DrawLine3D(sPos, ePos, 0xff0000);
		}
		for (int i = 0; i < 30; i++)
		{
			const VECTOR sPos = VGet(1000, 0, -1000 + (70 * i));
			const VECTOR ePos = VGet(-1000, 0, -1000 + (70 * i));
			DrawLine3D(sPos, ePos, 0xffff00);
		}
	}
}
