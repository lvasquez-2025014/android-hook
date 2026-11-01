#include <list>
#include <vector>
#include <pthread.h>
#include <cstring>
#include <cmath>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include "Tools/Includes/Logger.h"
#include "Tools/Includes/obfuscate.h"
#include "Tools/Includes/Utils.h"
#include "Tools/SOCKET/client.h"
#include "Tools/SOCKET/IncludeClient.h"
#include "Widgets/ImportWidgets.h"
#include "Tools/DrawTools/Draw.h"

extern "C" bool ngSessionActive(void);

static JavaVM* gJvm = nullptr;
static jobject gAppContext = nullptr;


struct {
    bool ActivateAll = false;
    bool loginIndia = false;
    bool SilentGhostPullV2_Enable = false;
    bool mode360v2 = false;
    bool teleCombo = false;
    bool resetGuest = false;
} MasterBool;

struct {
    bool Aimkillsend = false;
    bool AimkillTake = false;
    bool backFakeFix = false;
    bool downkill = false;
    bool downkillv2 = false;
    float aimbotFOV = 0.0f;
} pAimbotPlayer;

struct {
    bool espLine = false;
    bool espBox = false;
    bool espHealth = false;
    bool espDrawFov = false;
    Color espColor = Color::White();
    bool espNickName = false;
} pEspPlayer;

using namespace std;



extern "C"
JNIEXPORT void JNICALL
Java_com_kanishkcheat_Menu_Init(JNIEnv *env, jclass thiz, jobject context) {
    startClient();
}

float fovRadius = 104;

extern "C"
JNIEXPORT void JNICALL
Java_com_kanishkcheat_Menu_Functions(JNIEnv *env, jclass clazz) {
    // Anti extraction / skip-login native guard: the cheat UI must never init
    // unless successful Crypton Auth login mirrors its session token into native memory.
    if (!ngSessionActive()) {
        LOGE("BLOCKED: native menu without active session");
        return;
    }

    Widget widget = Widget(env);

    widget.Tab(OBFUSCATE("AIMBOT"));

    widget.Switch(OBFUSCATE("Enable Functions"), 102);
    widget.FloatingSwitch(OBFUSCATE("Silent kill"), OBFUSCATE("pull"), 777781);
    widget.Switch(OBFUSCATE("Mode 360"), 59926);
    widget.Switch(OBFUSCATE("Login India 400"), 5020);
    widget.Switch(OBFUSCATE("Teleport"), 59910);

    widget.Switch(OBFUSCATE("Enable Keybind"), 6000);
    widget.Switch(OBFUSCATE("Clear Keybinds"), 6001);
    widget.Tab(OBFUSCATE("MISC"));
    widget.Switch(OBFUSCATE("RESET GUEST"), 6010);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_kanishkcheat_Menu_ChangesID(JNIEnv *env, jclass clazz, jint id, jint value) {
    switch (id) {
        // ESP Toggles
        case 1:
            pEspPlayer.espLine = (value == 1);
            break;
        case 2:
            pEspPlayer.espBox = (value == 1);
            break;
        case 3:
            pEspPlayer.espHealth = (value == 1);
            break;
        case 9:
            pEspPlayer.espNickName = (value == 1);
            break;
        case 16:
            pEspPlayer.espDrawFov = (value == 1);
            break;

        // Settings
        case 5020:
            MasterBool.loginIndia = (value == 1);
            SendFeatuere(5020, MasterBool.loginIndia);
            break;

        // Aimbot & AimKill
        case 102:
            MasterBool.ActivateAll = (value == 1);
            SendFeatuere(3, MasterBool.ActivateAll);
            break;
        case 104:
            pAimbotPlayer.aimbotFOV = (float)value;
            SendFOV(104, (float)value);
            break;
       
        case 1305:
            pAimbotPlayer.backFakeFix = (value == 1);
            SendFeatuere(1305, pAimbotPlayer.backFakeFix);
            break;
        case 59926:
            MasterBool.mode360v2 = (value == 1);
            SendFeatuere(59926, MasterBool.mode360v2);
            break;
        case 1045:
            pAimbotPlayer.downkill = (value == 1);
            SendFeatuere(1045, pAimbotPlayer.downkill);
            break;

        case 1047:
            pAimbotPlayer.downkillv2 = (value == 1);
            SendFeatuere(1047, pAimbotPlayer.downkillv2);
            break;

        case 777781:
            MasterBool.SilentGhostPullV2_Enable = (value == 1);
            SendFeatuere(777781, MasterBool.SilentGhostPullV2_Enable);
            break;

        case 59910:
            MasterBool.teleCombo = (value == 1);
            SendFeatuere(59910, MasterBool.teleCombo);
            break;
        case 6010:
            MasterBool.resetGuest = (value == 1);
            SendFeatuere(6010, MasterBool.resetGuest);
            break;
    }
}

struct EspDrawEntry {
    PlayerData data;
    Rect rect;
    float boxCenterX;
    float boxTopY;
    float boxH;
    bool isCaido;
    float scale;
    Color espColor;
};

static void ParseEspPlayerName(const char *raw, char *outName, size_t outNameSize) {
    if (outName && outNameSize > 0) outName[0] = '\0';
    if (raw == nullptr || raw[0] == '\0') return;

    const char *tab1 = strchr(raw, '\t');
    if (tab1 == nullptr) {
        strncpy(outName, raw, outNameSize - 1);
        outName[outNameSize - 1] = '\0';
        return;
    }

    size_t nameLen = (size_t)(tab1 - raw);
    if (nameLen > 0 && nameLen < outNameSize) {
        strncpy(outName, raw, nameLen);
        outName[nameLen] = '\0';
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_kanishkcheat_Menu_OnDrawLoad(JNIEnv *env, jclass clazz, jobject draw_view, jobject canvas) {
    DrawView draw = DrawView(env, draw_view, canvas);

    if (draw.isValid()) {
        const float screenW = (float)draw.getWidth();
        const float screenH = (float)draw.getHeight();
        if (screenW <= 0.0f || screenH <= 0.0f) return;
        Response response = getData((int)screenW, (int)screenH);

        if (!response.Success) {
            return;
        }

        std::vector<EspDrawEntry> entries;

        const int playerCount = std::max(0, std::min(response.PlayerCount, maxplayerCount));
        for (int i = 0; i < playerCount; ++i) {
            PlayerData data = response.Players[i];
            Vector3 HeadLoc = data.headPosition;
            Vector3 PesLoc = data.bottomPlayerPosition;

            if (!std::isfinite(data.distance) || data.distance <= 0 || data.distance > 280) continue;
            if (!std::isfinite(data.health) || data.health <= 0) continue;
            if (!std::isfinite(HeadLoc.X) || !std::isfinite(HeadLoc.Y) || !std::isfinite(HeadLoc.Z)
                || !std::isfinite(PesLoc.X) || !std::isfinite(PesLoc.Y) || !std::isfinite(PesLoc.Z)) continue;
            if (HeadLoc.Z < 0.1f || PesLoc.Z < 0.1f) continue;

            float distance = data.distance;
            float scale = std::max(0.5f, std::min(1.0f, 500.0f / distance));
            float headScreenY = screenH - HeadLoc.Y;
            float rootScreenY = screenH - PesLoc.Y;
            float rawHeight = fabsf(rootScreenY - headScreenY);
            if (rawHeight < 5.0f || rawHeight > 1000.0f) continue;

            float boxHeight = rawHeight * 1.12f;
            float boxWidth = rawHeight * 0.58f;
            float boxY = std::min(headScreenY, rootScreenY) - (rawHeight * 0.06f);
            float boxCenterX = (HeadLoc.X + PesLoc.X) * 0.5f;
            float boxX = boxCenterX - (boxWidth * 0.5f);
            float boxW = boxWidth;
            float boxH = boxHeight;

            if (boxX >= screenW || boxX + boxW <= 0
                || boxY >= screenH || boxY + boxH <= 0) continue;

            EspDrawEntry entry;
            entry.data = data;
            entry.rect = Rect(boxX, boxY, boxW, boxH);
            entry.boxCenterX = boxCenterX;
            entry.boxTopY = boxY;
            entry.boxH = boxH;
            entry.isCaido = data.isDieing;
            entry.scale = scale;
            entry.espColor = entry.isCaido ? Color::Red() : pEspPlayer.espColor;
            entries.push_back(entry);
        }

        if (pEspPlayer.espDrawFov) {
            // screen dimensions already cached above

            float centerX = screenW / 2.0f;
            float centerY = screenH / 2.0f;
            float fullScreenRadius = std::max(screenW, screenH) / 2.0f;

            float currentFov = pAimbotPlayer.aimbotFOV;
            if (currentFov <= 360.0f || currentFov <= 0.0f) {
                currentFov = fullScreenRadius;
                pAimbotPlayer.aimbotFOV = fullScreenRadius;
                SendFOV(104, (int)fullScreenRadius);
            }
            draw.DrawCircle(pEspPlayer.espColor, 1.5f, Vector2(centerX, centerY), currentFov);
        }

        for (const EspDrawEntry &entry : entries) {
            const PlayerData &data = entry.data;
            const Rect &PlayerRect = entry.rect;
            const float boxCenterX = entry.boxCenterX;
            const float boxTopY = entry.boxTopY;
            const float boxH = entry.boxH;
            const float boxY = PlayerRect.y;
            const float boxW = PlayerRect.w;
            const bool IsCaido = entry.isCaido;
            const float scale = entry.scale;
            Color espColor = entry.espColor;

            if (pEspPlayer.espBox) {
                draw.DrawCornerBox(espColor, 1.0f, PlayerRect, 2, 2);
            }

            char playerNameBuf[256] = {0};
            ParseEspPlayerName(data.name, playerNameBuf, sizeof(playerNameBuf));

            const float healthBarGap = 3.0f;
            const float healthBarW = std::max(6.0f, 7.0f * scale);

            if (pEspPlayer.espHealth && !IsCaido) {
                float healthBarX = PlayerRect.x - healthBarGap - healthBarW;
                draw.DrawVerticalHealthBar(Vector2(healthBarX, boxY), boxH, 200.0f, data.health);
            }

            float fontSize = 11.0f * scale;
            if (pEspPlayer.espNickName && !IsCaido && playerNameBuf[0] != '\0') {
                float namePosY = boxY + boxH + 12.0f * scale;
                draw.DrawTextCenter(Color(0, 0, 0, 255), playerNameBuf, boxCenterX, namePosY - 1.f, fontSize);
                draw.DrawTextCenter(Color(0, 0, 0, 255), playerNameBuf, boxCenterX, namePosY + 1.f, fontSize);
                draw.DrawTextCenter(Color(255, 255, 255, 255), playerNameBuf, boxCenterX, namePosY, fontSize);
            }

            if (pEspPlayer.espLine) {
                Vector2 lineStart(screenW / 2.0f, 0.0f);
                Vector2 lineEnd(boxCenterX, boxTopY);
                draw.DrawLine(espColor, 1.0f, lineStart, lineEnd);
            }
        }
    }
}
