#pragma once

namespace DEBUG
{
	/// <summary>
	/// �t���[���̊m�F
	/// </summary>
	/// <param name="name">���O</param>
	/// <param name="posX">�ʒu</param>
	/// <param name="posY">�ʒu</param>
	/// <param name="frameMaxNum">�ő�̃t���[����</param>
	/// <param name="frameNow">���݂̃t���[����</param>
	/// <param name="meterSize">���[�^�[���m�̊�</param>
	/// <param name="color">���[�^�[�̐F</param>
	void FrameMeter(const char* const name,int posX, int posY, int frameMaxNum, int frameNow, int meterSize,int color);

	// �n�ʂ�`��
	void Field();
}

