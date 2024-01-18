#pragma once
#include <vector>
#include <string>

namespace CSVSoundData
{
	// �t�@�C���ʒu
	const char* const fileNamePath = "Data/CSV/SoundData.csv";
	// �������ރf�[�^
	std::vector<std::vector<std::string>> firstData =
	{
		{"No","Type","Name", "Volume", "Extension"},
		{"0","SE","Attack", "255", ".mp3"},
		{"1","SE","Guard", "255", ".mp3"},
		{"2","SE","GuardResult", "255", ".mp3"},
		{"3","SE","JustGurad", "255", ".mp3"},
		{"4","SE","SpeedMove2", "255", ".mp3"},
		{"5","SE","Stun", "255", ".mp3"},
		{"6","SE","Damage", "255", ".mp3"},
		{"7","BGM","NONE", "255", ".mp3"},
		{"8","BGM","NONE", "255", ".mp3"},
		{"9","BGM","NONE", "255", ".mp3"},
	};
}