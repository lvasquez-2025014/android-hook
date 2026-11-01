#ifndef ANDROID_MOD_MENU_MACROS_H
#define ANDROID_MOD_MENU_MACROS_H

#include "dobby.h"

#define HOOK(offset, ptr, orig) DobbyHook((void *)getAbsoluteAddress(targetLibName, string2Offset(OBFUSCATE(offset))), (void *)ptr, (void **)&orig)
#define HOOK_NO_ORIG(offset, ptr) DobbyHook((void *)getAbsoluteAddress(targetLibName, string2Offset(OBFUSCATE(offset))), (void *)ptr, NULL)

#endif //ANDROID_MOD_MENU_MACROS_H


