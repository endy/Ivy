#include "Camera.h"


#define PI 3.14159

Camera::Camera(void)
{
	m_Eye = Vector3(0,0,1);
	m_Look = Vector3(0,0,0);
	m_Up = Vector3(0,1,0);
	CalcCamVectors();
}

Camera::Camera(Vector3 eyePosition, Vector3 lookAt, Vector3 upDirection)
{
    m_Eye = eyePosition;
    m_Look = lookAt;
    m_Up = upDirection;
    CalcCamVectors();
}

Camera::Camera(Vector3 eyePosition, Vector3 lookAt, Vector3 upDirection, float viewAngle, float aspectRatio, float nearPlane, float farPlane)
{
	set(eyePosition, lookAt, upDirection);
	SetViewVolume(viewAngle, aspectRatio, nearPlane, farPlane);
}
Camera::~Camera(void)
{

}

void Camera::SetEye(float x, float y, float z){
	m_Eye = Vector3(x, y, z);
}
void Camera::SetLook(float x, float y, float z){
	m_Look = Vector3(x, y, z);
}
void Camera::SetUp(float x, float y, float z){
	m_Up = Vector3(x, y, z);
}

void Camera::set(Vector3 Eye, Vector3 look, Vector3 up)
{
	m_Eye = Vector3(Eye); // store the given eye position
	m_Look = Vector3(look);
	m_Up = Vector3(m_Up);

    CalcCamVectors();

}

void Camera::GetModelViewMatrix(void)
{
    float camera[16];
    Vector3 eyeVector(m_Eye.X, m_Eye.Y, m_Eye.Z);

    camera[0] =  m_U.X; camera[4] = m_U.Y; camera[8]  = m_U.Z; camera[12] = - Dot(eyeVector, m_U);
    camera[1] =  m_V.X; camera[5] = m_V.Y; camera[9]  = m_V.Z; camera[13] = - Dot(eyeVector, m_V);
    camera[2] =  m_N.X; camera[6] = m_N.Y; camera[10] = m_N.Z; camera[14] = - Dot(eyeVector, m_N);
    camera[3] =  0;     camera[7] = 0;       camera[11] = 0;   camera[15] = 1.0;
}

void Camera::SetViewVolume(float vAng, float asp, float nearD, float farD)
{
	m_ViewAngle = vAng;
	m_Aspect = asp;
	m_NearClipPlane = nearD;
	m_FarClipPlane = farD;
	m_WorldHeight = m_NearClipPlane * tan((float)(m_ViewAngle*3.14159/360.0));
	m_WorldWidth = m_WorldHeight * m_Aspect;
}


void Camera::Roll(float angle)
{
	float cs = (float)cos(PI / 180 * angle);
	float sn = (float)sin(PI / 180 * angle);

	Vector3 t = m_U; // remember old u
	m_U = Vector3(cs*t.X - sn*m_V.X, cs*t.Y - sn*m_V.Y, cs*t.Z - sn*m_V.Z);
	m_V = Vector3(sn*t.X + cs*m_V.X, sn*t.Y + cs*m_V.Y, sn*t.Z + cs*m_V.Z);
}

void Camera::Pitch(float angle)
{
	float cs = (float)cos(PI / 180.0 * angle);
	float sn = (float)sin(PI / 180.0 * angle);

	Vector3 t = m_V; // remember old u
	m_V = Vector3(cs*t.X - sn*m_N.X, cs*t.Y - sn*m_N.Y, cs*t.Z - sn*m_N.Z);
	m_N = Vector3(sn*t.X + cs*m_N.X, sn*t.Y + cs*m_N.Y, sn*t.Z + cs*m_N.Z);
}

void Camera::Yaw(float angle)
{
	float cs = (float)cos(PI / 180 * angle);
	float sn = (float)sin(PI / 180 * angle);

	Vector3 t = m_U; // remember old u
	m_U = Vector3(cs*t.X - sn*m_N.X, cs*t.Y - sn*m_N.Y, cs*t.Z - sn*m_N.Z);
	m_N = Vector3(sn*t.X + cs*m_N.X, sn*t.Y + cs*m_N.Y, sn*t.Z + cs*m_N.Z);
}


void Camera::rotateAboutLook(float deltaX, float deltaY){
	float cs = (float)cos(PI / 180 * -deltaX);
	float sn = (float)sin(PI / 180 * -deltaX);

	Vector3 t = m_Eye - m_Look; // remember old u
	t = Vector3(cs*t.X + sn*t.Z, t.Y, -sn*t.X + cs*t.Z);
	t = t + m_Look;

	cs = (float)cos(PI / 180 * deltaY);
	sn = (float)sin(PI / 180 * deltaY);

	t = Vector3(t.X, cs*t.Y + sn*t.Z, -sn*t.Y + cs*t.Z);
	t = t + m_Look;
	m_Eye = Vector3(t.X, t.Y, t.Z);
	m_Up = Vector3(m_Up.X, cs*m_Up.Y + sn*m_Up.Z, -sn*m_Up.Y + cs*m_Up.Z);
	
	CalcCamVectors();
}

void Camera::Slide(float deltaX, float deltaY, float deltaZ)
{
	m_Look.X += deltaX * m_U.X + deltaY * m_V.X + deltaZ * m_N.X;
	m_Look.Y += deltaX * m_U.Y + deltaY * m_V.Y + deltaZ * m_N.Y;
	m_Look.Z += deltaX * m_U.Z + deltaY * m_V.Z + deltaZ * m_N.Z;

	m_Eye.X += deltaX * m_U.X + deltaY * m_V.X + deltaZ * m_N.X;
	m_Eye.Y += deltaX * m_U.Y + deltaY * m_V.Y + deltaZ * m_N.Y;
	m_Eye.Z += deltaX * m_U.Z + deltaY * m_V.Z + deltaZ * m_N.Z;
}

void Camera::CalcCamVectors()
{
    m_N = Vector3(m_Eye.X - m_Look.X, m_Eye.Y - m_Look.Y, m_Eye.Z - m_Look.Z);	// make n
    m_U = Vector3(Cross(m_Up, m_N)); // make u = up X n

    m_N.Normalize();
    m_U.Normalize();

    m_V = Vector3(Cross(m_N, m_U));	// make v = n X u
}
