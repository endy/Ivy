///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine - Generic DirectX Test App
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DXTESTAPP_H_
#define _DXTESTAPP_H_

#include "DxApp.h"

class DxTestApp : public DxApp
{
public:
    static DxTestApp* Create();

    virtual void Destroy();

    virtual void Run();

protected:
    DxTestApp();
    virtual ~DxTestApp();

    virtual bool Init();

    void Draw2D();

private:
    DxTestApp(const DxTestApp& copy);	// disallow copy constructor

    ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
    ID2D1SolidColorBrush* m_pCornflowerBlueBrush;

};

#endif // _DXTESTAPP_H_