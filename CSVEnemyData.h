#pragma once

#include <DxLib.h>
#include <vector>
#include <string>

namespace CSVSoundData
{
	// ファイル位置
	const char* const fileNamePath = "Data/CSV/EnemyData.csv";
	// 書き込むデータ
	std::vector<std::vector<std::string>> firstData =
	{
		{"No","Name","ValueNum","Parameter"},

		{"1","attackFrameMax","1","30"},
		{"2","attackFrameGapMax","1","0"},
		{"3","attackTotalFrame","1","30"},

		{"4","attackAfterStopFrameMax","1","60"},

		{"5","strongAttackFrameMax","1","5"},
		{"6","strongAttackFrameGapMax","1","60"},
		{"7","strongAttackTotalFrame","1","65"},

		{"8","guardFrameMax","1","20"},
		{"9","justGuardFrameMax","1","15"},

		{"10","stunFrameMax","1","180"},

		{"11","hpMax","1","6"},
		{"12","fightingMeterMax","1","100"},

		{"13","weaponRelativePos","3","-80","100","-50"},
		{"14","shieldRelativePos","3","100","100","-50"},

		{"15","weaponAttackRadius","1","100"},
		{"16","shieldRadius","1","50"},
		{"17","modelRadius","1","180"},

		{"18","weaponAttackPos","3","0","0","-210"},
		{"19","knockBackPos","3","0","0","-20"},

		{"20","weaponBackSpeed","1","30"},
		{"21","shieldBackSpeed","1","30"},		
	};
}