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
    // std::print("main(): Booting Application...\n");
    fprintf(stdout, std::format("{}: Booting Application...\n", "main()").c_str());

    auto app = Container::Bind<Application>();

    app->Run();

    app.reset();
    delete app.get();

    return 0;
}
