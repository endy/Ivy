///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _DXLIGHT_H_
#define _DXLIGHT_H_

#include "IvyLight.h"

#include <D3D11.h>

struct DxLightCreateInfo : public Ivy::LightCreateInfo
{
    ID3D11Device* pDevice;
    ID3D11DeviceContext* pContext;
};

// todo: migrate this light out into a seperate Dx header

class DxLight : public Ivy::Light
{
public:
    static DxLight* Create(DxLightCreateInfo* pInfo);
    virtual void Destroy();

    ID3D11Buffer* GetBuffer() { return m_pLightBuffer; }

protected:
    DxLight(DxLightCreateInfo* pInfo);
    virtual ~DxLight();

    bool Init(ID3D11Device* pDevice);
    void Update();

    struct LightBufferDef
    {
        Ivy::Point4 position;
        Ivy::Point4 color;
    };

    ID3D11DeviceContext* m_pContext;
    ID3D11Buffer* m_pLightBuffer;

private:
    DxLight(const DxLight& copy);	// disallow copy constructor

};

#endif // _DXLIGHT_H_
