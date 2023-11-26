#pragma once

// タイトル画面でスライドを行い選択をします
class SlideSelect
{
public:
	SlideSelect();
	~SlideSelect();

	void Init();
	void End();
	void Update();
	void Draw();

public:
	// 選択したボタンの結果の番号を返します
	int GetResult();
private:
	enum SelectButton
	{
		UP,
		DOWN,

		BUTTONMAX,
	};
private:
	int m_handle;
	int m_selectNo;
	int m_selectNoResult;

	// 選択用フレームカウント
	int m_selectFramesPressCount[SelectButton::BUTTONMAX];
	int m_selectUpSpeedFramePressCount[SelectButton::BUTTONMAX];
	// 選択変更が可能かどうか
	int m_isSelect;
	
	// 画面に画像を表示させるかどうか
	int m_isImageDraw;
};

