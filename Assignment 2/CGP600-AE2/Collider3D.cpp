#include "Collider3D.h"

Collider3D::Collider3D()
{
	m_x = 0;
	m_y = 0;
	m_z = 0;

	m_length  = 0;
	m_breadth = 0;
	m_height  = 0;

}

Collider3D::~Collider3D()
{

}

void Collider3D::CalculateColliderCentre(ObjFileModel* model)
{
	float min_x = 0;
	float min_y = 0;
	float min_z = 0;

	float max_x = 0;
	float max_y = 0;
	float max_z = 0;

	for (int i = 0; i < model->numverts; i++)
	{
		if (model->vertices[i].Pos.x < min_x)
		{
			min_x = model->vertices[i].Pos.x;
		}
		else if (model->vertices[i].Pos.x > max_x)
		{
			max_x = model->vertices[i].Pos.x;
		}

		if (model->vertices[i].Pos.y < min_y)
		{
			min_y = model->vertices[i].Pos.y;
		}
		else if (model->vertices[i].Pos.y > max_y)
		{
			max_y = model->vertices[i].Pos.y;
		}

		if (model->vertices[i].Pos.z < min_z)
		{
			min_z = model->vertices[i].Pos.z;
		}
		else if (model->vertices[i].Pos.z > max_z)
		{
			max_z = model->vertices[i].Pos.z;
		}
	}

	m_x = min_x + ((max_x - min_x) / 2);
	m_y = min_y + ((max_y - min_y) / 2);
	m_z = min_z + ((max_z - min_z) / 2);

}

void Collider3D::CalculateDimensions(ObjFileModel* model)
{
	float dist_x = 0.0f;
	float dist_y = 0.0f;
	float dist_z = 0.0f;


	for (int i = 0; i < model->numverts; i++)
	{
		if (model->vertices[i].Pos.x - m_x > dist_x)
		{
			dist_x = model->vertices[i].Pos.x - m_x;
		}

		if (model->vertices[i].Pos.y - m_y > dist_y)
		{
			dist_y = model->vertices[i].Pos.y - m_y;
		}

		if (model->vertices[i].Pos.z - m_z > dist_z)
		{
		dist_z = model->vertices[i].Pos.z - m_z;
		}

	}		

	// Length  represents the dimension in x-axis
	// Height  represents the dimension in y-axis
	// Breadth represents the dimension in z-axis

	m_length = dist_x * 2;
	m_height = dist_y * 2;
	m_breadth = dist_z * 2;
}

void Collider3D::CalculateRadius(ObjFileModel* model)
{
	float dist_x = 0.0f;
	float dist_y = 0.0f;
	float dist_z = 0.0f;
	float cal_dist = 0.0f;
	float max_dist = 0.0f;


	for (int i = 0; i < model->numverts; i++)
	{
		//if (model->vertices[i].Pos.x - m_x > dist_x)
		//{
			dist_x = model->vertices[i].Pos.x - m_x;
		//}

		//if (model->vertices[i].Pos.y - m_y > dist_y)
		//{
			dist_y = model->vertices[i].Pos.y - m_y;
		//}

		//if (model->vertices[i].Pos.z - m_z > dist_z)
		//{
			dist_z = model->vertices[i].Pos.z - m_z;
		//}

		cal_dist = (dist_x * dist_x) + (dist_y * dist_y) + (dist_z * dist_z);

		if (cal_dist > max_dist)
		{
			max_dist = cal_dist;
		}

	}

	m_sqr_radius = sqrt(max_dist);

	// Length  represents the dimension in x-axis
	// Height  represents the dimension in y-axis
	// Breadth represents the dimension in z-axis

	//m_length = dist_x * 2;
	//m_height = dist_y * 2;
	//m_breadth = dist_z * 2;
	

}

XMVECTOR Collider3D::GetColliderPos()
{
	XMVECTOR pos = XMVectorSet(m_x, m_y, m_z, 0.0f);
	
	return pos;

}

float Collider3D::GetLength(float x_scale)
{
	return m_length * x_scale;
}

float Collider3D::GetHeight(float y_scale)
{
	return m_height * y_scale;
}

float Collider3D::GetBreadth(float z_scale)
{
	return m_breadth * z_scale;
}

float Collider3D::GetColliderRadius()
{
	return m_sqr_radius;
}