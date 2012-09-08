


#include "RTApp.h"

int main()
{
    RTApp* app = RTApp::Create();

    if (app != NULL)
    {
        app->Run();
    }

    app->Destroy();
    app = NULL;

	return 0;
}
