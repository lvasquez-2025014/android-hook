LOCAL_PATH := $(call my-dir)
MAIN_LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libdobby
LOCAL_SRC_FILES := Tools/Dobby/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := amithnew
LOCAL_CFLAGS := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w
LOCAL_CFLAGS += -fno-rtti -fno-exceptions -fpermissive
LOCAL_CPPFLAGS := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w -Werror -s -std=c++17
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fms-extensions -fno-rtti -fno-exceptions -fpermissive
LOCAL_LDFLAGS += -Wl,--gc-sections,--strip-all, -llog
LOCAL_ARM_MODE := arm

LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH)

LOCAL_SRC_FILES := client.cpp \
	Tools/SOCKET/client.cpp \
	Hack/Namegun.cpp \
	NativeGuard.cpp

LOCAL_LDLIBS := -llog -landroid -lGLESv2
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := amithcore

LOCAL_CFLAGS := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w -DkNO_KEYSTONE
LOCAL_CFLAGS += -fno-rtti -fno-exceptions -fpermissive
LOCAL_CPPFLAGS := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w -Werror -s -std=c++17 -DkNO_KEYSTONE
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fms-extensions -fno-rtti -fno-exceptions -fpermissive
LOCAL_LDFLAGS += -Wl,--gc-sections,--strip-all, -llog
LOCAL_ARM_MODE := arm

LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH) \
                    $(MAIN_LOCAL_PATH)/AutoUpdate/xdl/include

LOCAL_SRC_FILES := EliteMain.cpp \
   Hack/Namegun.cpp \
   AutoUpdate/Il2Cpp.cpp \
   AutoUpdate/xdl/xdl.c \
   AutoUpdate/xdl/xdl_iterate.c \
   AutoUpdate/xdl/xdl_linker.c \
   AutoUpdate/xdl/xdl_util.c \
   AutoUpdate/xdl/xdl_lzma.c \
   Tools/KittyMemory/KittyMemory.cpp \
   Tools/KittyMemory/KittyUtils.cpp \
   Tools/KittyMemory/MemoryBackup.cpp \
   Tools/KittyMemory/MemoryPatch.cpp \
   Tools/SOCKET/server.cpp

LOCAL_LDLIBS := -llog -landroid -lGLESv2
LOCAL_STATIC_LIBRARIES := libdobby
include $(BUILD_SHARED_LIBRARY)







