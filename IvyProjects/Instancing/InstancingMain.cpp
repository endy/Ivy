///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Instancing Demo
///
///     Copyright 2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "IvyInput.h"
#include "InstancingApp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// main
///////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    input();

    InstancingAppCreateInfo appCreateInfo;
    memset(&appCreateInfo, 0, sizeof(InstancingAppCreateInfo));

    appCreateInfo.screenWidth = 800;
    appCreateInfo.screenHeight = 800;
    appCreateInfo.nearZ = 1.0f;
    appCreateInfo.farZ = 200.0f;

    InstancingApp* app = InstancingApp::Create(&appCreateInfo);

    if (app != NULL)
    {
        app->Run();
    }

    app->Destroy();
    app = NULL;

    return 0;
}

