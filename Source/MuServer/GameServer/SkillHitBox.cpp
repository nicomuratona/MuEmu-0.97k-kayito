#include "stdafx.h"
#include "SkillHitBox.h"
#include "ReadScript.h"
#include "Util.h"

CSkillHitBox gSkillHitBox;

CSkillHitBox::CSkillHitBox()
{

}

CSkillHitBox::~CSkillHitBox()
{

}

void CSkillHitBox::Init()
{
	memset(this->m_table, 0, sizeof(this->m_table));
}

void CSkillHitBox::Load(char* path)
{
	this->Init();

	HANDLE file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, 0);

	if (file == INVALID_HANDLE_VALUE)
	{
		ErrorMessageBox(READ_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (GetFileSize(file, 0) != sizeof(this->m_table))
	{
		ErrorMessageBox(READ_SCRIPT_UNKNOWN_ERROR, path);

		CloseHandle(file);

		return;
	}

	DWORD OutSize = 0;

	if (ReadFile(file, this->m_table, sizeof(this->m_table), &OutSize, 0) == 0)
	{
		ErrorMessageBox(READ_SCRIPT_FILE_ERROR, path);

		CloseHandle(file);

		return;
	}

	CloseHandle(file);
}

int CSkillHitBox::Check(int dir, int x, int y)
{
	if (dir < 0 || dir >= 36 || x < 0 || x >= 21 || y < 0 || y >= 21)
	{
		return 0;
	}

	return this->m_table[dir][(y * 21) + x];
}

int CSkillHitBox::HitCheck(int dir, int x, int y, int tx, int ty)
{
	dir /= 10;

	if (dir < 0 || dir >= 36)
	{
		return 0;
	}

	int px = (tx - x) + 10;

	int py = (ty - y) + 10;

	if (px < 0 || abs(px) >= 21 || py < 0 || abs(py) >= 21)
	{
		return 0;
	}

	return this->m_table[dir][(py * 21) + px];
}