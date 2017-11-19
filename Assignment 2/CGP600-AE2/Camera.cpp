#include "Camera.h"

Camera::Camera(float x, float y, float z, float camera_rotation)
{

	m_x = x;
	m_y = y;
	m_z = z;

	m_prev_x = x;
	m_prev_y = y;
	m_prev_z = z;

	m_camera_rotation = XMConvertToRadians(camera_rotation);

	m_dx = sin(XMConvertToRadians(camera_rotation));
	m_dy = sin(XMConvertToRadians(camera_rotation));
	m_dz = cos(XMConvertToRadians(camera_rotation));

	m_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
	m_forward = XMVectorSet(0.0f, 0.0, 1.0f, 0.0f);
	m_position = XMVectorSet(m_x, m_y, m_z, 0.0f);

	m_isColliding = false;

	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0), 1280.0 / 768.0, 1.0, 100.0);

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

void Camera::Move(float x_distance, float y_distance, float z_distance)
{
	
	m_forward = XMVector3Normalize(m_lookAt - m_position);
	m_right = XMVector3Cross(m_forward, m_up);

	m_position += (-x_distance * m_right);
	m_position += (y_distance * m_up);
	m_position += (z_distance * m_forward);

	m_x = XMVectorGetX(m_position);
	m_y = XMVectorGetY(m_position);
	m_z = XMVectorGetZ(m_position);
	
}

void Camera::SetCollidingState(bool state)
{
	m_isColliding = state;
}

XMMATRIX Camera::GetViewMatrix()
{
	m_lookAt = XMVectorSet(m_x + m_dx, m_y + m_dy, m_z + m_dz, 0.0);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);

	return XMMatrixLookAtLH(m_position, m_lookAt, m_up);
}

XMMATRIX Camera::GetProjectionMatrix()
{
	return projection;
}

bool Camera::GetCollidingState()
{
	return m_isColliding;
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