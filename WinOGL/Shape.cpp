#include "pch.h"
#include "Shape.h"

CShape::CShape() 
{
}

CShape::CShape(CVertex* new_v) {
	vertex_head = new_v;
}

CShape::~CShape()
{
	vertex_head->FreeVertex();
}

void CShape::SetV(CVertex* v) 
{
	vertex_head = v;
}

CVertex* CShape::GetV() 
{
	return vertex_head;
}

void CShape::SetNextShape(CShape* new_nextS) {
	next_shape = new_nextS;
}

CShape* CShape::GetNextShape() {
	return next_shape;
}

void CShape::FreeShape() {
	CShape* nowS = this;

	while (nowS != NULL) {
		CShape* del_cell = nowS;
		nowS = nowS->GetNextShape();
		delete del_cell;
	}
}

CVertex* CShape::AppendVertex(float x, float y)
{
	CVertex* nowV = vertex_head;
	CVertex* newVertex = new CVertex(x, y);

	if (nowV != NULL)
	{
		while (nowV->GetNext() != NULL) {
			nowV = nowV->GetNext();
		}
		nowV->SetNext(newVertex);
	}
	else
	{
		vertex_head = newVertex;
	}

	return newVertex;
}

CVertex* CShape::AppendVertex(CVertex* preV, float x, float y)
{
	CVertex* newVertex = new CVertex(x, y);

	newVertex->SetNext(preV->GetNext());
	preV->SetNext(newVertex);
	
	return newVertex;
}

int CShape::CountVertex()
{
	int c = 0;

	for (CVertex* vp = vertex_head; vp != NULL; vp = vp->GetNext()) {
		c++;
	}

	return c;
}

void CShape::ChangeVHead(CVertex* new_head)
{
	vertex_head = new_head;
	vertex_head->SetNext(new_head->GetNext());
}

void CShape::InitVHead()
{
	vertex_head = NULL;
}


