#include <windows.h>

#include "source/Application.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    fprintf(stdout, R"(Running WinMain())");
    MessageBox(nullptr, "Running LifeSim from WinMain", "WinMain Demo", MB_OK);

    auto app = Application::Init();

    app->Run();

    app.reset();
    delete app.get();

    return 0;
}

int main(int, char**)
{
    fprintf(stdout, R"(Running main())");

    auto app = Application::Init();

    app->Run();

    app.reset();
    delete app.get();

    return 0;
}
