#pragma once
#include "Vertex.h"

class CShape
{

public:
	CShape();
	CShape(CVertex* new_shape);
	~CShape();

	void SetV(CVertex* v);
	CVertex* GetV();
	
	void SetNextShape(CShape* new_next);
	CShape* GetNextShape();

	void FreeShape();

	CVertex* AppendVertex(float x, float y);
	int CountVertex();
	boolean CShape::cross(CVertex* Be);

private:
	CShape* next_shape = NULL;
	CVertex* vertex_head = NULL;
};

