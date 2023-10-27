#include <DxLib.h>
#include <EffekseerForDXLib.h> 

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

	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	// DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)		
	{
		// エラーが起きたら直ちに終了
		return -1;			
	}

	// Effekseerを初期化する。
// 引数には画面に表示する最大パーティクル数を設定する。
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	// ただし、DirectX11を使用する場合は実行する必要はない。
//	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// 3D関連の設定
	// Zバッファを有効にする。
	SetUseZBuffer3D(true);
	// Zバッファへの書き込みを有効にする。
	// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetWriteZBuffer3D(true);
	// ポリゴンの裏面を描画しない
	SetUseBackCulling(true);

	SceneManager* pScene = new SceneManager();

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

	// Effekseerを終了する。
	Effkseer_End();

	// ＤＸライブラリ使用の終了処理
	DxLib_End();				

	// ソフトの終了 
	return 0;				
}