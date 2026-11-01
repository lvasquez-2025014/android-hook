#ifndef ANDROID_MOD_MENU_HOOK_H
#define ANDROID_MOD_MENU_HOOK_H
#include <AutoUpdate/Il2Cppp.h>
#include "Hack/il2cpp.h"

#define offset_getdamage (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("KBIFKMJODNA"), 0)
#define offset_WeaponOnHand (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetWeaponOnHand"), 0)
#define offset_swapweaopon (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("SwapWeapon"), 3)
#define offset_ResetSwapWeaponTime (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("ResetSwapWeaponTime"), 0)
#define offset_GKHECDLGAJA (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("JGIEOGCGFEP"), 1)
#define offset_HeadTF2 (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetHeadTF"), 0)
#define offset_IHAAMHPPLMG_s (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("EOBFCLNNIJH"), OBFUSCATE("BEADLMGGGGL"))
#define offset_DGLCOGJJFMI_s (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("EOBFCLNNIJH"), OBFUSCATE("FIKKBFDEJOK"))
#define Die (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsDieing"), 0)
#define offset_GameFacade_Send (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("Send"), 4)
#define offset_QuatToSyncAimMsg (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("KANJBNIANHC"), OBFUSCATE("HDMMEFFMIJP"), 1)
#define offset_Transform_INTERNAL_GetPosition (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position_Injected"), 1)
#define offset_Component_GetTransform (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_transform"), 0)
#define offset_get_main (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("get_main"), 0)
#define offset_WorldToScreenPoint (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("WorldToScreenPoint"), 1)
#define offset_GetHp (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_CurHP"), 0)
#define offset_NoReload (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerAttributes"), OBFUSCATE("ShootNoReload"))
#define uniqueid (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("IEFJGCGOMID"), OBFUSCATE("ODFDGMOCJLL"))
#define GetUniqueID(WeaponHand) *(uint32_t*)((uint64_t)WeaponHand + uniqueid)
#define get_LHGGPDLOPAH (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("LMCBHLCPAJD"),OBFUSCATE("DJNIOABIFCA"),1)
#define offset_Transform_SetPosition (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("set_position"), 1)
#define offset_IsLocalTeammate (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsLocalTeammate"), 1)
#define offset_TransformNode (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("GCommon"), OBFUSCATE("TransformNode"), OBFUSCATE("get_transform"), 0)
#define offset_Current_Local_Player (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentLocalPlayer"), 0)
#define offset_get_IsDieing (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsDieing"), 0)
#define offset_itemid (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("IEFJGCGOMID"), OBFUSCATE("MJCENIGKEPH"), 0)
#define offset_get_HeadCollider (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_HeadCollider"), 0)
#define offset_get_gameObject (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_gameObject"), 0)
#define offset_Physics_Raycast (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("IPBGFJNGLOH"), OBFUSCATE("BKIAMGKDCLD"), 4)
#define offset_HeadTF (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GBKFHDFCPMD"))
#define offset_CurrentGameSimulationTimer (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentGameSimulationTimer"),0)
#define offset_get_realtimeSinceStartup  (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Time"),OBFUSCATE("get_realtimeSinceStartup"),0)
#define offset_GetRange (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("OEOBPHDFJMO"),0)

#define offset_PlayerID (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_PlayerID"),0)
#define offset_CFFPIACECIG (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("BEADLMGGGGL"), OBFUSCATE("DEIPJDIGKIP"),1)
#define offset_SyncSwapWeapon (uintptr_t) Il2CppCreateClassInstance(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("EOBFCLNNIJH"))
#define offset_StartFiring (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("StartFiring"),1)
#define offset_PlayerNetwork_StopFire (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("StopFire"), 1)
#define Match (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentMatch"), 0)
#define Local (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIHudDetectorController"), OBFUSCATE("GetLocalPlayer"), 0)
#define HeadColider (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_HeadCollider"), 0)
static int GetDamage(void *pthis)
{
    return ((int (*)(void *))offset_getdamage)(pthis);
}

static void *GetWeaponOnHand(void *local) {
    void *(*GetWeaponOnHand)(void *local) = (void *(*)(void *))offset_WeaponOnHand;
    return GetWeaponOnHand(local);
}


#include "Hack/Memory.h"


static void* GetTimeService()
{
    typedef void* (*fn)(const MethodInfo*);
    auto func = (fn)offset_CurrentGameSimulationTimer;
    return func ? func(nullptr) : nullptr;
}

uint32_t GetCurrentTick()
{
    void* timeService = GetTimeService();
    if (!timeService)
        return 0;
    return *(uint32_t*)((uintptr_t)timeService + 0x18);
}

typedef void* (*uint32_ToString_t)(uint32_t* instance);
typedef void* (*NPLKBIKNMLH_t)(void* listPtr, void* stringPtr);


void SwapWeapon(void *Pthis,int32_t FANMJANBFIL,bool GDKLMFLNNGM)
{
    return ((void (*)(void *,int,bool,void*))offset_swapweaopon)(Pthis,FANMJANBFIL,GDKLMFLNNGM,nullptr);
}

bool GameFacade_Send(uint32_t messageID,void *msg,uint8_t sendOption,bool cacheMsgAnyWay)
{
    return ((bool (*)(uint32_t,void *,uint8_t,bool))offset_GameFacade_Send)(messageID,msg,sendOption,cacheMsgAnyWay);
}

void *GKHECDLGAJA(void *pthis, void* a1)
{
    if (!pthis || !a1 || !offset_GKHECDLGAJA) return nullptr;
    return ((void* (*)(void *,void *))offset_GKHECDLGAJA)(pthis,a1);
}

static Vector3 Transform_INTERNAL_GetPosition(void *player)
{
    Vector3 out = Vector3::Zero();
    if (!player || !offset_Transform_INTERNAL_GetPosition) return out;
    void (*_Transform_INTERNAL_GetPosition)(void *transform, Vector3 * out) = (void (*)(void *, Vector3 *))offset_Transform_INTERNAL_GetPosition;
    _Transform_INTERNAL_GetPosition(player, &out);
    return out;
}

static void *Component_get_transform(void *player)
{
    if (!player || !offset_Component_GetTransform) return nullptr;
    void *(*_Component_GetTransform)(void *nullo) = (void *(*)(void *))offset_Component_GetTransform;
    return _Component_GetTransform(player);
}

static void *GetHeadTF(void* player) {
    void *(*_GetHeadTF)(void *_this) = (void *(*)(void *))offset_HeadTF2;
    return _GetHeadTF(player);
}
static bool get_IsDieing(void *player) {
    bool (*_get_die)(void *players) = (bool (*)(void *))(Die);
    return _get_die(player);
}
static void *Curent_Match() {
    void *(*_Curent_Match) (void *nuls) = (void *(*)(void *))(Match);
    return _Curent_Match(NULL);
}

static void *Player_GetHeadCollider(void *player) {
    void *(*_Player_GetHeadCollider)(void *players) = (void *(*)(void *))(HeadColider);
    return _Player_GetHeadCollider(player);
}

static void *GetLocalPlayer(void* Game) {
    void *(*_GetLocalPlayer)(void *match) = (void *(*)(void *))(Local);
    return _GetLocalPlayer(Game);
}
Vector3 GetHeadPosition(void* player) {
    if (!player) return Vector3::Zero();
    void* head = GetHeadTF(player);
    if (!head) return Vector3::Zero();
    void* tf = Component_get_transform(head);
    if (!tf) return Vector3::Zero();
    return Transform_INTERNAL_GetPosition(tf);
}

static void *get_main() {
    void *(*nget_main)(void *Instance) = (void *(*)(void *))offset_get_main;
    return nget_main(nullptr);
}

Vector3 WorldToScreenPoint(Vector3 pos) {
    auto main = get_main();
    if (main) {
        auto Camera_WorldToScreenPoint = (Vector3 (*)(void *, Vector3))offset_WorldToScreenPoint;
        return Camera_WorldToScreenPoint(main, pos);
    }
    return {0, 0, 0};
}


static int GetHp(void *instance) {
    return ((int (*)(void *)) offset_GetHp)(instance);
}




message_DEACEIFBHJK_o *LHGGPDLOPAH(Vector3 JOGHOHLEJFL) {
    return ((message_DEACEIFBHJK_o * (*)(Vector3))get_LHGGPDLOPAH)(JOGHOHLEJFL);
}





static void set_position_Injected(void* transform, Vector3 position);

static void Transform_set_position(void *player, Vector3 inn)
{
    if (!player) return;
    set_position_Injected(player, inn);
}




static bool IsLocalTeammate(void *instance) {
    return ((bool (*)(void *)) offset_IsLocalTeammate)(instance);
}

static Vector3 CameraPosition(void *player)
{
    return Transform_INTERNAL_GetPosition(Component_get_transform(get_main()));
}

void* TransformNode(void *_this)
{
    return ((void* (*)(void *))offset_TransformNode)(_this);
}


static void *Current_Local_Player() {
    void *(*_Local_Player)(void *players) = (void *(*)(void *))offset_Current_Local_Player;
    return _Local_Player(NULL);
}


static bool IsDieing(void *player) {
    if (!player) return false;
    bool (*_IsDieing)(void *players) = (bool (*)(void *))offset_get_IsDieing;
    return _IsDieing ? _IsDieing(player) : false;
}

static int GetWeaponID(void* enemy) {
    int (*GetWeapon)(void *player) = (int(*)(void *))offset_itemid;
    return GetWeapon(enemy);
}




void *get_HeadCollider(void *pthis)
{
    return ((void* (*)(void *))offset_get_HeadCollider)(pthis);
}


void *get_gameObject(void *Pthis)
{
    return ((void* (*)(void *))offset_get_gameObject)(Pthis);
}

static bool Physics_Raycast(Vector3 camLocation, Vector3 headLocation, unsigned int LayerID, void* collider) {
    bool (*_Physics_Raycast)(Vector3 camLocation, Vector3 headLocation, unsigned int LayerID, void* collider) = (bool(*)(Vector3, Vector3, unsigned int, void*))offset_Physics_Raycast;
    return _Physics_Raycast(camLocation, headLocation, LayerID, collider);
}

static inline bool IsPullTargetValid(void *enemy) {
    if (!enemy) return false;
    if (*(void **)((uintptr_t) enemy) == nullptr) return false;
    if (GetHp(enemy) <= 0) return false;
    if (IsDieing(enemy)) return false;
    void *headNode = *(void **) ((uintptr_t) enemy + offset_HeadTF);
    if (!headNode) return false;
    if (!TransformNode(headNode)) return false;
    if (!Component_get_transform(enemy)) return false;
    return true;
}

static inline void *GetPlayerHeadTransform(void *player) {
    if (!player) return nullptr;
    void *headNode = *(void **) ((uintptr_t) player + offset_HeadTF);
    if (!headNode) return nullptr;
    return TransformNode(headNode);
}

bool isVisible_Aimbot(void * player)
{
    if (!IsPullTargetValid(player)) return false;
    void *hitObj = NULL;
    void *HeadTF = GetPlayerHeadTransform(player);
    if (!HeadTF) return false;
    void *local = Current_Local_Player();
    if (!local) return false;
    Vector3 EnemyHeadPosition = Transform_INTERNAL_GetPosition(HeadTF);
    Vector3 LocalPlayerPos = CameraPosition(local);
    return !Physics_Raycast(LocalPlayerPos, EnemyHeadPosition, 12, &hitObj);
}

void* CurrentGameSimulationTimer()
{
    return ((void* (*)(void *))(offset_CurrentGameSimulationTimer))(NULL);
}


float get_realtimeSinceStartup() {
    if (offset_get_realtimeSinceStartup){
        float (*func)();
        func = (float (*)())(offset_get_realtimeSinceStartup);
        return func();
    } return 0;
}

static float get_Range(void *pthis)
{
    return ((float (*)(void *))offset_GetRange)(pthis);
}
bool isEnemyInRangeWeapon(void *player, void *enemy, void* weapon)
{
    if (player != nullptr && enemy != nullptr)
    {
        void *HeadTF = TransformNode(*(void **) ((uint64_t) enemy + offset_HeadTF));
        void *HeadTF2 = TransformNode(*(void **) ((uint64_t) player + offset_HeadTF));
        if (!HeadTF || !HeadTF2) return false;
        Vector3 EnemyHeadPosition = Transform_INTERNAL_GetPosition(HeadTF);
        Vector3 PlayerHeadPosition = Transform_INTERNAL_GetPosition(HeadTF2);
        float distance = Vector3::Distance(PlayerHeadPosition, EnemyHeadPosition);
        float maxRangeMeters = 120.0f;
        if (weapon != nullptr) {
            float r = get_Range(weapon);
            if (r > 0.0f) {
                if (r <= 2.0f) {
                    maxRangeMeters = r * 250.0f;
                } else {
                    maxRangeMeters = r;
                }
            }
        }
        return (distance <= maxRangeMeters);
    }
    return false;
}




bool SmartEnemyMovementTracker(void *enemy, bool apply = true) {
    if (!IsPullTargetValid(enemy)) return false;
    if (isVisible_Aimbot(enemy)) return true;
    void *enemyTransform = Component_get_transform(enemy);
    if (!enemyTransform) return false;
    void *HeadTF = GetPlayerHeadTransform(enemy);
    if (!HeadTF) return false;
    void *local = Current_Local_Player();
    if (!local) return false;
    Vector3 LocalPlayerPos = CameraPosition(local);
    Vector3 OriginalRootPos = Transform_INTERNAL_GetPosition(enemyTransform);
    Vector3 EnemyHeadPos = Transform_INTERNAL_GetPosition(HeadTF);
    void *hitObj = NULL;
    static const float ultraRadii[] = {1.5f, 2.7f, 3.9f, 5.0f};
    static const float ultraDirs[][2] = {
        {1.000f, 0.000f}, {0.966f, 0.259f}, {0.866f, 0.500f}, {0.707f, 0.707f},
        {0.500f, 0.866f}, {0.259f, 0.966f}, {0.000f, 1.000f}, {-0.259f, 0.966f},
        {-0.500f, 0.866f}, {-0.707f, 0.707f}, {-0.866f, 0.500f}, {-0.966f, 0.259f},
        {-1.000f, 0.000f}, {-0.966f, -0.259f}, {-0.866f, -0.500f}, {-0.707f, -0.707f},
        {-0.500f, -0.866f}, {-0.259f, -0.966f}, {0.000f, -1.000f}, {0.259f, -0.966f},
        {0.500f, -0.866f}, {0.707f, -0.707f}, {0.866f, -0.500f}, {0.966f, -0.259f}
    };
    static const float microY[] = {0.0f, 0.25f};
    for (float r : ultraRadii) {
        for (int i = 0; i < 24; i++) {
            float dx = ultraDirs[i][0] * r;
            float dz = ultraDirs[i][1] * r;
            for (float dy : microY) {
                Vector3 testHead = EnemyHeadPos;
                testHead.X += dx;
                testHead.Y += dy;
                testHead.Z += dz;
                if (!Physics_Raycast(LocalPlayerPos, testHead, 12, &hitObj)) {
                    if (apply) {
                        if (!IsPullTargetValid(enemy)) return false;
                        void *tf = Component_get_transform(enemy);
                        if (!tf) return false;
                        Vector3 finalRoot = OriginalRootPos;
                        finalRoot.X += dx;
                        finalRoot.Y += dy;
                        finalRoot.Z += dz;
                        Transform_set_position(tf, finalRoot);
                    }
                    return true;
                }
            }
        }
    }

    float emergencyY = 0.30f;
    Vector3 testHeadUp = EnemyHeadPos; testHeadUp.Y += emergencyY;
    if (!Physics_Raycast(LocalPlayerPos, testHeadUp, 12, &hitObj)) {
        if (apply) {
            if (!IsPullTargetValid(enemy)) return false;
            void *tf = Component_get_transform(enemy);
            if (!tf) return false;
            Vector3 finalRoot = OriginalRootPos;
            finalRoot.Y += emergencyY;
            Transform_set_position(tf, finalRoot);
        }
        return true;
    }

    return false;
}

COW_GamePlay_IHAAMHPPLMG_o GetplayerID(void *_this)
{
    return ((COW_GamePlay_IHAAMHPPLMG_o (*)(void *))offset_PlayerID)(_this);
}

uint32_t CFFPIACECIG(COW_GamePlay_IHAAMHPPLMG_o IDNEFEOPGIF)
{
    if (!offset_CFFPIACECIG) return 0;
    return ((uint32_t (*)(COW_GamePlay_IHAAMHPPLMG_o))offset_CFFPIACECIG)(IDNEFEOPGIF);
}

static void Syns_SwapWeapon_Impl(void *LocalPlayer, void *WeaponOnHand) {
    if (LocalPlayer == nullptr || WeaponOnHand == nullptr) return;
    if (!offset_SyncSwapWeapon) return;
    void *RUDP_CHANGE_INVENTORY_ON_HAND = (void*)offset_SyncSwapWeapon;
    if (RUDP_CHANGE_INVENTORY_ON_HAND) {
        if (offset_IHAAMHPPLMG_s != 0 && offset_CFFPIACECIG != 0) {
            *(uint32_t * )((uint64_t) RUDP_CHANGE_INVENTORY_ON_HAND + offset_IHAAMHPPLMG_s) = CFFPIACECIG(GetplayerID(LocalPlayer));
        }
        if (offset_DGLCOGJJFMI_s != 0) {
            *(uint32_t * )((uint64_t) RUDP_CHANGE_INVENTORY_ON_HAND + offset_DGLCOGJJFMI_s) = GetUniqueID(WeaponOnHand);
        }
        if (offset_GameFacade_Send != 0) {
            GameFacade_Send(108, RUDP_CHANGE_INVENTORY_ON_HAND, 2, 0);
        }
    }
}






static void StartFiring(void* player, void* weapon) {
    if (!player || !weapon || !offset_StartFiring) return;
    auto fn = reinterpret_cast<void(*)(void*, void*)>(offset_StartFiring);
    fn(player, weapon);
}


static void StopFire(void* player, void* weapon) {
    if (!player || !weapon || !offset_PlayerNetwork_StopFire) return;
    auto fn = reinterpret_cast<void(*)(void*, void*)>(offset_PlayerNetwork_StopFire);
    fn(player, weapon);
}

static void ResetSwapWeaponTime(void* player) {
    if (!player || !offset_ResetSwapWeaponTime) return;
    auto fn = reinterpret_cast<void(*)(void*)>(offset_ResetSwapWeaponTime);
    fn(player);
}






static void set_position_Injected(void* transform, Vector3 position)
{
    if (!transform) return;
    static void (*_set_position_Injected)(void*, Vector3*) = nullptr;
    if (!_set_position_Injected)
        _set_position_Injected = (void (*)(void*, Vector3*)) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"),OBFUSCATE("UnityEngine"),OBFUSCATE("Transform"),OBFUSCATE("set_position_Injected"),1);
    if (!_set_position_Injected) return;
    _set_position_Injected(transform, &position);
}

bool SafeSilentPullEx(void *enemy, float *outBestXZRadius,
                      float maxRadius = 10.0f, float step = 0.5f,
                      float maxYRadius = 1.5f, float yStep = 0.5f)
{
    if (!enemy) return false;
    void *enemyTF = Component_get_transform(enemy);
    if (!enemyTF) return false;
    Vector3 enemyRoot = Transform_INTERNAL_GetPosition(enemyTF);
    uintptr_t headNodePtr = *(uintptr_t*)((uintptr_t)enemy + offset_HeadTF);
    if (!headNodePtr) return false;
    void *headTF = TransformNode((void*)headNodePtr);
    if (!headTF) return false;
    Vector3 enemyHead = Transform_INTERNAL_GetPosition(headTF);
    void *lp = Current_Local_Player();
    if (!lp) return false;
    uintptr_t lpHeadNodePtr = *(uintptr_t*)((uintptr_t)lp + offset_HeadTF);
    if (!lpHeadNodePtr) return false;
    void *lpHeadTF = TransformNode((void*)lpHeadNodePtr);
    if (!lpHeadTF) return false;
    Vector3 lpHead = Transform_INTERNAL_GetPosition(lpHeadTF);
    void* hitObj = nullptr;
    if (!Physics_Raycast(lpHead, enemyHead, 12, &hitObj)) {
        if (outBestXZRadius) *outBestXZRadius = 0.0f;
        return true;
    }

    for (float r = step; r <= maxRadius; r += step) {
        for (int angle = 0; angle < 360; angle += 45) {
            float rad = angle * (float)(M_PI / 180.0);
            float dx = r * cosf(rad);
            float dz = r * sinf(rad);
            Vector3 testHead = { enemyHead.X + dx, enemyHead.Y, enemyHead.Z + dz };
            if (!Physics_Raycast(lpHead, testHead, 12, &hitObj)) {
                Vector3 newRoot = { enemyRoot.X + dx, enemyRoot.Y, enemyRoot.Z + dz };
                Transform_set_position(enemyTF, newRoot);
                if (outBestXZRadius) *outBestXZRadius = r;
                return true;
            }
        }

        if (r <= maxYRadius) {
            const float dy[2] = { r, -r };
            for (int yi = 0; yi < 2; ++yi) {
                Vector3 testHead = { enemyHead.X, enemyHead.Y + dy[yi], enemyHead.Z };
                if (!Physics_Raycast(lpHead, testHead, 12, &hitObj)) {
                    Vector3 newRoot = { enemyRoot.X, enemyRoot.Y + dy[yi], enemyRoot.Z };
                    Transform_set_position(enemyTF, newRoot);
                    if (outBestXZRadius) *outBestXZRadius = 0.0f;
                    return true;
                }
            }
        }
    }

    return false;
}

inline bool SilentGhostPullSafeV2(void *enemy, float *outBestXZRadius = nullptr, bool apply = true) {
    return SafeSilentPullEx(enemy, outBestXZRadius);
}

// ── Native Helper Functions Ported from Root Class.h ─────────────────
#define uniqueid_weapon (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("LGMNCCAPNHJ"), OBFUSCATE("DEGODFCOKGC"))
#define offset_SyncSwapWeapon_AK (uintptr_t) Il2CppCreateClassInstance(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("JDIHDLDIMCK"))
#define offset_IHAAMHPPLMG_s (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("JDIHDLDIMCK"), OBFUSCATE("BHGGAEEHJCO"))
#define offset_DGLCOGJJFMI_s (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("JDIHDLDIMCK"), OBFUSCATE("DBPPPOBFJNP"))
#define offset_GameFacade_Send (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("Send"), 4)

#undef GetUniqueID
static uint32_t GetUniqueID(void *weapon) {
    if (!weapon || !uniqueid_weapon) return 0;
    return *(uint32_t *)((uintptr_t)weapon + uniqueid_weapon);
}

#define offset_StopFire_PN (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("StopFire"), 1)
static void StopFire_PN(void *player, void *weapon) {
    if (!player || !weapon || !offset_StopFire_PN) return;
    ((void (*)(void *, void *))offset_StopFire_PN)(player, weapon);
}

#define offset_Weapon_StopFiring (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("NFFJEBKJCAK"), 0)
static void StopFiring2(void* weapon) {
    if (!weapon) return;
    auto fn = reinterpret_cast<void(*)(void*)>(offset_Weapon_StopFiring);
    fn(weapon);
}

#define offset_LCLHHHKFCFP (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("NPLMKLBKJPE"), 4)
static void *LCLHHHKFCFP(void *weapon, void *hitObj, void *collider, bool flag, void *dmgInfo) {
    if (!weapon || !offset_LCLHHHKFCFP) return nullptr;
    return ((void *(*)(void *, void *, void *, bool, void *))offset_LCLHHHKFCFP)(weapon, hitObj, collider, flag, dmgInfo);
}

#define offset_Player_TakeDamage (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("TakeDamage"), 9)
static int Call_Player_TakeDamage(void *enemy, int damage, COW_GamePlay_IHAAMHPPLMG_o pid, void *dmgInfo, int weaponID, Vector3 firePos, Vector3 hitPos, void *params, void *wdi, uint32_t veh) {
    if (!enemy || !offset_Player_TakeDamage) return 0;
    return ((int (*)(void *, int, COW_GamePlay_IHAAMHPPLMG_o, void *, int, Vector3, Vector3, void *, void *, uint32_t))offset_Player_TakeDamage)(
        enemy, damage, pid, dmgInfo, weaponID, firePos, hitPos, params, wdi, veh);
}

#endif
