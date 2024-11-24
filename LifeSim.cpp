#include <ServiceContainer.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "source/Application.h"


#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    fprintf(stdout, "WinMain(): Booting Application...\n");

    auto app = Container::Bind<Application>();

    app->Run();

    app.reset();
    delete app.get();

    return 0;
}
#endif

int main(int, char**)
{
    fprintf(stdout, "main(): Booting Application...\n");

    auto app = Container::Bind<Application>();

    app->Run();

    app.reset();
    delete app.get();

    return 0;
}
