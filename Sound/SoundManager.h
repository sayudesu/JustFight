#pragma once
#include <string>
#include <vector>

#include "SoundData.h"

namespace SoundManager
{
	void Init();
	void End();
	void Update();

	std::vector<std::string> split(std::string& input, char delimiter);

	// CSV�t�@�C���Ƀf�[�^���������ފ֐�
	void WriteCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data);

	// �T�E���h���Đ�
	void Play(SoundName name);

	
};

