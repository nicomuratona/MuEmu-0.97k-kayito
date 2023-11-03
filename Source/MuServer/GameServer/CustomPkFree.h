#pragma once

struct MOVE_PKFREE_INFO
{
	int Map;
	int X;
	int Y;
	int TX;
	int TY;
};

class CPKFree
{
public:

	CPKFree();

	~CPKFree();

	void Load(char* path);

	bool CheckPKFree(int map, int x, int y);

private:

	std::vector<MOVE_PKFREE_INFO> m_PKFreeInfo;
};

extern CPKFree gPKFree;