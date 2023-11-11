#pragma once

class CGuildLogo
{
public:

	CGuildLogo();

	virtual ~CGuildLogo();

	void Init();

private:

	static void DrawGuildLogo(int x, int y, DWORD c);
};

extern CGuildLogo gGuildLogo;