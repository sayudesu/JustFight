#pragma once

#include <string>
#include <map>
#include "Tips.h"

class TutorialDrawer
{
public:
	TutorialDrawer();
	~TutorialDrawer();

	void Init();
	void End();

	void Update();
	void Draw();

public:

	// •`‰æ‚µ‚½‚¢à–¾‚ğŒˆ‚ß‚é
	void SetTips(Tips tips);

private:
	// à–¾—p•¶š
	std::string m_text;
	std::string m_step;

	bool m_tips[static_cast<int>(Tips::MAX)];

};

