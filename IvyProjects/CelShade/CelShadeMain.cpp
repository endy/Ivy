///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Cel Shading Demo
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "IvyInput.h"
#include "CelShadeApp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// main
///////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    input();

    CelShadeAppCreateInfo appCreateInfo;
    memset(&appCreateInfo, 0, sizeof(CelShadeAppCreateInfo));

    appCreateInfo.screenWidth = 800;
    appCreateInfo.screenHeight = 800;
    appCreateInfo.nearZ = 1.0f;
    appCreateInfo.farZ = 200.0f;

    CelShadeApp* app = CelShadeApp::Create(&appCreateInfo);

    if (app != NULL)
    {
        app->Run();
    }

    app->Destroy();
    app = NULL;

    return 0;
}

