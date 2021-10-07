#include "pch.h"
#include "AdminControl.h"

CAdminControl::CAdminControl() 
{
}

CAdminControl::~CAdminControl()
{
	vertex_head->FreeVertex();
}

void CAdminControl::Draw()
{
	// TODO: ‚±‚±‚ÉÀ‘•ƒR[ƒh‚ğ’Ç‰Á‚µ‚Ü‚·.
	glColor3f(1.0, 1.0, 1.0);
	glPointSize(10);
	glBegin(GL_POINTS);

	CVertex* nowV = vertex_head;

	while (nowV != NULL) {
		glVertex2f(nowV->GetX(), nowV->GetY());

		nowV = nowV->GetNext();
	}

	glEnd();
}

CVertex* CAdminControl::AppendVertex(float x, float y) 
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

