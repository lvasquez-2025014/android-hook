#include <list>
#include <vector>
#include <string.h>
#include <string>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <map>
#include <unordered_map>
#include <chrono>
#include <sys/mman.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <mutex>
#include <thread>
#include <atomic>
#include "Tools/Includes/Logger.h"
#include "Tools/Includes/obfuscate.h"
#include "Tools/Includes/Utils.h"
#include "Tools/SOCKET/Server.h"
#include "Tools/KittyMemory/KittyInclude.hpp"
#include "Hack/Memory.h"





static std::string kh(const unsigned char *b, size_t n) {
    std::string s;
    s.reserve(n);
    for (size_t i = 0; i < n; i++) s += (char)(b[i] ^ 0x5A);
    return s;
}

bool isInsideFOV(int x, int y, float customFOV = 0.0f);
std::vector<void*> GetEntities(void* currentGame);

#include "Hack/class.h"
#include <stdarg.h>



#include "Tools/Includes/Macros.h"
#include "dobby.h"
#include "Tools/Canvas/Color.hpp"
#include <AutoUpdate/Il2Cppp.h>
#include "Unity/Unity.h"

#define targetLibName OBFUSCATE("libil2cpp.so")
#define LOGI(...) ((void)0)

#pragma once
struct {
    bool enableESP = false;
    float aimbotFOV = 9999.0f;
    bool autoswitchsafe = false;
    bool targetLock = false;
    bool Aimkillsend = false;
    bool backFakeFix = false;
    bool mode360v2 = false;
    bool loginIndia = false;
    bool downkill = false;
    bool downkillv2 = false;
    bool SilentGhostPullV2_Enable = false;
    bool AimkillTake = false;
    std::atomic<bool> resetGuest{false};
} MasterBool;


void* lockedEnemy = nullptr;
char lockedEnemyName[128] = {0};
bool wasLockedEnemyDown = false;
int g_screenWidth, g_screenHeight;

enum Mode {
    InitMode = 1,

    HackMode = 2,

    StopMode = 98,

    EspMode = 99,
};

struct Request {
    int Mode;
    bool boolean;
    int value;
    int ScreenWidth;
    int ScreenHeight;
    uint64_t playerPtr;
};

#define maxplayerCount 60

struct PlayerData {
    Vector3 headPosition;
    Vector3 bottomPlayerPosition;
    float health;
    char name[128];
    bool isDieing;
    float distance;
};

struct Response {
    bool Success;
    int PlayerCount;
    PlayerData Players[maxplayerCount];
};

uintptr_t getLibBase(const char* libName) {
    uintptr_t base = 0;
    FILE* fp = fopen("/proc/self/maps", "rt");
    if (!fp) return 0;
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, libName)) {
            base = strtoul(line, NULL, 16);
            break;
        }
    }

    fclose(fp);
    return base;
}

void* BestEnemyFind(void* skipTarget);
void* BestEnemyFind360();
void* GetEnemyInsideScreen();
static bool InActiveMatch();

std::string getPackageName() {
    char pkg[256] = {0};
    FILE *fp = fopen("/proc/self/cmdline", "r");
    if (fp) {
        fgets(pkg, sizeof(pkg), fp);
        fclose(fp);
    }

    return std::string(pkg);
}

SocketServer server;

int InitServer() {
    if (!server.Create()) return -1;
    if (!server.Bind()) return -1;
    if (!server.Listen()) return -1;
    return 0;
}

#define _GameFacade (uintptr_t) Il2CppGetClassType(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"))
#define offset_IsCSLikePreparePhase (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("IsCSLikePreparePhase"), 0)
#define _MatchGame (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentMatchGame"))
#define _Match (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("MatchGame"), OBFUSCATE("m_Match"))
#define _MatchState (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("JMAGGLCNGIG"), OBFUSCATE("LOEPAKMFNJO"))
#define _DicPlayer (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("JMAGGLCNGIG"), OBFUSCATE("NOLEDCGOJKC"))
#define _HeadTF (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GBKFHDFCPMD"))
#define _RootTF (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IIPBIDIBJDK"))
#define _OriginalName (uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("BDHGDLFDHAL"))
#define _HitObjectInfoWp (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("FDMIEDDNCEC"))
#define offset_NoReload (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerAttributes"), OBFUSCATE("ShootNoReload"))
#define _GH_GhostHackFlag   0x548

#define Hit_GameObject (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("CGKJLKPMGDJ"), OBFUSCATE("JHAINOBIFMA"))
#define Hit_HeadCollider (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("CGKJLKPMGDJ"), OBFUSCATE("JINDNOFABKB"))
#define Hit_Ignore (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("CGKJLKPMGDJ"), OBFUSCATE("EFMBNPNFGHM"))
#define Hit_HitLoc (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("CGKJLKPMGDJ"), OBFUSCATE("HPCOJHGFEEG"))
#define Hit_Normal (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("CGKJLKPMGDJ"), OBFUSCATE("DOJDNHCGFGK"))
#define Hit_RayDir (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("CGKJLKPMGDJ"), OBFUSCATE("HLDECMJFKJK"))
#define Hit_StartPos (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("CGKJLKPMGDJ"), OBFUSCATE("EKPLMDDKOGB"))
#define Hit_OrgStrtPos (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("CGKJLKPMGDJ"), OBFUSCATE("ONMKMEGMHOL"))
#define Hit_Part (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("CGKJLKPMGDJ"), OBFUSCATE("DKPDBLLJEAA"))
#define offset_LCLHHHKFCFP (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("NPLMKLBKJPE"),4)
#define offset_SetStartDamage (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("LHNJLFOINDO"),1)
#define offset_WeaponFire (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("LHNJLFOINDO"),1)
#define offset_DamageS2C (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PhyXShield"), OBFUSCATE("GLEMOCGDGHM"),0)
#define offset_CheckParametros_Validate (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("NCHOCBKLNEJ"), 2)
#define offset_GlobalStruct_Instance (uintptr_t) Il2CppGetClassType(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameVarDef"))
#define offset_TypeInfo_MCDLKHAOJJG (uintptr_t) Il2CppGetClassType(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("KMLICIEHMHC"))
#define offset_CurrentGameModeSetting (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentGameModeSetting"), 0)
#define offset_HMAC_ComputeHash (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("mscorlib.dll"), OBFUSCATE("System.Security.Cryptography"), OBFUSCATE("HashAlgorithm"), OBFUSCATE("ComputeHash"), 1)
#define offset_BitConverter_ToInt32 (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("mscorlib.dll"), OBFUSCATE("System"), OBFUSCATE("BitConverter"), OBFUSCATE("ToInt32"), 2)
#define offset_JMCENJNEHKE (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("EAJDPCJDMFG"), 1)
static inline bool IsValidPtr(void* p) {
    if (!p) return false;
    uintptr_t addr = (uintptr_t)p;
    if (addr < 0x10000) return false;
    uintptr_t page = addr & ~(0xFFFULL);
    unsigned char vec = 0;
    return mincore((void*)page, 0x1000, &vec) == 0;
}

std::vector<void*> GetEntities(void* currentGame) {
    std::vector<void *> entityList;
    if (!currentGame) return entityList;
    uintptr_t entityDictionary = *(uintptr_t *) ((uintptr_t) currentGame + _DicPlayer);
    if (!entityDictionary || !IsValidPtr((void*)entityDictionary)) return entityList;
    bool newLayout = false;
    uintptr_t entries = *(uintptr_t *) (entityDictionary + 0xC);
    int count = *(int *) (entityDictionary + 0x10);
    if (entries && IsValidPtr((void*)entries) && count > 0 && count < 10000)

        newLayout = true;
    if (newLayout) {
        for (int i = 0; i < count; i++) {
            void *entity = *(void **) (entries + (i * 0x28) + 0x30);
            if (!entity) continue;
            entityList.push_back(entity);
        }
    } 
    return entityList;
}

#include "Hack/DownKill.h"

#include <algorithm>

static bool IsPlayerInMatch(void* player, void* current_match) {
    if (!player || !current_match) return false;
    auto players = GetEntities(current_match);
    for (void* p : players) {
        if (p == player) return true;
    }
    return false;
}

static std::mutex g_espMutex;
static Response g_espSnapshot{false, 0};
static bool g_espSnapshotHasData = false;

static void PublishEspSnapshot(const Response &resp) {
    std::lock_guard<std::mutex> lk(g_espMutex);
    g_espSnapshot = resp;
    g_espSnapshotHasData = true;
}

void NewEspForUnity31(Response &response);

static void RefreshEspSnapshot() {
    Response tmp{false, 0};
    NewEspForUnity31(tmp);
    PublishEspSnapshot(tmp);
}

void NewEspForUnity31(Response &response) {
    void *MatchGame = *(void **) ((uint64_t) _GameFacade + _StaticClass);
    if (!MatchGame) return;
    void *ClassMatchGame = *(void **) ((uint64_t) MatchGame + _MatchGame);
    if (!ClassMatchGame) return;
    void *current_match = *(void **) ((uint64_t) ClassMatchGame + _Match);
    if (!current_match) return;
    auto matchStatus = *(uint32_t *) ((uint64_t) current_match + _MatchState);
    if (matchStatus != 1) return;

    if (!MasterBool.enableESP) return;
    void *CurrentLocalPlayer = Current_Local_Player();
    if (!CurrentLocalPlayer) return;

    auto players = GetEntities(current_match);
    for (auto player: players) {
        if (!player) continue;
        if (!IsValidPtr(player)) continue;
        if (player == CurrentLocalPlayer) continue;
        if (IsLocalTeammate(player)) continue;
        int hp = GetHp(player);
        if (hp <= 0) continue;
        void *HeadTF = TransformNode(*(void **) ((uintptr_t) player + _HeadTF));
        void *RootTF = TransformNode(*(void **) ((uintptr_t) player + _RootTF));
        void *LocalHeadTF = TransformNode(*(void **) ((uintptr_t) CurrentLocalPlayer + _HeadTF));
        if (!RootTF) continue;
        if (!HeadTF) {
            HeadTF = Component_get_transform(player);
            if (!HeadTF) continue;
        }
        if (!LocalHeadTF) {
            LocalHeadTF = Component_get_transform(CurrentLocalPlayer);
            if (!LocalHeadTF) continue;
        }

        Vector3 headPos = Transform_INTERNAL_GetPosition(HeadTF);
        Vector3 rootPos = Transform_INTERNAL_GetPosition(RootTF);
        Vector3 localHeadPos = Transform_INTERNAL_GetPosition(LocalHeadTF);
        if (!TransformNode(*(void **) ((uintptr_t) player + _HeadTF))) {
            headPos.X = rootPos.X;
            headPos.Y = rootPos.Y + 1.8f;
            headPos.Z = rootPos.Z;
        }

        Vector3 screenHead = WorldToScreenPoint(headPos);
        Vector3 screenRoot = WorldToScreenPoint(rootPos);
        bool isDieing = IsDieing(player);
        float distance = Vector3::Distance(localHeadPos, headPos);
        monoString *Nick = *(monoString **) ((uint64_t) player + _OriginalName);
        if (response.PlayerCount >= maxplayerCount) break;
        PlayerData *data = &response.Players[response.PlayerCount];
        if (data) {
            data->headPosition = screenHead;
            data->bottomPlayerPosition = screenRoot;
            data->distance = distance;
            data->isDieing = isDieing;
            data->health = (float)hp;
            if (Nick != nullptr && IsValidPtr((void*)Nick) && Nick->toChars() != nullptr && Nick->toChars()[0] != '\0') {
                if (MasterBool.targetLock && player == lockedEnemy) {
                    char lockName[128];
                    snprintf(lockName, sizeof(lockName), "%s [LOCK]", Nick->toChars());
                    strncpy(data->name, lockName, sizeof(data->name) - 1);
                    data->name[sizeof(data->name) - 1] = '\0';
                } else {
                    strncpy(data->name, Nick->toChars(), sizeof(data->name) - 1);
                    data->name[sizeof(data->name) - 1] = '\0';
                }
            }
        }

        response.PlayerCount++;
    }
}




// ==================== TeleSpot Combo (Ported from D:\TP+360) ====================
static std::atomic<bool> g_teleComboActive{false};
static std::atomic<bool> teleSpotArmed{false};
static std::atomic<bool> teleSpot{false};
static std::atomic<bool> flyhack{false};
static std::atomic<bool> flyStuck{false};
static std::atomic<bool> speedTimerV2{false};
static std::atomic<int> speedTimerV2Value{0};

#define m_currentUi (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentInGameUIScene"), 0)
static void* CurrentInGameUIScene() {
    using fnCurrentUIScene = void* (*)();
    auto _CurrentUIScene = reinterpret_cast<fnCurrentUIScene>(m_currentUi);
    return _CurrentUIScene ? _CurrentUIScene() : nullptr;
}

// Reads the position of the map mark placed by the local player
static Vector3 GetMapMarkedLoc() {
    void* uiScene = CurrentInGameUIScene();
    if (!uiScene) return {0, 0, 0};

    static uintptr_t off_BigMapCtrl = 0;
    if (!off_BigMapCtrl)
        off_BigMapCtrl = (uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIInGameScene"), OBFUSCATE("m_BigMapCtrl"));
    if (!off_BigMapCtrl) return {0, 0, 0};
    void* bigMapCtrl = *(void**)((uint64_t)uiScene + off_BigMapCtrl);
    if (!bigMapCtrl) return {0, 0, 0};

    static uintptr_t off_MapContent = 0;
    if (!off_MapContent)
        off_MapContent = (uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIMapBaseController"), OBFUSCATE("m_MapContentCtrl"));
    if (!off_MapContent) return {0, 0, 0};
    void* mapContent = *(void**)((uint64_t)bigMapCtrl + off_MapContent);
    if (!mapContent) return {0, 0, 0};

    static uintptr_t off_LocalMark = 0;
    if (!off_LocalMark)
        off_LocalMark = (uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIMapContentController"), OBFUSCATE("m_LocalMapMarkController"));
    if (!off_LocalMark) return {0, 0, 0};
    void* localMarkCtrl = *(void**)((uint64_t)mapContent + off_LocalMark);
    if (!localMarkCtrl) return {0, 0, 0};

    static uintptr_t off_MarkPos = 0;
    if (!off_MarkPos)
        off_MarkPos = (uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIHudPlayerMarkController"), OBFUSCATE("m_pos"));
    if (!off_MarkPos) return {0, 0, 0};

    return *(Vector3*)((uint64_t)localMarkCtrl + off_MarkPos);
}

// Reads the CS-mode match phase from game memory.
// Phase 1 = Preparation (red zone / waiting)
// Phase 2 = Fight (round started)
// Returns 0 if not in CS mode or offsets not found.
static uint32_t ReadCSMatchPhase() {
    void* uiScene = CurrentInGameUIScene();
    if (!uiScene) return 0;

    static uintptr_t off_InfoCtrl = 0;
    if (!off_InfoCtrl)
        off_InfoCtrl = (uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIInGameScene"), OBFUSCATE("m_CSMatchInfoInfoCtrl"));
    if (!off_InfoCtrl) return 0;
    void* infoCtrl = *(void**)((uint64_t)uiScene + off_InfoCtrl);
    if (!infoCtrl) return 0;

    static uintptr_t off_MatchGame = 0;
    if (!off_MatchGame)
        off_MatchGame = (uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIHudCSMatchInfoController"), OBFUSCATE("mGame"));
    if (!off_MatchGame) return 0;
    void* matchGame = *(void**)((uint64_t)infoCtrl + off_MatchGame);
    if (!matchGame) return 0;

    static uintptr_t off_Phase = 0;
    if (!off_Phase)
        off_Phase = (uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("FOMLLGKJKAC"), OBFUSCATE("OLDBFCKDDMA"));
    if (!off_Phase) return 0;

    return *(uint32_t*)((uint64_t)matchGame + off_Phase);
}

static void StopTeleCombo() {
    flyhack.store(false);
    flyStuck.store(false);
    speedTimerV2.store(false);
    teleSpot.store(false);
    teleSpotArmed.store(false);
    g_teleComboActive.store(false);
    void* timeService = GetTimeService();
    if (timeService) {
        static uintptr_t offFixedDeltaTime = (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("GCommon"), OBFUSCATE("TimeService"), OBFUSCATE("m_FixedDeltaTime"));
        if (!offFixedDeltaTime) offFixedDeltaTime = 0x24;
        *(float*)((uintptr_t)timeService + offFixedDeltaTime) = 0.033f;
    }
}

static void StartTeleCombo() {
    bool expected = false;
    if (g_teleComboActive.compare_exchange_strong(expected, true)) {
        flyhack.store(false);
        flyStuck.store(false);
        speedTimerV2.store(false);
        teleSpot.store(false);
        teleSpotArmed.store(false);
        std::thread([]{
            // Step 1: FlyHack ON for 2 seconds (player rises)
            flyhack.store(true);
            usleep(2000000); // 2 seconds

            if (!g_teleComboActive.load()) return;

            // Step 2: FlyHack OFF, FlyStuck ON, SpeedTimerV2 ON (value=0)
            flyhack.store(false);
            speedTimerV2Value.store(0);     // explicitly freeze game time
            speedTimerV2.store(true);
            flyStuck.store(true);

            // Step 3: Arm teleSpot, wait for round start
            teleSpotArmed.store(true);

            // Poll until hook fires teleport (teleSpotArmed cleared) or timeout ~30s
            int waited = 0;
            while (teleSpotArmed.load() && g_teleComboActive.load() && waited < 600) { // 600 * 50ms = 30s
                usleep(50000); // 50ms
                waited++;
            }

            if (!g_teleComboActive.load()) return;

            // Step 4: Wait a brief moment after teleport fires
            usleep(200000); // 200ms for teleport to settle

            // Step 5: All OFF
            flyStuck.store(false);
            speedTimerV2.store(false);
            teleSpot.store(false);
            teleSpotArmed.store(false);
            g_teleComboActive.store(false);
            void* timeService = GetTimeService();
            if (timeService) {
                static uintptr_t offFixedDeltaTime = (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("GCommon"), OBFUSCATE("TimeService"), OBFUSCATE("m_FixedDeltaTime"));
                if (!offFixedDeltaTime) offFixedDeltaTime = 0x24;
                *(float*)((uintptr_t)timeService + offFixedDeltaTime) = 0.033f;
            }
        }).detach();
    }
}

void* cachedTarget = nullptr;
void* cachedTarget360 = nullptr;

void *CreateServer(void *) {
    if (InitServer() == 0) {
        while(true) {
            if (server.Accept()) {
                Request request{};
                size_t requestSize = 0;
                while ((requestSize = server.receive((void*)&request, sizeof(request))) == sizeof(request)) {
                    Response response{};
                    if (request.Mode == Mode::InitMode) {
                        response.Success = true;
                    } else if (request.Mode == Mode::EspMode) {
                        g_screenWidth = request.ScreenWidth;
                        g_screenHeight = request.ScreenHeight;
                        response.Success = true;
                        {
                            std::lock_guard<std::mutex> lk(g_espMutex);
                            if (g_espSnapshotHasData) {
                                response = g_espSnapshot;
                            }
                        }
                        response.Success = true;
                    } else if (request.Mode == 3) {
                        MasterBool.enableESP = request.boolean;
                        response.Success = true;
                    } else if (request.Mode == 104) {
                        MasterBool.aimbotFOV = (float)request.value;
                        response.Success = true;
                    } else if (request.Mode == 508) {
                        MasterBool.autoswitchsafe = request.boolean;
                        response.Success = true;

                    } else if (request.Mode == 523) {
                        MasterBool.targetLock = request.boolean;
                        if (request.boolean) {
                            if (lockedEnemy == nullptr && cachedTarget != nullptr) {
                                lockedEnemy = cachedTarget;
                                monoString *Nick = *(monoString **) ((uint64_t) lockedEnemy + _OriginalName);
                                if (Nick && Nick->toChars() != NULL) {
                                    strncpy(lockedEnemyName, Nick->toChars(), 127);
                                }
                                wasLockedEnemyDown = false;
                            }
                        }
                        response.Success = true;
                    } else if (request.Mode == 1044) {
                        MasterBool.Aimkillsend = request.boolean;
                        response.Success = true;
                    } else if (request.Mode == 1305) {
                        MasterBool.backFakeFix = request.boolean;
                        response.Success = true;
                    } else if (request.Mode == 59926) {
                        MasterBool.mode360v2 = request.boolean;
                        response.Success = true;
                    } else if (request.Mode == 777781) {
                        MasterBool.SilentGhostPullV2_Enable = request.boolean;
                        response.Success = true;
                    }
                    else if (request.Mode == 1045) {
                        MasterBool.downkill = request.boolean;
                        DownKill = request.boolean;
                        if (DownKill || DownKillV2) {
                            GhostOn();
                        } else {
                            GhostOff();
                        }
                        response.Success = true;
                    } else if (request.Mode == 1047) {
                        MasterBool.downkillv2 = request.boolean;
                        DownKillV2 = request.boolean;
                        if (DownKill || DownKillV2) {
                            GhostOn();
                        } else {
                            GhostOff();
                        }
                        response.Success = true;
                    } else if (request.Mode == 5020) {
                        MasterBool.loginIndia = request.boolean;
                        response.Success = true;


                    } else if (request.Mode == 991925) {
                        MasterBool.AimkillTake = request.boolean;
                    } else if (request.Mode == 59910) {
                        if (request.boolean) {
                            StartTeleCombo();
                        } else {
                            StopTeleCombo();
                        }
                        response.Success = true;
                    }

                    else if (request.Mode == 6010) {
                        MasterBool.resetGuest.store(request.boolean, std::memory_order_relaxed);
                        response.Success = true;
                    }
                    server.send((void*)& response, sizeof(response));
                }
                // Connection dropped: rebind a fresh listener so the overlay
                // can reconnect instead of the socket thread dying forever.
                InitServer();
            }
            usleep(200 * 1000);
        }
    }

    return NULL;
}



float Vector3Distance(const Vector3& a, const Vector3& b) {
    float dx = a.X - b.X; float dy = a.Y - b.Y; float dz = a.Z - b.Z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

void* GetEnemyInsideScreen()

{
    void* closestEnemy = nullptr;
    float closestDistSq = FLT_MAX;
    if (!_GameFacade) return nullptr;
    void* MatchGame = *(void**)((uint64_t)_GameFacade + _StaticClass);
    if (!MatchGame) return nullptr;
    void* ClassMatchGame = *(void**)((uint64_t)MatchGame + _MatchGame);
    if (!ClassMatchGame) return nullptr;
    void* current_match = *(void**)((uint64_t)ClassMatchGame + _Match);
    if (!current_match) return nullptr;
    if (*(uint32_t*)((uint64_t)current_match + _MatchState) != 1)

        return nullptr;
    void* LocalPlayer = Current_Local_Player();
    if (!LocalPlayer) return nullptr;
    Vector3 screenCenter(

            (float)(g_screenWidth * 0.5f),

            (float)(g_screenHeight * 0.5f),

            0.0f

    );
    auto players = GetEntities(current_match);
    for (auto player: players) {
        void* enemy = player;
        if (!enemy || enemy == LocalPlayer || !IsValidPtr(enemy)) continue;
        if (IsDieing(enemy)) continue;
        if (IsLocalTeammate(enemy)) continue;
        if (GetHp(enemy) <= 0) continue;
        void* headTF = TransformNode(*(void**)((uint64_t)enemy + _HeadTF));
        if (!headTF) continue;
        Vector3 worldPos = Transform_INTERNAL_GetPosition(headTF);
        Vector3 screenPos = WorldToScreenPoint(worldPos);
        if (screenPos.Z <= 0.01f) continue;
        if (screenPos.X < 0 || screenPos.X > g_screenWidth) continue;
        if (screenPos.Y < 0 || screenPos.Y > g_screenHeight) continue;
        float dx = screenPos.X - screenCenter.X;
        float dy = screenPos.Y - screenCenter.Y;
        float distSq = dx * dx + dy * dy;
        if (distSq < closestDistSq)

        {
            closestDistSq = distSq;
            closestEnemy = enemy;
        }
    }

    return closestEnemy;
}

void *GetKnockedEnemyNearby()

{
    float closestDist = 999999.0f;
    void* targetTeammate = nullptr;
    if (!_GameFacade) return nullptr;
    void* MatchGame = *(void**)((uintptr_t)_GameFacade + _StaticClass);
    if (!MatchGame) return nullptr;
    void* ClassMatchGame = *(void**)((uintptr_t)MatchGame + _MatchGame);
    if (!ClassMatchGame) return nullptr;
    void* current_match = *(void**)((uintptr_t)ClassMatchGame + _Match);
    if (!current_match) return nullptr;
    void* LocalPlayer = Current_Local_Player();
    if (!LocalPlayer) return nullptr;
    Vector3 localPos = Transform_INTERNAL_GetPosition(TransformNode(*(void**)((uintptr_t)LocalPlayer + _HeadTF)));
    auto players = GetEntities(current_match);
    for (auto playerSS: players) {
        void* player = playerSS;
        if (!player || player == LocalPlayer) continue;
        if (IsLocalTeammate(player)) continue;
        if (!IsDieing(player)) continue;
        if (GetHp(player) <= 0) continue;
        void* TargetHead = TransformNode(*(void**)((uintptr_t)player + _HeadTF));
        if (!TargetHead) continue;
        Vector3 targetPos = Transform_INTERNAL_GetPosition(TargetHead);
        float dx = targetPos.X - localPos.X;
        float dy = targetPos.Y - localPos.Y;
        float dz = targetPos.Z - localPos.Z;
        float actualDistance = sqrtf(dx*dx + dy*dy + dz*dz);
        if (actualDistance < 5.0f && actualDistance < closestDist)

        {
            closestDist = actualDistance;
            targetTeammate = player;
        }
    }

    return targetTeammate;
}



struct DamageInfo2_o {
    void *klass;
    void *monitor;
    int32_t BaseDamage;
    int32_t HitColliderType;
    monoString* HitColliderName;
    bool isBackArea;
    COW_GamePlay_IHAAMHPPLMG_o Damager;
    void* Weapon;
    int32_t WeaponDataID;
    Vector3 FirePos;
    Vector3 HitPos;
    Vector3 HitNormal;
    uint8_t SpecialHitType;
    bool ForceNoHeadShot;
    int32_t ExtraInfo;
    monoDictionary<uint8_t*, void **> *SpecialHitDic;
};

struct WeaponDynamicInfo2_o {
    void *klass;
    void *monitor;
    float BiteArmor;
    int32_t ContinuousFireDamage;
    int32_t DynamicChangeDamage;
    float DynamicDamageRatioBody;
    float DynamicDamageRatioHead;
    float DynamicDamageRatioLimb;
    float BoostDamage;
    float ShieldDamageRatioWeapon;
    float ShieldDamageRatioBody;
    float ShieldDamageRatioHead;
    float ShieldDamageRatioLimb;
};

monoList<float *> *LCLHHHKFCFP(void *Weapon, void *CAGCICACKCF, void *HFBDJJDICLN, bool LDGHPOPPPNL, DamageInfo2_o *DamageInfo)

{
    return ((monoList<float *> * (*)(void*, void*, void*, bool, DamageInfo2_o*))offset_LCLHHHKFCFP)(Weapon, CAGCICACKCF, HFBDJJDICLN, LDGHPOPPPNL, DamageInfo);
}
















static bool AimkillMethodPull(void* enemy, void** outTf, Vector3* outOriginal) {
    if (!enemy) return false;
    if (!IsPullTargetValid(enemy)) return false;
    void* tf = Component_get_transform(enemy);
    if (!tf) return false;
    if (outTf) *outTf = tf;
    if (outOriginal) *outOriginal = Transform_INTERNAL_GetPosition(tf);
    if (isVisible_Aimbot(enemy)) return false;
    return SilentGhostPullSafeV2(enemy, nullptr, true);
}

static void AimkillMethodRestore(void* enemy, void* tf, const Vector3& original) {
    if (enemy && tf && IsPullTargetValid(enemy)) {
        set_position_Injected(tf, original);
    }
}

static bool g_inAimkillSend = false;
static bool g_inAimkillFiring = false;
static bool g_inSafeAimkill = false;
static bool g_inAimkillSendOB51 = false;
static bool (*original_WeaponFire)(void* weapon, COW_GamePlay_MADMMIICBNN_o *hitInfo) = nullptr;
extern bool (*NoBUlletTractOriginal)(void* weapon, COW_GamePlay_MADMMIICBNN_o *hitInfo);
static bool ResolveWeaponFireFn();
static void FillHitInfoDirectly(COW_GamePlay_MADMMIICBNN_o *hitInfo, void *headCollider,
                                Vector3 enemyHeadPos, Vector3 localHeadPos, Vector3 direction,
                                float dist, int baseDamage);
static void* g_lastMatchPtr = nullptr;
static bool g_hasStartedDamage = false;
static DamageInfo2_o* g_dmgInfo = nullptr;
static WeaponDynamicInfo2_o* g_weaponDynamicInfo = nullptr;
static void (*HitFly_DirectMethod)(void*, void*) = nullptr;

void ResetAimkillStatics() {
    g_inAimkillSend = false;
    g_inAimkillFiring = false;
    g_inSafeAimkill = false;
    g_hasStartedDamage = false;
    original_WeaponFire = nullptr;
    g_dmgInfo = nullptr;
    g_weaponDynamicInfo = nullptr;
    HitFly_DirectMethod = nullptr;
    g_inAimkillSendOB51 = false;
}

struct message_C2S_RUDP_TakeDamage_Req_o2 {
    void *klass;
    void *monitor;
    bool UDPClientMessageBase_m_GetFromPool;
    uint32_t ALFINFGBOBE;
    uint16_t ECDBFHHNPMI;
    uint16_t EKCONDDBKFO;
    uint32_t BJBPPEBIPFA;
    uint32_t LIIGLCNGOHG;
    int32_t PIAMIOFEBKF;
    uint32_t HCMIEJEBKAL;
    uint8_t ODCJPCEJHPK;
    uint32_t CEDJCPLOLNE;
    message_DEACEIFBHJK_o* CNEICNJFGLM;
    message_DEACEIFBHJK_o* PGDEDHFOMCN;
    monoList<float*> * AALHLOAJLEE;
    uint32_t HOBOHHJNDNH;
    float AILHIPMKJKJ;
    uint64_t LHGGPCFJNOO;
    int8_t ACAKHEABPEJ;
    bool MJIHLDJNHLF;
    bool MBGCAHPACOH;
    System_Collections_Generic_List_byte__o * FIKOAMIDEHL;
    float IOGIIEFAALP;
    bool HDEJLJKNLCI;
};

static bool InActiveMatch() {
    if (!_GameFacade) return false;
    void *matchGame = *(void **)((uintptr_t)_GameFacade + _StaticClass);
    if (!matchGame) return false;
    void *classMatchGame = *(void **)((uintptr_t)matchGame + _MatchGame);
    if (!classMatchGame) return false;
    void *current_match = *(void **)((uintptr_t)classMatchGame + _Match);
    if (!current_match) return false;
    return *(uint32_t *)((uintptr_t)current_match + _MatchState) == 1;
}

static bool ResolveWeaponFireFn() {
    if (NoBUlletTractOriginal) {
        original_WeaponFire = NoBUlletTractOriginal;
        return true;
    }
    if (original_WeaponFire) return true;
    if (offset_WeaponFire) {
        original_WeaponFire = (bool (*)(void*, COW_GamePlay_MADMMIICBNN_o*))offset_WeaponFire;
        return true;
    }
    return false;
}

static void FillHitInfoDirectly(COW_GamePlay_MADMMIICBNN_o *hitInfo, void *headCollider,

                                Vector3 enemyHeadPos, Vector3 localHeadPos, Vector3 direction,

                                float dist, int baseDamage) {
    if (!hitInfo || !headCollider) return;
    void* go = get_gameObject(headCollider);
    if (!go) return;
    *(void**)((uintptr_t)hitInfo + Hit_GameObject) = go;
    *(void**)((uintptr_t)hitInfo + Hit_HeadCollider) = headCollider;
    *(Vector3*)((uintptr_t)hitInfo + Hit_HitLoc) = enemyHeadPos;
    *(Vector3*)((uintptr_t)hitInfo + Hit_Normal) = direction;
    *(Vector3*)((uintptr_t)hitInfo + Hit_RayDir) = direction;
    *(Vector3*)((uintptr_t)hitInfo + Hit_StartPos) = localHeadPos;
    *(Vector3*)((uintptr_t)hitInfo + Hit_OrgStrtPos) = localHeadPos;
    *(int*)((uintptr_t)hitInfo + Hit_Part) = 1;
    *(bool*)((uintptr_t)hitInfo + Hit_Ignore) = false;
    hitInfo->FFDIOGPKCKF = dist;
    hitInfo->IHNCAADOAAE = baseDamage;
}


static bool IsValidGun(void* weapon) {
    if (!weapon) return false;
    int weaponId = GetWeaponID(weapon);
    if (weaponId <= 1 ||
        weaponId == 16 || weaponId == 17 || weaponId == 27 || weaponId == 34 || weaponId == 51 || weaponId == 53 ||
        (weaponId >= 600 && weaponId < 700) ||
        (weaponId >= 1000 && weaponId < 1100) ||
        (weaponId >= 1200 && weaponId < 1300) ||
        (weaponId >= 1400 && weaponId < 1500) ||
        (weaponId >= 6000 && weaponId < 6100) ||
        (weaponId >= 9400 && weaponId < 9500)) {
        return false;
    }
    return true;
}

static bool IsEnemyModelHidden(void* player) {
    if (!player) return false;
    static uintptr_t offAM = 0, offAv = 0, offVis = 0;
    static bool initDone = false;
    if (!initDone) {
        initDone = true;
        offAM = (uintptr_t) Il2CppGetFieldOffset(
            OBFUSCATE("Assembly-CSharp.dll"),
            OBFUSCATE("COW.GamePlay"),
            OBFUSCATE("Player"),
            OBFUSCATE("GGJOLDEEIEN"));
        offAv = (uintptr_t) Il2CppGetFieldOffset(
            OBFUSCATE("Assembly-CSharp.dll"),
            OBFUSCATE("COW.GamePlay"),
            OBFUSCATE("AvatarManager"),
            OBFUSCATE("LHEDBMJJOLM"));
        offVis = (uintptr_t) Il2CppGetFieldOffset(
            OBFUSCATE("Assembly-CSharp.dll"),
            OBFUSCATE("UMA.Simple"),
            OBFUSCATE("UmaAvatarSimple"),
            OBFUSCATE("IsVisible"));
    }
    if (!offAM || !offAv || !offVis) return false;
    void* avatarManager = *(void**)((uintptr_t)player + offAM);
    if (!avatarManager) return false;
    void* avatar = *(void**)((uintptr_t)avatarManager + offAv);
    if (!avatar) return false;
    bool isVisible = *(bool*)((uintptr_t)avatar + offVis);
    return !isVisible;
}

#include "AutoUpdate/Aimbot/Aimbot.h"

#include <math.h>

Quaternion Quaternion_Euler(float x, float y, float z)

{
    float radX = x * (M_PI / 180.0f);
    float radY = y * (M_PI / 180.0f);
    float radZ = z * (M_PI / 180.0f);
    float cx = cos(radX * 0.5f);
    float sx = sin(radX * 0.5f);
    float cy = cos(radY * 0.5f);
    float sy = sin(radY * 0.5f);
    float cz = cos(radZ * 0.5f);
    float sz = sin(radZ * 0.5f);
    Quaternion q;
    q.W = cx * cy * cz + sx * sy * sz;
    q.X = sx * cy * cz - cx * sy * sz;
    q.Y = cx * sy * cz + sx * cy * sz;
    q.Z = cx * cy * sz - sx * sy * cz;
    return q;
}

bool GameFacade_IsCSLikePreparePhase()
{
    if (!offset_IsCSLikePreparePhase) return false;
    auto _IsCSLikePreparePhase = (bool(*)())offset_IsCSLikePreparePhase;
    return _IsCSLikePreparePhase();
}

std::chrono::steady_clock::time_point last_update_time = std::chrono::steady_clock::now();
static std::chrono::steady_clock::time_point last_update_time_exploit = std::chrono::steady_clock::now();
GCommon_AnimationRuntimeHandle_o *(*GetCurrentRunningHandler)(GCommon_AnimationSystemComponent_o *Instance,int32_t layerIndex);

static void PullEnemyWhenCrosshairNear() {
    ApplyDownKill();
    if (!MasterBool.enableESP) return;
    if (!MasterBool.SilentGhostPullV2_Enable && !MasterBool.Aimkillsend && !MasterBool.AimkillTake) return;
    static std::chrono::steady_clock::time_point lastPullTime = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPullTime).count();
    if (elapsedMs < 1) return;
    void* StaticGameFacade = *(void**)((uintptr_t)_GameFacade + _StaticClass);
    if (!StaticGameFacade) return;
    void* ClassMatchGame = *(void**)((uintptr_t)StaticGameFacade + _MatchGame);
    if (!ClassMatchGame) return;
    void* current_match = *(void**)((uintptr_t)ClassMatchGame + _Match);
    if (!current_match) return;
    if (*(uint32_t*)((uintptr_t)current_match + _MatchState) != 1) return;
    void* LocalPlayer = Current_Local_Player();
    if (!LocalPlayer) return;
    auto players = GetEntities(current_match);
    bool pulledAny = false;
    for (void* player : players) {
        if (!player || player == LocalPlayer) continue;
        if (IsDieing(player)) continue;
        if (IsLocalTeammate(player)) continue;
        if (GetHp(player) <= 0) continue;
        if (!isVisible_Aimbot(player)) {
            SafeSilentPullEx(player, nullptr, 10.0f, 0.5f, 0.7f, 0.5f);
            pulledAny = true;
        }
    }
    if (pulledAny) {
        lastPullTime = std::chrono::steady_clock::now();
    }
}

GCommon_AnimationRuntimeHandle_o *_GetCurrentRunningHandler(GCommon_AnimationSystemComponent_o *Instance,int32_t layerIndex)

{
    if (Instance != nullptr && layerIndex == 0) {
        std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_update_time).count();
        auto elapsed_time_exploit = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_update_time_exploit).count();
        if (elapsed_time_exploit > 17)
        {
            
            last_update_time_exploit = current_time;
        }

        static bool g_wasInPreparePhase = false;
        bool isPreparePhase = GameFacade_IsCSLikePreparePhase();
        g_wasInPreparePhase = isPreparePhase;


        if (elapsed_time >= 15) { // 60 FPS update (~16ms)
            last_update_time = current_time;

            // ==================== TeleSpot Combo (Ported from D:\TP+360) ====================
            void* comboLocal = Current_Local_Player();
            if (comboLocal) {
                // 1. FlyHack (player rises)
                static float opGroundY = 0.0f;
                static float opFlyY = 0.0f;
                static bool op_initialized = false;
                const float opRiseSpeed = 0.15f;
                const float opHeightLimit = 15.0f;

                if (flyhack.load()) {
                    void* localTF = Component_get_transform(comboLocal);
                    if (localTF) {
                        Vector3 pos = Transform_INTERNAL_GetPosition(localTF);
                        if (!op_initialized) {
                            opGroundY = pos.Y;
                            opFlyY = pos.Y;
                            op_initialized = true;
                        }
                        float maxY = opGroundY + opHeightLimit;
                        if (opFlyY < maxY) {
                            opFlyY += opRiseSpeed;
                            if (opFlyY > maxY) opFlyY = maxY;
                        }
                        pos.Y = opFlyY;
                        set_position_Injected(localTF, pos);
                    }
                } else {
                    op_initialized = false;
                }

                // 2. TeleSpot Armed (waits for round start)
                static bool wasInPreparePhase = false;
                bool inPrepare = GameFacade_IsCSLikePreparePhase();
                if (teleSpotArmed.load()) {
                    uint32_t phase = ReadCSMatchPhase();
                    // Phase 2 = Fight (round started)
                    bool roundStarted = (phase == 2);
                    if (!roundStarted && wasInPreparePhase && !inPrepare) {
                        roundStarted = true; // CS prepare phase dropped!
                    }
                    if (!roundStarted && phase == 0 && !inPrepare) {
                        roundStarted = true; // non-CS mode or countdown ended
                    }
                    if (roundStarted) {
                        teleSpotArmed.store(false);
                        teleSpot.store(true);
                        LOGI("TeleSpot: round start detected (phase=%u), firing teleport", phase);
                    }
                }
                wasInPreparePhase = inPrepare;

                // 3. TeleSpot execution (sky teleport +11m above target)
                if (teleSpot.load()) {
                    // Priority 1: Map mark location
                    Vector3 target = GetMapMarkedLoc();

                    // Priority 2: Fallback to closest alive enemy anywhere in match
                    if (target.X == 0.0f && target.Y == 0.0f && target.Z == 0.0f) {
                        if (_GameFacade) {
                            void* StaticGameFacade = *(void**)((uintptr_t)_GameFacade + _StaticClass);
                            if (StaticGameFacade) {
                                void* ClassMatchGame = *(void**)((uintptr_t)StaticGameFacade + _MatchGame);
                                if (ClassMatchGame) {
                                    void* current_match = *(void**)((uintptr_t)ClassMatchGame + _Match);
                                    if (current_match) {
                                        void* localTF = Component_get_transform(comboLocal);
                                        Vector3 localPos = localTF ? Transform_INTERNAL_GetPosition(localTF) : Vector3{0,0,0};
                                        float closestDist = 999999.0f;
                                        auto players = GetEntities(current_match);
                                        for (auto p : players) {
                                            if (!p || p == comboLocal || !IsValidPtr(p)) continue;
                                            if (IsDieing(p) || IsLocalTeammate(p) || GetHp(p) <= 0) continue;
                                            void* headTF = TransformNode(*(void**)((uintptr_t)p + _HeadTF));
                                            if (!headTF) headTF = Component_get_transform(p);
                                            if (!headTF) continue;
                                            Vector3 ePos = Transform_INTERNAL_GetPosition(headTF);
                                            float d = Vector3Distance(localPos, ePos);
                                            if (d < closestDist) {
                                                closestDist = d;
                                                target = ePos;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // Teleport +11m above target (sky teleport)
                    if (target.X != 0.0f || target.Y != 0.0f || target.Z != 0.0f) {
                        Vector3 upPos = target;
                        upPos.Y = target.Y + 11.0f;
                        void* localTF = Component_get_transform(comboLocal);
                        if (localTF) {
                            set_position_Injected(localTF, upPos);
                        }
                        LOGI("TeleSpot: teleported to (%.1f, %.1f, %.1f) +11m", target.X, target.Y, target.Z);
                    } else {
                        LOGI("TeleSpot: no mark and no enemy found, skipping");
                    }
                    teleSpot.store(false);
                }
            }

            // 4. FixedDeltaTime freeze for FlyStuck & SpeedTimerV2
            static bool speedTimerV2Patch = false;
            static uintptr_t offFixedDeltaTime = 0;
            static bool offFixedInit = false;
            if (!offFixedInit) {
                offFixedInit = true;
                offFixedDeltaTime = (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("GCommon"), OBFUSCATE("TimeService"), OBFUSCATE("m_FixedDeltaTime"));
                if (!offFixedDeltaTime) offFixedDeltaTime = 0x24;
            }

            void* timeService = GetTimeService();
            if (timeService && offFixedDeltaTime) {
                if (flyStuck.load()) {
                    *(float*)((uintptr_t)timeService + offFixedDeltaTime) = 0.00001f;
                    speedTimerV2Patch = true;
                } else if (speedTimerV2.load()) {
                    float v = (float) speedTimerV2Value.load();
                    float delta;
                    if (v <= 0.0f) {
                        delta = 0.00001f;
                    } else if (v <= 1.0f) {
                        delta = 0.033f;
                    } else {
                        static const float anchorsX[9] = {1.0f, 2.0f, 3.0f, 5.0f, 8.0f, 10.0f, 13.0f, 16.0f, 20.0f};
                        static const float anchorsY[9] = {0.033f, 0.035f, 0.038f, 0.045f, 0.058f, 0.070f, 0.095f, 0.130f, 0.200f};
                        int idx = 0;
                        for (int i = 0; i < 9; i++) {
                            if (v <= anchorsX[i]) { idx = i; break; }
                            idx = i;
                        }
                        int i0 = idx - 1;
                        if (i0 < 0) i0 = 0;
                        float t = (v - anchorsX[i0]) / (anchorsX[idx] - anchorsX[i0]);
                        if (t < 0.0f) t = 0.0f;
                        if (t > 1.0f) t = 1.0f;
                        delta = anchorsY[i0] + (anchorsY[idx] - anchorsY[i0]) * t;
                    }
                    *(float*)((uintptr_t)timeService + offFixedDeltaTime) = delta;
                    speedTimerV2Patch = true;
                } else if (speedTimerV2Patch) {
                    *(float*)((uintptr_t)timeService + offFixedDeltaTime) = 0.033f;
                    speedTimerV2Patch = false;
                }
            }

            // ==================== Match / Aimbot Logic ====================
            if (!InActiveMatch()) {
                lockedEnemy = nullptr;
                GetTarget = nullptr;
                cachedTarget = nullptr;
                cachedTarget360 = nullptr;
                ResetAimkillStatics();
                g_lastMatchPtr = nullptr;
                {
                    std::lock_guard<std::mutex> lk(g_espMutex);
                    g_espSnapshot = Response{false, 0};
                    g_espSnapshotHasData = true;
                }
                return GetCurrentRunningHandler(Instance, layerIndex);
            }

            
            if (MasterBool.mode360v2) {
                cachedTarget360 = BestEnemyFind360();
            } else {
                cachedTarget360 = nullptr;
            }

            void *current_match = nullptr;
            uint32_t matchStatus = 0;
            void* StaticGameFacade = *(void**)((uint64_t)_GameFacade + _StaticClass);
            if (StaticGameFacade) {
                void *ClassMatchGame = *(void **) ((uint64_t) StaticGameFacade + _MatchGame);
                if (ClassMatchGame) {
                    current_match = *(void **) ((uint64_t) ClassMatchGame + _Match);
                    if (current_match) {
                        matchStatus = *(uint32_t *) ((uint64_t) current_match + _MatchState);
                    }
                }
            }

            if (current_match != g_lastMatchPtr || matchStatus != 1) {
                lockedEnemy = nullptr;
                GetTarget = nullptr;
                cachedTarget = nullptr;
                cachedTarget360 = nullptr;
                ResetAimkillStatics();
                g_lastMatchPtr = current_match;
            }

            if (MasterBool.targetLock) {
                if (lockedEnemy != nullptr && IsValidPtr(lockedEnemy) && IsPlayerInMatch(lockedEnemy, current_match)) {
                    bool isDown = (IsDieing(lockedEnemy) || GetHp(lockedEnemy) <= 0);
                    if (isDown) {
                        if (!wasLockedEnemyDown) {
                            wasLockedEnemyDown = true;
                        }

                        lockedEnemy = nullptr;
                    }
                } else if (lockedEnemyName[0] != '\0') {
                    auto players = GetEntities(current_match);
                    for (auto p : players) {
                        if (p && IsValidPtr(p) && !IsLocalTeammate(p)) {
                            monoString *n = *(monoString **) ((uint64_t) p + _OriginalName);
                            if (n && n->toChars() != NULL && strcmp(n->toChars(), lockedEnemyName) == 0) {
                                if (!IsDieing(p) && GetHp(p) > 0) {
                                    lockedEnemy = p;
                                    wasLockedEnemyDown = false;
                                    break;
                                }
                            }
                        }
                    }
                }
            } else {
                lockedEnemyName[0] = '\0';
                lockedEnemy = nullptr;
            }

            void* prevTarget = cachedTarget;
            cachedTarget = nullptr;
            if (MasterBool.targetLock && lockedEnemy != nullptr && IsValidPtr(lockedEnemy) && IsPlayerInMatch(lockedEnemy, current_match)) {
                if (SmartEnemyMovementTracker(lockedEnemy, false)) {
                    cachedTarget = lockedEnemy;
                }
            }

            if (cachedTarget == nullptr) {
                void* bestFovCandidate = BestEnemyFind(lockedEnemy);
                if (bestFovCandidate != nullptr) {
                    if (MasterBool.Aimkillsend || MasterBool.AimkillTake) {
                        cachedTarget = bestFovCandidate;
                    } else if (SmartEnemyMovementTracker(bestFovCandidate, false)) {
                        cachedTarget = bestFovCandidate;
                    } else if (isVisible_Aimbot(bestFovCandidate)) {
                        cachedTarget = bestFovCandidate;
                    }
                }
            }

            // Active UI Loops
            void *LocalPlayer = Current_Local_Player();
            if (LocalPlayer != nullptr) {
                PullEnemyWhenCrosshairNear();
            }

            RefreshEspSnapshot();

            // last_update_time is already updated at frame start to maintain 60 FPS cadence
        }

        return GetCurrentRunningHandler(Instance, layerIndex);
    }

    return GetCurrentRunningHandler(Instance, layerIndex);
}

bool (*NoBUlletTractOriginal)(void* weapon, COW_GamePlay_MADMMIICBNN_o *hitInfo);

bool NoBUlletTract(void* weapon, COW_GamePlay_MADMMIICBNN_o *hitInfo) {
    if (hitInfo == nullptr) {
        return NoBUlletTractOriginal ? NoBUlletTractOriginal(weapon, nullptr) : false;
    }

    if (MasterBool.SilentGhostPullV2_Enable || MasterBool.Aimkillsend || MasterBool.AimkillTake) {
        void *LocalPlayer = Current_Local_Player();
        if (LocalPlayer != nullptr) {
            void *weaponOnHand = GetWeaponOnHand(LocalPlayer);
            if (weaponOnHand != nullptr) {
                void *ClosestEnemys = GetEnemyInsideScreen();
                if (ClosestEnemys == nullptr && _GameFacade) {
                    void* MatchGame = *(void**)((uint64_t)_GameFacade + _StaticClass);
                    if (MatchGame) {
                        void* ClassMatchGame = *(void**)((uint64_t)MatchGame + _MatchGame);
                        if (ClassMatchGame) {
                            void* current_match = *(void**)((uint64_t)ClassMatchGame + _Match);
                            if (current_match) {
                                auto players = GetEntities(current_match);
                                for (auto p : players) {
                                    if (!p || p == LocalPlayer || !IsValidPtr(p) || IsDieing(p) || GetHp(p) <= 0 || IsLocalTeammate(p)) continue;
                                    ClosestEnemys = p;
                                    break;
                                }
                            }
                        }
                    }
                }

                if (ClosestEnemys != nullptr) {
                    void *enemyTF = Component_get_transform(ClosestEnemys);
                    Vector3 originalPos = {0.0f, 0.0f, 0.0f};
                    bool wasPulled = false;
                    if (enemyTF) {
                        originalPos = Transform_INTERNAL_GetPosition(enemyTF);
                    }

                    if (MasterBool.SilentGhostPullV2_Enable && !isVisible_Aimbot(ClosestEnemys)) {
                        wasPulled = SilentGhostPullSafeV2(ClosestEnemys, nullptr);
                    }

                    if (isVisible_Aimbot(ClosestEnemys) || wasPulled) {
                        {
                            void *HeadTF = TransformNode(*(void **) ((uint64_t) ClosestEnemys + _HeadTF));
                            if (HeadTF != nullptr) {
                                Vector3 EnemayHead = Transform_INTERNAL_GetPosition(HeadTF);
                                void *HeadTF2 = TransformNode(*(void **) ((uint64_t) LocalPlayer + _HeadTF));
                                if (HeadTF2 != nullptr) {
                                    Vector3 LocalHead = Transform_INTERNAL_GetPosition(HeadTF2);
                                    auto HeadCollider = get_HeadCollider(ClosestEnemys);
                                    if (HeadCollider != nullptr) {
                                        void *headGameObj = get_gameObject(HeadCollider);
                                        if (headGameObj != nullptr) {
                                            *(void **) ((uint64_t) hitInfo + Hit_GameObject) = headGameObj;
                                            *(void **) ((uint64_t) hitInfo + Hit_HeadCollider) = HeadCollider;
                                            *(Vector3 *) ((uint64_t) hitInfo + Hit_HitLoc) = EnemayHead;
                                            float v19 = EnemayHead.X - LocalHead.X;
                                            float dy = EnemayHead.Y - LocalHead.Y;
                                            float dz = EnemayHead.Z - LocalHead.Z;
                                            float v21 = sqrtf(v19 * v19 + dy * dy + dz * dz);
                                            Vector3 direction = {0, 0, 0};
                                            if (v21 > 0.001f) {
                                                direction.X = v19 / v21;
                                                direction.Y = dy / v21;
                                                direction.Z = dz / v21;
                                            }

                                            *(Vector3 *) ((uint64_t) hitInfo + Hit_Normal) = direction;
                                            *(Vector3 *) ((uint64_t) hitInfo + Hit_RayDir) = direction;
                                            *(Vector3 *) ((uint64_t) hitInfo + Hit_StartPos) = LocalHead;
                                            *(Vector3 *) ((uint64_t) hitInfo + Hit_OrgStrtPos) = LocalHead;
                                            *(int *) ((uint64_t) hitInfo + Hit_Part) = 1;
                                            *(bool *) ((uint64_t) hitInfo + Hit_Ignore) = false;
                                            hitInfo->FFDIOGPKCKF = 0.01f;
                                            hitInfo->IHNCAADOAAE = 500;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    bool result = NoBUlletTractOriginal(weapon, hitInfo);
                    if (wasPulled && enemyTF) {
                        set_position_Injected(enemyTF, originalPos);
                    }

                    return result;
                }
            }
        }
    }

    return NoBUlletTractOriginal(weapon, hitInfo);
}

void* (*old_EventLogger_LoginInfo)(void) = nullptr;
void* new_EventLogger_LoginInfo(void) {
    void* info = old_EventLogger_LoginInfo();
    if (info && MasterBool.loginIndia) {
        *(void**)((uintptr_t)info + 0x24) = Il2CppString::Create(OBFUSCATE("2.131.22"));
        *(void**)((uintptr_t)info + 0x28) = Il2CppString::Create(OBFUSCATE("2019118334"));
    }
    return info;
}

static bool (*orig_get_isGrounded)(void* instance) = nullptr;
bool hook_get_isGrounded(void* instance) {
    if (flyhack.load()) return true;
    return orig_get_isGrounded ? orig_get_isGrounded(instance) : true;
}
static bool (*orig_get_ResetGuest)(void *instance) = nullptr;
static bool hook_get_ResetGuest(void *instance) {
    if (MasterBool.resetGuest.load(std::memory_order_relaxed)) return true;
    return orig_get_ResetGuest ? orig_get_ResetGuest(instance) : false;
}

void *pthreadcreate(void *arg) {
    while (true) {
        if (getLibBase(targetLibName) != 0) {
            Il2CppAttach();
            void* igAddr = (void*) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.PhysicsModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("CharacterController"), OBFUSCATE("get_isGrounded"), 0);
            if (igAddr) {
                DobbyHook(igAddr, (void *) hook_get_isGrounded, (void **) &orig_get_isGrounded);
            }
            void *resetGuestAddress = (void *)Il2CppGetMethodOffset(
                    OBFUSCATE("Assembly-CSharp.dll"),
                    OBFUSCATE("COW"),
                    OBFUSCATE("GameConfig"),
                    OBFUSCATE("get_ResetGuest"),
                    0);
            if (resetGuestAddress) {
                DobbyHook(resetGuestAddress, (void *)hook_get_ResetGuest,
                          (void **)&orig_get_ResetGuest);
            }
            DobbyHook((void *) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"),OBFUSCATE("IsWeaponInAutoCharge"), 0),(void *) IsWeaponInAutoCharge_Hook, (void **) &old_IsWeaponInAutoCharge);
            DobbyHook((void *) offset_SetStartDamage, (void *) NoBUlletTract,(void **) &NoBUlletTractOriginal);
            DobbyHook((void *) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("GCommon"),OBFUSCATE("AnimationSystemComponent"),OBFUSCATE("GetCurrentRunningHandler"), 1),(void *) _GetCurrentRunningHandler, (void **) &GetCurrentRunningHandler);


            DobbyHook((void*) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetAuxAimRotation"), 0), (void*)Lasttry_Hook, (void**)&orif_Tryfixfake);
            {
                void* garAddr = (void*) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetAimRotation"), 0);
                if (garAddr) DobbyHook(garAddr, (void*)hook_Player_GetAimRotation, (void**)&orig_Player_GetAimRotation);
                void* ctaAddr = (void*) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetCameraTrackableEntityAimRotation"), 0);
                if (ctaAddr) DobbyHook(ctaAddr, (void*)hook_Player_GetCameraTrackableEntityAimRotation, (void**)&orig_Player_GetCameraTrackableEntityAimRotation);
            }


            {
                void* liAddr = (void*) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("EventLogger"), OBFUSCATE("LoginInfo"), 0);
                if (liAddr) {
                    DobbyHook(liAddr, (void *) new_EventLogger_LoginInfo, (void **) &old_EventLogger_LoginInfo);
                    LOGI("Login India: EventLogger.LoginInfo hooked");
                }
            }

            DobbyHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_ShowDamageNum"), 0), (void*)hook_get_ShowDamageNum, (void**)&orig_get_ShowDamageNum);
            void* isShowDamageInfo = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsShowDamageInfo"), 0);
            if (isShowDamageInfo) {
                DobbyHook(isShowDamageInfo, (void*)hook_get_IsShowDamageInfo, (void**)&orig_get_IsShowDamageInfo);
            }
            DobbyHook((void *) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.HUD"), OBFUSCATE("UIHudNameController"), OBFUSCATE("ShowDamage"), 6), (void *)hook_ShowDamage, (void **) &orig_ShowDamage);
            InstallSilentDamageHooks();


            pthread_exit(0);
        }

        return NULL;
    }
}

#ifndef STANDALONE_LIB

__attribute__((constructor))

void initializer() {
    pthread_t ptid12;
    pthread_create(&ptid12, nullptr, pthreadcreate, nullptr);
    pthread_t ptid1;
    pthread_create(&ptid1, nullptr, CreateServer, nullptr);
}

#endif
