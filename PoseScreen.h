#pragma once

#include <memory>
#include <vector>

class SlideSelect;

class PoseScreen
{
private:
	PoseScreen() = default;
	static PoseScreen* m_pInstance;
public:
	virtual~PoseScreen() = default;
	// �R�s�[�R���X�g���N�^�̋֎~
	PoseScreen(const PoseScreen&) = delete;
	PoseScreen& operator=(const PoseScreen&) = delete;
	PoseScreen(PoseScreen&&) = delete;
	PoseScreen& operator=(PoseScreen&&) = delete;

	// �C���X�^���X�̍쐬
	static PoseScreen& GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new PoseScreen();
		}
		return *m_pInstance;
	}

	// �������
	static void Destroy()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

	// ���[�h
	void Load();
	// �A�����[�h
	void Unload();
public:
	void Update();
	void Draw();
private:
	struct Box
	{
		int x;
		int y;
		int x2;
		int y2;

		int color;
		bool fill;
	};

//	std::unique_ptr<SlideSelect> m_pSlide;
	SlideSelect* m_pSlide[3];

	// �A���t�@�u�����h��
	int m_blendModeRate[2];

	std::vector<Box> m_box;
};

