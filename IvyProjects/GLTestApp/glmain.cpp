///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Generic OpenGL Test App
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#include "IvyInput.h"
#include "GLTestApp.h"

int main()
{
    GLTestAppCreateInfo appCreateInfo;
    memset(&appCreateInfo, 0, sizeof(GLTestAppCreateInfo));

    appCreateInfo.screenWidth = 800;
    appCreateInfo.screenHeight = 450;
    appCreateInfo.nearZ = 1.0f;
    appCreateInfo.farZ = 20.0f;

    GLTestApp* app = GLTestApp::Create(&appCreateInfo);

    if (app != NULL)
    {
        app->Run();
    }

    app->Destroy();
    app = NULL;

	return 0;
}



