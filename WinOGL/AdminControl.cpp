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

//�ʂ̕\�����s���֐�
void CAdminControl::DrawArea(CShape* printS)
{

	CVertex* nowV = printS->GetV();

	//control_shape�̕���
	CShape* copyShape = new CShape();
	while (nowV != NULL) 
	{
		copyShape->AppendVertex(nowV->GetX(), nowV->GetY());
		nowV = nowV->GetNext();
	}

	CVertex* v1 = copyShape->GetV();
	CVertex* v2 = v1->GetNext();
	CVertex* v3 = v2->GetNext();
	float centerX, centerY;
	
	//�O�p�`�ɂȂ�܂�
	while (copyShape->CountVertex() >= 5) 
	{
		//v1,v2,v3�ō����O�p�`�̏d�S�����߂�
		centerX = (v1->GetX() + v2->GetX() + v3->GetX()) / 3;
		centerY = (v1->GetY() + v2->GetY() + v3->GetY()) / 3;
		//�O�p�`�̏d�S���\������`��̒��ɂ����ĎO�p�`�̒��ɎO�p�`�̒��_�ȊO�̓_���Ȃ��ꍇ�ɎO�p�`��\������
		if (!Inclusion(copyShape, centerX, centerY) && TriangleInclusion(copyShape, v1, v2, v3)) 
		{
			glColor3f(0.65, 0.65, 0.65);
			glBegin(GL_TRIANGLES);
			glVertex2f(v1->GetX(), v1->GetY());
			glVertex2f(v2->GetX(), v2->GetY());
			glVertex2f(v3->GetX(), v3->GetY());
			glEnd();
			DeleteTriangle(copyShape, v2);
		}
		else 
		{
			//�O�p�`�̎n�_����i�߂�
			v1 = v2;
		}
		//�O�p�`�̎n�_�ȊO�̓_����i�߂�
		v2 = v3;
		v3 = v3->GetNext();
		if (v3->GetNext() == NULL)
		{
			v3 = copyShape->GetV();
		}
	}
	//�Ō�̎O�p�`�\��
	if (copyShape->CountVertex() == 4) 
	{
		glColor3f(0.65, 0.65, 0.65);
		glBegin(GL_TRIANGLES);
		glVertex2f(v1->GetX(), v1->GetY());
		glVertex2f(v2->GetX(), v2->GetY());
		glVertex2f(v3->GetX(), v3->GetY());
		glEnd();
	}

	//���������`��Ƃ��̒��̒��_�̍폜
	copyShape->GetV()->FreeVertex();
	copyShape->InitVHead();
	copyShape->FreeShape();
}

//���̖̂ʕ\��
void CAdminControl::DrawSolid(CShape* printS)
{
	//�`�󂪔����v���肩���肵�A���v���̏ꍇ�����v����ɂ���
	if (IsClockwise(printS)) {
		ReverseShape(printS);
	}

	//control_shape��copyShape�Ƃ��ĕ���
	CVertex* nowV = printS->GetV();
	CShape* copyShape = new CShape();
	while (nowV != NULL)
	{
		copyShape->AppendVertex(nowV->GetX(), nowV->GetY());
		nowV = nowV->GetNext();
	}

	//���C�e�B���O�̐ݒ�
	float amb[4] = { 1.0,1.0,1.0,1.0 };
	float dif0[4] = { 1.0,1.0,1.0,1.0 };
	float pos0[4] = { 1.0,1.0,1.0,0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos0);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	CVertex* v1 = copyShape->GetV();
	CVertex* v2 = v1->GetNext();
	CVertex* v3 = v2->GetNext();
	float centerX, centerY;

	//�O�p�`�ɂȂ�܂ŕ���
	while (copyShape->CountVertex() >= 5)
	{
		//v1,v2,v3�ō����O�p�`�̏d�S�����߂�
		centerX = (v1->GetX() + v2->GetX() + v3->GetX()) / 3;
		centerY = (v1->GetY() + v2->GetY() + v3->GetY()) / 3;
		//�O�p�`�̏d�S���\������`��̒��ɂ����ĎO�p�`�̒��ɎO�p�`�̒��_�ȊO�̓_���Ȃ��ꍇ�ɎO�p�`��\������
		if (!Inclusion(copyShape, centerX, centerY) && TriangleInclusion(copyShape, v1, v2, v3))
		{
			glColor3f(0.65, 0.65, 0.65);
			glBegin(GL_TRIANGLES);
			glNormal3f(0, 0, -1);
			glVertex2f(v1->GetX(), v1->GetY());
			glVertex2f(v2->GetX(), v2->GetY());
			glVertex2f(v3->GetX(), v3->GetY());
			glNormal3f(0, 0, 1);
			glVertex3f(v1->GetX(), v1->GetY(), GetDepth());
			glVertex3f(v2->GetX(), v2->GetY(), GetDepth());
			glVertex3f(v3->GetX(), v3->GetY(), GetDepth());
			glEnd();
			DeleteTriangle(copyShape, v2);
		}
		else
		{
			//�O�p�`�̎n�_����i�߂�
			v1 = v2;
		}
		//�O�p�`�̎n�_�ȊO�̓_����i�߂�
		v2 = v3;
		v3 = v3->GetNext();
		if (v3->GetNext() == NULL)
		{
			v3 = copyShape->GetV();
		}
	}
	//�Ō�̎O�p�`�\��
	if (copyShape->CountVertex() == 4)
	{
		glColor3f(0.65, 0.65, 0.65);
		glBegin(GL_TRIANGLES);
		glNormal3f(0, 0, -1);
		glVertex2f(v1->GetX(), v1->GetY());
		glVertex2f(v2->GetX(), v2->GetY());
		glVertex2f(v3->GetX(), v3->GetY());
		glNormal3f(0, 0, 1);
		glVertex3f(v1->GetX(), v1->GetY(), GetDepth());
		glVertex3f(v2->GetX(), v2->GetY(), GetDepth());
		glVertex3f(v3->GetX(), v3->GetY(), GetDepth());
		glEnd();
	}

	//���������`��Ƃ��̒��̒��_�̍폜
	copyShape->GetV()->FreeVertex();
	copyShape->InitVHead();
	copyShape->FreeShape();

	float x, y, nx, ny;
	nowV = printS->GetV();
	CVertex* nextV = nowV->GetNext();
	while (nextV != NULL) {
		x = nextV->GetX() - nowV->GetX();
		y = nextV->GetY() - nowV->GetY();

		nx = y * GetDepth();
		ny = x * GetDepth() * -1;
		nx = nx / sqrtf(pow(nx, 2) + pow(ny, 2));
		ny = ny / sqrtf(pow(nx, 2) + pow(ny, 2));

		glColor3f(0.65, 0.65, 0.65);
		glBegin(GL_POLYGON);
		glNormal3f(nx,ny,0);
		glVertex3f(nowV->GetX(), nowV->GetY(), 0);
		glVertex3f(nextV->GetX(), nextV->GetY(), 0);
		glVertex3f(nextV->GetX(), nextV->GetY(), GetDepth());
		glVertex3f(nowV->GetX(), nowV->GetY(), GetDepth());
		glEnd();

		nowV = nextV;
		nextV = nextV->GetNext();
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	return;
}

//�ʈȊO�̕\��
void CAdminControl::Draw()
{
	if (viewMode && solidMode) 
	{
		CShape* nowS = shape_head;

		//�ʂ̕\�����[�h��true�̂Ƃ��������Ă���S�Ă̌`��̖ʂ�\������
		if (shape_head != NULL && shape_head->GetNextShape() != NULL)
		{
			nowS = nowS->GetNextShape();
			while (nowS != NULL)
			{
				//���̖̂ʕ\��
				DrawSolid(nowS);
				
				//���̂̕ӕ\��
				glColor3f(1.0, 1.0, 1.0);
				glLineWidth(5);
				glBegin(GL_LINE_STRIP);
				CVertex* nowV = nowS->GetV();
				while (nowV != NULL)
				{
					glVertex2f(nowV->GetX(), nowV->GetY());

					nowV = nowV->GetNext();
				}
				glEnd();

				glColor3f(1.0, 1.0, 1.0);
				glLineWidth(5);
				glBegin(GL_LINE_STRIP);
				nowV = nowS->GetV();
				while (nowV != NULL)
				{
					glVertex3f(nowV->GetX(), nowV->GetY(), GetDepth());

					nowV = nowV->GetNext();
				}
				glEnd();

				nowV = nowS->GetV();
				while (nowV != NULL)
				{
					glColor3f(1.0, 1.0, 1.0);
					glLineWidth(5);
					glBegin(GL_LINE_STRIP);
					glVertex2f(nowV->GetX(), nowV->GetY());
					glVertex3f(nowV->GetX(), nowV->GetY(),GetDepth());
					glEnd();
					nowV = nowV->GetNext();
				}

				nowS = nowS->GetNextShape();
			}
		}
	}
	else
	{
		CShape* nowS = shape_head;
		CVertex* nowV;

		//�ʂ̕\�����[�h��true�̂Ƃ��������Ă���S�Ă̌`��̖ʂ�\������
		if (shape_head != NULL && shape_head->GetNextShape() != NULL && surfaceMode)
		{
			nowS = nowS->GetNextShape();
			while (nowS != NULL)
			{
				DrawArea(nowS);
				nowS = nowS->GetNextShape();
			}
		}

		nowS = shape_head;
		while (nowS != NULL)
		{
			//���_�̕\��
			if (nowS == control_shape)
			{
				if (editMode)
				{
					glColor3f(1.0, 0.7, 0);
				}
				else if (deleteMode)
				{
					glColor3f(1.0, 0.3, 0.5);
				}
			}
			else
			{
				if (deleteMode && NowSelect() == 0)
				{
					glColor3f(1.0, 0.3, 0.5);
				}
				else
				{
					glColor3f(1.0, 1.0, 1.0);
				}
			}
			glPointSize(10);
			glBegin(GL_POINTS);

			nowV = nowS->GetV();
			while (nowV != NULL)
			{
				glVertex2f(nowV->GetX(), nowV->GetY());
				nowV = nowV->GetNext();
			}
			glEnd();

			//�ӂ̕\��
			if (nowS == control_shape)
			{
				if (editMode)
				{
					glColor3f(1.0, 0.7, 0);
				}
				else if (deleteMode)
				{
					glColor3f(1.0, 0.3, 0.5);
				}
			}
			else
			{
				if (deleteMode && NowSelect() == 0)
				{
					glColor3f(1.0, 0.3, 0.5);
				}
				else
				{
					glColor3f(1.0, 1.0, 1.0);
				}
			}
			glLineWidth(4);
			glBegin(GL_LINE_STRIP);

			nowV = nowS->GetV();
			while (nowV != NULL)
			{
				glVertex2f(nowV->GetX(), nowV->GetY());

				nowV = nowV->GetNext();
			}
			glEnd();

			nowS = nowS->GetNextShape();
		}
		//�I�����ꂽ���_�̕\��
		if (control_point != NULL)
		{
			if (editMode)
			{
				glColor3f(1.0, 0.7, 0);
			}
			else if (deleteMode)
			{
				glColor3f(1.0, 0.3, 0.5);
			}
			glPointSize(10);
			glBegin(GL_POINTS);

			glVertex2f(control_point->GetX(), control_point->GetY());

			glEnd();
		}
		//�I�����ꂽ�ӂ̕\��
		if (control_edge != NULL)
		{
			if (editMode)
			{
				glColor3f(1.0, 0.7, 0);
			}
			else if (deleteMode)
			{
				glColor3f(1.0, 0.3, 0.5);
			}
			glLineWidth(4);
			glBegin(GL_LINE_STRIP);

			nowV = control_edge;
			CVertex* nextV = nowV->GetNext();

			glVertex2f(nowV->GetX(), nowV->GetY());
			glVertex2f(nextV->GetX(), nextV->GetY());

			glEnd();
		}

		//��_�̕\��
		if (base_point != NULL)
		{
			glColor3f(0.85, 0.05, 0.85);
			glPointSize(10);
			glBegin(GL_POINTS);

			glVertex2f(base_point->GetX(), base_point->GetY());

			glEnd();
		}
	}
}

//�J�[�\���̕\��
void CAdminControl::DrawCursor(CRect rect, float x, float y)
{
	//�}�E�X�̈ʒu�̌v�Z
	/*
	double mx, my, hi;
	CPoint point;
	GetCursorPos(&point);
	HWND hWnd = FindWindow(NULL, TEXT("���� - WinOGL"));
	ScreenToClient(hWnd, &point);
	mx = (double)(point.x - 1) / rect.Width(); //X���K�����W�n
	my = (double)(point.y - 25) / rect.Height();
	my = (my - 1) * -1; //Y���K�����W�n
	mx = mx * 2 - 1; //X���[���h���W�n
	my = my * 2 - 1; //Y���[���h���W�n

	//��ʃT�C�Y�ɍ��킹��X,Y�𒲐�
	if (rect.Width() > rect.Height()) 
	{
		hi = (double)rect.Width() / rect.Height();
		mx = mx * hi;
	}
	else 
	{
		hi = (double)rect.Height() / rect.Width();
		my = my * hi;
	}
	*/

	//�E�B���h�E�T�C�Y�̌v�Z
	double wx, wy;
	wx = 1;
	wy = 1;
	if (rect.Width() > rect.Height())
	{
		wx = (double)rect.Width() / rect.Height();
	}
	else
	{
		wy = (double)rect.Height() / rect.Width();
	}

	//�J�[�\���̕\��
	glColor3f(0, 1.0, 1.0);
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2f(x, -1 * wy);
	glVertex2f(x, wy);
	glVertex2f(-1 * wx, y);
	glVertex2f(wx, y);
	glEnd();
}

//xyz���̕\��
void CAdminControl::DrawAxis()
{
	glLineWidth(2);
	glBegin(GL_LINES);
	// x��
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	// y��
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	// z��
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -1.0);
	glVertex3f(0.0, 0.0, 1.0);
	glEnd();
}

//�V�����`����쐬
void CAdminControl::AppendShape()
{
	CShape* newShape = new CShape();
	newShape->SetNextShape(shape_head);
	shape_head = newShape;
}

//�I������Ă���_��ԋp
CVertex* CAdminControl::GetControlPoint()
{
	if (control_point != NULL) 
	{
		return control_point;
	}
	
	return NULL;
}

//�I������Ă���`���ԋp
CShape* CAdminControl::GetControlShape()
{
	if (control_shape != NULL) 
	{
		return control_shape;
	}

	return NULL;
}

//�I���̉���
void CAdminControl::InitSelect()
{
	control_point = NULL;
	control_edge = NULL;
	control_shape = NULL;
	DeleteBasePoint();
}

//�_�Ɠ_�̋����v�Z
float CAdminControl::Distance(CVertex* s, float x, float y) 
{
	float d;
	float X, Y;

	X = x - s->GetX();
	Y = y - s->GetY();

	d = sqrt(pow(X, 2) + pow(Y, 2));

	return d;
}

//�_�Ɠ_�̋����v�Z
float CAdminControl::Distance(float x1, float y1, float x2, float y2)
{
	float d;
	float X, Y;

	X = x2 - x1;
	Y = y2 - y1;

	d = sqrt(pow(X, 2) + pow(Y, 2));

	return d;
}

//�_�Ɛ����̋���
float CAdminControl::PointLineDistance(CVertex* nowV, CVertex* nextV, float x, float y) 
{

	float apX = x - nowV->GetX();
	float apY = y - nowV->GetY();
	float bpX = x - nextV->GetX();
	float bpY = y - nextV->GetY();
	float baX = nowV->GetX() - nextV->GetX();
	float baY = nowV->GetY() - nextV->GetY();

	float gaiseki, naiseki, naiseki2;

	float distance, s;

	naiseki = bpX * baX + baY * bpY;
	naiseki2 = baX * baX + baY * baY;

	s = naiseki / naiseki2;

	if (0 <= s && s <= 1) 
	{
		float d = sqrt(pow(baX, 2) + pow(baY, 2));
		gaiseki = apX * baY - baX * apY;
		distance = fabs(gaiseki / d);
	}
	else if (s > 1) 
	{
		distance = Distance(nowV, x, y);
	}
	else 
	{
		distance = Distance(nextV, x, y);
	}

	return distance;
}

//�Ȃ��p�̌v�Z(����ab�ƒ���bc�̂Ȃ��p�̌v�Z)
float CAdminControl::AngleCalc(float ax, float ay, float bx, float by, float cx, float cy)
{
	float x1, y1, x2, y2, gaiseki, naiseki, nasukaku;

	x1 = ax - bx;
	y1 = ay - by;
	x2 = cx - bx;
	y2 = cy - by;

	gaiseki = x1 * y2 - x2 * y1;
	naiseki = x1 * x2 + y1 * y2;
	nasukaku = atan2(gaiseki, naiseki);

	return nasukaku;
}

//����������@true�Ȃ�������ĂȂ��Afalse�Ȃ�������Ă���
boolean CAdminControl::Cross(float mx, float my) 
{
	CVertex* As = shape_head->GetV();
	CVertex* Ae = As->GetNext();

	CVertex* Bs = Ae->GetNext();
	while (Bs->GetNext() != NULL)
	{
		Bs = Bs->GetNext();
	}

	//�`�����Ƃ��͍ŏ��̕ӂ��Ƃ΂�
	if (mx == As->GetX() && my == As->GetY()) 
	{
		As = Ae;
		Ae = Ae->GetNext();
	}

	while (Ae != Bs) 
	{
		//As,Ae��Bs,Be�̒������m�Ō�������
		if (CrossCalc(As, Ae, Bs, mx, my)) 
		{
			return false;
		}

		As = Ae;
		Ae = Ae->GetNext();
	}

	return true;
}

//��_���Ȃ������Ƒ��̒����̎���������(Bs, Be�̍��W��ʂ̒l�ɒu��������)�@true�Ȃ�������ĂȂ��Afalse�Ȃ�������Ă���
boolean CAdminControl::Cross(CShape* nowS, CVertex* Bs, CVertex* Be, float Bs_x, float Bs_y, float Be_x, float Be_y)
{
	//As,Ae���I�_�̏ꍇ�͎n�_�ɕς���
	CVertex* As = Be->GetNext();
	if (As->GetNext() == NULL) 
	{
		As = nowS->GetV();
	}
	CVertex* Ae = As->GetNext();
	if (Ae->GetNext() == NULL) 
	{
		Ae = nowS->GetV();
	}
	while (Ae != Bs) 
	{
		if (CrossCalc(As->GetX(), As->GetY(), Ae->GetX(), Ae->GetY(), Bs_x, Bs_y, Be_x, Be_y)) 
		{
			return false;
		}

		As = Ae;
		Ae = Ae->GetNext();
		if (Ae->GetNext() == NULL) 
		{
			Ae = nowS->GetV();
		}
	}

	return true;
}

//��_���Ȃ������Ƒ��̒����̎���������@true�Ȃ�������ĂȂ��Afalse�Ȃ�������Ă���
boolean CAdminControl::Cross(CShape* nowS, CVertex* Bs, CVertex* Be)
{
	//As,Ae���I�_�̏ꍇ�͎n�_�ɕς���
	CVertex* As = Be->GetNext();
	if (As->GetNext() == NULL) 
	{
		As = nowS->GetV();
	}
	CVertex* Ae = As->GetNext();
	if (Ae->GetNext() == NULL) 
	{
		Ae = nowS->GetV();
	}
	while (Ae != Bs) 
	{
		if (CrossCalc(As, Ae, Bs, Be->GetX(), Be->GetY())) 
		{
			return false;
		}

		As = Ae;
		Ae = Ae->GetNext();
		if (Ae->GetNext() == NULL) 
		{
			Ae = nowS->GetV();
		}
	}
	
	return true;
}

//����������@true�Ȃ�������ĂȂ��Afalse�Ȃ�������Ă���
boolean CAdminControl::OtherCross(float mx, float my)
{
	CShape* nowS = shape_head;
	CVertex* As;
	CVertex* Ae;
	CVertex* Bs = nowS->GetV();

	if (nowS->GetNextShape() != NULL) 
	{
		nowS = nowS->GetNextShape();
	}
	else 
	{
		return true;
	}

	while (Bs->GetNext() != NULL) 
	{
		Bs = Bs->GetNext();
	}

	while (nowS != NULL) 
	{
		As = nowS->GetV();
		Ae = As->GetNext();
		while (Ae != NULL) 
		{

			if (CrossCalc(As, Ae, Bs, mx, my)) 
			{
				return false;
			}

			As = Ae;
			Ae = Ae->GetNext();
		}

		nowS = nowS->GetNextShape();
	}

	return true;
}

//��̍��W���Ȃ������̑���������@true�Ȃ�������ĂȂ��Afalse�Ȃ�������Ă���
boolean CAdminControl::OtherCross(CShape* skipS, float Bs_x, float Bs_y, float Be_x, float Be_y)
{
	CShape* nowS = shape_head->GetNextShape();
	if (nowS == skipS)
	{
		nowS = nowS->GetNextShape();
	}
	CVertex* As;
	CVertex* Ae;
	while (nowS != NULL) 
	{
		As = nowS->GetV();
		Ae = As->GetNext();
		while (Ae != NULL) 
		{
			if (CrossCalc(As->GetX(), As->GetY(), Ae->GetX(), Ae->GetY(), Bs_x, Bs_y, Be_x, Be_y)) 
			{
				return false;
			}

			As = Ae;
			Ae = Ae->GetNext();
		}
		nowS = nowS->GetNextShape();
		if (nowS == skipS && nowS != NULL) 
		{
			nowS = nowS->GetNextShape();
		}
	}

	return true;
}

//��_���Ȃ������Ƃ̑���������@true�Ȃ�������ĂȂ��Afalse�Ȃ�������Ă���
boolean CAdminControl::OtherCross(CShape* skipS, CVertex* Bs, CVertex* Be)
{
	CShape* nowS = shape_head->GetNextShape();
	if (nowS == skipS) 
	{
		nowS = nowS->GetNextShape();
	}
	CVertex* As;
	CVertex* Ae;
	while (nowS != NULL) 
	{
		As = nowS->GetV();
		Ae = As->GetNext();
		while (Ae != NULL) 
		{
			if (CrossCalc(As, Ae, Bs, Be->GetX(), Be->GetY())) 
			{
				return false;
			}

			As = Ae;
			Ae = Ae->GetNext();
		}
		nowS = nowS->GetNextShape();
		if (nowS == skipS && nowS != NULL) 
		{
			nowS = nowS->GetNextShape();
		}
	}

	return true;
}

//�����v�Z  true�Ȃ�������Ă�Afalse�Ȃ�������Ă��Ȃ�
boolean CAdminControl::CrossCalc(CVertex* As, CVertex* Ae, CVertex* Bs, float mx, float my)
{
	CVertex a, b, a1, b1, a2, b2;
	float ca1, ca2, cb1, cb2;

	//a
	a.SetX(Ae->GetX() - As->GetX());
	a.SetY(Ae->GetY() - As->GetY());
	//b
	b.SetX(mx - Bs->GetX());
	b.SetY(my - Bs->GetY());
	//a1
	a1.SetX(Bs->GetX() - As->GetX());
	a1.SetY(Bs->GetY() - As->GetY());
	//b1
	b1.SetX(As->GetX() - Bs->GetX());
	b1.SetY(As->GetY() - Bs->GetY());
	//a2
	a2.SetX(mx - As->GetX());
	a2.SetY(my - As->GetY());
	//b2
	b2.SetX(Ae->GetX() - Bs->GetX());
	b2.SetY(Ae->GetY() - Bs->GetY());

	ca1 = a.GetX() * a1.GetY() - a1.GetX() * a.GetY();
	ca2 = a.GetX() * a2.GetY() - a2.GetX() * a.GetY();
	cb1 = b.GetX() * b1.GetY() - b1.GetX() * b.GetY();
	cb2 = b.GetX() * b2.GetY() - b2.GetX() * b.GetY();

	if (ca1 * ca2 <= 0 && cb1 * cb2 <= 0) 
	{
		return true;
	}

	return false;
}

//�����v�Z  true�Ȃ�������Ă�Afalse�Ȃ�������Ă��Ȃ�
boolean CAdminControl::CrossCalc(float  As_x, float As_y, float Ae_x, float Ae_y, float Bs_x, float Bs_y, float Be_x, float Be_y)
{
	CVertex a, b, a1, b1, a2, b2;
	float ca1, ca2, cb1, cb2;

	//a
	a.SetX(Ae_x - As_x);
	a.SetY(Ae_y - As_y);
	//b
	b.SetX(Be_x - Bs_x);
	b.SetY(Be_y - Bs_y);
	//a1
	a1.SetX(Bs_x - As_x);
	a1.SetY(Bs_y - As_y);
	//b1
	b1.SetX(As_x - Bs_x);
	b1.SetY(As_y - Bs_y);
	//a2
	a2.SetX(Be_x - As_x);
	a2.SetY(Be_y - As_y);
	//b2
	b2.SetX(Ae_x - Bs_x);
	b2.SetY(Ae_y - Bs_y);

	ca1 = a.GetX() * a1.GetY() - a1.GetX() * a.GetY();
	ca2 = a.GetX() * a2.GetY() - a2.GetX() * a.GetY();
	cb1 = b.GetX() * b1.GetY() - b1.GetX() * b.GetY();
	cb2 = b.GetX() * b2.GetY() - b2.GetX() * b.GetY();

	if (ca1 * ca2 <= 0 && cb1 * cb2 <= 0) 
	{
		return true;
	}

	return false;
}

//�_�̓����(�S�Ă̌`��Ŕ��肷��)�@true�Ȃ�O�Afalse�Ȃ��
boolean CAdminControl::Inclusion(float x, float y)
{
	CShape* nowS = shape_head;
	CVertex* nowV;
	CVertex* nextV;

	float ax, ay, bx, by;
	float sum, gaiseki, naiseki;

	if (nowS->GetNextShape() != NULL) 
	{
		nowS = nowS->GetNextShape();
	}
	else 
	{
		return true;
	}

	while (nowS != NULL) 
	{
		nowV = nowS->GetV();
		nextV = nowV->GetNext();
		sum = 0;

		while (nextV != NULL) 
		{
			ax = nowV->GetX() - x;
			ay = nowV->GetY() - y;
			bx = nextV->GetX() - x;
			by = nextV->GetY() - y;

			gaiseki = ax * by - bx * ay;
			naiseki = ax * bx + ay * by;
			float tan = atan2(gaiseki, naiseki);
			sum = sum + tan;

			nowV = nextV;
			nextV = nextV->GetNext();
		}
		if (sum < 0)
		{
			sum *= -1;
		}
		if ((2 * pi - sum) < 0.0001) 
		{
			return false;
		}
		nowS = nowS->GetNextShape();
	}

	return true;
}

//�_�̓����(��̌`��Ŕ��肷��)�@true�Ȃ�O�Afalse�Ȃ��
boolean CAdminControl::Inclusion(CShape* nowS, float x, float y)
{
	if (nowS == NULL && control_shape == NULL) 
	{
		return true;
	}
	else if (nowS == NULL) 
	{
		nowS = control_shape;
	}

	CVertex* nowV = nowS->GetV();
	CVertex* nextV = nowV->GetNext();

	float ax, ay, bx, by;
	float sum, gaiseki, naiseki;

	sum = 0;

	while (nextV != NULL) 
	{
		ax = nowV->GetX() - x;
		ay = nowV->GetY() - y;
		bx = nextV->GetX() - x;
		by = nextV->GetY() - y;

		gaiseki = ax * by - bx * ay;
		naiseki = ax * bx + ay * by;
		float tan = atan2(gaiseki, naiseki);
		sum = sum + tan;

		nowV = nextV;
		nextV = nextV->GetNext();
	}
	if (sum < 0) 
	{
		sum *= -1;
	}
	if ((2 * pi - sum) < 0.0001) 
	{
		return false;
	}

	return true;
}

//�`��̎O�p�`�����̒��Ɍ`����̑��̒��_�����݂��邩�̔��� true�Ȃ�O�p�`�����ɒ��_�����Afalse�Ȃ�O�p�`�����ɒ��_�L��
boolean CAdminControl::TriangleInclusion(CShape* nowS, CVertex* v1, CVertex* v2, CVertex* v3)
{
	float ax, ay, bx, by, cx, cy;
	float sum = 0, gaiseki, naiseki, tan;
	
	CVertex* nowV = nowS->GetV();
	if (nowV == v1) 
	{
		nowV = nowV->GetNext();
	}
	if (nowV == v2) 
	{
		nowV = nowV->GetNext();
	}
	if (nowV == v3) 
	{
		nowV = nowV->GetNext();
	}
	//�`��̏I�_�̈�O�̓_�܂Ō���(�n�_�ƏI�_�������ʒu�ł��邽��)
	while (nowV->GetNext() != NULL) 
	{
		ax = v1->GetX() - nowV->GetX();
		ay = v1->GetY() - nowV->GetY();
		bx = v2->GetX() - nowV->GetX();
		by = v2->GetY() - nowV->GetY();
		cx = v3->GetX() - nowV->GetX();
		cy = v3->GetY() - nowV->GetY();

		gaiseki = ax * by - bx * ay;
		naiseki = ax * bx + ay * by;
		tan = atan2(gaiseki, naiseki);
		sum = sum + tan;

		gaiseki = bx * cy - cx * by;
		naiseki = bx * cx + by * cy;
		tan = atan2(gaiseki, naiseki);
		sum = sum + tan;

		gaiseki = cx * ay - ax * cy;
		naiseki = cx * ax + cy * ay;
		tan = atan2(gaiseki, naiseki);
		sum = sum + tan;

		if (2 * pi - fabs(sum) < 0.0001)
		{
			return false;
		}

		nowV = nowV->GetNext();
		//�O�p�`�̎O�_�͌��Ȃ��̂łƂ΂�
		if (nowV == v1) 
		{
			nowV = nowV->GetNext();
		}
		if (nowV == v2) 
		{
			nowV = nowV->GetNext();
		}
		if (nowV == v3) 
		{
			nowV = nowV->GetNext();
		}
	}

	return true;
}

//�`��̎O�p�`�����̒��ɎO�p�`�̎O�_���������̒��_�����݂��Ă��邩�̔���@true�Ȃ�O�p�`�����ɒ��_�����Afalse�Ȃ�O�p�`�����ɒ��_�L��
boolean CAdminControl::TriangleInclusion(CVertex* v1, CVertex* v2, CVertex* v3)
{
	float ax, ay, bx, by, cx, cy;
	float sum = 0, gaiseki, naiseki, tan;
	
	CShape* nowS = shape_head->GetNextShape();
	CVertex* nowV;

	while (nowS != NULL) 
	{
		nowV = nowS->GetV();
		if (nowV == v1) 
		{
			nowV = nowV->GetNext();
		}
		if (nowV == v2) 
		{
			nowV = nowV->GetNext();
		}
		if (nowV == v3) 
		{
			nowV = nowV->GetNext();
		}
		//�`��̏I�_�̈�O�̓_�܂Ō���(�n�_�ƏI�_�������ʒu�ł��邽��)
		while (nowV->GetNext() != NULL) 
		{
			ax = v1->GetX() - nowV->GetX();
			ay = v1->GetY() - nowV->GetY();
			bx = v2->GetX() - nowV->GetX();
			by = v2->GetY() - nowV->GetY();
			cx = v3->GetX() - nowV->GetX();
			cy = v3->GetY() - nowV->GetY();

			gaiseki = ax * by - bx * ay;
			naiseki = ax * bx + ay * by;
			tan = atan2(gaiseki, naiseki);
			sum = sum + tan;

			gaiseki = bx * cy - cx * by;
			naiseki = bx * cx + by * cy;
			tan = atan2(gaiseki, naiseki);
			sum = sum + tan;

			gaiseki = cx * ay - ax * cy;
			naiseki = cx * ax + cy * ay;
			float tan = atan2(gaiseki, naiseki);
			sum = sum + tan;

			if (2 * pi - fabs(sum) < 0.0001) 
			{
				return false;
			}

			nowV = nowV->GetNext();
			//�O�p�`�̎O�_�͌��Ȃ��̂łƂ΂�
			if (nowV == v1) 
			{
				nowV = nowV->GetNext();
			}
			if (nowV == v2) 
			{
				nowV = nowV->GetNext();
			}
			if (nowV == v3) 
			{
				nowV = nowV->GetNext();
			}
		}
		nowS = nowS->GetNextShape();
	}

	return true;
}

//�`�󂪎��v���ɕ��Ă��邩���肷�� ���v����Ȃ�true�A�����v����Ȃ�false
boolean CAdminControl::IsClockwise(CShape* checkS)
{
	CVertex* nowV = checkS->GetV();
	CVertex* nextV = nowV->GetNext();

	float sum = 0;

	while (nextV != NULL) 
	{
		sum = sum + (nowV->GetX() * nextV->GetY() - nextV->GetX() * nowV->GetY());
		
		nowV = nowV->GetNext();
		nextV = nextV->GetNext();
	}
	//fabs(sum)/2 �Ŗʐς����܂�
	if (sum < 0) 
	{
		return true;
	}

	return false;
}

//�`��̓���� true�Ȃ����Ă��Ȃ��Afalse�Ȃ����Ă���
boolean CAdminControl::Wrap()
{
	CShape* nowS = shape_head;
	//nowS(�`��)�̎n�_
	CVertex* nowSV;
	CVertex* nowV;
	CVertex* nextV;

	float ax, ay, bx, by;
	float naiseki, gaiseki, sum;

	if (nowS->GetNextShape() != NULL) 
	{
		nowS = nowS->GetNextShape();
	}
	else 
	{
		return true;
	}
	
	while (nowS != NULL)
	{
		nowV = shape_head->GetV();
		nextV = nowV->GetNext();
		nowSV = nowS->GetV();
		sum = 0;
		while (nowV != NULL) 
		{
			ax = nowV->GetX() - nowSV->GetX();
			ay = nowV->GetY() - nowSV->GetY();
			bx = nextV->GetX() - nowSV->GetX();
			by = nextV->GetY() - nowSV->GetY();

			gaiseki = ax * by - bx * ay;
			naiseki = ax * bx + ay * by;
			float tan = atan2(gaiseki, naiseki);
			sum = sum + tan;

			nowV = nowV->GetNext();
			nextV = nextV->GetNext();
			if (nextV == NULL) 
			{
				nextV = shape_head->GetV();
			}
		}
		sum = fabs(sum);

		if ((2 * pi - sum) < 0.0001) 
		{
			return false;
		}
		nowS = nowS->GetNextShape();
	}

	return true;
}

//�`��̓����(�_�̈ړ��Ŏg�p,v2�̍��W��mx,my�ɒu��������)
boolean CAdminControl::Wrap(CShape* checkS, CVertex* v2, float mx, float my)
{
	CShape* nowS = shape_head->GetNextShape();
	if (nowS == checkS) 
	{
		nowS = nowS->GetNextShape();
	}
	//nowS(�`��)�̎n�_
	CVertex* nowSV;
	CVertex* nowV;
	CVertex* nextV;

	float ax, ay, bx, by;
	float naiseki, gaiseki, sum;

	while (nowS != NULL) 
	{
		nowV = checkS->GetV();
		nextV = nowV->GetNext();
		nowSV = nowS->GetV();
		sum = 0;
		while (nowV->GetNext() != NULL) 
		{
			if (nowV == v2) 
			{
				ax = mx - nowSV->GetX();
				ay = my - nowSV->GetY();
			}
			else 
			{
				ax = nowV->GetX() - nowSV->GetX();
				ay = nowV->GetY() - nowSV->GetY();
			}
			if (nextV == v2) 
			{
				bx = mx - nowSV->GetX();
				by = my - nowSV->GetY();
			}
			else 
			{
				bx = nextV->GetX() - nowSV->GetX();
				by = nextV->GetY() - nowSV->GetY();
			}

			gaiseki = ax * by - bx * ay;
			naiseki = ax * bx + ay * by;
			float tan = atan2(gaiseki, naiseki);
			sum = sum + tan;

			nowV = nowV->GetNext();
			nextV = nextV->GetNext();
			if (nextV->GetNext() == NULL) 
			{
				nextV = checkS->GetV();
			}
		}
		sum = fabs(sum);

		if ((2 * pi - sum) < 0.0001)
		{
			return false;
		}
		nowS = nowS->GetNextShape();
		if (nowS == checkS) 
		{
			nowS = nowS->GetNextShape();
		}
	}

	return true;
}

//�`��̓����
boolean CAdminControl::Wrap(CShape* checkS)
{
	CShape* nowS = shape_head->GetNextShape();
	if (nowS == checkS) 
	{
		nowS = nowS->GetNextShape();
	}
	//nowS(�`��)�̎n�_
	CVertex* nowSV;
	CVertex* nowV;
	CVertex* nextV;

	float ax, ay, bx, by;
	float naiseki, gaiseki, sum;

	while (nowS != NULL) 
	{
		nowV = checkS->GetV();
		nextV = nowV->GetNext();
		nowSV = nowS->GetV();
		sum = 0;
		while (nowV->GetNext() != NULL) 
		{
			ax = nowV->GetX() - nowSV->GetX();
			ay = nowV->GetY() - nowSV->GetY();
			bx = nextV->GetX() - nowSV->GetX();
			by = nextV->GetY() - nowSV->GetY();

			gaiseki = ax * by - bx * ay;
			naiseki = ax * bx + ay * by;
			float tan = atan2(gaiseki, naiseki);
			sum = sum + tan;

			nowV = nowV->GetNext();
			nextV = nextV->GetNext();
			if (nextV->GetNext() == NULL)
			{
				nextV = checkS->GetV();
			}
		}
		sum = fabs(sum);

		if ((2 * pi - sum) < 0.0001) 
		{
			return false;
		}
		nowS = nowS->GetNextShape();
		if (nowS == checkS) 
		{
			nowS = nowS->GetNextShape();
		}
	}

	return true;
}

//���_�̒ǉ�
void CAdminControl::CreateShape(float x, float y)
{
	//��ԍŏ��N���b�N���ꂽ�Ƃ���shape����
	if (shape_head == NULL) 
	{
		AppendShape();
	}

	//��O�̓_�̈ʒu�ƃN���b�N�����ʒu���r����
	if (shape_head->CountVertex() >= 1) 
	{
		CVertex* endV = shape_head->GetV();
		while (endV->GetNext() != NULL) 
		{
			endV = endV->GetNext();
		}
		//�S�������ꏊ�ɑł����ꍇreturn
		if (endV->GetX() == x && endV->GetY() == y) 
		{
			return;
		}
	}
	//�_��3�_�����̏ꍇ
	if (shape_head->CountVertex() < 3) 
	{
		//��_�ڂ��ǂ���
		if (shape_head->CountVertex() == 0) 
		{
			if (Inclusion(x, y)) 
			{
				shape_head->AppendVertex(x, y);
			}
		}
		else 
		{
			//��_�ڂƍ���łO�_�ڂ��߂��ꍇ�A�O�_�ڂ�ǉ�������return
			if (shape_head->CountVertex() == 2 && Distance(shape_head->GetV(), x, y) <= point_dist) 
			{
				return;
			}
			//���������Ă��Ȃ���Γ_�̒ǉ�
			if (OtherCross(x, y)) 
			{
				shape_head->AppendVertex(x, y);
			}
		}
	}
	//4�_�ڈȍ~�ŃN���b�N�����ʒu�Ǝn�_���߂��ꍇ
	else if (Distance(shape_head->GetV(), x, y) <= 0.1) 
	{
		float sx = shape_head->GetV()->GetX();
		float sy = shape_head->GetV()->GetY();
		
		//�n�_�Ɍ����Đ����������Ƃ��A�����������������`��̓�������Ă��Ȃ���Ύn�_�̈ʒu�ɓ_��ǉ����Č`������
		if (OtherCross(sx, sy) && Cross(sx, sy) && Wrap()) 
		{
			shape_head->AppendVertex(sx, sy);
			AppendShape();
		}
		//�����������������ĂȂ���Γ_��ǉ�
		else if (OtherCross(x, y) && Cross(x, y)) 
		{
			shape_head->AppendVertex(x, y);
		}
	}
	//4�_�ڈȍ~
	else 
	{
		//�����������������ĂȂ���Γ_��ǉ�
		if (OtherCross(x, y) && Cross(x, y)) 
		{
			shape_head->AppendVertex(x, y);
		}
	}
}

//�_�̍폜
void CAdminControl::DeletePoint(CVertex* deleteV)
{
	if (deleteV == NULL && control_point == NULL) 
	{
		return;
	}
	CShape* nowS = shape_head->GetNextShape();
	CVertex* nowV = NULL;
	CVertex* preV = NULL;

	if (deleteV == NULL) 
	{
		deleteV = control_point;
	}

	while (nowS != NULL) 
	{
		nowV = nowS->GetV();
		while (nowV != NULL) 
		{
			//�`��̒��_��4�_�ȉ��̏ꍇ�폜���Ȃ�
			if (nowS->CountVertex() <= 4) 
			{
				break;
			}
			if (nowV == deleteV)
			{
				//�I�����ꂽ�_���`��̎n�_�̏ꍇ
				if (preV == NULL) 
				{
					while (nowV->GetNext() != NULL) 
					{
						preV = nowV;
						nowV = nowV->GetNext();
					}
					//�I�����ꂽ�_���폜���Ă悢���̔���
					if (TriangleInclusion(preV, deleteV, deleteV->GetNext())) 
					{
						nowS->ChangeVHead(deleteV->GetNext());
						delete nowV;
						delete deleteV;
						preV->SetNext(NULL);
						nowS->AppendVertex(nowS->GetV()->GetX(), nowS->GetV()->GetY());
						control_point = NULL;
					}
				}
				//�I�_��O�̓_���I������Ă���ꍇ&&�I�����ꂽ�_���폜���Ă悢���̔���
				else if (deleteV->GetNext()->GetNext() == NULL && TriangleInclusion(preV, deleteV, nowS->GetV())) 
				{
					preV->SetNext(deleteV->GetNext());
					delete deleteV;
					control_point = NULL;
				}
				//����ȊO�̓_���I������Ă���ꍇ&&�I�����ꂽ�_���폜���Ă悢���̔���
				else if (deleteV->GetNext()->GetNext() != NULL && TriangleInclusion(preV, deleteV, deleteV->GetNext())) 
				{
					preV->SetNext(deleteV->GetNext());
					delete deleteV;
					control_point = NULL;
				}
				return;
			}
			preV = nowV;
			nowV = nowV->GetNext();
		}
		preV = NULL;
		nowS = nowS->GetNextShape();
	}

	return;
}

//�O�p�`�������̎O�p�`�̍폜
void CAdminControl::DeleteTriangle(CShape* printS, CVertex* deleteV)
{
	CVertex* nowV = printS->GetV();
	CVertex* preV = NULL;

	while (nowV != NULL) 
	{
		if (nowV == deleteV) 
		{
			//�I�����ꂽ�_���`��̎n�_�̏ꍇ
			if (preV == NULL) 
			{
				while (nowV->GetNext() != NULL) 
				{
					preV = nowV;
					nowV = nowV->GetNext();
				}
				printS->ChangeVHead(deleteV->GetNext());
				delete nowV;
				delete deleteV;
				preV->SetNext(NULL);
				printS->AppendVertex(printS->GetV()->GetX(), printS->GetV()->GetY());
			}
			//�n�_�ȊO�̓_���I������Ă���ꍇ
			else 
			{
				preV->SetNext(deleteV->GetNext());
				delete deleteV;
			}
			return;
		}
		preV = nowV;
		nowV = nowV->GetNext();
	}

	return;
}

//���_�̈ړ�
void CAdminControl::MovePoint(CVertex* moveV, float x, float y)
{

	if (moveV == NULL && control_point == NULL) 
	{
		return;
	}
	if (moveV == NULL) 
	{
		moveV = control_point;
	}

	CShape* nowS = shape_head->GetNextShape();
	CVertex* v1 = NULL;
	CVertex* v2 = NULL;
	CVertex* v3 = NULL;

	while (nowS != NULL) 
	{
		v2 = nowS->GetV();
		v3 = v2->GetNext();
		while (v2->GetNext() != NULL) 
		{
			if (v2 == moveV) 
			{
				//�I�����ꂽ�_���`��̎n�_�̏ꍇ
				if (v1 == NULL) 
				{
					v1 = v2;
					while (v1->GetNext()->GetNext() != NULL)
					{
						v1 = v1->GetNext();
					}
					if (Cross(nowS, v1, v2, v1->GetX(), v1->GetY(), x, y) && Cross(nowS, v2, v3, x, y, v3->GetX(), v3->GetY()) && 
						OtherCross(nowS,v1->GetX(), v1->GetY(), x, y) && OtherCross(nowS, x, y, v3->GetX(), v3->GetY()) && Wrap(nowS, v2, x, y))
					{
						v2->SetX(x);
						v2->SetY(y);
						v1->GetNext()->SetX(x);
						v1->GetNext()->SetY(y);
					}
				}
				//�I�_��O�̓_���I������Ă���ꍇ
				else if (v3->GetNext() == NULL) 
				{
					v3 = nowS->GetV();
					if (Cross(nowS, v1, v2, v1->GetX(), v1->GetY(), x, y) && Cross(nowS, v2, v3, x, y, v3->GetX(), v3->GetY()) && 
						OtherCross(nowS, v1->GetX(), v1->GetY(), x, y) && OtherCross(nowS, x, y, v3->GetX(), v3->GetY()) && Wrap(nowS, v2, x, y)) 
					{
						v2->SetX(x);
						v2->SetY(y);
					}
				}
				//����ȊO�̓_���I������Ă���ꍇ
				else 
				{
					if (Cross(nowS, v1, v2, v1->GetX(), v1->GetY(), x, y) && Cross(nowS, v2, v3, x, y, v3->GetX(), v3->GetY()) && 
						OtherCross(nowS, v1->GetX(), v1->GetY(), x, y) && OtherCross(nowS, x, y, v3->GetX(), v3->GetY()) && Wrap(nowS, v2, x, y)) 
					{
						v2->SetX(x);
						v2->SetY(y);
					}
				}
				return;
			}
			v1 = v2;
			v2 = v3;
			v3 = v3->GetNext();
		}
		v1 = NULL;
		v2 = NULL;
		v3 = NULL;
		nowS = nowS->GetNextShape();
	}
	
	return;
}

//���_�̈ړ�
/*void CAdminControl::MovePoint(CVertex* moveV, float x, float y)
{
	if (moveV == NULL && control_point == NULL) 
	{
		return;
	}

	if (moveV == NULL) 
	{
		moveV = control_point;
	}

	CShape* nowS = shape_head->GetNextShape();
	CVertex* v1 = NULL;
	CVertex* v2 = NULL;
	CVertex* v3 = NULL;

	float oldX, oldY;

	while (nowS != NULL)
	{
		v2 = nowS->GetV();
		v3 = v2->GetNext();
		while (v2->GetNext() != NULL) 
		{
			if (v2 == moveV) 
			{
				oldX = v2->GetX();
				oldY = v2->GetY();
				v2->SetX(x);
				v2->SetY(y);
				//�I�����ꂽ�_���`��̎n�_�̏ꍇ
				if (v1 == NULL)
				{
					v1 = v2;
					while (v1->GetNext()->GetNext() != NULL) 
					{
						v1 = v1->GetNext();
					}
					v1->GetNext()->SetX(x);
					v1->GetNext()->SetY(y);
					if (!Cross(nowS, v1, v2) || !Cross(nowS, v2, v3) || !OtherCross(nowS, v1, v2) || !OtherCross(nowS, v2, v3) || !Wrap(nowS)) 
					{
						v2->SetX(oldX);
						v2->SetY(oldY);
						v1->GetNext()->SetX(oldX);
						v1->GetNext()->SetY(oldY);
					}
				}
				//�I�_��O�̓_���I������Ă���ꍇ
				else if (v3->GetNext() == NULL) 
				{
					v3 = nowS->GetV();
					if (!Cross(nowS, v1, v2) || !Cross(nowS, v2, v3) || !OtherCross(nowS, v1, v2) || !OtherCross(nowS, v2, v3) || !Wrap(nowS)) 
					{
						v2->SetX(oldX);
						v2->SetY(oldY);
					}
				}
				//����ȊO�̓_���I������Ă���ꍇ
				else 
				{
					if (!Cross(nowS, v1, v2) || !Cross(nowS, v2, v3) || !OtherCross(nowS, v1, v2) || !OtherCross(nowS, v2, v3) || !Wrap(nowS)) 
					{
						v2->SetX(oldX);
						v2->SetY(oldY);
					}
				}
				return;
			}
			v1 = v2;
			v2 = v3;
			v3 = v3->GetNext();
		}
		v1 = NULL;
		v2 = NULL;
		v3 = NULL;
		nowS = nowS->GetNextShape();
	}

	return;
}*/

//�`��̈ړ�(x, y���`��̏d�S�ɂȂ�)
void CAdminControl::MoveShape(CShape* moveS, float x, float y)
{
	if (moveS == NULL && control_shape == NULL) 
	{
		return;
	}
	if (moveS == NULL) 
	{
		moveS = control_shape;
	}
	if (moveS->CountVertex() < 4)
	{
		return;
	}

	float centerX, centerY, moveX, moveY, sumX = 0, sumY = 0;

	CVertex* nowV = moveS->GetV();
	//�d�S�����߂�
	while (nowV->GetNext() != NULL) 
	{
		sumX = sumX + nowV->GetX();
		sumY = sumY + nowV->GetY();
		nowV = nowV->GetNext();
	}
	centerX = sumX / (moveS->CountVertex()-1);
	centerY = sumY / (moveS->CountVertex()-1);

	//�ړ��ʂ����߂�
	moveX = x - centerX;
	moveY = y - centerY;

	nowV = moveS->GetV();
	//�S�Ă̒��_����x�}�E�X�̈ړ����ړ�������
	while (nowV != NULL) 
	{
		nowV->SetX(nowV->GetX() + moveX);
		nowV->SetY(nowV->GetY() + moveY);
		nowV = nowV->GetNext();
	}

	//�`��ɓ����Ă��Ȃ������肷��
	nowV = moveS->GetV();
	CShape* nowS = shape_head->GetNextShape();
	if (nowS == moveS) 
	{
		nowS = nowS->GetNextShape();
	}
	while (nowS != NULL) 
	{
		if (!Inclusion(nowS, moveS->GetV()->GetX(), moveS->GetV()->GetY())) 
		{
			while (nowV != NULL) 
			{
				nowV->SetX(nowV->GetX() - moveX);
				nowV->SetY(nowV->GetY() - moveY);
				nowV = nowV->GetNext();
			}
			return;
		}
		nowS = nowS->GetNextShape();
		if (nowS == moveS) 
		{
			nowS = nowS->GetNextShape();
		}
	}

	//�`������Ă��Ȃ������肷��
	if (!Wrap(moveS)) 
	{
		while (nowV != NULL)
		{
			nowV->SetX(nowV->GetX() - moveX);
			nowV->SetY(nowV->GetY() - moveY);
			nowV = nowV->GetNext();
		}
		return;
	}

	//�ړ�������`��̑S�Ă̕ӂ����������Ă��Ȃ������肷��
	CVertex* nextV = nowV->GetNext();
	while (nowV->GetNext() != NULL) 
	{	
		if (!OtherCross(moveS, nowV, nextV)) 
		{
			nowV = moveS->GetV();
			while (nowV != NULL) 
			{
				nowV->SetX(nowV->GetX() - moveX);
				nowV->SetY(nowV->GetY() - moveY);
				nowV = nowV->GetNext();
			}
			return;
		}
		nowV = nowV->GetNext();
		nextV = nextV->GetNext();
		if (nextV->GetNext() == NULL)
		{
			nextV = moveS->GetV();
		}
	}

	return;
}

//�ӏ�ɓ_��ǉ�����
void CAdminControl::DivideEdge(CVertex* v1, float x, float y)
{
	if (control_edge == NULL && v1 == NULL) 
	{
		return;
	}
	if (v1 == NULL) 
	{
		v1 = control_edge;
	}
	if (v1->GetNext() == NULL) 
	{
		return;
	}

	CVertex* v2 = v1->GetNext();
	CShape* nowS = shape_head->GetNextShape();
	CVertex* nowV;
	//�I�������ӂ��܂܂�Ă���`���T��
	while (nowS != NULL) 
	{
		nowV = nowS->GetV();
		while (nowV != NULL) 
		{
			//nowV���I�������_�������ꍇ
			if (nowV == v1) 
			{
				//�N���b�N�������W���߂��ꍇ
				if (PointLineDistance(v1, v2, x, y) <= select_dist) 
				{
					float apX = x - v1->GetX();
					float apY = y - v1->GetY();
					float bpX = x - v2->GetX();
					float bpY = y - v2->GetY();
					float baX = v1->GetX() - v2->GetX();
					float baY = v1->GetY() - v2->GetY();

					float naiseki, naiseki2, s;

					naiseki = bpX * baX + baY * bpY;
					naiseki2 = baX * baX + baY * baY;

					s = naiseki / naiseki2;
					if (0 <= s && s <= 1) 
					{
						float newX = v1->GetX() + (v2->GetX() - v1->GetX()) * (1-s);
						float newY = v1->GetY() + (v2->GetY() - v1->GetY()) * (1-s);
						CVertex* newVertex = new CVertex(newX, newY);
						newVertex->SetNext(v1->GetNext());
						v1->SetNext(newVertex);
						control_edge = NULL;
					}
				}
				
				return;
			}
			nowV = nowV->GetNext();
		}
		nowS = nowS->GetNextShape();
	}

	return;
}

//��_�̕ԋp
CVertex* CAdminControl::GetBasePoint()
{
	if (base_point != NULL) 
	{
		return base_point;
	}

	return NULL;
}

//��_�̒ǉ�
void CAdminControl::AddBasePoint(float x, float y)
{
	if (editMode) 
	{
		CVertex* nowV = control_shape->GetV();
		float sumX = 0, sumY = 0, aveX = 0, aveY = 0;
		while (nowV->GetNext() != NULL) 
		{
			sumX += nowV->GetX();
			sumY += nowV->GetY();

			nowV = nowV->GetNext();
		}
		aveX = sumX / (control_shape->CountVertex() - 1);
		aveY = sumY / (control_shape->CountVertex() - 1);

		if (Distance(aveX, aveY, x, y) < 0.0001) 
		{
			return;
		}
	}

	if (base_point == NULL) 
	{
		CVertex* new_point = new CVertex(x, y);
		base_point = new_point;
	}
	else 
	{
		base_point->SetXY(x, y);
	}

	return;
}

//��_�̍폜
void CAdminControl::DeleteBasePoint()
{
	if (base_point != NULL) 
	{
		CVertex* del = base_point;
		base_point = NULL;
		delete del;
	}

	return;
}

//�`��̉�]�ړ�
void CAdminControl::RotateShape(CShape* rotateS,float baseX, float baseY, float x, float y)
{
	if (rotateS == NULL && control_shape == NULL) 
	{
		return;
	}
	if (rotateS == NULL) 
	{
		rotateS = control_shape;
	}
	if (rotateS->CountVertex() < 4) 
	{
		return;
	}

	float centerX, centerY, sumX = 0, sumY = 0, newX, newY, nasukaku;

	CVertex* nowV = rotateS->GetV();
	//�d�S�����߂�
	while (nowV->GetNext() != NULL) 
	{
		sumX = sumX + nowV->GetX();
		sumY = sumY + nowV->GetY();
		nowV = nowV->GetNext();
	}
	centerX = sumX / (rotateS->CountVertex() - 1);
	centerY = sumY / (rotateS->CountVertex() - 1);

	//��_����d�S�܂ł̒����Ɗ�_����}�E�X�̍��W�܂ł̒����̂Ȃ��p�����߂�
	nasukaku = AngleCalc(centerX, centerY, baseX, baseY, x, y);

	nowV = rotateS->GetV();
	//�S�Ă̒��_��nasukaku����]������
	while (nowV != NULL) 
	{
		newX = (nowV->GetX() - baseX) * cos(nasukaku) - (nowV->GetY() - baseY) * sin(nasukaku) + baseX;
		newY = (nowV->GetX() - baseX) * sin(nasukaku) + (nowV->GetY() - baseY) * cos(nasukaku) + baseY;
		nowV->SetX(newX);
		nowV->SetY(newY);
		nowV = nowV->GetNext();
	}

	//�`��ɓ����Ă��Ȃ������肷��
	nowV = rotateS->GetV();
	CShape* nowS = shape_head->GetNextShape();
	if (nowS == rotateS) 
	{
		nowS = nowS->GetNextShape();
	}
	while (nowS != NULL) 
	{
		if (!Inclusion(nowS, rotateS->GetV()->GetX(), rotateS->GetV()->GetY())) 
		{
			while (nowV != NULL) 
			{
				newX = (nowV->GetX() - baseX) * cos(-1 * nasukaku) - (nowV->GetY() - baseY) * sin(-1 * nasukaku) + baseX;
				newY = (nowV->GetX() - baseX) * sin(-1 * nasukaku) + (nowV->GetY() - baseY) * cos(-1* nasukaku) + baseY;
				nowV->SetX(newX);
				nowV->SetY(newY);
				nowV = nowV->GetNext();
			}
			return;
		}
		nowS = nowS->GetNextShape();
		if (nowS == rotateS) 
		{
			nowS = nowS->GetNextShape();
		}
	}

	//�`������Ă��Ȃ������肷��
	if (!Wrap(rotateS)) 
	{
		while (nowV != NULL) 
		{
			newX = (nowV->GetX() - baseX) * cos(-1 * nasukaku) - (nowV->GetY() - baseY) * sin(-1 * nasukaku) + baseX;
			newY = (nowV->GetX() - baseX) * sin(-1 * nasukaku) + (nowV->GetY() - baseY) * cos(-1 * nasukaku) + baseY;
			nowV->SetX(newX);
			nowV->SetY(newY);
			nowV = nowV->GetNext();
		}
		return;
	}

	//�ړ�������`��̑S�Ă̕ӂ����������Ă��Ȃ������肷��
	CVertex* nextV = nowV->GetNext();
	while (nowV->GetNext() != NULL) 
	{
		if (!OtherCross(rotateS, nowV, nextV)) 
		{
			nowV = rotateS->GetV();
			while (nowV != NULL) 
			{
				newX = (nowV->GetX() - baseX) * cos(-1 * nasukaku) - (nowV->GetY() - baseY) * sin(-1 * nasukaku) + baseX;
				newY = (nowV->GetX() - baseX) * sin(-1 * nasukaku) + (nowV->GetY() - baseY) * cos(-1 * nasukaku) + baseY;
				nowV->SetX(newX);
				nowV->SetY(newY);
				nowV = nowV->GetNext();
			}
			return;
		}
		nowV = nowV->GetNext();
		nextV = nextV->GetNext();
		if (nextV->GetNext() == NULL)
		{
			nextV = rotateS->GetV();
		}
	}

	return;
}

//�`��̊g��k��
void CAdminControl::ResizeShape(CShape* resizeS, float baseX, float baseY, short zDelta)
{
	if (resizeS == NULL && control_shape == NULL) 
	{
		return;
	}
	if (resizeS == NULL) 
	{
		resizeS = control_shape;
	}
	if (resizeS->CountVertex() < 4) 
	{
		return;
	}

	CVertex* nowV;
	int angle = (int)zDelta / 15, i = 0;
	float add = pow(1.005, abs(angle));
	float sub = pow(0.995, abs(angle));

	nowV = resizeS->GetV();
	while (nowV != NULL) 
	{
		if (angle > 0) 
		{
			nowV->SetX(add * (nowV->GetX() - baseX) + baseX);
			nowV->SetY(add * (nowV->GetY() - baseY) + baseY);
		}
		else if (angle < 0) {
			nowV->SetX(sub * (nowV->GetX() - baseX) + baseX);
			nowV->SetY(sub * (nowV->GetY() - baseY) + baseY);
		}
		nowV = nowV->GetNext();
	}

	angle *= -1;

	//�S�Ă̕ӂ�0.05���Z���ꍇ�͖߂�
	nowV = resizeS->GetV();
	while (nowV->GetNext() != NULL) 
	{
		if (Distance(nowV->GetX(), nowV->GetY(), nowV->GetNext()->GetX(), nowV->GetNext()->GetY()) > 0.05) 
		{
			break;
		}
		nowV = nowV->GetNext();
	}
	if (nowV->GetNext() == NULL) 
	{
		nowV = resizeS->GetV();
		while (nowV != NULL) 
		{
			if (angle > 0) 
			{
				nowV->SetX(add * (nowV->GetX() - baseX) + baseX);
				nowV->SetY(add * (nowV->GetY() - baseY) + baseY);
			}
			else if (angle < 0) 
			{
				nowV->SetX(sub * (nowV->GetX() - baseX) + baseX);
				nowV->SetY(sub * (nowV->GetY() - baseY) + baseY);
			}
			nowV = nowV->GetNext();
		}

		return;
	}

	//�`��ɓ����Ă��Ȃ������肷��
	nowV = resizeS->GetV();
	CShape* nowS = shape_head->GetNextShape();
	if (nowS == resizeS) {
		nowS = nowS->GetNextShape();
	}
	while (nowS != NULL) {
		if (!Inclusion(nowS, resizeS->GetV()->GetX(), resizeS->GetV()->GetY())) {
			nowV = resizeS->GetV();
			while (nowV != NULL) {
				if (angle > 0) {
					nowV->SetX(add * (nowV->GetX() - baseX) + baseX);
					nowV->SetY(add * (nowV->GetY() - baseY) + baseY);
				}
				else if (angle < 0) {
					nowV->SetX(sub * (nowV->GetX() - baseX) + baseX);
					nowV->SetY(sub * (nowV->GetY() - baseY) + baseY);
				}
				nowV = nowV->GetNext();
			}

			return;
		}
		nowS = nowS->GetNextShape();
		if (nowS == resizeS) {
			nowS = nowS->GetNextShape();
		}
	}

	//�`������Ă��Ȃ������肷��
	if (!Wrap(resizeS)) {
		nowV = resizeS->GetV();
		while (nowV != NULL) {
			if (angle > 0) {
				nowV->SetX(add * (nowV->GetX() - baseX) + baseX);
				nowV->SetY(add * (nowV->GetY() - baseY) + baseY);
			}
			else if (angle < 0) {
				nowV->SetX(sub * (nowV->GetX() - baseX) + baseX);
				nowV->SetY(sub * (nowV->GetY() - baseY) + baseY);
			}
			nowV = nowV->GetNext();
		}

		return;
	}

	//�ړ�������`��̑S�Ă̕ӂ����������Ă��Ȃ������肷��
	CVertex* nextV = nowV->GetNext();
	while (nowV->GetNext() != NULL) {
		if (!OtherCross(resizeS, nowV, nextV)) {
			nowV = resizeS->GetV();
			while (nowV != NULL) {
				if (angle > 0) {
					nowV->SetX(add * (nowV->GetX() - baseX) + baseX);
					nowV->SetY(add * (nowV->GetY() - baseY) + baseY);
				}
				else if (angle < 0) {
					nowV->SetX(sub * (nowV->GetX() - baseX) + baseX);
					nowV->SetY(sub * (nowV->GetY() - baseY) + baseY);
				}
				nowV = nowV->GetNext();
			}

			return;
		}
		nowV = nowV->GetNext();
		nextV = nextV->GetNext();
		if (nextV->GetNext() == NULL) {
			nextV = resizeS->GetV();
		}
	}
	
	return;
}

//�`��̑I�������ėǂ����̔��肷��@�ł���Ƃ���true
boolean CAdminControl::CanSelect() 
{
	//�`�󂪈�ȏ㑶�݂��Ă��āA�S�Ă̌`�󂪕��Ă���Ƃ�
	if (shape_head != NULL && shape_head->GetV() == NULL) 
	{
		return true;
	}

	return false;
}

//�`�󂪑I������Ă��邩�̔���
int CAdminControl::NowSelect()
{
	if (control_point != NULL) 
	{
		return 1;
	}
	else if (control_edge != NULL) 
	{
		return 2;
	}
	else if (control_shape != NULL) 
	{
		return 3;
	}

	return 0;
}

//�`�󂪑��̌`��Ɖ��_�������Ă邩�𔻒�
int CAdminControl::CountCross(CShape* skipS)
{
	CShape* nowS = shape_head->GetNextShape();
	CVertex* As = NULL;
	CVertex* Ae = NULL;
	CVertex* Bs = NULL;
	CVertex* Be = NULL;

	int cnt = 0;

	while (nowS != NULL) 
	{
		As = skipS->GetV();
		Ae = As->GetNext();
		while (Ae != NULL) 
		{
			Bs = nowS->GetV();
			Be = Bs->GetNext();
			while (Be != NULL) {
				if (CrossCalc(As, Ae, Bs, Be->GetX(), Be->GetY()))
				{
					cnt++;
				}
				
				Bs = Be;
				Be = Be->GetNext();
			}

			As = Ae;
			Ae = Ae->GetNext();
		}
		
		nowS = nowS->GetNextShape();
		if (nowS == skipS) 
		{
			nowS = nowS->GetNextShape();
		}
	}

	return cnt;
}

//�`��̑I��(�`��̏I�_���I�΂�邱�Ƃ͂Ȃ�)
void CAdminControl::SelectShape(float x, float y)
{
	CShape* nowS = shape_head->GetNextShape();
	CVertex* nowV = nowS->GetV();
	CVertex* nextV = nowV->GetNext();

	CVertex* selectPoint = NULL;
	CVertex* selectEdge = NULL;
	CShape* selectShape = NULL;

	float nowPdist, nowEdist;
	float minPdist = select_dist, minEdist = select_dist;

	while (nowS != NULL) 
	{
		nowV = nowS->GetV();
		nextV = nowV->GetNext();

		//�n�_�����߂�������
		nowPdist = Distance(nowV, x, y);
		if (nowPdist < minPdist) 
		{
			selectPoint = nowV;
			minPdist = nowPdist;
		}
		while (nextV != NULL) 
		{
			//select_dist���߂��_�̒��ň�ԋ߂��_���i�[
			nowPdist = Distance(nextV, x, y);
			nowEdist = PointLineDistance(nowV, nextV, x, y);
			if (nowPdist < minPdist) 
			{
				selectPoint = nextV;
				minPdist = nowPdist;
			}
			//select_dist���߂��Ő��̒��ň�ԋ߂��ӂ��i�[
			if (nowEdist < minEdist) 
			{
				selectEdge = nowV;
				minEdist = nowEdist;
			}
			nowV = nextV;
			nextV = nextV->GetNext();
		}
		//�N���b�N�����ʒu�����Ă���`����i�[
		if (!Inclusion(nowS, x, y)) 
		{
			selectShape = nowS;
		}

		nowS = nowS->GetNextShape();
	}
	//���_�̑I��
	if (selectPoint != NULL) {
		if (control_point != selectPoint)
		{
			InitSelect();
			control_point = selectPoint;
		}
		return;
	}
	//�Ő��̑I��(deleteMode�̂Ƃ��͕ӂ̑I�������Ȃ�)
	if (selectEdge != NULL && !deleteMode)
	{
		if (control_edge != selectEdge) 
		{
			InitSelect();
			control_edge = selectEdge;
		}
		return;
	}
	//�`��̑I��
	if (selectShape != NULL) 
	{
		if (control_shape != selectShape) 
		{
			InitSelect();
			control_shape = selectShape;
		}
		return;
	}
	InitSelect();
}

//�`�󂪑I������Ă��邩�̔���(�_�A�ӁA�`�󂪑I������Ă���ꍇ�A���ꂼ��1,2,3��Ԃ��B)
void CAdminControl::Delete(float x, float y)
{
	//�_���I������Ă���ꍇ
	if (NowSelect() == 1) 
	{
		DeletePoint(control_point);
	}
	//�`�󂪑I������Ă���ꍇ
	else if (NowSelect() == 3) 
	{
		CShape* nowS = shape_head;
		while (nowS != NULL) 
		{
			if (nowS->GetNextShape() == control_shape) 
			{
				//�`�󂪈�����̂Ƃ�
				if (nowS == shape_head && control_shape->GetNextShape() == NULL) 
				{
					shape_head = NULL;
					deleteMode = false;
					delete nowS;
				}
				else 
				{
					nowS->SetNextShape(control_shape->GetNextShape());
				}
				CShape* deleteS = control_shape;
				control_shape = NULL;
				deleteS->GetV()->FreeVertex();
				deleteS->InitVHead();
				delete deleteS;
				break;
			}
			nowS = nowS->GetNextShape();
		}
	}
	//�����I������Ă��Ȃ��ꍇ(�S�`����폜����)
	else if (NowSelect() == 0) 
	{
		CShape* deleteS = shape_head;
		deleteS->FreeShape();
		shape_head = NULL;
		deleteMode = false;
	}

	return;
}

//�S�Ă̌`��̕��s�ړ�
void CAdminControl::AllMove(float MoveX, float MoveY)
{
	CShape* nowS = shape_head;
	if (nowS->GetV() == NULL) 
	{
		nowS = nowS->GetNextShape();
	}
	CVertex* nowV = NULL;

	while (nowS != NULL) 
	{
		nowV = nowS->GetV();
		while (nowV != NULL) 
		{
			nowV->SetXY(nowV->GetX() + MoveX, nowV->GetY() + MoveY);
			nowV = nowV->GetNext();
		}
		nowS = nowS->GetNextShape();
	}

	return;
}

//�S�Ă̌`��̉�]�ړ�
void CAdminControl::AllRotate(float baseX, float baseY, float x, float y)
{
	CShape* nowS = shape_head;
	if (nowS->GetV() == NULL) 
	{
		nowS = nowS->GetNextShape();
	}
	CVertex* nowV = nowS->GetV();

	float centerX, centerY, sumX = 0, sumY = 0, newX, newY, nasukaku;

	//�d�S�����߂�
	while (nowV->GetNext() != NULL) 
	{
		sumX = sumX + nowV->GetX();
		sumY = sumY + nowV->GetY();
		nowV = nowV->GetNext();
	}
	centerX = sumX / (nowS->CountVertex() - 1);
	centerY = sumY / (nowS->CountVertex() - 1);

	//��_����d�S�܂ł̒����Ɗ�_����}�E�X�̍��W�܂ł̒����̂Ȃ��p�����߂�
	nasukaku = AngleCalc(centerX, centerY, baseX, baseY, x, y);

	while (nowS != NULL) 
	{
		nowV = nowS->GetV();
		while (nowV != NULL) 
		{
			newX = (nowV->GetX() - baseX) * cos(nasukaku) - (nowV->GetY() - baseY) * sin(nasukaku) + baseX;
			newY = (nowV->GetX() - baseX) * sin(nasukaku) + (nowV->GetY() - baseY) * cos(nasukaku) + baseY;
			nowV->SetX(newX);
			nowV->SetY(newY);
			nowV = nowV->GetNext();
		}
		nowS = nowS->GetNextShape();
	}

	return;
}

//�S�Ă̌`��̊g��k��
void CAdminControl::AllResize(float baseX, float baseY, short zDelta)
{
	CShape* nowS = shape_head;
	if (nowS->GetV() == NULL) 
	{
		nowS = nowS->GetNextShape();
	}
	CVertex* nowV = NULL;

	//�g�k�̔{�������߂�
	int angle = (int)zDelta / 15, i = 0;
	float add = pow(1.001, abs(angle));
	float sub = pow(0.999, abs(angle));

	while (nowS != NULL) 
	{
		nowV = nowS->GetV();
		while (nowV != NULL) 
		{
			if (angle > 0) 
			{
				nowV->SetX(add * (nowV->GetX() - baseX) + baseX);
				nowV->SetY(add * (nowV->GetY() - baseY) + baseY);
			}
			else if (angle < 0) 
			{
				nowV->SetX(sub * (nowV->GetX() - baseX) + baseX);
				nowV->SetY(sub * (nowV->GetY() - baseY) + baseY);
			}
			nowV = nowV->GetNext();
		}
		nowS = nowS->GetNextShape();
	}
}

//�`��̕�������̕ύX
void CAdminControl::ReverseShape(CShape* reverseS)
{
	CVertex* nowV = reverseS->GetV();
	CVertex* newPenultV = NULL;  //�I�_��O�̓_���i�[����
	CVertex* changeV = NULL;     //�I�_��O�̓_���i�[����
	
	//�}�`�̓�ڂ̓_����I�_�̈�O�̓_�܂ł̏��Ԃ��t�ɂ���
	while (nowV->GetNext()->GetNext()->GetNext() != NULL) 
	{
		newPenultV = nowV;
		changeV = nowV->GetNext();
		while (changeV->GetNext()->GetNext() != NULL) 
		{
			newPenultV = changeV;
			changeV = changeV->GetNext();
		}

		newPenultV->SetNext(changeV->GetNext());
		changeV->SetNext(nowV->GetNext());
		nowV->SetNext(changeV);
		
		nowV = nowV->GetNext();
	}

	return;
}

void CAdminControl::SetDepth(float new_depth)
{
	now_depth = new_depth;
}

float CAdminControl::GetDepth()
{
	return now_depth;
}
