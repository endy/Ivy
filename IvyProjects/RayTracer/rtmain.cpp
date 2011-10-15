


#include "RTApp.h"

int main()
{
    RTAppCreateInfo appCreateInfo;
    memset(&appCreateInfo, 0, sizeof(RTAppCreateInfo));

    appCreateInfo.screenWidth = 512;
    appCreateInfo.screenHeight = 512;
    appCreateInfo.nearZ = 0.0f;
    appCreateInfo.farZ = 1.0f;

    RTApp* app = RTApp::Create(&appCreateInfo);

    if (app != NULL)
    {
        app->Run();
    }

    app->Destroy();
    app = NULL;

	return 0;
}
