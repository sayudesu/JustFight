#pragma once

// タイトル画面でスライドを行い選択をします
class SlideSelect
{
public:
	SlideSelect();
	~SlideSelect();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="selectMaxNum">選択肢の最大巣 0〜 </param>
	/// <param name="isUpDown">上下に選択するかどうあ</param>
	void Init(int selectMaxNum, bool isUpDown);

	// 更新処理
	void Update();

public:
	// 選択したボタンの結果の番号を返します
	int GetResult();
	// 現在の選択している番号を返します
	int GetSelect();
	// 上のボタンを押したかどうかを返します
	bool IsUpBotton();
	// 下のボタンを押したかどうかを返します
	bool IsDownBotton();
	// 現在の選択状態をリセットする
	void Reset();
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

	bool m_isUpBotton;
	bool m_isDownBotton;

	int m_selectButton[2];


};

