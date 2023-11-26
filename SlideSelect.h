#pragma once

// �^�C�g����ʂŃX���C�h���s���I�������܂�
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
	// �I�������{�^���̌��ʂ̔ԍ���Ԃ��܂�
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

	// �I��p�t���[���J�E���g
	int m_selectFramesPressCount[SelectButton::BUTTONMAX];
	int m_selectUpSpeedFramePressCount[SelectButton::BUTTONMAX];
	// �I��ύX���\���ǂ���
	int m_isSelect;
	
	// ��ʂɉ摜��\�������邩�ǂ���
	int m_isImageDraw;
};

