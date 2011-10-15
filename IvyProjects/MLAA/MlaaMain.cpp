
/**************************************************************************************************
TODO:
0.  Init area texture
1.  Add ability to reload shaders at runtime
2.  Add perf stats to display
3.  Add pass info to display

**************************************************************************************************/


#include "IvyInput.h"
#include "MlaaApp.h"

int main()
{
    input();
    
    MlaaAppCreateInfo appCreateInfo;
    memset(&appCreateInfo, 0, sizeof(MlaaAppCreateInfo));

    appCreateInfo.screenWidth = 900;
    appCreateInfo.screenHeight = 600;
    appCreateInfo.nearZ = 1.0f;
    appCreateInfo.farZ = 20.0f;

    MlaaApp* app = MlaaApp::Create(&appCreateInfo);

    if (app != NULL)
    {
        app->Run();
    }

    app->Destroy();
    app = NULL;

	return 0;
}