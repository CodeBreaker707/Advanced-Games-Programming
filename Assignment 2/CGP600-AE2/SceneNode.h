#pragma once

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3d11.h>
#include <xnamath.h>
#include <time.h>

// Including children of Asset class
// to use them as nodes
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Statik.h"
#include "Dynamic.h"

class SceneNode
{
private:

	// Vector of children nodes
	vector<SceneNode*> m_children;

	// Position of nodes
	float m_pos_x, m_pos_y, m_pos_z;

	// Initial Position of Nodes
	float m_init_x, m_init_y, m_init_z;

	// Current Z position of the node
	// (Used by weapons for combat)
	float m_cur_pos_z;

	// Previous positions of the node
	float m_prev_x, m_prev_y, m_prev_z;

	// Degree values for rotating nodes
	float m_xangle, m_yangle, m_zangle;

	// To store previous pitch angle
	// to restrict them between limits
	float m_prev_xangle;

	// This is to store the scale of the node
	float m_scale_x, m_scale_y, m_scale_z;

	// These values are used for box collision
	// calculations. More info can be 
	// seen in calculation code
	float x1, y1, z1, r1;
	float l1, h1, b1;

	float x2, y2, z2, r2;
	float l2, h2, b2;

	// This is to store the distance
	// between 2 objects
	float m_dist_x;
	float m_dist_y;
	float m_dist_z;

	// This is the distance between an entity
	// and a point or object and to check
	// if they arrived at their destination
	// or not
	float m_lookAt_dist_x;
	float m_lookAt_dist_z;

	// This is the power of the gravitional
	// pull applied to objects
	float m_gravity_speed;

	// This stores the postions
	// for the entity to move towards
	// randomly
	XMVECTOR m_move_spots[4];

	// This to determine which spot
	// the entity should move towards
	int spot_num;
	int prev_spot_num;

	// This is to store the final radius
	// after calculations
	float main_dist;

	// This is to store the sum or radii
	// for sphere collision checks
	float sum_radius;

	// These are to store the values
	// from the colliders of asset's
	// children classes
	float m_collider_centre_x;
	float m_collider_centre_y;
	float m_collider_centre_z;

	// This is to determine if the object
	// is colliding against something
	bool m_is_colliding;

	// This is to determine if the object is
	// interacting with something
	bool m_is_interacting;

	// This is to determine if the node is
	// on top of something
	bool m_on_ground;

	// This is to determine if gravity is
	// applied to that object
	int m_gravity_applied;

	// This is to determine if the entity
	// is in range of the player
	bool m_in_range;

	// This is to halt the movement of the entity
	// if it approached the player
	bool m_halt_movement;

public:

	Player* m_p_asset;
	Enemy*  m_e_asset;
	Weapon* m_w_asset;
	Statik* m_s_asset;
	Dynamic* m_d_asset;

	SceneNode(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char c, char* assetFile, char* textureFile, float x_pos, float y_pos, float z_pos, float x_scale, float y_scale, float z_scale, int gravityState);
	~SceneNode();

	void ReleaseAll();

	void AddChildNode(SceneNode* n);

	bool DetachNode(SceneNode* n);

	void Execute(XMMATRIX* world, XMMATRIX* view, XMMATRIX* projection);

	void UpdateCollisionTree(XMMATRIX* world);

	bool CheckCollision(SceneNode* compare_tree);

	bool CheckActionCollision(SceneNode* compare_tree);

	void CalculateSphereCollisionDetails(SceneNode* compare_tree);
	void CalculateBoxCollisionDetails(SceneNode* compare_tree);

	bool CheckNodeBottomCollision(SceneNode* compare_tree);

	void CalculateBoxDimensions1(XMVECTOR v, Asset* obj);
	void CalculateSphereDimensions1(XMVECTOR v, Asset* obj);

	void CalculateBoxDimensions2(XMVECTOR v, Asset* obj);
	void CalculateSphereDimensions2(XMVECTOR v, Asset* obj);

	void ApplyGravity(double deltaTime);

	XMVECTOR GetWorldColliderCentrePos();
	XMVECTOR GetWorldPos();

	void SetXPos(float x);
	void SetYPos(float y);
	void SetZPos(float z);

	void RestrictPitch();

	void SetYAngle(float angle);

	void LookAt();
	void CheckInRange(XMVECTOR other_pos);

	void SetCurZPos();
	float GetCurZPos();

	float GetXPos();
	float GetYPos();
	float GetZPos();

	float GetXAngle();
	float GetYAngle();
	float GetZAngle();

	float GetXScale();
	float GetYScale();
	float GetZScale();

	float GetGravitySpeed();

	void RestrictPos(bool isCollding);
	void UpdatePos(bool isColliding);

	void MoveAsset(float x_dist, float y_dist, float z_dist);
	void RotateAsset(float pitch_degrees, float yaw_degrees, float roll_degrees);

	void ResetToInitalPos();

	float GetRandomSpot();
	void SetToPreviousSpot();

	void SetCollideState(bool state);
	bool IsColliding();

	void SetInteractState(bool state);
	bool IsInteracting();

	void SetOnGroundState(bool state);
	bool GetOnGroundState();

	void SetInRangeState(bool state);
	bool GetInRangeState();

	void SetHaltState(bool state);
	bool isHalted();

	int GetChildrenSize();

	vector<SceneNode*> GetChildren();

	SceneNode* GetEquippedWeaponNode();
	SceneNode* GetPushingCrate();

};