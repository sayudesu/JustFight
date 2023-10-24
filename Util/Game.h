#pragma once

namespace Game
{
	// ウインドウモード設定
	constexpr bool kWindowMode = true;
	// ウインドウ名
	const char* const kTitleText = "ドンピシャふぁいと";
	// ウインドウサイズ
	constexpr int kScreenWidth = 1920;
	constexpr int kScreenHeight = 1080;
	// カラーモード
	constexpr int kColorDepth = 32;
	// windowモード時にwindowサイズを変更するかどうか
	constexpr bool kWindowSizeChange = true;
	// ログテキストを作成するかどうか
	constexpr bool kCreateLogText = false;
};