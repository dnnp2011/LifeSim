#include <iostream>
#include <windows.h>

#include "source/Application.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    std::cout << R"(Running WinMain())" << std::endl;
    MessageBox(nullptr, "Running LifeSim from WinMain", "WinMain Demo", MB_OK);

    g_Application.Run();

    return 0;
}

int main(int, char**) {
    std::cout << R"(Running main())" << std::endl;

    g_Application.Run();

    return 0;
}
