#include "Camera.h"

Camera::Camera(float x, float y, float z, float camera_rotation)
{

	m_x = x;
	m_y = y;
	m_z = z;

	m_init_x = m_x;
	m_init_y = m_y;
	m_init_z = m_z;

	m_camera_rotation_x = XMConvertToRadians(camera_rotation);
	m_camera_rotation_y = XMConvertToRadians(camera_rotation);

	m_dx = sin(XMConvertToRadians(camera_rotation));
	m_dy = sin(XMConvertToRadians(camera_rotation));
	m_dz = cos(XMConvertToRadians(camera_rotation));

	m_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
	m_forward = XMVectorSet(0.0f, 0.0, 1.0f, 0.0f);
	m_position = XMVectorSet(m_x, m_y, m_z, 0.0f);

	m_isColliding = false;

	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0), 1280.0 / 768.0, 0.5, 100.0);

}

void Camera::RotateCameraX(float amount)
{
	m_camera_rotation_x += XMConvertToRadians(amount);

	m_dx = sin(m_camera_rotation_x);
	m_dz = cos(m_camera_rotation_x);

	/*m_camera_rotation_x += amount;

	m_dx = sin(XMConvertToRadians(m_camera_rotation_x));
	m_dz = cos(XMConvertToRadians(m_camera_rotation_x));*/
}

void Camera::RotateCameraY(float amount)
{
	m_camera_rotation_y += XMConvertToRadians(amount);

	m_dy = sin(m_camera_rotation_y);
	m_dz = cos(m_camera_rotation_y);

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

void Camera::MoveWithPlayer(float x, float y, float z)
{
	m_position = XMVectorSet(x, y, z, 0.0f);

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

void Camera::SetX(float x)
{
	m_x = x;
}

void Camera::SetY(float y)
{
	m_y = y;
}

void Camera::SetZ(float z)
{
	m_z = z;
}

void Camera::ResetToInitalPos()
{
	m_position = XMVectorSet(m_init_x, m_init_y, m_init_z, 0.0f);

	m_x = XMVectorGetX(m_position);
	m_y = XMVectorGetY(m_position);
	m_z = XMVectorGetZ(m_position);

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

float Camera::GetYaw()
{
	return m_dx;
}