LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
LOCAL_LDFLAGS := libvirtualsim.a
LOCAL_MODULE:= vsim
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)\softsim\  $(LOCAL_PATH)
LOCAL_SRC_FILES := \
softsim_jni.c \
softsim/softsim_demo.c \
softsim/softsim_release.c \
softsim/softsim_version.c \
softsim/softsim_system.c \
softsim/softsim_timer.c \
softsim/softsim_message.c \
softsim/softsim_fs_linux.c \
softsim/softsim_socket_linux.c \
softsim/softsim_socket_dns_linux.c \
softsim/softsim_socket.c \
softsim/softsim_public_api.c \
softsim/softsim_trace.c \
softsim/softsim_main.c \
softsim/softsim_test.c \
softsim/softsim_uicc.c \
softsim/softsim_session.c \
softsim/softsim_authentication.c \
softsim/softsim_heartbeat.c \
softsim/softsim_monitor.c \
softsim/softsim_exception.c \
softsim/softsim_console.c \
softsim/softsim_driver.c \
softsim/softsim_iso_7816.c \
softsim/softsim_cos_api.c \
softsim/softsim_auth_demo.c \

include $(BUILD_SHARED_LIBRARY)
