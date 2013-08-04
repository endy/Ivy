///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Generic OpenGL Test App
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#include "GLTestApp.h"
#include "IvyConfig.h"

using namespace Ivy;

int main(unsigned int argc, const char** argv)
{
    IvyApp::Configure(argc, argv);

    GLTestApp* app = GLTestApp::Create();

    if (app != NULL)
    {
        app->Run();
    }

    app->Destroy();
    app = NULL;

    return 0;
}



