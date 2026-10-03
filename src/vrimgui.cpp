#include "vrimgui.h"
#include "imgui.h"
#include "backends/imgui_impl_android.h"
#include "backends/imgui_impl_opengl3.h"

#include <android/native_window.h>
#include <android/log.h>
#include <GLES3/gl3.h>

static bool g_initialized = false;
static bool g_menu_open = true;
static float g_scale = 1.0f;
static bool g_debug = false;

static void DrawMenu()
{
    if (!g_menu_open)
        return;

    ImGui::SetNextWindowSize(ImVec2(520.0f, 360.0f), ImGuiCond_FirstUseEver);
    ImGui::Begin("VR Native Menu", &g_menu_open,
                 ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Dear ImGui native Android/OpenGL ES 3");
    ImGui::Separator();

    if (ImGui::BeginTabBar("MainTabs"))
    {
        if (ImGui::BeginTabItem("Interface"))
        {
            ImGui::Checkbox("Debug overlay", &g_debug);
            ImGui::SliderFloat("UI scale", &g_scale, 0.75f, 2.0f, "%.2fx");
            ImGui::TextWrapped(
                "This menu exposes controls through the host application's "
                "native integration layer."
            );
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Diagnostics"))
        {
            ImGuiIO& io = ImGui::GetIO();
            ImGui::Text("FPS: %.1f", io.Framerate);
            ImGui::Text("Frame time: %.2f ms",
                        io.Framerate > 0.0f ? 1000.0f / io.Framerate : 0.0f);
            ImGui::Text("Display: %.0fx%.0f",
                        io.DisplaySize.x, io.DisplaySize.y);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

extern "C" JNIEXPORT void JNICALL
vrimgui_init(ANativeWindow* window)
{
    if (g_initialized || window == nullptr)
        return;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    if (!ImGui_ImplAndroid_Init(window))
    {
        __android_log_print(ANDROID_LOG_ERROR, "VRImGui",
                            "ImGui Android backend initialization failed");
        ImGui::DestroyContext();
        return;
    }

    if (!ImGui_ImplOpenGL3_Init("#version 300 es"))
    {
        __android_log_print(ANDROID_LOG_ERROR, "VRImGui",
                            "ImGui OpenGL ES 3 backend initialization failed");
        ImGui_ImplAndroid_Shutdown();
        ImGui::DestroyContext();
        return;
    }

    g_initialized = true;
}

extern "C" void vrimgui_shutdown()
{
    if (!g_initialized)
        return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
    g_initialized = false;
}

extern "C" void vrimgui_new_frame()
{
    if (!g_initialized)
        return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    if (g_scale != 1.0f)
        ImGui::GetStyle().ScaleAllSizes(g_scale);

    DrawMenu();

    ImGui::Render();
}

extern "C" void vrimgui_render()
{
    if (!g_initialized)
        return;

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

extern "C" bool vrimgui_handle_input(AInputEvent* event)
{
    if (!g_initialized || event == nullptr)
        return false;

    return ImGui_ImplAndroid_HandleInputEvent(event);
}

extern "C" void vrimgui_set_menu_open(bool open)
{
    g_menu_open = open;
}

extern "C" bool vrimgui_is_menu_open()
{
    return g_menu_open;
}
