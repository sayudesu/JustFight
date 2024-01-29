#pragma once

// �^�C�g����ʂŃX���C�h���s���I�������܂�
class SlideSelect
{
public:
	SlideSelect();
	~SlideSelect();

	void Init(int selectMaxNum, bool isUpDown);
	void Update();

public:
	// �I�������{�^���̌��ʂ̔ԍ���Ԃ��܂�
	int GetResult();
	// ���݂̑I�����Ă���ԍ���Ԃ��܂�
	int GetSelect();
	// ��̃{�^�������������ǂ�����Ԃ��܂�
	bool IsUpBotton();
	// ���̃{�^�������������ǂ�����Ԃ��܂�
	bool IsDownBotton();
	// ���݂̑I����Ԃ����Z�b�g����
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

	// �I���ł���ő�̐�
	int m_selectMaxNum;

	// �I��p�t���[���J�E���g
	int m_selectFramesPressCount[SelectButton::BUTTONMAX];
	int m_selectUpSpeedFramePressCount[SelectButton::BUTTONMAX];

	// �I��ύX���\���ǂ���
	int m_isSelect;

	// �I����ύX����ׂ̃t���[��
	int m_selectFrameMax[SelectButton::BUTTONMAX];

	bool m_isUpBotton;
	bool m_isDownBotton;

	int m_selectButton[2];


};

