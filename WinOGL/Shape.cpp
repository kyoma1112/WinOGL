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

int CShape::CountVertex()
{
	int c = 0;

	for (CVertex* vp = vertex_head; vp != NULL; vp = vp->GetNext()) {
		c++;
	}

	return c;
}

boolean CShape::cross(CVertex* Be) {
	CVertex* As = vertex_head;
	CVertex* Ae = As->GetNext();
	CVertex* Bs = Ae->GetNext();

	CVertex a, b, a1, b1, a2, b2;
	float ca1, ca2, cb1, cb2;

	while (Bs->GetNext() != Be) 
	{
		Bs = Bs->GetNext();
	}
	
	while (Ae->GetNext() != Be) {
		//a
		a.SetX(Ae->GetX() - As->GetX());
		a.SetY(Ae->GetY() - As->GetY());
		//b
		b.SetX(Be->GetX() - Bs->GetX());
		b.SetY(Be->GetY() - Bs->GetY());
		//a1
		a1.SetX(Bs->GetX() - As->GetX());
		a1.SetY(Bs->GetY() - As->GetY());
		//b1
		b1.SetX(As->GetX() - Bs->GetX());
		b1.SetY(As->GetY() - Bs->GetY());
		//a2
		a2.SetX(Be->GetX() - As->GetX());
		a2.SetY(Be->GetY() - As->GetY());
		//b2
		b2.SetX(Ae->GetX() - Bs->GetX());
		b2.SetY(Ae->GetY() - Bs->GetY());

		ca1 = a.GetX() * a1.GetY() - a1.GetX() * a.GetY();
		ca2 = a.GetX() * a2.GetY() - a2.GetX() * a.GetY();
		cb1 = b.GetX() * b1.GetY() - b1.GetX() * b.GetY();
		cb2 = b.GetX() * b2.GetY() - b2.GetX() * b.GetY();

		if (ca1 * ca2 <= 0 && cb1 * cb2 <= 0) {
			return true;
		}

		As = Ae;
		Ae = Ae->GetNext();
	}
	
	return false;
}