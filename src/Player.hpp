#pragma once

#include <cmath>
#include <d3dx8math.h>

#include "AnmVm.hpp"
#include "BulletManager.hpp"
#include "Chain.hpp"
#include "ZunBool.hpp"
#include "ZunResult.hpp"
#include "inttypes.hpp"

struct Player;

enum PlayerDirection
{
    MOVEMENT_NONE,
    MOVEMENT_UP,
    MOVEMENT_DOWN,
    MOVEMENT_LEFT,
    MOVEMENT_RIGHT,
    MOVEMENT_UP_LEFT,
    MOVEMENT_UP_RIGHT,
    MOVEMENT_DOWN_LEFT,
    MOVEMENT_DOWN_RIGHT
};

enum Character
{
    CHARA_REIMU,
    CHARA_MARISA,
};

enum ShotType
{
    SHOT_TYPE_A,
    SHOT_TYPE_B,
};

enum BulletType
{
    BULLET_TYPE_0,
    BULLET_TYPE_1,
    BULLET_TYPE_2,
    BULLET_TYPE_LASER
};

enum PlayerState
{
    PLAYER_STATE_ALIVE,
    PLAYER_STATE_SPAWNING,
    PLAYER_STATE_DEAD,
    PLAYER_STATE_INVULNERABLE,
};

enum OrbState
{
    ORB_HIDDEN,
    ORB_UNFOCUSED,
    ORB_FOCUSING,
    ORB_FOCUSED,
    ORB_UNFOCUSING,
};

enum BulletState
{
    BULLET_STATE_UNUSED,
    BULLET_STATE_FIRED,
    BULLET_STATE_COLLIDED,
};

struct BombData
{
    void (*calc)(Player *p);
    void (*draw)(Player *p);
};

struct PlayerRect
{
    D3DXVECTOR2 pos;
    D3DXVECTOR2 size;
};
C_ASSERT(sizeof(PlayerRect) == 0x10);

struct PlayerBullet
{
    AnmVm sprite;
    D3DXVECTOR3 position;
    D3DXVECTOR3 size;
    D3DXVECTOR2 velocity;
    f32 sidewaysMotion;
    D3DXVECTOR3 unk_134;
    ZunTimer unk_140;
    u16 unk_14c;
    i16 bulletState;
    u16 bulletType;
    u16 unk_152;
    u16 spawnPositionIdx;
};
C_ASSERT(sizeof(PlayerBullet) == 0x158);

struct PlayerBombInfo
{
    u32 isInUse;
    u32 duration;
    ZunTimer timer;
    void (*calc)(Player *p);
    void (*draw)(Player *p);
    u32 unk_1c[8];
    f32 unk_3c[8];
    D3DXVECTOR3 unk_5c[8];
    D3DXVECTOR3 unk_bc[8];
    AnmVm sprites[8][4];
};
C_ASSERT(sizeof(PlayerBombInfo) == 0x231c);

typedef i32 FireBulletResult;
#define FBR_STOP_SPAWNING (-2)
#define FBR_SPAWN_MORE (-1)

typedef FireBulletResult (*FireBulletCallback)(Player *, PlayerBullet *, u32, u32);
struct CharacterData
{
    f32 orthogonalMovementSpeed;
    f32 orthogonalMovementSpeedFocus;
    f32 diagonalMovementSpeed;
    f32 diagonalMovementSpeedFocus;
    FireBulletCallback fireBulletCallback;
    FireBulletCallback fireBulletFocusCallback;
};
C_ASSERT(sizeof(CharacterData) == 0x18);

struct CharacterPowerBulletData
{
    i16 waitBetweenBullets;
    i16 bulletFrame;
    ZunVec2 motion;
    ZunVec2 size;
    f32 direction;
    f32 velocity;
    u16 unk_1c;
    u8 spawnPositionIdx;
    u8 bulletType;
    i16 anmFileIdx;
    i16 bulletSoundIdx;

    f32 HorizontalDirection(f32 direction)
    {
        return cos(direction);
    }

    f32 VerticalDirection(f32 direction)
    {
        return sin(direction);
    }
};
C_ASSERT(sizeof(CharacterPowerBulletData) == 0x24);

struct CharacterPowerData
{
    i32 numBullets;
    i32 power;
    CharacterPowerBulletData *bullets;
};
C_ASSERT(sizeof(CharacterPowerData) == 0xc);

struct Player
{
    Player();

    static ZunResult RegisterChain(u8 unk);
    static void CutChain();
    static ChainCallbackResult OnUpdate(Player *p);
    static ChainCallbackResult OnDrawHighPrio(Player *p);
    static ChainCallbackResult OnDrawLowPrio(Player *p);
    static ZunResult AddedCallback(Player *p);
    static ZunResult DeletedCallback(Player *p);

    static FireBulletResult FireSingleBullet(Player *, PlayerBullet *bullet, i32 bullet_idx, i32 framesSinceLastBullet,
                                             CharacterPowerData *powerData);

    static FireBulletResult FireBulletReimuA(Player *, PlayerBullet *, u32, u32);
    static FireBulletResult FireBulletReimuB(Player *, PlayerBullet *, u32, u32);
    static FireBulletResult FireBulletMarisaA(Player *, PlayerBullet *, u32, u32);
    static FireBulletResult FireBulletMarisaB(Player *, PlayerBullet *, u32, u32);

    static void BombReimuACalc(Player *);
    static void BombReimuBCalc(Player *);
    static void BombMarisaACalc(Player *);
    static void BombMarisaBCalc(Player *);
    static void BombReimuADraw(Player *);
    static void BombReimuBDraw(Player *);
    static void BombMarisaADraw(Player *);
    static void BombMarisaBDraw(Player *);

    static void StartFireBulletTimer(Player *);
    ZunResult HandlePlayerInputs();
    static void UpdatePlayerBullets(Player *);
    static ZunResult UpdateFireBulletsTimer(Player *);

    static void SpawnBullets(Player *, u32 timer);
    static void DrawBullets(Player *);
    static void DrawBulletExplosions(Player *);

    f32 AngleToPlayer(D3DXVECTOR3 *pos);
    i32 CheckGraze(D3DXVECTOR3 *center, D3DXVECTOR3 *hitbox);
    i32 CalcKillBoxCollision(D3DXVECTOR3 *bulletCenter, D3DXVECTOR3 *bulletSize);
    i32 CalcLaserHitbox(D3DXVECTOR3 *laserCenter, D3DXVECTOR3 *laserSize, D3DXVECTOR3 *rotation, f32 angle,
                        i32 canGraze);
    i32 CalcDamageToEnemy(D3DXVECTOR3 *enemyPos, D3DXVECTOR3 *enemySize, i32 *unk);
    void Die();

    AnmVm playerSprite;
    AnmVm orbsSprite[3];
    D3DXVECTOR3 positionCenter;
    D3DXVECTOR3 unk_44c;
    D3DXVECTOR3 hitboxTopLeft;
    D3DXVECTOR3 hitboxBottomRight;
    D3DXVECTOR3 grabItemTopLeft;
    D3DXVECTOR3 grabItemBottomRight;
    D3DXVECTOR3 hitboxSize;
    D3DXVECTOR3 grabItemSize;
    D3DXVECTOR3 orbsPosition[2];
    D3DXVECTOR3 unk_4b8[32];
    D3DXVECTOR3 unk_638[32];
    i32 unk_7b8[32];
    i32 unk_838[32];
    PlayerRect bombProjectiles[16];
    ZunTimer laserTimer[2];
    f32 horizontalMovementSpeedMultiplierDuringBomb;
    f32 verticalMovementSpeedMultiplierDuringBomb;
    i32 respawnTimer;
    i32 bulletGracePeriod;
    i8 playerState;
    u8 unk_9e1;
    i8 orbState;
    i8 isFocus;
    u8 unk_9e4;
    ZunTimer focusMovementTimer;
    CharacterData characterData;
    PlayerDirection playerDirection;
    f32 previousHorizontalSpeed;
    f32 previousVerticalSpeed;
    i16 previousFrameInput;
    D3DXVECTOR3 positionOfLastEnemyHit;
    PlayerBullet bullets[80];
    ZunTimer fireBulletTimer;
    ZunTimer invulnerabilityTimer;
    FireBulletCallback fireBulletCallback;
    FireBulletCallback fireBulletFocusCallback;
    PlayerBombInfo bombInfo;
    ChainElem *chainCalc;
    ChainElem *chainDraw1;
    ChainElem *chainDraw2;
};
C_ASSERT(sizeof(Player) == 0x98f0);

DIFFABLE_EXTERN(Player, g_Player);
