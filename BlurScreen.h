#pragma once
class EffectScreen
{
private:
	EffectScreen() = default;
	static EffectScreen* m_pInstanceSound;
public:
	virtual~EffectScreen() = default;
	// �R�s�[�R���X�g���N�^�̋֎~
	EffectScreen(const EffectScreen&) = delete;
	EffectScreen& operator=(const EffectScreen&) = delete;
	EffectScreen(EffectScreen&&) = delete;
	EffectScreen& operator=(EffectScreen&&) = delete;

	// �C���X�^���X�̍쐬
	static EffectScreen& GetInstance()
	{
		if (!m_pInstanceSound)
		{
			m_pInstanceSound = new EffectScreen();
		}
		return *m_pInstanceSound;
	}

	// �������
	static void Destroy()
	{
		delete m_pInstanceSound;
		m_pInstanceSound = nullptr;
	}
public:

	// MakeScreen���ĉ�ʃT�C�Y���擾����
	void Load();
	void Unload();
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

