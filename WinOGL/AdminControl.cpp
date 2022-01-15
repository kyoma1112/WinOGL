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

//面の表示を行う関数
void CAdminControl::DrawArea(CShape* printS)
{

	CVertex* nowV = printS->GetV();

	//control_shapeの複製
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
	
	//三角形になるまで
	while (copyShape->CountVertex() >= 5) 
	{
		//v1,v2,v3で作られる三角形の重心を求める
		centerX = (v1->GetX() + v2->GetX() + v3->GetX()) / 3;
		centerY = (v1->GetY() + v2->GetY() + v3->GetY()) / 3;
		//三角形の重心が表示する形状の中にあって三角形の中に三角形の頂点以外の点がない場合に三角形を表示する
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
			//三角形の始点を一つ進める
			v1 = v2;
		}
		//三角形の始点以外の点を一つ進める
		v2 = v3;
		v3 = v3->GetNext();
		if (v3->GetNext() == NULL)
		{
			v3 = copyShape->GetV();
		}
	}
	//最後の三角形表示
	if (copyShape->CountVertex() == 4) 
	{
		glColor3f(0.65, 0.65, 0.65);
		glBegin(GL_TRIANGLES);
		glVertex2f(v1->GetX(), v1->GetY());
		glVertex2f(v2->GetX(), v2->GetY());
		glVertex2f(v3->GetX(), v3->GetY());
		glEnd();
	}

	//複製した形状とその中の頂点の削除
	copyShape->GetV()->FreeVertex();
	copyShape->InitVHead();
	copyShape->FreeShape();
}

//立体の面表示
void CAdminControl::DrawSolid(CShape* printS)
{
	//形状が半時計周りか判定し、時計回りの場合半時計周りにする
	if (IsClockwise(printS)) {
		ReverseShape(printS);
	}

	//control_shapeをcopyShapeとして複製
	CVertex* nowV = printS->GetV();
	CShape* copyShape = new CShape();
	while (nowV != NULL)
	{
		copyShape->AppendVertex(nowV->GetX(), nowV->GetY());
		nowV = nowV->GetNext();
	}

	//ライティングの設定
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

	//三角形になるまで分割
	while (copyShape->CountVertex() >= 5)
	{
		//v1,v2,v3で作られる三角形の重心を求める
		centerX = (v1->GetX() + v2->GetX() + v3->GetX()) / 3;
		centerY = (v1->GetY() + v2->GetY() + v3->GetY()) / 3;
		//三角形の重心が表示する形状の中にあって三角形の中に三角形の頂点以外の点がない場合に三角形を表示する
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
			//三角形の始点を一つ進める
			v1 = v2;
		}
		//三角形の始点以外の点を一つ進める
		v2 = v3;
		v3 = v3->GetNext();
		if (v3->GetNext() == NULL)
		{
			v3 = copyShape->GetV();
		}
	}
	//最後の三角形表示
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

	//複製した形状とその中の頂点の削除
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

//面以外の表示
void CAdminControl::Draw()
{
	if (viewMode && solidMode) 
	{
		CShape* nowS = shape_head;

		//面の表示モードがtrueのとき完成している全ての形状の面を表示する
		if (shape_head != NULL && shape_head->GetNextShape() != NULL)
		{
			nowS = nowS->GetNextShape();
			while (nowS != NULL)
			{
				//立体の面表示
				DrawSolid(nowS);
				
				//立体の辺表示
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

		//面の表示モードがtrueのとき完成している全ての形状の面を表示する
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
			//頂点の表示
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

			//辺の表示
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
		//選択された頂点の表示
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
		//選択された辺の表示
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

		//基点の表示
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

//カーソルの表示
void CAdminControl::DrawCursor(CRect rect, float x, float y)
{
	//マウスの位置の計算
	/*
	double mx, my, hi;
	CPoint point;
	GetCursorPos(&point);
	HWND hWnd = FindWindow(NULL, TEXT("無題 - WinOGL"));
	ScreenToClient(hWnd, &point);
	mx = (double)(point.x - 1) / rect.Width(); //X正規化座標系
	my = (double)(point.y - 25) / rect.Height();
	my = (my - 1) * -1; //Y正規化座標系
	mx = mx * 2 - 1; //Xワールド座標系
	my = my * 2 - 1; //Yワールド座標系

	//画面サイズに合わせてX,Yを調整
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

	//ウィンドウサイズの計算
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

	//カーソルの表示
	glColor3f(0, 1.0, 1.0);
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2f(x, -1 * wy);
	glVertex2f(x, wy);
	glVertex2f(-1 * wx, y);
	glVertex2f(wx, y);
	glEnd();
}

//xyz軸の表示
void CAdminControl::DrawAxis()
{
	glLineWidth(2);
	glBegin(GL_LINES);
	// x軸
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	// y軸
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	// z軸
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -1.0);
	glVertex3f(0.0, 0.0, 1.0);
	glEnd();
}

//新しい形状を作成
void CAdminControl::AppendShape()
{
	CShape* newShape = new CShape();
	newShape->SetNextShape(shape_head);
	shape_head = newShape;
}

//選択されている点を返却
CVertex* CAdminControl::GetControlPoint()
{
	if (control_point != NULL) 
	{
		return control_point;
	}
	
	return NULL;
}

//選択されている形状を返却
CShape* CAdminControl::GetControlShape()
{
	if (control_shape != NULL) 
	{
		return control_shape;
	}

	return NULL;
}

//選択の解除
void CAdminControl::InitSelect()
{
	control_point = NULL;
	control_edge = NULL;
	control_shape = NULL;
	DeleteBasePoint();
}

//点と点の距離計算
float CAdminControl::Distance(CVertex* s, float x, float y) 
{
	float d;
	float X, Y;

	X = x - s->GetX();
	Y = y - s->GetY();

	d = sqrt(pow(X, 2) + pow(Y, 2));

	return d;
}

//点と点の距離計算
float CAdminControl::Distance(float x1, float y1, float x2, float y2)
{
	float d;
	float X, Y;

	X = x2 - x1;
	Y = y2 - y1;

	d = sqrt(pow(X, 2) + pow(Y, 2));

	return d;
}

//点と線分の距離
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

//なす角の計算(直線abと直線bcのなす角の計算)
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

//自交差判定　trueなら交差してない、falseなら交差している
boolean CAdminControl::Cross(float mx, float my) 
{
	CVertex* As = shape_head->GetV();
	CVertex* Ae = As->GetNext();

	CVertex* Bs = Ae->GetNext();
	while (Bs->GetNext() != NULL)
	{
		Bs = Bs->GetNext();
	}

	//形状閉じるときは最初の辺をとばす
	if (mx == As->GetX() && my == As->GetY()) 
	{
		As = Ae;
		Ae = Ae->GetNext();
	}

	while (Ae != Bs) 
	{
		//As,AeとBs,Beの直線同士で交差判定
		if (CrossCalc(As, Ae, Bs, mx, my)) 
		{
			return false;
		}

		As = Ae;
		Ae = Ae->GetNext();
	}

	return true;
}

//二点をつなぐ直線と他の直線の自交差判定(Bs, Beの座標を別の値に置き換える)　trueなら交差してない、falseなら交差している
boolean CAdminControl::Cross(CShape* nowS, CVertex* Bs, CVertex* Be, float Bs_x, float Bs_y, float Be_x, float Be_y)
{
	//As,Aeが終点の場合は始点に変える
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

//二点をつなぐ直線と他の直線の自交差判定　trueなら交差してない、falseなら交差している
boolean CAdminControl::Cross(CShape* nowS, CVertex* Bs, CVertex* Be)
{
	//As,Aeが終点の場合は始点に変える
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

//他交差判定　trueなら交差してない、falseなら交差している
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

//二つの座標をつなぐ直線の他交差判定　trueなら交差してない、falseなら交差している
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

//二点をつなぐ直線との他交差判定　trueなら交差してない、falseなら交差している
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

//交差計算  trueなら交差してる、falseなら交差していない
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

//交差計算  trueなら交差してる、falseなら交差していない
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

//点の内包判定(全ての形状で判定する)　trueなら外、falseなら内
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

//点の内包判定(一つの形状で判定する)　trueなら外、falseなら内
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

//形状の三角形部分の中に形状内の他の頂点が存在するかの判定 trueなら三角形内部に頂点無し、falseなら三角形内部に頂点有り
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
	//形状の終点の一つ前の点まで見る(始点と終点が同じ位置であるため)
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
		//三角形の三点は見ないのでとばす
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

//形状の三角形部分の中に三角形の三点を除く他の頂点が存在しているかの判定　trueなら三角形内部に頂点無し、falseなら三角形内部に頂点有り
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
		//形状の終点の一つ前の点まで見る(始点と終点が同じ位置であるため)
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
			//三角形の三点は見ないのでとばす
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

//形状が時計回りに閉じているか判定する 時計周りならtrue、半時計周りならfalse
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
	//fabs(sum)/2 で面積が求まる
	if (sum < 0) 
	{
		return true;
	}

	return false;
}

//形状の内包判定 trueなら内包していない、falseなら内包している
boolean CAdminControl::Wrap()
{
	CShape* nowS = shape_head;
	//nowS(形状)の始点
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

//形状の内包判定(点の移動で使用,v2の座標をmx,myに置き換える)
boolean CAdminControl::Wrap(CShape* checkS, CVertex* v2, float mx, float my)
{
	CShape* nowS = shape_head->GetNextShape();
	if (nowS == checkS) 
	{
		nowS = nowS->GetNextShape();
	}
	//nowS(形状)の始点
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

//形状の内包判定
boolean CAdminControl::Wrap(CShape* checkS)
{
	CShape* nowS = shape_head->GetNextShape();
	if (nowS == checkS) 
	{
		nowS = nowS->GetNextShape();
	}
	//nowS(形状)の始点
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

//頂点の追加
void CAdminControl::CreateShape(float x, float y)
{
	//一番最初クリックされたときにshape生成
	if (shape_head == NULL) 
	{
		AppendShape();
	}

	//一つ前の点の位置とクリックした位置を比較する
	if (shape_head->CountVertex() >= 1) 
	{
		CVertex* endV = shape_head->GetV();
		while (endV->GetNext() != NULL) 
		{
			endV = endV->GetNext();
		}
		//全く同じ場所に打った場合return
		if (endV->GetX() == x && endV->GetY() == y) 
		{
			return;
		}
	}
	//点が3点未満の場合
	if (shape_head->CountVertex() < 3) 
	{
		//一点目かどうか
		if (shape_head->CountVertex() == 0) 
		{
			if (Inclusion(x, y)) 
			{
				shape_head->AppendVertex(x, y);
			}
		}
		else 
		{
			//一点目と今回打つ三点目が近い場合、三点目を追加せずにreturn
			if (shape_head->CountVertex() == 2 && Distance(shape_head->GetV(), x, y) <= point_dist) 
			{
				return;
			}
			//他交差していなければ点の追加
			if (OtherCross(x, y)) 
			{
				shape_head->AppendVertex(x, y);
			}
		}
	}
	//4点目以降でクリックした位置と始点が近い場合
	else if (Distance(shape_head->GetV(), x, y) <= 0.1) 
	{
		float sx = shape_head->GetV()->GetX();
		float sy = shape_head->GetV()->GetY();
		
		//始点に向けて線を引いたとき、自交差＆他交差＆形状の内包をしていなければ始点の位置に点を追加して形状を閉じる
		if (OtherCross(sx, sy) && Cross(sx, sy) && Wrap()) 
		{
			shape_head->AppendVertex(sx, sy);
			AppendShape();
		}
		//自交差＆他交差してなければ点を追加
		else if (OtherCross(x, y) && Cross(x, y)) 
		{
			shape_head->AppendVertex(x, y);
		}
	}
	//4点目以降
	else 
	{
		//自交差＆他交差してなければ点を追加
		if (OtherCross(x, y) && Cross(x, y)) 
		{
			shape_head->AppendVertex(x, y);
		}
	}
}

//点の削除
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
			//形状の頂点が4点以下の場合削除しない
			if (nowS->CountVertex() <= 4) 
			{
				break;
			}
			if (nowV == deleteV)
			{
				//選択された点が形状の始点の場合
				if (preV == NULL) 
				{
					while (nowV->GetNext() != NULL) 
					{
						preV = nowV;
						nowV = nowV->GetNext();
					}
					//選択された点を削除してよいかの判定
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
				//終点一つ前の点が選択されている場合&&選択された点を削除してよいかの判定
				else if (deleteV->GetNext()->GetNext() == NULL && TriangleInclusion(preV, deleteV, nowS->GetV())) 
				{
					preV->SetNext(deleteV->GetNext());
					delete deleteV;
					control_point = NULL;
				}
				//それ以外の点が選択されている場合&&選択された点を削除してよいかの判定
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

//三角形分割中の三角形の削除
void CAdminControl::DeleteTriangle(CShape* printS, CVertex* deleteV)
{
	CVertex* nowV = printS->GetV();
	CVertex* preV = NULL;

	while (nowV != NULL) 
	{
		if (nowV == deleteV) 
		{
			//選択された点が形状の始点の場合
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
			//始点以外の点が選択されている場合
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

//頂点の移動
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
				//選択された点が形状の始点の場合
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
				//終点一つ前の点が選択されている場合
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
				//それ以外の点が選択されている場合
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

//頂点の移動
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
				//選択された点が形状の始点の場合
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
				//終点一つ前の点が選択されている場合
				else if (v3->GetNext() == NULL) 
				{
					v3 = nowS->GetV();
					if (!Cross(nowS, v1, v2) || !Cross(nowS, v2, v3) || !OtherCross(nowS, v1, v2) || !OtherCross(nowS, v2, v3) || !Wrap(nowS)) 
					{
						v2->SetX(oldX);
						v2->SetY(oldY);
					}
				}
				//それ以外の点が選択されている場合
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

//形状の移動(x, yが形状の重心になる)
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
	//重心を求める
	while (nowV->GetNext() != NULL) 
	{
		sumX = sumX + nowV->GetX();
		sumY = sumY + nowV->GetY();
		nowV = nowV->GetNext();
	}
	centerX = sumX / (moveS->CountVertex()-1);
	centerY = sumY / (moveS->CountVertex()-1);

	//移動量を求める
	moveX = x - centerX;
	moveY = y - centerY;

	nowV = moveS->GetV();
	//全ての頂点を一度マウスの移動分移動させる
	while (nowV != NULL) 
	{
		nowV->SetX(nowV->GetX() + moveX);
		nowV->SetY(nowV->GetY() + moveY);
		nowV = nowV->GetNext();
	}

	//形状に内包されていないか判定する
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

	//形状を内包していないか判定する
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

	//移動させる形状の全ての辺が他交差していないか判定する
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

//辺上に点を追加する
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
	//選択した辺が含まれている形状を探す
	while (nowS != NULL) 
	{
		nowV = nowS->GetV();
		while (nowV != NULL) 
		{
			//nowVが選択した点だった場合
			if (nowV == v1) 
			{
				//クリックした座標が近い場合
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

//基点の返却
CVertex* CAdminControl::GetBasePoint()
{
	if (base_point != NULL) 
	{
		return base_point;
	}

	return NULL;
}

//基点の追加
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

//基点の削除
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

//形状の回転移動
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
	//重心を求める
	while (nowV->GetNext() != NULL) 
	{
		sumX = sumX + nowV->GetX();
		sumY = sumY + nowV->GetY();
		nowV = nowV->GetNext();
	}
	centerX = sumX / (rotateS->CountVertex() - 1);
	centerY = sumY / (rotateS->CountVertex() - 1);

	//基点から重心までの直線と基点からマウスの座標までの直線のなす角を求める
	nasukaku = AngleCalc(centerX, centerY, baseX, baseY, x, y);

	nowV = rotateS->GetV();
	//全ての頂点をnasukaku分回転させる
	while (nowV != NULL) 
	{
		newX = (nowV->GetX() - baseX) * cos(nasukaku) - (nowV->GetY() - baseY) * sin(nasukaku) + baseX;
		newY = (nowV->GetX() - baseX) * sin(nasukaku) + (nowV->GetY() - baseY) * cos(nasukaku) + baseY;
		nowV->SetX(newX);
		nowV->SetY(newY);
		nowV = nowV->GetNext();
	}

	//形状に内包されていないか判定する
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

	//形状を内包していないか判定する
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

	//移動させる形状の全ての辺が他交差していないか判定する
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

//形状の拡大縮小
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

	//全ての辺が0.05より短い場合は戻す
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

	//形状に内包されていないか判定する
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

	//形状を内包していないか判定する
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

	//移動させる形状の全ての辺が他交差していないか判定する
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

//形状の選択をして良いかの判定する　できるときはtrue
boolean CAdminControl::CanSelect() 
{
	//形状が一つ以上存在していて、全ての形状が閉じているとき
	if (shape_head != NULL && shape_head->GetV() == NULL) 
	{
		return true;
	}

	return false;
}

//形状が選択されているかの判定
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

//形状が他の形状と何点交差してるかを判定
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

//形状の選択(形状の終点が選ばれることはない)
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

		//始点だけ近いか見る
		nowPdist = Distance(nowV, x, y);
		if (nowPdist < minPdist) 
		{
			selectPoint = nowV;
			minPdist = nowPdist;
		}
		while (nextV != NULL) 
		{
			//select_distより近い点の中で一番近い点を格納
			nowPdist = Distance(nextV, x, y);
			nowEdist = PointLineDistance(nowV, nextV, x, y);
			if (nowPdist < minPdist) 
			{
				selectPoint = nextV;
				minPdist = nowPdist;
			}
			//select_distより近い稜線の中で一番近い辺を格納
			if (nowEdist < minEdist) 
			{
				selectEdge = nowV;
				minEdist = nowEdist;
			}
			nowV = nextV;
			nextV = nextV->GetNext();
		}
		//クリックした位置を内包している形状を格納
		if (!Inclusion(nowS, x, y)) 
		{
			selectShape = nowS;
		}

		nowS = nowS->GetNextShape();
	}
	//頂点の選択
	if (selectPoint != NULL) {
		if (control_point != selectPoint)
		{
			InitSelect();
			control_point = selectPoint;
		}
		return;
	}
	//稜線の選択(deleteModeのときは辺の選択をしない)
	if (selectEdge != NULL && !deleteMode)
	{
		if (control_edge != selectEdge) 
		{
			InitSelect();
			control_edge = selectEdge;
		}
		return;
	}
	//形状の選択
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

//形状が選択されているかの判定(点、辺、形状が選択されている場合、それぞれ1,2,3を返す。)
void CAdminControl::Delete(float x, float y)
{
	//点が選択されている場合
	if (NowSelect() == 1) 
	{
		DeletePoint(control_point);
	}
	//形状が選択されている場合
	else if (NowSelect() == 3) 
	{
		CShape* nowS = shape_head;
		while (nowS != NULL) 
		{
			if (nowS->GetNextShape() == control_shape) 
			{
				//形状が一つだけのとき
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
	//何も選択されていない場合(全形状を削除する)
	else if (NowSelect() == 0) 
	{
		CShape* deleteS = shape_head;
		deleteS->FreeShape();
		shape_head = NULL;
		deleteMode = false;
	}

	return;
}

//全ての形状の平行移動
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

//全ての形状の回転移動
void CAdminControl::AllRotate(float baseX, float baseY, float x, float y)
{
	CShape* nowS = shape_head;
	if (nowS->GetV() == NULL) 
	{
		nowS = nowS->GetNextShape();
	}
	CVertex* nowV = nowS->GetV();

	float centerX, centerY, sumX = 0, sumY = 0, newX, newY, nasukaku;

	//重心を求める
	while (nowV->GetNext() != NULL) 
	{
		sumX = sumX + nowV->GetX();
		sumY = sumY + nowV->GetY();
		nowV = nowV->GetNext();
	}
	centerX = sumX / (nowS->CountVertex() - 1);
	centerY = sumY / (nowS->CountVertex() - 1);

	//基点から重心までの直線と基点からマウスの座標までの直線のなす角を求める
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

//全ての形状の拡大縮小
void CAdminControl::AllResize(float baseX, float baseY, short zDelta)
{
	CShape* nowS = shape_head;
	if (nowS->GetV() == NULL) 
	{
		nowS = nowS->GetNextShape();
	}
	CVertex* nowV = NULL;

	//拡縮の倍率を求める
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

//形状の閉じる向きの変更
void CAdminControl::ReverseShape(CShape* reverseS)
{
	CVertex* nowV = reverseS->GetV();
	CVertex* newPenultV = NULL;  //終点二つ前の点を格納する
	CVertex* changeV = NULL;     //終点一つ前の点を格納する
	
	//図形の二つ目の点から終点の一つ前の点までの順番を逆にする
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
