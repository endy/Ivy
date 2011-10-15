
#ifndef _RTTESTAPP_H_
#define _RTTESTAPP_H_

#include "DxApp.h"

class DxTexture;

struct RTAppCreateInfo : public DxAppCreateInfo
{

};

class RTApp : public DxApp
{
public:
    static RTApp* Create(RTAppCreateInfo* pAppInfo);
    virtual void Destroy();

    virtual void Run();

protected:
    RTApp(RTAppCreateInfo* pAppInfo);
    virtual ~RTApp();

    bool Init();

    void Render(DxTexture* pImage);
    void SetupHwRtEyeTexture(DxTexture* pTexture);

private:
    RTApp(const RTApp& copy);	// disallow copy constructor
};

#endif // _RTTESTAPP_H_