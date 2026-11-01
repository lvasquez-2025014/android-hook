#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>
#include <map>

#include "class.h"

// --- DOWNKILL & GHOST HACK STATE ---
inline bool DownKill = false;
inline bool DownKillV2 = false;

#ifndef offset_GhostHack
#define offset_GhostHack (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("EPPMGCJDEDC"), OBFUSCATE("AGDFODEDCCP"), 1)
#endif

// ARM32 patch: MOV R0, #0; BX LR
static const uint8_t ghostPatchBytes[] = { 0x00, 0x00, 0xA0, 0xE3, 0x1E, 0xFF, 0x2F, 0xE1 };
static uint8_t ghostOrigBytes[8] = {0};
static bool ghostOrigSaved = false;

static void ghost_write_bytes(uintptr_t addr, const uint8_t* data, size_t len) {
    addr = addr & ~1;
    uintptr_t pageStart = addr & ~(uintptr_t)(getpagesize() - 1);
    mprotect((void*)pageStart, getpagesize() * 2, PROT_READ | PROT_WRITE | PROT_EXEC);
    memcpy((void*)addr, data, len);
    __builtin___clear_cache((char*)addr, (char*)(addr + len));
}

inline void GhostOn() {
    uintptr_t ghostAddress = offset_GhostHack;
    if (!ghostAddress) return;

    if (!ghostOrigSaved) {
        memcpy(ghostOrigBytes, (void*)(ghostAddress & ~1), sizeof(ghostPatchBytes));
        ghostOrigSaved = true;
    }

    ghost_write_bytes(ghostAddress, ghostPatchBytes, sizeof(ghostPatchBytes));
}

inline void GhostOff() {
    uintptr_t ghostAddress = offset_GhostHack;
    if (!ghostAddress || !ghostOrigSaved) return;

    ghost_write_bytes(ghostAddress, ghostOrigBytes, sizeof(ghostOrigBytes));

    void *local = Current_Local_Player();
    if (local) {
        void *tfLocal = Component_get_transform(local);
        if (tfLocal) {
            Vector3 currentPos = Transform_INTERNAL_GetPosition(tfLocal);
            currentPos.Y += 3.5f;
            Transform_set_position(tfLocal, currentPos);
        }
    }
}

inline bool ISValidDAT(void** outCurrentMatch = nullptr, void** outLocalPlayer = nullptr) {
    void* StaticGameFacade = *(void**)((uintptr_t)_GameFacade + _StaticClass);
    if (!StaticGameFacade) return false;

    void* ClassMatchGame = *(void**)((uintptr_t)StaticGameFacade + _MatchGame);
    if (!ClassMatchGame) return false;

    void* current_match = *(void**)((uintptr_t)ClassMatchGame + _Match);
    if (!current_match) return false;

    uint32_t matchStatus = *(uint32_t*)((uintptr_t)current_match + _MatchState);
    if (matchStatus != 1) return false;

    void* LocalPlayer = Current_Local_Player();
    if (!LocalPlayer) return false;

    if (IsDieing(LocalPlayer)) return false;

    if (outCurrentMatch) *outCurrentMatch = current_match;
    if (outLocalPlayer) *outLocalPlayer = LocalPlayer;
    return true;
}

inline void ApplyFlyHackDownKill(void* LocalPlayer) {
    static bool flySet = false;
    static float lockedY = 0.0f;
    static float lastX = 0.0f;
    static float lastZ = 0.0f;

    if (!DownKill && !DownKillV2) {
        flySet = false;
        return;
    }

    auto tf = Component_get_transform(LocalPlayer);
    if (tf == nullptr) return;

    Vector3 livePos = Transform_INTERNAL_GetPosition(tf);

    if (flySet) {
        // Auto-detect round transition or spawn location change (>15m displacement)
        float dx = livePos.X - lastX;
        float dz = livePos.Z - lastZ;
        if ((dx * dx + dz * dz) > 225.0f) {
            flySet = false;
        }
    }

    if (!flySet) {
        lockedY = livePos.Y - 3.5f; // Local player drops 3.5f from current round spawn ground
        flySet = true;
    }

    lastX = livePos.X;
    lastZ = livePos.Z;

    livePos.Y = lockedY;
    Transform_set_position(tf, livePos);
}

inline std::vector<void*> GetAllValidEnemies() {
    std::vector<void*> validEnemies;
    void* current_match = nullptr;
    void* localPlayer = nullptr;
    if (!ISValidDAT(&current_match, &localPlayer)) return validEnemies;

    auto entities = GetEntities(current_match);
    for (void* enemy : entities) {
        if (!enemy || enemy == localPlayer) continue;
        if (IsDieing(enemy) || GetHp(enemy) <= 0 || IsLocalTeammate(enemy)) continue;
        validEnemies.push_back(enemy);
    }
    return validEnemies;
}

struct EnemyLockInfo {
    float lockedY;
    float lastX;
    float lastZ;
};
static std::map<void*, EnemyLockInfo> s_enemyLockedY;

inline void ApplyDownPlayer(void* localPlayer) {
    if (localPlayer == nullptr || (!DownKill && !DownKillV2)) {
        s_enemyLockedY.clear();
        return;
    }

    std::vector<void*> allEnemies = GetAllValidEnemies();
    if (allEnemies.empty()) return;

    for (void* enemy : allEnemies) {
        if (!enemy) continue;
        auto enemyTransform = Component_get_transform(enemy);
        if (!enemyTransform) continue;

        Vector3 livePos = Transform_INTERNAL_GetPosition(enemyTransform);

        auto it = s_enemyLockedY.find(enemy);
        if (it == s_enemyLockedY.end()) {
            s_enemyLockedY[enemy] = { livePos.Y - 1.8f, livePos.X, livePos.Z };
        } else {
            // Auto-detect new round spawn or large map position change (>15m displacement)
            float dx = livePos.X - it->second.lastX;
            float dz = livePos.Z - it->second.lastZ;
            if ((dx * dx + dz * dz) > 225.0f) {
                it->second.lockedY = livePos.Y - 1.8f;
            }
            it->second.lastX = it->second.lastX;
            it->second.lastZ = it->second.lastZ;
        }

        livePos.Y = s_enemyLockedY[enemy].lockedY;
        Transform_set_position(enemyTransform, livePos);
    }
}

inline void ApplyDownPlayerV2(void* localPlayer) {
    if (localPlayer == nullptr || !DownKillV2) {
        return;
    }

    auto localTf = Component_get_transform(localPlayer);
    if (!localTf) return;
    Vector3 localPos = Transform_INTERNAL_GetPosition(localTf);

    std::vector<void*> allEnemies = GetAllValidEnemies();
    if (allEnemies.empty()) return;

    for (void* enemy : allEnemies) {
        if (!enemy) continue;
        auto enemyTransform = Component_get_transform(enemy);
        if (!enemyTransform) continue;

        Vector3 livePos = Transform_INTERNAL_GetPosition(enemyTransform);
        // DownKill V2: Bring enemy Y down to local player's height level (+0.5f)
        livePos.Y = localPos.Y + 0.5f;
        Transform_set_position(enemyTransform, livePos);
    }
}

inline void ApplyDownKill() {
    if (!DownKill && !DownKillV2) return;
    void *localPlayer = Current_Local_Player();
    if (localPlayer != nullptr) {
        ApplyFlyHackDownKill(localPlayer);
        if (DownKillV2) {
            ApplyDownPlayerV2(localPlayer);
        } else if (DownKill) {
            ApplyDownPlayer(localPlayer);
        }
    }
}
