#pragma once
#include <gl/GL.h>
#include "Vertex.h"
#include "pch.h"
#include "Shape.h"

class CAdminControl
{
	
public:
	CAdminControl();
	~CAdminControl();

	void Draw();

	void AppendShape();
	float Distance(CVertex* s, float x, float y);
	void CreateShape(float x, float y);
	
private:
	CShape* shape_head = NULL;
};

