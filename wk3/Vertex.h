#ifndef VERTEX_H
#define VERTEX_H

#include "D3DUtil.h"

struct Vertex
{
	
	Vertex(){}
	Vertex(float x, float y, float z){
		pos= D3DXVECTOR3(x,y,z);
	}
	D3DXVECTOR3 pos;
};
struct VertexPNT
{
	VertexPNT(){}
	VertexPNT(float x, float y, float z, 
		float nx, float ny, float nz, 
		float u, float v){
		pos= D3DXVECTOR3(x,y,z);
		normal= D3DXVECTOR3(nx,ny,nz);
		texC = D3DXVECTOR2(u,v);
	}

	void Update(float x, float y, float z, 
		float nx, float ny, float nz, 
		float u, float v){
		pos= D3DXVECTOR3(x,y,z);
		normal= D3DXVECTOR3(nx,ny,nz);
		texC = D3DXVECTOR2(u,v);
	}
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texC;
};

#endif // VERTEX_H


