#include "Camera.h"

Camera::Camera(float x, float y, float z, float camera_rotation)
{

	m_x = x;
	m_y = y;
	m_z = z;
	m_camera_rotation = XMConvertToRadians(camera_rotation);

	m_dx = sin(XMConvertToRadians(camera_rotation));
	m_dy = sin(XMConvertToRadians(camera_rotation));
	m_dz = cos(XMConvertToRadians(camera_rotation));

}

void Camera::YawRotate(float degrees)
{
	m_camera_rotation = XMConvertToRadians(degrees);

	m_dx = sin(m_camera_rotation);
	m_dz = cos(m_camera_rotation);

}

void Camera::PitchRotate(float degrees)
{
	m_camera_rotation = XMConvertToRadians(degrees);

	m_dy = sin(m_camera_rotation);
	m_dz = cos(m_camera_rotation);

}

void Camera::Forward(float distance)
{
	m_x = m_dx * distance;
	m_z = m_dz * distance;
}

XMMATRIX Camera::GetViewMatrix()
{
	m_position = XMVectorSet(m_x, m_y, m_z, 0.0);
	m_lookAt = XMVectorSet(m_x + m_dx, m_y + m_dy, m_z + m_dz, 0.0);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);

	return XMMatrixLookAtLH(m_position, m_lookAt, m_up);
}