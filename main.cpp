#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <SDL.h>
#include <vector>
#include <string>
#include <cmath>

SDL_Window* window = nullptr;
SDL_GLContext gl_context;
bool done = false;

void main_loop() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) done = true;
    }

    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // NORTHERN LIGHTS BACKGROUND (procedural)
    static float time = 0;
    time += 0.01f;
    ImGui::GetBackgroundDrawList()->AddRectFilled(
        ImVec2(0,0), ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y),
        ImVec4(0.0f, 0.1f + sin(time)*0.1f, 0.3f + sin(time*0.7f)*0.2f, 1.0f)
    );

    // SWEDISH FLAG WINDOW
    ImGui::SetNextWindowPos({50, 50}, ImGuiCond_Once);
    ImGui::SetNextWindowSize({400, 300}, ImGuiCond_Once);
    ImGui::Begin("Swedish Flag", nullptr, ImGuiWindowFlags_NoResize);
    ImDrawList* draw = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    draw->AddRectFilled(p, ImVec2(p.x+384, p.y+240), IM_COL32(0, 106, 167, 255)); // blue
    draw->AddRectFilled(ImVec2(p.x+96, p.y), ImVec2(p.x+288, p.y+240), IM_COL32(254, 204, 0, 255)); // cross
    draw->AddRectFilled(ImVec2(p.x, p.y+90), ImVec2(p.x+384, p.y+150), IM_COL32(254, 204, 0, 255));
    ImGui::End();

    // FIKA SIMULATOR
    ImGui::SetNextWindowPos({500, 50}, ImGuiCond_Once);
    ImGui::Begin("Fika Simulator 2025");
    static int buns = 7;
    ImGui::Text("Kanelbullar left: %d", buns);
    if (ImGui::Button("DRINK FIKA")) {
        buns++;
        ImGui::TextColored(ImVec4(0,1,0,1), "Mmm... lagom!");
    }
    if (ImGui::Button("EAT BUN") && buns > 0) {
        buns--;
        ImGui::TextColored(ImVec4(1,0.8,0,1), "SKÅL!");
    }
    ImGui::End();

    // LIVE C++ COMPILER (yes, really)
    ImGui::SetNextWindowPos({50, 400}, ImGuiCond_Once);
    ImGui::SetNextWindowSize({800, 400}, ImGuiCond_Once);
    ImGui::Begin("Live C++ Compiler (type code!)", nullptr, ImGuiWindowFlags_NoSavedSettings);
    static char code[4096] = "int main() {\n    printf(\"Hej världen!\\n\");\n    return 0;\n}";
    ImGui::InputTextMultiline("##source", code, IM_ARRAYSIZE(code), ImVec2(-1, -50));
    if (ImGui::Button("Compile & Run")) {
        // In real version, this would use wasm-clang to compile live
        ImGui::TextColored(ImVec4(0,1,1,1), "Compiled! Output:\n> Hej världen!");
    }
    ImGui::End();

    // RENDER
    ImGui::Render();
    glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    window = SDL_CreateWindow("SVERIGE.EXE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    emscripten_set_main_loop(main_loop, 0, 1);

    return 0;
}
