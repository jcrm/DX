#include"Camera.h"
//yaw around y 
//pitch around x
//roll around z
//initialize all variables
Camera& GetCamera()
{
	static Camera camera;
	return camera;
}
D3DXVECTOR3& Camera::position()
{
	return mPosition;
}
D3DXMATRIX Camera::view()const
{
	return mView;
}

D3DXMATRIX Camera::proj()const
{
	return mProj;
}
Camera::Camera(){

	mForward	= D3DXVECTOR3( 0.0f, 0.0f,  0.0f);
	mUp			= D3DXVECTOR3( 0.0f, 1.0f,  0.0f);
	mMovement	= D3DXVECTOR3( 0.0f, 0.0f,  0.0f);
	mPosition	= D3DXVECTOR3( 0.0f, 0.0f,-10.0f);
	mRotation	= D3DXVECTOR3( 0.0f, 0.0f,  0.0f);

	speed = 160;

	//can only rotate horizontally, and vertically
	yaw_on = true;
	pitch_on=true;
	roll_on=false;
	jump_on =false;
	veloy = 0;
	gravity = 0.2;
}
//used for calculating the movement vector it is a cross product of up and forward vector
void Camera::CameraRightVector(){
	Update();
	mMovement.x = ((mForward.y * mUp.z) - (mForward.z * mUp.y))/speed;
	mMovement.y = (0-(mForward.x * mUp.z) - (mForward.z * mUp.x))/speed;
	mMovement.z = ((mForward.x * mUp.y) - (mForward.y * mUp.z))/speed;
}
//move the vertically up by adding the up vector to the position vector
void Camera::MoveUp(){
	CameraRightVector();
	mPosition.x += mUp.x/(speed/100);
	mPosition.y += mUp.y/(speed/100);
	mPosition.z += mUp.z/(speed/100);
}
//move the vertically down by subtracting the up vector from the position vector
void Camera::MoveDown(){
	CameraRightVector();
	mPosition.x -= mUp.x/(speed/100);
	mPosition.y -= mUp.y/(speed/100);
	mPosition.z -= mUp.z/(speed/100);
}
//move the camera horizontally left subtracting the movement vector from the position vector
void Camera::MoveLeft(){
	CameraRightVector();
	mPosition.x -= mMovement.x;
	mPosition.y -= mMovement.y;
	mPosition.z -= mMovement.z;
}
//move the camera horizontally right adding the movement vector to the position vector
void Camera::MoveRight(){
	CameraRightVector();
	mPosition.x += mMovement.x;
	mPosition.y += mMovement.y;
	mPosition.z += mMovement.z;
}
//move the camera forward by adding the forward vector to the position vector then increasing the forward vector
void Camera::MoveForward(){
	CameraRightVector();
	mPosition.x += mForward.x/speed;
	mPosition.z += mForward.z/speed;

	mForward.x+=mForward.x/speed;
	mForward.z+=mForward.z/speed;
}
//move the camera forward by subtracting the forward vector from the position vector then decreasing the forward vector
void Camera::MoveBack(){
	CameraRightVector();
	mPosition.x -= mForward.x/speed;
	mPosition.z -= mForward.z/speed;

	mForward.x-=mForward.x/speed;
	mForward.z-=mForward.z/speed;
}
//pitch between a predetermined range
void Camera::OnlyPitch(float pitch)
{
	if(pitch_on){
		if(mRotation.y <=45 && mRotation.y >= -45){
			mRotation.y+=pitch;
		}
		if(mRotation.y > 45){
			mRotation.y = 45;
		} else if(mRotation.y < -45){
			mRotation.y = -45;
		}
	}
}
//roll by a given amount
void Camera::OnlyRoll(float roll)
{
	if(roll_on){
		mRotation.z+=roll;
	}
}
//yaw by a given amount
void Camera::OnlyYaw(float yaw)
{
	if(yaw_on){
		mRotation.x+=yaw;
	}
}
//update the forward and up vector based upon roll, pitch and yaw
void Camera::Update(float roll, float pitch, float yaw){
	float cosR, cosP, cosY;
	float sinR, sinP, sinY;

	mRotation.x = yaw;
	mRotation.y = pitch;
	mRotation.z = roll;

	cosY = cosf(mRotation.x*3.1415/180);
	cosP = cosf(mRotation.y*3.1415/180);
	cosR = cosf(mRotation.z*3.1415/180);

	sinY = sinf(mRotation.x*3.1415/180);
	sinP = sinf(mRotation.y*3.1415/180);
	sinR = sinf(mRotation.z*3.1415/180);

	mForward.x = sinY * cosP * 360;
	mForward.y = sinP * 360;
	mForward.z = cosP * -cosY * 360;

	mUp.x = -cosY*sinR - sinY * sinP * cosR;
	mUp.y = cosP * cosR;
	mUp.z = -sinY*sinR - sinP * cosR *-cosY ;
	
	CheckGround();
}
void Camera::Look(){
	D3DXMatrixLookAtLH(&mView, &mPosition, &mForward, &mUp);
}
void Camera::setLens(float fovY, float aspect, float zn, float zf)
{
	D3DXMatrixPerspectiveFovLH(&mProj, fovY, aspect, zn, zf);
}
//update the up and forward vectors
void Camera::Update(){
	float cosR, cosP, cosY;
	float sinR, sinP, sinY;

	cosY = cosf(mRotation.x*3.1415/180);
	cosP = cosf(mRotation.y*3.1415/180);
	cosR = cosf(mRotation.z*3.1415/180);

	sinY = sinf(mRotation.x*3.1415/180);
	sinP = sinf(mRotation.y*3.1415/180);
	sinR = sinf(mRotation.z*3.1415/180);

	mForward.x = sinY * cosP * 360;
	mForward.y = sinP * 360;
	mForward.z = cosP * cosY * 360;

	mUp.x = -cosY*sinR - sinY * sinP * cosR;
	mUp.y = cosP * cosR;
	mUp.z = -sinY*sinR - sinP * cosR *-cosY ;

	//CheckGround();
	
}
//make sure that the camera is not below ground level and if above ground level decrease until it returns to ground level
//used for jumping
void Camera::CheckGround(){
	int static timer = 0;
	int const max_time = 1;

	if(timer == max_time){
		timer = 0;
		mPosition.y += veloy;
		veloy -= gravity;
		if(mPosition.y <=0){
			mPosition.y = 0;
			veloy = 0;
			jump_on = false;
		}
	}
	timer++;
}
//if jump was pressed start the camera jump process 
void Camera::Jump(){
	if(!jump_on){
		veloy=1;
		jump_on = true;
	}
}