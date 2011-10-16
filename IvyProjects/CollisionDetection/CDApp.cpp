

#include "CDApp.h"
#include "DxShader.h"
#include "DxTexture.h"

CDApp::CDApp(CDAppCreateInfo* pAppInfo)
	:
	DxApp(pAppInfo)
{

}

CDApp::~CDApp()
{

}

CDApp* CDApp::Create(CDAppCreateInfo* pAppInfo)
{
	CDApp* pApp = new CDApp(pAppInfo);

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