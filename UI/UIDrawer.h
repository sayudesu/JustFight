#pragma once
#include "../Util/CharacterParameter.h";
#include "../Util/CharacterName.h";
#include <vector>

class UIDrawer
{
public:
	UIDrawer();
	~UIDrawer();

	void Draw();
public:
	/// <summary>
	/// パラメーターを受け取ります
	/// </summary>
	/// <param name="name">名前を指定</param>
	/// <param name="param">名前で指定したキャラのパラメーター</param>
	void SetParam(CharacterName name, CharacterParameter param);
private:
	enum class HandleType
	{
		BG,// 背景

		CHARACTOR,// キャラクターの画像

		HP,// 体力
		HP_BG,// 体力の背景

		SP,// スキル
		SP_BG,// スキルの背景

		BAR_OUTSIDE,// 枠

		Max,// 最大数
	};
	// 画像用ハンドル
	int m_handle[static_cast<int>(CharacterName::MAX)][static_cast<int>(HandleType::Max)];
};

