// PCK++ by May/MattNL :3

#include "Application/Application.h"
#include "Application/Layer.h"
#include "UI/UILayer.h"
#include <SDL3/SDL.h>

int main(int argc, const char **argv)
{
    Application app;
    ApplicationSpecification specs;
    specs.title = "PCK++";
    specs.width = 1280;
    specs.height = 720;
    if (!app.Initialize(specs))
        return 1;

    app.ParseCommandLineArguments(argc, argv);
    app.AddLayer<UILayer>();

    app.Run();

    app.Shutdown();

    return 0;
}