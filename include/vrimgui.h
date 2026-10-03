#pragma once

#include <jni.h>
#include <android/input.h>
#include <android/native_window.h>

#ifdef __cplusplus
extern "C" {
#endif

void vrimgui_init(ANativeWindow* window);
void vrimgui_shutdown();
void vrimgui_new_frame();
void vrimgui_render();

bool vrimgui_handle_input(AInputEvent* event);
void vrimgui_set_menu_open(bool open);
bool vrimgui_is_menu_open();

#ifdef __cplusplus
}
#endif
