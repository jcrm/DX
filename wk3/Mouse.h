#include <Windows.h>
#include "Camera.h"

class Mouse{
public:
	int x,y;
	int oldX,oldY;
	int difX, difY;

	Mouse():x(0),y(0),oldX(0),oldY(0),difX(0),difY(0){}
	void Move(LPARAM lParam){
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		difX = x - oldX;
		difY = y - oldY;
		oldX = x;
		oldY = y;
	}
	int getX(){
		return x;
	}
	int getY(){
		return y;
	}
	int getDifX(){
		return difX;
	}
	int getDifY(){
		return difY;
	}
};