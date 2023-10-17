#include <DxLib.h>

#include "Util/Game.h"
#include "Scene/SceneManager.h"

// プログラムはWinMainから始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// windowモード設定
	ChangeWindowMode(Game::kWindowMode);
	// ウインドウ名設定
	SetMainWindowText(Game::kTitleText);
	// 画面サイズの設定
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);
	// windowモード時にwindowサイズを変更するかどうか
	SetWindowSizeChangeEnableFlag(Game::kWindowSizeChange);
	// ログテキストを作成するかどうか
	SetOutApplicationLogValidFlag(Game::kCreateLogText);


	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)		
	{
		// エラーが起きたら直ちに終了
		return -1;			
	}

	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	SceneManager* pScene = new SceneManager;

	pScene->Init();

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();

		// 画面のクリア
		ClearDrawScreen();

		pScene->Update();

		pScene->Draw();

		// 裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	pScene->End();

	// ＤＸライブラリ使用の終了処理
	DxLib_End();				

	// ソフトの終了 
	return 0;				
}