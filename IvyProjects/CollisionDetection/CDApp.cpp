

#include "CDApp.h"
#include "DxShader.h"
#include "DxTexture.h"

CDApp::CDApp()
	:
	DxApp()
{

}

CDApp::~CDApp()
{

}

CDApp* CDApp::Create()
{
	CDApp* pApp = new CDApp();

	if (pApp->Init() == false)
	{
		pApp->Destroy();
		pApp = NULL;
	}

	return pApp;
}

void CDApp::Destroy()
{
	DxApp::Destroy();
}

bool CDApp::Init()
{
	return DxApp::Init();
}

void CDApp::Run()
{

}