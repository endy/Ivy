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

int main(unsigned int argc, const char** argv)
{
    Ivy::IvyApp::Configure(argc, argv);

    DxTestApp* app = DxTestApp::Create();

    if (app != NULL)
    {
        app->Run();
    }

    app->Destroy();
    app = NULL;

    return 0;
}



