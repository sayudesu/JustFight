#pragma once
#include <vector>
#include <string>

namespace CSVModelData
{
	const char* const filePath = "Data/CSV/ModelData.csv";

	std::vector<std::vector<std::string>> firstData =
	{
		{"No","Name", "Extension"},

		// キャラクターモデル
		{"0","Pawn_B", ".mv1"},
		{"1","Pawn_W", ".mv1"},

		{"2","Knight_B", ".mv1"},
		{"3","Knight_W", ".mv1"},

		{"4","Queen_B", ".mv1"},
		{"5","Queen_W", ".mv1"},

		// マップモデル
		{"6","洋館部屋1", ".mv1"},// 名前は変えます
		{"7","Stage", ".mv1"},
		{"8","Map/model/Map", ".mv1"},

		// 装備
		{"9","SwordCollTest", ".mv1"},
		{"10","Shield", ".mv1"},
	};
}
