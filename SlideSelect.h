#pragma once

// タイトル画面でスライドを行い選択をします
class SlideSelect
{
public:
	SlideSelect();
	~SlideSelect();

	void Init(int selectMaxNum);
	void Update();

public:
	// 選択したボタンの結果の番号を返します
	int GetResult();
	// 現在の選択している番号を返します
	int GetSelect();
private:
	enum SelectButton
	{
		UP,
		DOWN,

		BUTTONMAX,
	};
private:
	int m_selectNo;
	int m_selectNoResult;

	// 選択できる最大の数
	int m_selectMaxNum;

	// 選択用フレームカウント
	int m_selectFramesPressCount[SelectButton::BUTTONMAX];
	int m_selectUpSpeedFramePressCount[SelectButton::BUTTONMAX];

	// 選択変更が可能かどうか
	int m_isSelect;

	// 選択を変更する為のフレーム
	int m_selectFrameMax[SelectButton::BUTTONMAX];
};

