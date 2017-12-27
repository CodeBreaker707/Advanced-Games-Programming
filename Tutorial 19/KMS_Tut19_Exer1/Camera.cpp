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

	m_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
	m_forward = XMVectorSet(0.0f, 0.0, 1.0f, 0.0f);
	m_position = XMVectorSet(m_x, m_y, m_z, 0.0f);

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

void Camera::Move(float x_distance, float z_distance)
{

	m_forward = XMVector3Normalize(m_lookAt - m_position);
	m_right = XMVector3Cross(m_forward, m_up);

	m_position += (x_distance * m_right);
	m_position += (z_distance * m_forward);

	m_x = XMVectorGetX(m_position);
	m_y = XMVectorGetY(m_position);
	m_z = XMVectorGetZ(m_position);

	//m_z = m_dz * distance;
}

float Camera::GetX()
{
	return m_x;
}

float Camera::GetY()
{
	return m_y;
}

float Camera::GetZ()
{
	return m_z;
}

XMMATRIX Camera::GetViewMatrix()
{
	//m_position = XMVectorSet(m_x, m_y, m_z, 0.0);
	m_lookAt = XMVectorSet(m_x + m_dx, m_y + m_dy, m_z + m_dz, 0.0);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
	//m_up = XMVector3Cross(m_forward, m_right);

	return XMMatrixLookAtLH(m_position, m_lookAt, m_up);
}

XMVECTOR Camera::GetPosition()
{
	return m_position;
}