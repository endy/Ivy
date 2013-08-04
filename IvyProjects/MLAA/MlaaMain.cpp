
/**************************************************************************************************
TODO:
0.  Init area texture
1.  Add ability to reload shaders at runtime
2.  Add perf stats to display
3.  Add pass info to display

**************************************************************************************************/


#include "IvyInput.h"
#include "MlaaApp.h"

int main(unsigned int argc, const char** argv)
{
    Ivy::IvyApp::Configure(argc, argv);

    MlaaApp* app = MlaaApp::Create();

    if (app != NULL)
    {
        app->Run();
    }

    app->Destroy();
    app = NULL;

	return 0;
}