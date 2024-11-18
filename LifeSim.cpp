#include <ServiceContainer.h>
#include <windows.h>

#include "source/Application.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    fprintf(stdout, "WinMain(): Booting Application...\n");

    auto app = Container::Bind<Application>();

    app->Run();

    app.reset();
    delete app.get();

    return 0;
}

int main(int, char**)
{
    fprintf(stdout, "main(): Booting Application...\n");

    auto app = Container::Bind<Application>();

    app->Run();

    app.reset();
    delete app.get();

    return 0;
}
