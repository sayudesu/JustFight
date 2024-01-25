#pragma once

#include <vector>
#include <string>

namespace CSVFontData
{

	const char* const filePath = "Data/CSV/FontData.csv";

	// èëÇ´çûÇﬁÉfÅ[É^
	std::vector<std::vector<std::string>> data =
	{
		{"No","FileName","Name", "Size","SizeNo", "Extension"},
		{"0","NikkyouSans-mLKax","Nikkyou Sans", "12","0", ".ttf"},
		{"1","NikkyouSans-mLKax","Nikkyou Sans", "40","1", ".ttf"},
		{"2","NikkyouSans-mLKax","Nikkyou Sans", "82", "2",".ttf"},
		{"3","NikkyouSans-mLKax","Nikkyou Sans", "132", "3",".ttf"},
	};
}
