#include <iostream>
#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>
#include <GLFW/glfw3.h>

#include "Common.h"
#include "Renderer.h"


static void glfw_error_callback(const int error, const char *description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

template<ShapeType T>
class Shape;

template<>
class Shape<ShapeType::Circle> {
public:
    static void draw(ImDrawList *draw_list, const ImVec2 &center, float radius, ImU32 color) {
        draw_list->AddCircleFilled(center, radius, color);
    }
};

template<>
class Shape<ShapeType::Rectangle> {
public:
    static void draw(ImDrawList *draw_list, const ImVec2 &topLeft, const ImVec2 &bottomRight, ImU32 color) {
        draw_list->AddRectFilled(topLeft, bottomRight, color);
    }
};

template<>
class Shape<ShapeType::Triangle> {
public:
    static void draw(ImDrawList *draw_list, const ImVec2 &p1, const ImVec2 &p2, const ImVec2 &p3, ImU32 color) {
        draw_list->AddTriangleFilled(p1, p2, p3, color);
    }
};

Renderer::Renderer() {
    std::cout << "Renderer()" << std::endl;

    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    // Create window with graphics context
    window = glfwCreateWindow(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y), "LifeSim", nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
    } else {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        std::cout << "Created GLFW window: " << width << "x" << height << std::endl;
    }

    IM_ASSERT(window != nullptr);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    if (GLenum err = glewInit(); GLEW_OK != err) {
        fprintf(stderr, "Error: %p\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %p\n", glewGetString(GLEW_VERSION));

    io = &ImGui::GetIO();
    (void) io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding              = window_rounding;
        style.PopupRounding               = popupRounding;
        style.Colors[ImGuiCol_WindowBg].w = windowBgAlpha;
        style.WindowMinSize               = windowMinSize;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);

    // #if(!MSVC)
    //     if (fopen_s(nullptr, "../assets/fonts/Roboto-Regular.ttf", "r") != 0) {
    //         std::cout << "Loading Roboto Font..." << std::endl;
    //         const ImFont *font = io->Fonts->AddFontFromFileTTF("../assets/fonts/Roboto-Regular.ttf", 14.0f, nullptr, io->Fonts->GetGlyphRangesDefault());
    //         IM_ASSERT(font != nullptr);
    //     }
    // #endif
}

Renderer::~Renderer() {
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    io     = nullptr;
    window = nullptr;
}

void Renderer::prerender() const {
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Renderer::render() const {
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    // If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!),
    // you may need to backup/reset/restore other state, e.g. for current shader using the commented lines below.
    //GLint last_program;
    //glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    //glUseProgram(0);
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    //glUseProgram(last_program);

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
}

ImVec2 Renderer::ScreenToViewport(const ImVec2 &screen_coords) {
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    const ImVec2 viewport_pos     = viewport->Pos;
    const ImVec2 viewport_size    = viewport->Size;

    ImVec2 windowSize{ 1920, 1080 }; // TODO: Figure out how to grab this from the Renderer instance, or make it static

    const auto viewport_coords   = ImVec2(screen_coords.x + viewport_pos.x, screen_coords.y + viewport_pos.y);
    const auto normalized_coords = ImVec2(viewport_coords.x * (windowSize.x / viewport_size.x), viewport_coords.y * (windowSize.y / viewport_size.y));

    return normalized_coords;
}

void Renderer::drawCircle(const ImVec2 &center, float radius, ImU32 color) {
    Shape<ShapeType::Circle>::draw(ImGui::GetWindowDrawList(), center, radius, color);
}

void Renderer::drawRectangle(const ImVec2 &topLeft, const ImVec2 &bottomRight, ImU32 color) {
    Shape<ShapeType::Rectangle>::draw(ImGui::GetWindowDrawList(), topLeft, bottomRight, color);
}

void Renderer::drawTriangle(const ImVec2 &p1, const ImVec2 &p2, const ImVec2 &p3, ImU32 color) {
    Shape<ShapeType::Triangle>::draw(ImGui::GetWindowDrawList(), p1, p2, p3, color);
}

void Renderer::drawShape(ShapeType shape, ImVec2 position, int size, ImU32 color) {
    switch (shape) {
        case ShapeType::Rectangle:
            drawRectangle(ScreenToViewport(position), ScreenToViewport(ImVec2(position.x + size, position.y + size)), color);
            break;
        case ShapeType::Circle:
            drawCircle(ScreenToViewport(ImVec2(position.x + size / 2, position.y + size / 2)), size / 2, color);
            break;
        case ShapeType::Triangle:
            drawTriangle(ScreenToViewport(position), ScreenToViewport(ImVec2(position.x + size, position.y)), ScreenToViewport(ImVec2(position.x + size / 2, position.y + size)), color);
            break;
        default:
            HandleError(ExitCode::INVALID_SHAPE_TYPE);
            break;
    }
}

void Renderer::drawGui() {
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f     = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);                              // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", reinterpret_cast<float*>(&clear_color)); // Edit 3 floats representing a color

        if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window) {
        ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
}
