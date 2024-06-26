#pragma once

#include <vector>
#include <string>

namespace CSVSubtitleData
{
	const char* const filePath = "Data/CSV/SubtitleData.csv";

	// 書き込むデータ
	std::vector<std::vector<std::string>> data =
	{
		{"No","String"},
		{"0","チュートリアル"},
		{"1","普通"},
		{"2","難しい"},
		{"3","オプション"},

		{"4","_説明�@_"},
		{"5"," 左のスティックで移動できる！"},
		{"6"," 敵に近づこう！"},

		{"7","_説明�A_"},
		{"8"," RB で 攻撃しよう！"},

		{"9","_説明�B_"},
		{"10"," LB で 防御しよう！"},

		{"11","_おつかれさま！_"},
		{"12"," これでチュートリアルは終了！"},
	};
}