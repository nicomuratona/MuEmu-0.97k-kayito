#pragma once

#include "DefaultClassInfo.h"
#include "User.h"

#define MAX_QUEST_OBJECTIVE 100

enum eQuestObjectiveType
{
	QUEST_OBJECTIVE_NONE = 0,
	QUEST_OBJECTIVE_ITEM = 1,
	QUEST_OBJECTIVE_MONEY = 2,
};

struct QUEST_OBJECTIVE_INFO
{
	int Number;
	int Type;
	int Index;
	int Value;
	int Level;
	int SkillOption;
	int LuckOption;
	int AddOption;
	int ExceOption;
	int DropMinLevel;
	int DropMaxLevel;
	int ItemDropRate;
	int RequireIndex;
	int RequireState;
	int RequireClass[MAX_CLASS];
};

class CQuestObjective
{
public:

	CQuestObjective();

	~CQuestObjective();

	void Load(char* path);

	void SetInfo(QUEST_OBJECTIVE_INFO info);

	QUEST_OBJECTIVE_INFO* GetInfo(int index);

	int GetQuestObjectiveCount(LPOBJ lpObj, QUEST_OBJECTIVE_INFO* lpInfo);

	bool CheckQuestObjectiveRequisite(LPOBJ lpObj, QUEST_OBJECTIVE_INFO* lpInfo);

	bool CheckQuestObjective(LPOBJ lpObj, int QuestIndex);

	bool CheckQuestObjectiveItemCount(LPOBJ lpObj, int ItemIndex, int ItemLevel);

	bool CanGetItem(LPOBJ lpObj, int ItemIndex, int ItemLevel);

	void RemoveQuestObjective(LPOBJ lpObj, int QuestIndex);

	bool MonsterItemDrop(LPOBJ lpMonster);

private:

	QUEST_OBJECTIVE_INFO m_QuestObjectiveInfo[MAX_QUEST_OBJECTIVE];

	int m_count;
};

extern CQuestObjective gQuestObjective;