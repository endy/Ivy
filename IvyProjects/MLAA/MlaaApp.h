
#ifndef _MLAAAPP_H_
#define _MLAAAPP_H_


#include "IvyApp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// MlaaApp Class
///////////////////////////////////////////////////////////////////////////////////////////////////
class MlaaApp : public Ivy::IvyApp
{
public:
    static MlaaApp* Create();

    virtual void Destroy();

    virtual void Run();

    virtual void ReceiveEvent(const Event* pEvent);

protected:
    MlaaApp();
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
