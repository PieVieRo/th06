#pragma once

#include "Chain.hpp"
#include "Enemy.hpp"
#include "ZunResult.hpp"
#include "inttypes.hpp"
#include <Windows.h>

struct EnemyManager
{
    void Initialize();
    static ZunResult RegisterChain(char *stgEnm1, char *stgEnm2);
    static ChainCallbackResult OnUpdate(EnemyManager *enemyManager);
    static ChainCallbackResult OnDraw(EnemyManager *enemyManager);
    static ZunResult AddedCallback(EnemyManager *enemyManager);
    static ZunResult DeletedCallback(EnemyManager *enemyManager);

    char *stgEnmAnmFilename;
    char *stgEnm2AnmFilename;
    Enemy enemyTemplate;
    Enemy enemies[256];
    Enemy dummyEnemyForFailedSpawns;
    Enemy *bosses[8];
    u16 randomItemSpawnIndex;
    u16 randomItemTableIndex;
    i32 enemyCount;
    i8 unk_ee5c0[4];
    i32 spellcardRelated;
    i32 spellcardCapture;
    i32 spellcardBonus;
    i32 spellcardNumber;
    i8 unk_ee5d4[8];
    void *timelineInstr;
    ZunTimer timelineTime;
};
C_ASSERT(sizeof(EnemyManager) == 0xee5ec);
