#pragma once

// ======================================================================
// AIMBOT & AIMKILL MODULE
// Features: Enable Functions (102), AimKill Take (991925),
//           Kill 360 (1305), MODE 360 V2 (59926), Weapon Switch (508)
// ======================================================================

static void ComputeAndStoreHashExact(int* attackerController, void* target, uint32_t tick);

// ======================================================================
// MODE 360 V2 (59926) - aim read focus + UDP aim-sync (opcode 106)
// Pendant le strike, on force les getters d'aim du local player a
// retourner la rotation vers la cible (meme derriere nous) et on sync
// la rotation par UDP au serveur avant d'envoyer le paquet de degats.
// ======================================================================
static bool g_aimkillAimReadFocus = false;
static void* g_aimkillAimFocusPlayer = nullptr;
static Quaternion g_aimkillAimFocusQuat;

void (*orig_Player_GetAimRotation)(void* outQuat, void* player);
void (*orig_Player_GetCameraTrackableEntityAimRotation)(void* outQuat, void* player);

static void AimkillForceQuatOut(void* outQuat) {
    float* f = (float*)outQuat;
    if (!f) return;
    f[0] = g_aimkillAimFocusQuat.X;
    f[1] = g_aimkillAimFocusQuat.Y;
    f[2] = g_aimkillAimFocusQuat.Z;
    f[3] = g_aimkillAimFocusQuat.W;
}

static void hook_Player_GetAimRotation(void* outQuat, void* player) {
    if (g_aimkillAimReadFocus && player == g_aimkillAimFocusPlayer) {
        AimkillForceQuatOut(outQuat);
        return;
    }
    orig_Player_GetAimRotation(outQuat, player);
}

static void hook_Player_GetCameraTrackableEntityAimRotation(void* outQuat, void* player) {
    if (g_aimkillAimReadFocus && player == g_aimkillAimFocusPlayer) {
        AimkillForceQuatOut(outQuat);
        return;
    }
    orig_Player_GetCameraTrackableEntityAimRotation(outQuat, player);
}

static void AimkillBeginAimReadFocus(void* localPlayer, Vector3 fromPos, Vector3 targetPos) {
    if (!MasterBool.mode360v2) return;
    g_aimkillAimFocusQuat = GetRotationToLocation(targetPos, 0.0f, fromPos);
    g_aimkillAimFocusPlayer = localPlayer;
    g_aimkillAimReadFocus = true;
}

static void AimkillEndAimReadFocus() {
    g_aimkillAimReadFocus = false;
    g_aimkillAimFocusPlayer = nullptr;
}

static const uint32_t kUdpSyncAimRotationOpcode = 106;

static void* GetTarget = nullptr;

void (*orif_Tryfixfake)(int* outQuat, void* player) = nullptr;
void Lasttry_Hook(int* outQuat, void* player) {
    // MODE 360 V2 : pendant le strike, force la rotation lue vers la cible
    if (g_aimkillAimReadFocus && player == g_aimkillAimFocusPlayer) {
        float* f = (float*)outQuat;
        if (f) {
            f[0] = g_aimkillAimFocusQuat.X;
            f[1] = g_aimkillAimFocusQuat.Y;
            f[2] = g_aimkillAimFocusQuat.Z;
            f[3] = g_aimkillAimFocusQuat.W;
        }
        return;
    }
    if (!player || !IsValidPtr(player)) {
        orif_Tryfixfake(outQuat, player);
        return;
    }
    orif_Tryfixfake(outQuat, player);
    if (!MasterBool.enableESP || !MasterBool.backFakeFix) return;
    void* local = Current_Local_Player();
    if (player != local) return;
    if (!GetTarget || !IsValidPtr(GetTarget)) {
        GetTarget = nullptr;
        return;
    }
    if (IsDieing(GetTarget) || IsLocalTeammate(GetTarget) || GetHp(GetTarget) <= 0) {
        GetTarget = nullptr;
        return;
    }

    void* localHead = TransformNode(*(void**)((uint64_t)player + _HeadTF));
    void* enemyHead = TransformNode(*(void**)((uint64_t)GetTarget + _HeadTF));
    if (!localHead || !enemyHead) return;
    Vector3 localPos = Transform_INTERNAL_GetPosition(localHead);
    Vector3 enemyPos = Transform_INTERNAL_GetPosition(enemyHead);
    Vector3 dir = enemyPos - localPos;
    float mag = sqrtf(dir.X * dir.X + dir.Y * dir.Y + dir.Z * dir.Z);
    if (mag < 0.0001f) return;
    dir.X /= mag;
    dir.Y /= mag;
    dir.Z /= mag;
    Vector3 forward = {0.0f, 0.0f, 1.0f};
    float dot = forward.X * dir.X + forward.Y * dir.Y + forward.Z * dir.Z;
    float quat[4];
    if (fabs(dot - 1.0f) < 0.0001f) {
        quat[0] = 0.0f; quat[1] = 0.0f; quat[2] = 0.0f; quat[3] = 1.0f;
    } else if (fabs(dot + 1.0f) < 0.0001f) {
        quat[0] = 0.0f; quat[1] = 1.0f; quat[2] = 0.0f; quat[3] = 0.0f;
    } else {
        Vector3 axis = Vector3::Cross(forward, dir);
        float axisMag = sqrtf(axis.X * axis.X + axis.Y * axis.Y + axis.Z * axis.Z);
        if (axisMag > 0.0001f) {
            axis.X /= axisMag;
            axis.Y /= axisMag;
            axis.Z /= axisMag;
        }
        float angle = acosf(dot);
        float half = angle * 0.5f;
        float s = sinf(half);
        quat[0] = axis.X * s;
        quat[1] = axis.Y * s;
        quat[2] = axis.Z * s;
        quat[3] = cosf(half);
    }

    float* outFloat = (float*)outQuat;
    outFloat[0] = quat[0];
    outFloat[1] = quat[1];
    outFloat[2] = quat[2];
    outFloat[3] = quat[3];
}

static const long long s_aimkilltakekimkc_CallDelayMs = 8;
static const long long s_hope22_SwitchDelayMs = 1;
static const int s_hope22_PacketsPerTarget = 2;

static void* COW_GameFacade__CurrentGameModeSetting() {
    return ((void* (*)())offset_CurrentGameModeSetting)();
}

static void* COW_GameFacade__CurrentGameSimulationTimer() {
    return CurrentGameSimulationTimer();
}

static void* GetStaticData() {
    void* typeInfo = (void*)offset_TypeInfo_MCDLKHAOJJG;
    if (!typeInfo) return nullptr;
    return *(void**)((uintptr_t)typeInfo + 0x5C);
}

static void* GetByteArray() {
    void* staticData = GetStaticData();
    if (!staticData) return nullptr;
    return *(void**)((uintptr_t)staticData + 0xC);
}

static void* GetHMAC() {
    void* staticData = GetStaticData();
    if (!staticData) return nullptr;
    return *(void**)((uintptr_t)staticData + 0x10);
}

static int JMCENJNEHKE(int a1) {
    if (!offset_JMCENJNEHKE) return 0;
    return ((int (*)(int))offset_JMCENJNEHKE)(a1);
}

static uint32_t GetCurrentGameTick() {
    void* timeService = COW_GameFacade__CurrentGameSimulationTimer();
    if (!timeService) return 0;
    return *(uint32_t*)((uintptr_t)timeService + 0x18);
}

static void ComputeAndStoreHashExact(int* attackerController, void* target, uint32_t tick) {
    void* gameModeSetting = COW_GameFacade__CurrentGameModeSetting();
    if (!gameModeSetting) return;
    bool flag_AC4 = *(bool*)((uintptr_t)gameModeSetting + 0xb68); // EnableHitTagHackCheck
    if (!flag_AC4) return;
    void* byteArrayObj = GetByteArray();
    if (!byteArrayObj) return;
    uint8_t* data = (uint8_t*)((uintptr_t)byteArrayObj + 0x10);
    COW_GamePlay_IHAAMHPPLMG_o targetID = GetplayerID(target);
    uint32_t seed = CFFPIACECIG(targetID);
    *(uint32_t*)(data + 0) = tick;
    *(uint32_t*)(data + 4) = seed;
    void* hmac = GetHMAC();
    if (!hmac) return;
    void* hashBytes = ((void* (*)(void*, void*, int))offset_HMAC_ComputeHash)(hmac, byteArrayObj, 0);
    if (!hashBytes) return;
    int hashInt = ((int (*)(void*, int, int))offset_BitConverter_ToInt32)(hashBytes, 0, 0);
    int finalHash = JMCENJNEHKE(hashInt);
    attackerController[16] = finalHash;
}

bool fastSwitchPatched = false;

bool (*old_IsWeaponInAutoCharge)(void *instance) = nullptr;
bool IsWeaponInAutoCharge_Hook(void *instance) {
    if ((MasterBool.Aimkillsend || MasterBool.AimkillTake) && instance != NULL) {
        return false;
    }

    return old_IsWeaponInAutoCharge ? old_IsWeaponInAutoCharge(instance) : false;
}

bool isInsideFOV(int x, int y, float customFOV) {
    float circle_x = (float)g_screenWidth / 2.0f;
    float circle_y = (float)g_screenHeight / 2.0f;
    float rad = (customFOV > 0) ? customFOV : MasterBool.aimbotFOV;
    return (x - circle_x) * (x - circle_x) + (y - circle_y) * (y - circle_y) <= rad * rad;
}

void* BestEnemyFind(void* skipTarget = nullptr)

{
    void* StaticGameFacade = *(void**)((uint64_t)_GameFacade + _StaticClass);
    if (!StaticGameFacade) return nullptr;
    void *ClassMatchGame = *(void **) ((uint64_t) StaticGameFacade + _MatchGame);
    if (ClassMatchGame == nullptr) return nullptr;
    void *current_match = *(void **) ((uint64_t) ClassMatchGame + _Match);
    if (current_match == nullptr) return nullptr;
    auto matchStatus = *(uint32_t *) ((uint64_t) current_match + _MatchState);
    if (matchStatus != 1) return nullptr;

    void *CurrentLocalPlayer = Current_Local_Player();
    if (CurrentLocalPlayer == nullptr) return nullptr;

    static std::vector<std::pair<float, void*>> s_adiFovList;
    s_adiFovList.clear();
    Vector3 v2Middle = Vector3((float)(g_screenWidth / 2), (float)(g_screenHeight / 2));
    auto players = GetEntities(current_match);
    for (auto player: players) {
        if (player == nullptr || player == CurrentLocalPlayer || player == skipTarget || !IsValidPtr(player)) continue;
        if (IsDieing(player) || GetHp(player) <= 0 || IsLocalTeammate(player)) continue;
        void *HeadTF = TransformNode(*(void **) ((uint64_t) player + _HeadTF));
        if (HeadTF == nullptr) continue;
        Vector3 WorldToScreenHead = WorldToScreenPoint(Transform_INTERNAL_GetPosition(HeadTF));
        if (WorldToScreenHead.Z < 0.01f) continue;
        if (!isInsideFOV((int)WorldToScreenHead.X, (int)WorldToScreenHead.Y)) continue;
        float dist = (float)Vector3Distance(v2Middle, Vector3(WorldToScreenHead.X, WorldToScreenHead.Y));
        s_adiFovList.push_back(std::make_pair(dist, player));
    }

    if (!s_adiFovList.empty()) {
        std::sort(s_adiFovList.begin(), s_adiFovList.end(), [](const std::pair<float, void*>& a, const std::pair<float, void*>& b) { return a.first < b.first; });
        return s_adiFovList[0].second;
    }

    return nullptr;
}

void* BestEnemyFind360()

{
    static std::vector<std::pair<float, void*>> enemyList;
    enemyList.clear();
    void* StaticGameFacade = *(void**)((uint64_t)_GameFacade + _StaticClass);
    if (!StaticGameFacade) return nullptr;
    void *ClassMatchGame = *(void **)((uint64_t)StaticGameFacade + _MatchGame);
    if (!ClassMatchGame) return nullptr;
    void *current_match = *(void **)((uint64_t)ClassMatchGame + _Match);
    if (!current_match) return nullptr;
    auto matchStatus = *(uint32_t*)((uint64_t)current_match + _MatchState);
    if (matchStatus != 1) return nullptr;
    void *LocalPlayer = Current_Local_Player();
    if (!LocalPlayer) return nullptr;
    void *LocalTF = Component_get_transform(LocalPlayer);
    if (!LocalTF) return nullptr;
    Vector3 LocalPos = Transform_INTERNAL_GetPosition(LocalTF);
    auto players = GetEntities(current_match);
    for (auto player: players) {
        void* enemy = player;
        if (!enemy || enemy == LocalPlayer || !IsValidPtr(enemy))continue;
        if (IsDieing(enemy))continue;
        if (IsLocalTeammate(enemy)) continue;
        if (GetHp(enemy) <= 0)continue;
        void *HeadTF = TransformNode(*(void**)((uint64_t)enemy + _HeadTF));
        if (!HeadTF) continue;
        Vector3 enemyPos = Transform_INTERNAL_GetPosition(HeadTF);
        float distance = Vector3Distance(LocalPos, enemyPos);
        enemyList.push_back(std::make_pair(distance, enemy));
    }

    if (enemyList.empty()) return nullptr;
    std::sort(enemyList.begin(), enemyList.end(),[](const std::pair<float, void*>& a, const std::pair<float, void*>& b){return a.first < b.first;});
    return enemyList[0].second;
}


static bool ShouldSuppressDamageVisual() {
    return MasterBool.AimkillTake;
}

static bool ShouldSuppressDamageFeedback() {
    return true;
}

static std::chrono::steady_clock::time_point g_silentDamageAudioUntil{};

static bool IsSilentDamageAudioActive() {
    if (MasterBool.Aimkillsend || MasterBool.AimkillTake) return true;
    return std::chrono::steady_clock::now() < g_silentDamageAudioUntil;
}

static void* GetIl2CppMethod(const char* namespaze, const char* klass, const char* method, int args) {
    return (void*)Il2CppGetMethodOffset(
            OBFUSCATE("Assembly-CSharp.dll"),
            namespaze,
            klass,
            method,
            args);
}

static void InstallSilentDamageHook(void* target, void* hook, void** orig) {
    if (target && hook) {
        DobbyHook(target, hook, (void**)orig);
    }
}

void (*orig_ShowDamage)(void* thiz, int damage, void* colliderT, void* p, int shieldDamage, int weaponID, float delay);
void hook_ShowDamage(void* thiz, int damage, void* colliderT, void* p, int shieldDamage, int weaponID, float delay) {
    if (ShouldSuppressDamageVisual()) {
        return;
    }
    orig_ShowDamage(thiz, damage, colliderT, p, shieldDamage, weaponID, delay);
}

bool (*orig_get_ShowDamageNum)(void* thiz);
bool hook_get_ShowDamageNum(void* thiz) {
    if (ShouldSuppressDamageVisual()) {
        return false;
    }
    return orig_get_ShowDamageNum(thiz);
}

bool (*orig_get_IsShowDamageInfo)(void* thiz);
bool hook_get_IsShowDamageInfo(void* thiz) {
    if (ShouldSuppressDamageVisual()) {
        return false;
    }
    return orig_get_IsShowDamageInfo ? orig_get_IsShowDamageInfo(thiz) : false;
}

bool (*orig_IsIgnorePlayerAudioComponent)(void* thiz);
bool hook_IsIgnorePlayerAudioComponent(void* thiz) {
    if (IsSilentDamageAudioActive()) {
        return true;
    }
    return orig_IsIgnorePlayerAudioComponent ? orig_IsIgnorePlayerAudioComponent(thiz) : false;
}

bool (*orig_get_SoundEffectOn)(void* thiz);
bool hook_get_SoundEffectOn(void* thiz) {
    if (IsSilentDamageAudioActive()) {
        return false;
    }
    return orig_get_SoundEffectOn ? orig_get_SoundEffectOn(thiz) : true;
}

void (*orig_PlayOneShot2)(void* thiz, void* resId, float volume);
void hook_PlayOneShot2(void* thiz, void* resId, float volume) {
    if (IsSilentDamageAudioActive()) return;
    if (orig_PlayOneShot2) orig_PlayOneShot2(thiz, resId, volume);
}

void (*orig_PlayOneShot3)(void* thiz, void* resId, float volume, bool allowDuplicate);
void hook_PlayOneShot3(void* thiz, void* resId, float volume, bool allowDuplicate) {
    if (IsSilentDamageAudioActive()) return;
    if (orig_PlayOneShot3) orig_PlayOneShot3(thiz, resId, volume, allowDuplicate);
}

void (*orig_Play3DShot3)(void* thiz, void* resId, void* position, float volume);
void hook_Play3DShot3(void* thiz, void* resId, void* position, float volume) {
    if (IsSilentDamageAudioActive()) return;
    if (orig_Play3DShot3) orig_Play3DShot3(thiz, resId, position, volume);
}

void (*orig_Play3DShot4)(void* thiz, void* resId, void* position, float volume, float minDistance);
void hook_Play3DShot4(void* thiz, void* resId, void* position, float volume, float minDistance) {
    if (IsSilentDamageAudioActive()) return;
    if (orig_Play3DShot4) orig_Play3DShot4(thiz, resId, position, volume, minDistance);
}

void (*orig_PlaySoundEffect2)(void* thiz, void* resId, float volume);
void hook_PlaySoundEffect2(void* thiz, void* resId, float volume) {
    if (IsSilentDamageAudioActive()) return;
    if (orig_PlaySoundEffect2) orig_PlaySoundEffect2(thiz, resId, volume);
}

void (*orig_PlaySoundEffect3)(void* thiz, void* resId, float volume, bool loop);
void hook_PlaySoundEffect3(void* thiz, void* resId, float volume, bool loop) {
    if (IsSilentDamageAudioActive()) return;
    if (orig_PlaySoundEffect3) orig_PlaySoundEffect3(thiz, resId, volume, loop);
}

void (*orig_PlayOneShotCB)(void* thiz, void* resId, float volume, void* callback);
void hook_PlayOneShotCB(void* thiz, void* resId, float volume, void* callback) {
    if (IsSilentDamageAudioActive()) return;
    if (orig_PlayOneShotCB) orig_PlayOneShotCB(thiz, resId, volume, callback);
}

#define SUPPRESS_VISUAL() if (ShouldSuppressDamageVisual()) return

void (*orig_LocalPlayerHitEffect1)(void* thiz, bool isHeadShot);
void hook_LocalPlayerHitEffect1(void* thiz, bool isHeadShot) {
    SUPPRESS_VISUAL();
    if (orig_LocalPlayerHitEffect1) orig_LocalPlayerHitEffect1(thiz, isHeadShot);
}

void (*orig_LocalPlayerHitEffect0)(void* thiz);
void hook_LocalPlayerHitEffect0(void* thiz) {
    SUPPRESS_VISUAL();
    if (orig_LocalPlayerHitEffect0) orig_LocalPlayerHitEffect0(thiz);
}

void (*orig_CrosshairPlayHitEffect0)(void* thiz);
void hook_CrosshairPlayHitEffect0(void* thiz) {
    SUPPRESS_VISUAL();
    if (orig_CrosshairPlayHitEffect0) orig_CrosshairPlayHitEffect0(thiz);
}

void (*orig_DelayShowHitMaker0)(void* thiz);
void hook_DelayShowHitMaker0(void* thiz) {
    SUPPRESS_VISUAL();
    if (orig_DelayShowHitMaker0) orig_DelayShowHitMaker0(thiz);
}

void (*orig_OnLocalPlayerHitOthers1)(void* thiz, void* data);
void hook_OnLocalPlayerHitOthers1(void* thiz, void* data) {
    SUPPRESS_VISUAL();
    if (orig_OnLocalPlayerHitOthers1) orig_OnLocalPlayerHitOthers1(thiz, data);
}

void (*orig_OnLocalPlayerHitOthersSimple0)(void* thiz);
void hook_OnLocalPlayerHitOthersSimple0(void* thiz) {
    SUPPRESS_VISUAL();
    if (orig_OnLocalPlayerHitOthersSimple0) orig_OnLocalPlayerHitOthersSimple0(thiz);
}

void (*orig_ShowFlashHint0)(void* thiz);
void hook_ShowFlashHint0(void* thiz) {
    SUPPRESS_VISUAL();
    if (orig_ShowFlashHint0) orig_ShowFlashHint0(thiz);
}

void (*orig_OnFlashHitEnemy1)(void* thiz, void* data);
void hook_OnFlashHitEnemy1(void* thiz, void* data) {
    SUPPRESS_VISUAL();
    if (orig_OnFlashHitEnemy1) orig_OnFlashHitEnemy1(thiz, data);
}

void (*orig_OnHitArmor1)(void* thiz, void* data);
void hook_OnHitArmor1(void* thiz, void* data) {
    SUPPRESS_VISUAL();
    if (orig_OnHitArmor1) orig_OnHitArmor1(thiz, data);
}

void (*orig_PlayHitTrace1)(void* thiz, void* playerId);
void hook_PlayHitTrace1(void* thiz, void* playerId) {
    SUPPRESS_VISUAL();
    if (orig_PlayHitTrace1) orig_PlayHitTrace1(thiz, playerId);
}

void (*orig_SetHitHintInfo3)(void* thiz, void* player, void* resId, int hint3DType);
void hook_SetHitHintInfo3(void* thiz, void* player, void* resId, int hint3DType) {
    SUPPRESS_VISUAL();
    if (orig_SetHitHintInfo3) orig_SetHitHintInfo3(thiz, player, resId, hint3DType);
}

void (*orig_ProjectileOnPlayHitEffect1)(void* thiz, void* hitInfo);
void hook_ProjectileOnPlayHitEffect1(void* thiz, void* hitInfo) {
    SUPPRESS_VISUAL();
    if (orig_ProjectileOnPlayHitEffect1) orig_ProjectileOnPlayHitEffect1(thiz, hitInfo);
}

void (*orig_OnStackKill1)(void* thiz, void* killInfo);
void hook_OnStackKill1(void* thiz, void* killInfo) {
    SUPPRESS_VISUAL();
    if (orig_OnStackKill1) orig_OnStackKill1(thiz, killInfo);
}

static void InstallDamageVisualHooks() {
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("COW"), OBFUSCATE("UIHudCrosshairsController"), OBFUSCATE("LocalPlayerHitEffect"), 1),
            (void*)hook_LocalPlayerHitEffect1,
            (void**)&orig_LocalPlayerHitEffect1);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("COW"), OBFUSCATE("UIHudCrosshairsController"), OBFUSCATE("LocalPlayerHitEffect"), 0),
            (void*)hook_LocalPlayerHitEffect0,
            (void**)&orig_LocalPlayerHitEffect0);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("COW"), OBFUSCATE("UIHudCrosshairsController"), OBFUSCATE("PlayHitEffect"), 0),
            (void*)hook_CrosshairPlayHitEffect0,
            (void**)&orig_CrosshairPlayHitEffect0);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("COW"), OBFUSCATE("UIHudCrosshairsController"), OBFUSCATE("DelayShowHitMaker"), 0),
            (void*)hook_DelayShowHitMaker0,
            (void**)&orig_DelayShowHitMaker0);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("COW"), OBFUSCATE("UIHudCrosshairsController"), OBFUSCATE("OnLocalPlayerHitOthers"), 1),
            (void*)hook_OnLocalPlayerHitOthers1,
            (void**)&orig_OnLocalPlayerHitOthers1);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("COW"), OBFUSCATE("UIHudCrosshairsController"), OBFUSCATE("OnLocalPlayerHitOthersSimple"), 0),
            (void*)hook_OnLocalPlayerHitOthersSimple0,
            (void**)&orig_OnLocalPlayerHitOthersSimple0);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("COW"), OBFUSCATE("UIHudCrosshairsController"), OBFUSCATE("ShowFlashHint"), 0),
            (void*)hook_ShowFlashHint0,
            (void**)&orig_ShowFlashHint0);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("COW"), OBFUSCATE("UIHudCrosshairsController"), OBFUSCATE("OnFlashHitEnemy"), 1),
            (void*)hook_OnFlashHitEnemy1,
            (void**)&orig_OnFlashHitEnemy1);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("COW"), OBFUSCATE("UIHudCrosshairsController"), OBFUSCATE("OnHitArmor"), 1),
            (void*)hook_OnHitArmor1,
            (void**)&orig_OnHitArmor1);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("COW"), OBFUSCATE("InGameHint3DManager"), OBFUSCATE("PlayHitTrace"), 1),
            (void*)hook_PlayHitTrace1,
            (void**)&orig_PlayHitTrace1);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("COW"), OBFUSCATE("UIHud3DHint"), OBFUSCATE("SetHitHintInfo"), 3),
            (void*)hook_SetHitHintInfo3,
            (void**)&orig_SetHitHintInfo3);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("COW.GamePlay"), OBFUSCATE("LevelProjectile"), OBFUSCATE("OnPlayHitEffect"), 1),
            (void*)hook_ProjectileOnPlayHitEffect1,
            (void**)&orig_ProjectileOnPlayHitEffect1);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("COW"), OBFUSCATE("UIHudStackedKillNotificationController"), OBFUSCATE("OnKill"), 1),
            (void*)hook_OnStackKill1,
            (void**)&orig_OnStackKill1);
}

static void InstallSilentDamageAudioHooks() {
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsIgnorePlayerAudioComponent"), 0),
            (void*)hook_IsIgnorePlayerAudioComponent,
            (void**)&orig_IsIgnorePlayerAudioComponent);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("GCommon"), OBFUSCATE("AudioEngine.Unity"), OBFUSCATE("get_SoundEffectOn"), 0),
            (void*)hook_get_SoundEffectOn,
            (void**)&orig_get_SoundEffectOn);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("GCommon"), OBFUSCATE("AudioEngine.Unity"), OBFUSCATE("PlayOneShot"), 2),
            (void*)hook_PlayOneShot2,
            (void**)&orig_PlayOneShot2);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("GCommon"), OBFUSCATE("AudioEngine.Unity"), OBFUSCATE("PlayOneShot"), 3),
            (void*)hook_PlayOneShot3,
            (void**)&orig_PlayOneShot3);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("GCommon"), OBFUSCATE("AudioEngine.Unity"), OBFUSCATE("Play3DShot"), 3),
            (void*)hook_Play3DShot3,
            (void**)&orig_Play3DShot3);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("GCommon"), OBFUSCATE("AudioEngine.Unity"), OBFUSCATE("Play3DShot"), 4),
            (void*)hook_Play3DShot4,
            (void**)&orig_Play3DShot4);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("GCommon"), OBFUSCATE("FFAudioEngine.Unity"), OBFUSCATE("PlaySoundEffect"), 2),
            (void*)hook_PlaySoundEffect2,
            (void**)&orig_PlaySoundEffect2);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("GCommon"), OBFUSCATE("FFAudioEngine.Unity"), OBFUSCATE("PlaySoundEffect"), 3),
            (void*)hook_PlaySoundEffect3,
            (void**)&orig_PlaySoundEffect3);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("GCommon"), OBFUSCATE("AudioEngine.Unity"), OBFUSCATE("PlayOneShotWithCallBack"), 3),
            (void*)hook_PlayOneShotCB,
            (void**)&orig_PlayOneShotCB);
    InstallSilentDamageHook(
            GetIl2CppMethod(OBFUSCATE("GCommon"), OBFUSCATE("AudioEngine.Unity"), OBFUSCATE("PlayOneShotWithCallBack"), 4),
            (void*)hook_PlayOneShotCB,
            (void**)&orig_PlayOneShotCB);
}

static void InstallSilentDamageHooks() {
    InstallSilentDamageAudioHooks();
    InstallDamageVisualHooks();
}
