#pragma once

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3d11.h>
#include <xnamath.h>
#include <math.h>

class Camera
{
private:
	float m_x, m_y, m_z;
	float m_prev_x, m_prev_y, m_prev_z;
	float m_dx, m_dy, m_dz;
	float m_camera_rotation_x;
	float m_camera_rotation_y;

	bool m_isColliding = false;

	XMVECTOR m_position, m_lookAt, m_right, m_up, m_forward;

	XMMATRIX projection;

public:

	Camera(float x, float y, float z, float camera_rotation);

	void Move(float x_distance, float y_distance, float z_distance);

	void RotateCameraX(float amount);
	void RotateCameraY(float amount);
	//void Increase(float amount);

	float GetX();
	float GetY();
	float GetZ();

	float GetYaw();

	void SetCollidingState(bool state);
	bool GetCollidingState();

	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
};
