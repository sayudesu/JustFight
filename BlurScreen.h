#pragma once
class EffectScreen
{
public:
	EffectScreen();

	~EffectScreen();

	// MakeScreen���ĉ�ʃT�C�Y���擾����
	void Init();
	//	��ʂɕ`���ꂽ���̂���������
	void ClearScreen();
	// �`��𗠂̉�ʂɂ���
	void ScreenBack();

	// �u���[
	void BlurIReplayInit(int alpha);
	void BlurRelease();
	void BlurPreRenderBlurScreen();
	void BlurPostRenderBlurScreen();

	// �h�炵
	void QuakeReplayInit();
	void QuakeUpdate();
	void QuakeRelease();
	void QuakePreRenderBlurScreen();
	void QuakePostRenderBlurScreen();

	//// �J���[
	//void ColorReplayInit();
	//void ColorRelease();
	//void ColorPreRenderBlurScreen();
	//void ColorPostRenderBlurScreen();

private:
	enum class EffectNo
	{
		BLUR_0,
		BLUR_1,
		BLUR_2,
		BLUR_3,
		QUAKE,
		COLOR,
		MAX,
	};
	// ���ʕϐ�
	int m_screen[static_cast<int>(EffectNo::MAX)];

	// �u���[��p
	int m_current;
	int m_alpha;
	int m_notBlendDraw;

	// ��ʗh��p
	float m_quakeX;
	int m_quakeTimer;
};

