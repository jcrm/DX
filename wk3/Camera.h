#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>
#include "D3DUtil.h"

class Camera{
public:
	
	bool yaw_on, pitch_on, roll_on, jump_on;
	int speed;
	float veloy, gravity;

	D3DXVECTOR3 mForward;
	D3DXVECTOR3 mUp;
	D3DXVECTOR3 mMovement;
	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mRotation;
	D3DXMATRIX	mView;
	D3DXMATRIX	mProj;

	D3DXMATRIX view()const;
	D3DXMATRIX proj()const;
		
	Camera();
	D3DXVECTOR3& position();
	void CameraRightVector();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void MoveForward();
	void MoveBack();
	void setLens(float fovY, float aspect, float zn, float zf);
	void Look();
	void Update(float roll, float pitch, float yaw);
	void Update();
	void OnlyPitch(float pitch);
	void OnlyRoll(float roll);
	void OnlyYaw(float yaw);
	void Jump();
	void CheckGround();

private:

};

Camera& GetCamera();

#endif 