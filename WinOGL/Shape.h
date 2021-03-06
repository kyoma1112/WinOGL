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
	CVertex* AppendVertex(CVertex* preV, float x, float y);
	int CountVertex();
	void ChangeVHead(CVertex* new_head);
	void InitVHead();

private:
	CShape* next_shape = NULL;
	CVertex* vertex_head = NULL;
};

