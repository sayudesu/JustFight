#pragma once
class LoadingDrawer
{
public:
	LoadingDrawer();
	~LoadingDrawer();

	void SetUseASync(bool aSync);

	bool CheckHandle(int handle);

	void Draw();
};

