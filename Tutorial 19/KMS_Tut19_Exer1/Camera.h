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
	float m_dx, m_dy, m_dz;
	float m_camera_rotation;

	XMVECTOR m_position, m_lookAt, m_up, m_forward, m_right;

public:
	Camera(float x, float y, float z, float camera_rotation);

	void YawRotate(float degrees);
	void PitchRotate(float degrees);
	//void Forward(float distance);
	void Move(float x_distance, float z_distance);

	float GetX();
	float GetY();
	float GetZ();

	XMMATRIX GetViewMatrix();
	XMVECTOR GetPosition();

};