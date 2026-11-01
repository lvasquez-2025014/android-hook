#pragma once

#include <chrono>

SocketClient client;

int startClient();
bool isConnected();
void stopClient();
bool initServer();
bool stopServer();

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

static bool ensureClientConnected();

int startClient() {
    return ensureClientConnected() ? 0 : -1;
}

bool isConnected() {
    return client.connected;
}

void stopClient() {
    if (client.connected) {
        stopServer();
        client.Close();
    }
}

bool initServer() {
    Request request{Mode::InitMode, true, 0, 0, 0, 0};
    if (!client.send(&request, sizeof(request))) return false;

    Response response{};
    return client.receive(&response, sizeof(response)) == sizeof(response) && response.Success;
}

static bool ensureClientConnected() {
    if (client.connected) return true;

    static std::chrono::steady_clock::time_point nextAttempt{};
    const auto now = std::chrono::steady_clock::now();
    if (now < nextAttempt) return false;
    nextAttempt = now + std::chrono::seconds(1);

    client.Close();
    if (!client.Create() || !client.Connect()) return false;
    if (!initServer()) {
        client.Close();
        return false;
    }
    LOGI("Start Client");
    return true;
}

bool stopServer() {
    if (!client.connected) return false;

    Request request{Mode::StopMode, false, 0, 0, 0, 0};
    if (!client.send(&request, sizeof(request))) return false;

    Response response{};
    return client.receive(&response, sizeof(response)) == sizeof(response) && response.Success;
}

bool SendFeatuere(int32_t func, bool ftr) {
    if (!ensureClientConnected()) return false;

    Request request{func, ftr, 0, 0, 0, 0};
    if (!client.send(&request, sizeof(request))) return false;

    Response response{};
    return client.receive(&response, sizeof(response)) == sizeof(response) && response.Success;
}

bool SendFOV(int32_t func, int value) {
    if (!ensureClientConnected()) return false;

    Request request{func, false, value, 0, 0, 0};
    if (!client.send(&request, sizeof(request))) return false;

    Response response{};
    return client.receive(&response, sizeof(response)) == sizeof(response) && response.Success;
}

Response getData(int screenWidth, int screenHeight) {
    Response response{};
    response.Success = false;
    if (!ensureClientConnected()) return response;

    Request request{Mode::EspMode, false, 0, screenWidth, screenHeight, 0};
    if (!client.send(&request, sizeof(request))) return response;
    if (client.receive(&response, sizeof(response)) != sizeof(response)) {
        client.Close();
        response.Success = false;
    }
    return response;
}
