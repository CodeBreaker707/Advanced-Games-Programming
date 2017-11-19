#pragma once

#include "objfilemodel.h"

class Collider3D
{
private:

	float m_x, m_y, m_z;

	float m_length, m_height, m_breadth;

public:

	Collider3D();
	~Collider3D();

	void CalculateColliderCentre(ObjFileModel* model);
	void CalculateDimensions(ObjFileModel* model);

	float GetLength(float x_scale);
	float GetHeight(float y_scale);
	float GetBreadth(float z_scale);	

	XMVECTOR GetColliderPos();

};
