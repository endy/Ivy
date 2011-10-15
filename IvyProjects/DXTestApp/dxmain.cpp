///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Generic DirectX Test App
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#include "IvyInput.h"
#include "DxTestApp.h"

int main()
{
    input();
    
    DxTestAppCreateInfo appCreateInfo;
    memset(&appCreateInfo, 0, sizeof(DxTestAppCreateInfo));

    appCreateInfo.screenWidth = 1600;
    appCreateInfo.screenHeight = 900;
    appCreateInfo.nearZ = 1.0f;
    appCreateInfo.farZ = 20.0f;

    DxTestApp* app = DxTestApp::Create(&appCreateInfo);

    if (app != NULL)
    {
        app->Run();
    }

    app->Destroy();
    app = NULL;

	return 0;
}



