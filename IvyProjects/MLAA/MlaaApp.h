
#ifndef _MLAAAPP_H_
#define _MLAAAPP_H_


#include "DxApp.h"

struct MlaaAppCreateInfo : public DxAppCreateInfo
{

};


class MlaaApp : public DxApp
{
public:
    static MlaaApp* Create(MlaaAppCreateInfo* pAppInfo);

    virtual void Destroy();

    virtual void Run();

    virtual void ReceiveEvent(const Event* pEvent);

protected:
    MlaaApp(MlaaAppCreateInfo* pAppInfo);
    virtual ~MlaaApp();

    //virtual bool Init();

    void HandleKeyboardInput(const Event* pEvent);

    BOOL m_linearZoomFilter;

    enum MlaaPass
    {
        MlaaPassPrePass             = 0,
        MlaaPassEdgeDetect,
        MlaaPassBlendWeightCalc,
        MlaaPassBlendEdge,

        // Debug Passes
        MlaaAreaTex,
    };

    MlaaPass m_passDisplayed;

};

#endif // _MLAAAPP_H_
