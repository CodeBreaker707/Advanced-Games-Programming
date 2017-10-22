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

	DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	DefaultForward = XMVectorSet(0.0f, 0.0, 1.0f, 0.0f);
	m_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
	m_forward = XMVectorSet(0.0f, 0.0, 1.0f, 0.0f);
	m_position = XMVectorSet(m_x, m_y, m_z, 0.0);

}

//void Camera::YawRotate(float degrees)
//{
//	m_camera_rotation = XMConvertToRadians(degrees);
//
//	m_dx = sin(m_camera_rotation);
//	m_dz = cos(m_camera_rotation);
//
//}
//
//void Camera::PitchRotate(float degrees)
//{
//	m_camera_rotation = XMConvertToRadians(degrees);
//
//	m_dy = sin(m_camera_rotation);
//	m_dz = cos(m_camera_rotation);
//
//}

void Camera::Rotate(float pitch_degrees, float yaw_degrees)
{
	camRotationMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(pitch_degrees), XMConvertToRadians(yaw_degrees), 0.0f);
	m_dy = XMConvertToRadians(yaw_degrees);
	m_lookAt = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	m_lookAt = XMVector3Normalize(m_lookAt);
}

void Camera::Move(float x_distance, float z_distance)
{
	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(m_dy);

	m_right = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	m_up = XMVector3TransformCoord(m_up, RotateYTempMatrix);
	m_forward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

	m_position += x_distance * m_right;
	m_position += z_distance * m_forward;
	//m_z = m_dz * distance;
}


XMMATRIX Camera::GetViewMatrix()
{

	//m_lookAt = XMVectorSet(m_x + m_dx, m_y + m_dy, m_z + m_dz, 0.0);
	m_lookAt = m_position + m_lookAt;
	//m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);

	return XMMatrixLookAtLH(m_position, m_lookAt, m_up);
}