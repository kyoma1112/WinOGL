#include "pch.h"
#include "AdminControl.h"
#include <math.h>

CAdminControl::CAdminControl() 
{
}

CAdminControl::~CAdminControl()
{
	shape_head->FreeShape();
}

void CAdminControl::Draw()
{
	CShape* nowS = shape_head;
	// TODO: ‚±‚±‚ÉŽÀ‘•ƒR[ƒh‚ð’Ç‰Á‚µ‚Ü‚·.
	while (nowS != NULL) {
		glColor3f(1.0, 1.0, 1.0);
		glPointSize(10);
		glBegin(GL_POINTS);

		CVertex* nowV = nowS->GetV();

		while (nowV != NULL) {
			glVertex2f(nowV->GetX(), nowV->GetY());
			nowV = nowV->GetNext();
		}

		glEnd();

		glColor3f(1.0, 1.0, 1.0);
		glPointSize(10);
		glBegin(GL_LINE_STRIP);

		nowV = nowS->GetV();
		while (nowV != NULL) {
			glVertex2f(nowV->GetX(), nowV->GetY());

			nowV = nowV->GetNext();
		}
		glEnd();
		nowS = nowS->GetNextShape();
	}
}

void CAdminControl::AppendShape()
{
	CShape* newShape = new CShape();
	newShape->SetNextShape(shape_head);
	shape_head = newShape;
}

float CAdminControl::Distance(CVertex* s, float x, float y) {
	float d;
	float X, Y;

	X = x - s->GetX();
	Y = y - s->GetY();

	d = sqrt(pow(X, 2) + pow(Y, 2));

	return d;
}

void CAdminControl::CreateShape(float x, float y)
{
	if (shape_head == NULL) {
		AppendShape();
	}
	if (shape_head->CountVertex() < 3) {

		if (shape_head->CountVertex() == 0) {
			shape_head->AppendVertex(x, y);
		}
		else if(!shape_head->OtherCross(x, y, shape_head)){
			shape_head->AppendVertex(x, y);
		}
	}
	else if (Distance(shape_head->GetV(), x, y) <= 0.1) {
		float vx = shape_head->GetV()->GetX();
		float vy = shape_head->GetV()->GetY();

		if (!shape_head->OtherCross(vx, vy, shape_head) && !shape_head->Cross(vx, vy)) {
			shape_head->AppendVertex(vx, vy);
			AppendShape();
		}
		else if(!shape_head->OtherCross(x, y, shape_head) && !shape_head->Cross(x, y)){
			shape_head->AppendVertex(x, y);
		}
	}
	else {
		
		if (!shape_head->OtherCross(x, y, shape_head) && !shape_head->Cross(x, y)){
			shape_head->AppendVertex(x, y);
		}
	}
}