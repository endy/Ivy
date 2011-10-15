///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "DxLight.h"
#include "DxUtils.h"

/**************************************************************************************************
    DxLight::DxLight
**************************************************************************************************/
DxLight::DxLight(
    DxLightCreateInfo* pInfo)
    :
Light(pInfo),
    m_pContext(pInfo->pContext),
    m_pLightBuffer(NULL)
{

}

/**************************************************************************************************
    DxLight::~DxLight
**************************************************************************************************/
DxLight::~DxLight()
{

}

/**************************************************************************************************
    DxLight::Create
**************************************************************************************************/
DxLight* DxLight::Create(
    DxLightCreateInfo* pInfo)
{
    DxLight* pLight = new DxLight(pInfo);

    if (pLight->Init(pInfo->pDevice) == false)
    {
        AssertAlways();

        pLight->Destroy();
        pLight = NULL;
    }

    return pLight;
}

/**************************************************************************************************
    DxLight::Destroy
**************************************************************************************************/
void DxLight::Destroy()
{
    if (m_pLightBuffer != NULL)
    {
        m_pLightBuffer->Release();
    }

    Light::Destroy();
}

/**************************************************************************************************
    DxLight::Init
**************************************************************************************************/
bool DxLight::Init(ID3D11Device* pDevice)
{
    bool statusOK = true;

    LightBufferDef lightDef;
    memset(&lightDef, 0, sizeof(LightBufferDef));

    lightDef.position = m_position;
    lightDef.color = m_color;

    D3D11_BUFFER_DESC lightDesc;
    memset(&lightDesc, 0, sizeof(D3D11_BUFFER_DESC));
    lightDesc.Usage = D3D11_USAGE_DYNAMIC;
    lightDesc.ByteWidth = sizeof( LightBufferDef );
    lightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    lightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    lightDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData;
    memset(&initData, 0, sizeof(D3D11_SUBRESOURCE_DATA));	
    initData.pSysMem = &lightDef;

    statusOK = DxOK(pDevice->CreateBuffer(&lightDesc, &initData, &m_pLightBuffer));

    return statusOK;
}

/**************************************************************************************************
    DxLight::Update
**************************************************************************************************/
void DxLight::Update()
{
    D3D11_MAPPED_SUBRESOURCE mappedBuffer;
    m_pContext->Map(m_pLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
    LightBufferDef* pLightCB = reinterpret_cast<LightBufferDef*>(mappedBuffer.pData);

    pLightCB->position = m_position;
    pLightCB->color = m_color;

    m_pContext->Unmap(m_pLightBuffer, 0);
}

