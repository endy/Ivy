

#include "CDApp.h"

int main()
{
    CDAppCreateInfo appCreateInfo;
    memset(&appCreateInfo, 0, sizeof(DxAppCreateInfo));

    appCreateInfo.screenWidth = 800;
    appCreateInfo.screenHeight = 800;
    appCreateInfo.nearZ = 1.0f;
    appCreateInfo.farZ = 20.0f;

    CDApp* app = CDApp::Create(&appCreateInfo);

    if (app != NULL)
    {
        app->Run();
    }

    app->Destroy();
    app = NULL;

	return 0;
}



