

#include "CDApp.h"

int main()
{
    CDApp* app = CDApp::Create();

    if (app != NULL)
    {
        app->Run();
    }

    app->Destroy();
    app = NULL;

	return 0;
}



