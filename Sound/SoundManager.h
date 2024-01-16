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

	// CSVファイルにデータを書き込む関数
	void WriteCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data);

	// サウンドを再生
	void Play(SoundName name);

	
};

