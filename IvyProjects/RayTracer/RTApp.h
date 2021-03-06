
#ifndef _RTTESTAPP_H_
#define _RTTESTAPP_H_

#include "IvyApp.h"

class DxTexture;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// RTApp Class
///////////////////////////////////////////////////////////////////////////////////////////////////
class RTApp : public IvyApp
{
public:
    static RTApp* Create();
    virtual void Destroy();

    virtual void Run();

protected:
    RTApp();
    virtual ~RTApp();

    bool Init();

    void Render(DxTexture* pImage);
    void SetupHwRtEyeTexture(DxTexture* pTexture);

private:
    RTApp(const RTApp& copy);	// disallow copy constructor
};

#endif // _RTTESTAPP_H_