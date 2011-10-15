#pragma once

#include <Windows.h>
#include <xnamath.h>

#include "Vector.h"

class Camera
{
public:
	Camera(void);
	Camera(Vector3 eyePosition, Vector3 lookAt, Vector3 upDirection);
	Camera(Vector3 eyePosition, Vector3 lookAt, Vector3 upDirection,
		  float viewAngle, float aspectRatio, float nearPlane, float farPlane);

    virtual ~Camera(void);

    //// ACCESSORS ////////////////////////////
	void set(Vector3 eye, Vector3 look, Vector3 up);

    Vector3 GetEye(){ return m_Eye; }
	void SetEye(float x, float y, float z);
    Vector3 GetLook(){ return m_Look; }
	void SetLook(float x, float y, float z);
    Vector3 GetUp(){ return m_Up; }
	void SetUp(float x, float y, float z);
	
    void SetViewVolume(float vAng, float asp, float nearD, float farD);

    Vector3 N(){ return m_N; }
    Vector3 U(){ return m_U; }
    Vector3 V(){ return m_V; }

    float GetViewAngle(){ return m_ViewAngle; }
    float GetAspect(){ return m_Aspect; }
    float GetNearClipPlane(){ return m_NearClipPlane; }
    float GetFarClipPlane(){ return m_FarClipPlane; }

    void setViewport(int width, int height){ m_ViewportWidth=width; m_ViewportHeight = height;}
    int GetViewportWidth(){ return m_ViewportWidth; }
    int GetViewportHeight(){ return m_ViewportHeight; }

    float GetWorldWidth(){ return m_WorldWidth; }
    float GetWorldHeight(){ return m_WorldHeight; }

    //// VIEWING  ////////////////////////////
    
	void GetModelViewMatrix(void);

    //// MOVEMENT ////////////////////////////
	void Roll(float angle);
	void Pitch(float angle);
	void Yaw(float angle);

    void Slide(float deltaX, float deltaY, float deltaZ);

    void rotateAboutLook(float deltaX, float deltaY);

private:
    void CalcCamVectors();

private:
	Vector3 m_Eye, m_Look;
	Vector3 m_Up;
	Vector3 m_U, m_V, m_N;
	float m_ViewAngle, m_Aspect, m_NearClipPlane, m_FarClipPlane, m_WorldWidth, m_WorldHeight;
	int m_ViewportWidth, m_ViewportHeight;

};
