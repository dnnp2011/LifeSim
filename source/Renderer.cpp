// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "ServiceContainer.h"


//region OpenGL Error Handling -------------------------

#define ASSERT (x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); \
x; \
GLLogCall(#x, __FILE__, __LINE__)

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static void GLLogCall(const char* function, const char* file, const int line)
{
    while (const GLenum error{ glGetError() }) {
        fprintf(stderr, "[OpenGL Error] (%u): %s %s:%d\n", error, function, file, line);
    }
}

//endregion --------------------------------------------

template<ShapeType T>
class Shape;

template<>
class Shape<ShapeType::Circle> {
public:
    static void draw(ImDrawList* draw_list, const ImVec2& center, const float radius, const ImU32 color)
    {
        draw_list->AddCircleFilled(center, radius, color);
    }
};

template<>
class Shape<ShapeType::Rectangle> {
public:
    static void draw(ImDrawList* draw_list, const ImVec2& topLeft, const ImVec2& bottomRight, const ImU32 color)
    {
        draw_list->AddRectFilled(topLeft, bottomRight, color, 0.5f);
    }
};

template<>
class Shape<ShapeType::Triangle> {
public:
    static void draw(ImDrawList* draw_list, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImU32 color)
    {
        draw_list->AddTriangleFilled(p1, p2, p3, color);
    }
};

Renderer::Renderer()
{
    fprintf(stdout, "Instantiating Renderer...\n");

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    // Create window with graphics context
    m_Window = glfwCreateWindow(
        static_cast<int>(m_windowSize.x),
        static_cast<int>(m_windowSize.y),
        "LifeSim",
        nullptr,
        nullptr
    );

    if (m_Window == nullptr) {
        fprintf(stderr, "Failed to create GLFW window\n");
    }
    else {
        glfwGetFramebufferSize(
            m_Window,
            reinterpret_cast<int*>(&m_BufferWidth),
            reinterpret_cast<int*>(&m_BufferHeight)
        );
        glfwGetWindowSize(m_Window, reinterpret_cast<int*>(&m_WindowWidth), reinterpret_cast<int*>(&m_WindowHeight));
        glfwGetWindowPos(m_Window, reinterpret_cast<int*>(&m_WindowXOffset), reinterpret_cast<int*>(&m_WindowYOffset));
        glfwSetFramebufferSizeCallback(m_Window, framebufferSizeCallback);
        glfwSetWindowRefreshCallback(m_Window, windowRefreshCallback);
        glfwSetWindowPosCallback(m_Window, windowPosCallback);
        glfwSetWindowSizeCallback(m_Window, windowSizeCallback);

        fprintf(stdout, "Created GLFW window: { %u, %u }\n", m_WindowWidth, m_WindowHeight);
    }

    IM_ASSERT(m_Window != nullptr);

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    if (const auto err{ glewInit() }; GLEW_OK != err)
        fprintf(stderr, "Error: %p\n", glewGetErrorString(err));

    fprintf(stdout, "Status: Using GLEW %s\n", reinterpret_cast<const char*>(glewGetString(GLEW_VERSION)));
    fprintf(stdout, "Status: Using OpenGL %s\n", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    m_IO = &ImGui::GetIO();
    (void)m_IO;
    // m_IO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // m_IO->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    m_IO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
    m_IO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style{ ImGui::GetStyle() };
    if (m_IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding              = m_windowRounding;
        style.PopupRounding               = m_popupRounding;
        style.Colors[ImGuiCol_WindowBg].w = m_windowBgAlpha;
        style.WindowMinSize               = m_windowMinSize;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init();

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
    //         fprintf(stdout, "Loading Roboto Font...\n");
    //         const ImFont *font = io->Fonts->AddFontFromFileTTF("../assets/fonts/Roboto-Regular.ttf", 14.0f, nullptr, io->Fonts->GetGlyphRangesDefault());
    //         IM_ASSERT(font != nullptr);
    //     }
    // #endif

    fprintf(stdout, "Renderer Instantiated\n");
}

Renderer::~Renderer()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_Window);
    glfwTerminate();

    m_IO     = nullptr;
    m_Window = nullptr;
}

void Renderer::NewFrame() const
{
    glDebugMessageCallback(
        [](
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam
) {
            fprintf(
                stderr,
                "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
                (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
                type,
                severity,
                message
            );
        },
        nullptr
    );
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Renderer::Draw() const
{
    // Rendering
    ImGui::Render();

    GLCall(
        glClearColor(
            m_backgroundColor.x * m_backgroundColor.w,
            m_backgroundColor.y * m_backgroundColor.w,
            m_backgroundColor.z * m_backgroundColor.w,
            m_backgroundColor.w
        )
    );
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    // TODO: Abstract this stage into a separate method to draw directly with OpenGL after prepping ImGui and getting OpenGL ready to render
    // Top Left -> Top Right -> Bottom Right -> Bottom Left
    GLCall(glBegin(GL_QUADS));
    // RED
    GLCall(glColor3f(1.0f, 0.0f, 0.0f));
    GLCall(glVertex2f(-0.5, 0.5f));
    // GREEN
    GLCall(glColor3f(0.0f, 1.0f, 0.0f));
    GLCall(glVertex2f(0.5f, 0.5f));
    // BLUE
    GLCall(glColor3f(0.0f, 0.0f, 1.0f));
    GLCall(glVertex2f(0.5f, -0.5f));
    // WHITE
    GLCall(glColor3f(1.0f, 1.0f, 1.0f));
    GLCall(glVertex2f(-0.5f, -0.5f));
    glEnd(); // Throws GL_INVALID_OPERATION, but renders black screen if not called ¯\_(ツ)_/¯

    // If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!),
    // you may need to backup/reset/restore other state, e.g. for current shader using the commented lines below.
    //GLint last_program;
    //glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    //glUseProgram(0);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    //glUseProgram(last_program);

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (m_IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context{ glfwGetCurrentContext() };
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapBuffers(m_Window);
}

ImVec2 Renderer::ScreenToViewport(const ImVec2& screen_coords) const
{
    #if (DEBUG && 0)
    Debounce(
        [&](const std::string& id) {
            static const ImGuiViewport* viewport{ ImGui::GetMainViewport() };
            static ImVec2 viewport_pos{ viewport->Pos };
            static ImVec2 viewport_size{ viewport->Size };

            fprintf(
                stdout,
                R"(
%s ----------------------------
Viewport Position: { %.1f, %.1f }
Viewport Size: { %.1f, %.1f }
Screen Size: { %d, %d }
Window Size: { %d, %d }
Viewport Offset: { %d, %d }
)",
                id.c_str(),
                viewport_pos.x,
                viewport_pos.y,
                viewport_size.x,
                viewport_size.y,
                screenHeight,
                screenWidth,
                windowHeight,
                windowWidth,
                xOffset,
                yOffset
            );
        },
        10000,
        "ScreenToViewport"
    );
    #endif

    // TODO: Update existing screen_coords instead of creating a new Vec2
    const ImVec2 normalized_coords{
        screen_coords.x * (m_WindowWidth / m_BufferWidth) + m_WindowXOffset,
        screen_coords.y * (m_WindowHeight / m_BufferHeight) + m_WindowYOffset
    };

    return normalized_coords;
}

void Renderer::DrawRect(const ImVec2& topLeft, const ImVec2& bottomRight, const ImU32 color)
{
    Shape<ShapeType::Rectangle>::draw(
        ImGui::GetWindowDrawList(),
        ScreenToViewport(topLeft),
        ScreenToViewport(bottomRight),
        color
    );
}

void Renderer::DrawCircle(const ImVec2& center, const float& radius, const ImU32 color)
{
    Shape<ShapeType::Circle>::draw(ImGui::GetWindowDrawList(), ScreenToViewport(center), radius, color);
}

void Renderer::DrawTriangle(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImU32 color)
{
    Shape<ShapeType::Triangle>::draw(
        ImGui::GetWindowDrawList(),
        ScreenToViewport(p1),
        ScreenToViewport(p2),
        ScreenToViewport(p3),
        color
    );
}

void Renderer::drawGui()
{
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (m_showDemoWindow)
        ImGui::ShowDemoWindow(&m_showDemoWindow);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f{ 0.0f };
        static int counter{ 0 };

        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &m_showDemoWindow); // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &m_showAnotherWindow);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", reinterpret_cast<float*>(&m_backgroundColor));
        // Edit 3 floats representing a color

        // Buttons return true when clicked (most widgets return true when edited/activated)
        if (ImGui::Button("Button"))
            counter++;

        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_IO->Framerate, m_IO->Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (m_showAnotherWindow) {
        ImGui::Begin("Another Window", &m_showAnotherWindow);
        // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            m_showAnotherWindow = false;
        ImGui::End();
    }
}

void Renderer::framebufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
    fprintf(stdout, "Framebuffer Resized: %d x %d\n", width, height);
    glViewport(0, 0, width, height);

    {
        const auto renderer = Container::Resolve<Renderer>();

        renderer->m_BufferHeight = height;
        renderer->m_BufferWidth  = width;
    }
}

void Renderer::windowPosCallback(GLFWwindow* window, const int x, const int y)
{
    fprintf(stdout, "Window Moved: %d, %d\n", x, y);

    {
        const auto renderer = Container::Resolve<Renderer>();

        renderer->m_WindowXOffset = x;
        renderer->m_WindowYOffset = y;
    }
}

void Renderer::windowSizeCallback(GLFWwindow* window, const int width, const int height)
{
    fprintf(stdout, "Window Resized: %d x %d\n", width, height);

    {
        const auto renderer = Container::Resolve<Renderer>();

        renderer->m_WindowHeight = height;
        renderer->m_WindowWidth  = width;
    }
}

void Renderer::windowRefreshCallback(GLFWwindow* window)
{
    // NOTE: This is a workaround for the issue where the window doesn't refresh when minimized
    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
        fprintf(stdout, "Restore Window\n");
        glfwRestoreWindow(window);
    }
}

void Renderer::errorCallback(const int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
