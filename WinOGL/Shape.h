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
	boolean Cross(float mx, float my);
	boolean OtherCross(float mx, float my, CShape* shape_head);
	boolean CrossCalc(CVertex* As, CVertex* Ae, CVertex* Bs, float mx, float my);
	//点の内包判定　trueなら外、falseなら内
	boolean inclusion(float x, float y, CShape* shape_head);

private:
	CShape* next_shape = NULL;
	CVertex* vertex_head = NULL;
};

