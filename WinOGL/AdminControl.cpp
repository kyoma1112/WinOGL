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

/*面の表示を行う関数
void CAdminControl::AreaDraw(CShape* printS) {
	CVertex* nowV = printS->GetV();

	//control_shapeの複製
	CShape* copyShape = new CShape();
	while (nowV != NULL) {
		copyShape->AppendVertex(nowV->GetX(), nowV->GetY());
		nowV = nowV->GetNext();
	}

	CVertex* startV = copyShape->GetV();
	CVertex* middleV = nowV->GetNext();
	CVertex* endV = middleV->GetNext();

	//図形の頂点の配置方向が時計周りの場合
	if (Direction(copyShape)) {
		while (copyShape->CountVertex() >= 3) {
			glColor3f(0, 1.0, 0);
			glPointSize(10);
			glBegin(GL_TRIANGLES);

			//三角形が時計回りの場合
			if (TriangleDirection(startV, middleV, endV)) {
				
			}
			else {
				startV = middleV;
				middleV = endV;
				endV = endV->GetNext();
				if (endV == NULL) {
					endV = copyShape->GetV()->GetNext();
				}
			}
			glEnd();
		}
	}
	//図形の頂点の配置方向が反時計周りの場合
	else {
	
	}

	//複製した形状とその中の頂点の削除
	copyShape->GetV()->FreeVertex();
	copyShape->FreeShape();
}
*/

//面以外の表示
void CAdminControl::Draw()
{
	CShape* nowS = shape_head;
	CVertex* nowV;
	// TODO: ここに実装コードを追加します.
	while (nowS != NULL) {
		if (nowS == control_shape) {
			glColor3f(0, 1.0, 0);
		}
		else {
			glColor3f(1.0, 1.0, 1.0);
		}
		glPointSize(10);
		glBegin(GL_POINTS);

		nowV = nowS->GetV();

		while (nowV != NULL) {
			glVertex2f(nowV->GetX(), nowV->GetY());
			nowV = nowV->GetNext();
		}

		glEnd();

		if (nowS == control_shape) {
			glColor3f(0, 1.0, 0);
		}
		else {
			glColor3f(1.0, 1.0, 1.0);
		}
		glLineWidth(4);
		glBegin(GL_LINE_STRIP);

		nowV = nowS->GetV();
		while (nowV != NULL) {
			glVertex2f(nowV->GetX(), nowV->GetY());

			nowV = nowV->GetNext();
		}
		glEnd();
		nowS = nowS->GetNextShape();
	}
	if (control_point != NULL) {
		glColor3f(0, 1.0, 0);
		glPointSize(10);
		glBegin(GL_POINTS);

		glVertex2f(control_point->GetX(), control_point->GetY());

		glEnd();
	}
	if (control_edge != NULL) {
		glColor3f(0, 1.0, 0);
		glLineWidth(4);
		glBegin(GL_LINE_STRIP);

		nowV = control_edge;
		CVertex* nextV = nowV->GetNext();

		glVertex2f(nowV->GetX(), nowV->GetY());
		glVertex2f(nextV->GetX(), nextV->GetY());

		glEnd();
	}
}

//カーソルの表示
void CAdminControl::CursorDraw(CRect rect) {
	//マウスの位置の計算
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

	if (rect.Width() > rect.Height()) {     //画面サイズに合わせてX,Yを調整
		hi = (double)rect.Width() / rect.Height();
		mx = mx * hi;
	}
	else {
		hi = (double)rect.Height() / rect.Width();
		my = my * hi;
	}

	//ウィンドウサイズの計算
	double wx, wy;
	wx = 1;
	wy = 1;
	if (rect.Width() > rect.Height()) {     
		wx = (double)rect.Width() / rect.Height();
	}
	else {
		wy = (double)rect.Height() / rect.Width();
	}

	//カーソルの表示
	glColor3f(0.6, 0.8, 1.0);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(mx, -1 * wy);
	glVertex2f(mx, wy);
	glVertex2f(-1 * wx, my);
	glVertex2f(wx, my);
	glEnd();
}

//新しい形状を作成
void CAdminControl::AppendShape()
{
	CShape* newShape = new CShape();
	newShape->SetNextShape(shape_head);
	shape_head = newShape;
}

//点と点の距離計算
float CAdminControl::Distance(CVertex* s, float x, float y) {
	float d;
	float X, Y;

	X = x - s->GetX();
	Y = y - s->GetY();

	d = sqrt(pow(X, 2) + pow(Y, 2));

	return d;
}

//自交差判定　trueなら交差してない、falseなら交差している
boolean CAdminControl::Cross(float mx, float my) {
	CVertex* As = shape_head->GetV();
	CVertex* Ae = As->GetNext();
	CVertex* Bs = Ae->GetNext();

	while (Bs->GetNext() != NULL)
	{
		Bs = Bs->GetNext();
	}

	if (mx == As->GetX() && my == As->GetY()) {
		As = Ae;
		Ae = Ae->GetNext();
	}

	while (Ae != Bs) {

		if (CrossCalc(As, Ae, Bs, mx, my)) {
			return false;
		}

		As = Ae;
		Ae = Ae->GetNext();
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

	if (nowS->GetNextShape() != NULL) {
		nowS = nowS->GetNextShape();
	}
	else {
		return true;
	}

	while (Bs->GetNext() != NULL) {
		Bs = Bs->GetNext();
	}

	while (nowS != NULL) {
		As = nowS->GetV();
		Ae = As->GetNext();
		while (Ae != NULL) {

			if (CrossCalc(As, Ae, Bs, mx, my)) {
				return false;
			}

			As = Ae;
			Ae = Ae->GetNext();
		}

		nowS = nowS->GetNextShape();
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

	if (ca1 * ca2 <= 0 && cb1 * cb2 <= 0) {
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

	if (nowS->GetNextShape() != NULL) {
		nowS = nowS->GetNextShape();
	}
	else {
		return true;
	}

	while (nowS != NULL) {
		nowV = nowS->GetV();
		nextV = nowV->GetNext();
		sum = 0;

		while (nextV != NULL) {
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
		if (sum < 0) {
			sum *= -1;
		}
		if ((2 * pi - sum) < 0.0001) {
			return false;
		}
		nowS = nowS->GetNextShape();
	}

	return true;
}

//点の内包判定(一つの形状で判定する)　trueなら外、falseなら内
boolean CAdminControl::Inclusion(CShape* nowS, float x, float y)
{
	CVertex* nowV = nowS->GetV();
	CVertex* nextV = nowV->GetNext();

	float ax, ay, bx, by;
	float sum, gaiseki, naiseki;

	sum = 0;

	while (nextV != NULL) {
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
	if (sum < 0) {
		sum *= -1;
	}
	if ((2 * pi - sum) < 0.0001) {
		return false;
	}

	return true;
}


//形状の内包判定 trueなら内包していない、falseなら内包している
boolean CAdminControl::Wrap()
{
	CShape* nowS = shape_head;
	CVertex* nowSV;      //nowS(形状)の始点
	CVertex* nowV;
	CVertex* nextV;

	float ax, ay, bx, by;
	float naiseki, gaiseki, sum;

	if (nowS->GetNextShape() != NULL) {
		nowS = nowS->GetNextShape();
	}
	else {
		return true;
	}
	
	while (nowS != NULL) {
		nowV = shape_head->GetV();
		nextV = nowV->GetNext();
		nowSV = nowS->GetV();
		sum = 0;
		while (nowV != NULL) {
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
			if (nextV == NULL) {
				nextV = shape_head->GetV();
			}
		}
		sum = fabs(sum);

		if ((2 * pi - sum) < 0.0001) {
			return false;
		}
		nowS = nowS->GetNextShape();
	}

	return true;
}

//点と線分の距離
float CAdminControl::PointLineDistance(CVertex* nowV, CVertex* nextV, float x, float y) {
	float apX = x - nowV->GetX();
	float apY = y - nowV->GetY();
	float abX = nextV->GetX() - nowV->GetX();
	float abY = nextV->GetY() - nowV->GetY();
	float bpX = x - nextV->GetX();
	float bpY = y - nextV->GetY();
	float baX = nowV->GetX() - nextV->GetX();
	float baY = nowV->GetY() - nextV->GetY();

	float gaiseki, naiseki, gaiseki2, naiseki2, nasukaku, nasukaku2;

	float distance;

	gaiseki = apX * abY - abX * apY;
	naiseki = apX * abX + apY * abY;
	gaiseki2 = bpX * baY - baX * bpY;
	naiseki2 = bpX * baX + baY * bpY;

	nasukaku = fabs(atan2(gaiseki, naiseki));
	nasukaku2 = fabs(atan2(gaiseki2, naiseki2));

	if (pi / 2 <= nasukaku) {
		//a点とp点の距離
		distance = Distance(nowV, x, y);
	}
	else if (pi / 2 <= nasukaku2) {
		//b点とp点の距離
		distance =  Distance(nextV, x, y);
	}
	else {
		//p点と直線abの距離の計算
		float a, b;

		a = sqrt(pow(abX, 2) + pow(abY, 2));
		b = sqrt(pow(apX, 2) + pow(apY, 2));

		distance = b * fabs((abX * apY - apX * abY) / (a * b));
	}

	return distance;
}

//形状の頂点の配置方向判定　trueなら時計周り、falseなら反時計周り
boolean CAdminControl::Direction(CShape* nowS)
{
	float sum = 0;
	CVertex* nowV = nowS->GetV();
	CVertex* nextV = nowV->GetNext();

	while (nextV != NULL) {
		
		sum += nowV->GetX() * nextV->GetY() - nextV->GetX() * nowV->GetY();

		nowV = nextV;
		nextV = nextV->GetNext();
	}
	if (sum < 0) {
		return true;
	}

	return false;
}

//三角形の配置方向判定 trueなら時計周り、falseなら半時計周り
boolean CAdminControl::TriangleDirection(CVertex* startV, CVertex* middleV, CVertex* endV) {
	
	float sum = 0;
	float sx, sy, mx, my, ex, ey;

	sx = startV->GetX();
	sy = startV->GetY();
	mx = middleV->GetX();
	my = middleV->GetY();
	ex = endV->GetX();
	ey = endV->GetY();

	sum += sx * my - mx * sy;
	sum += mx * ey - ex * my;
	sum += ex * sy - sx * ey;

	if (sum < 0) {
		return true;
	}

	return false;
}

//左クリックで点の追加やそれ以外の形状の編集をするとき
void CAdminControl::CreateShape(float x, float y)
{
	/*-----------------------*/
	/*  形状の選択をするとき */
	/*-----------------------*/
	
	/*形状が一つ以上存在していて、全ての形状が閉じているとき*/
	if (shape_head != NULL && shape_head->GetV() == NULL) {
		CShape* nowS = shape_head->GetNextShape();
		CVertex* nowV = nowS->GetV();
		CVertex* nextV = nowV->GetNext();

		while (nowS != NULL) {
			nowV = nowS->GetV();
			nextV = nowV->GetNext();

			//始点の選択
			if (Distance(nowV, x, y) <= select_dist) {
				control_point = nowV;
				control_edge = NULL;
				control_shape = NULL;
				return;
			}
			while (nextV != NULL) {
				//始点以外の頂点の選択
				if (Distance(nextV, x, y) <= select_dist) {
					control_point = nextV;
					control_edge = NULL;
					control_shape = NULL;
					return;
				}
				//稜線の選択
				if (PointLineDistance(nowV, nextV, x, y) <= select_dist) {
					control_point = NULL;
					control_edge = nowV;
					control_shape = NULL;
					return;
				}
				
				nowV = nextV;
				nextV = nextV->GetNext();
			}
			//形状の選択
			if (!Inclusion(nowS, x, y)) {
				control_shape = nowS;
				control_edge = NULL;
				control_point = NULL;
				return;
			}
			
			nowS = nowS->GetNextShape();
		}
		control_point = NULL;
		control_edge = NULL;
		control_shape = NULL;
	}

	/*------------------------*/
	/*  点を追加していくとき  */
	/*------------------------*/
	
	if (shape_head == NULL) {
		AppendShape();
	}

	//前の点と同じ座標に点を打った場合
	if (shape_head->CountVertex() >= 1) {
		CVertex* endV = shape_head->GetV();
		while (endV->GetNext() != NULL) {
			endV = endV->GetNext();
		}

		if (Distance(endV, x, y) <= point_dist) {
			return;
		}
	}

	if (shape_head->CountVertex() < 3) {
		if (shape_head->CountVertex() == 0) {
			if (Inclusion(x, y)) {
				shape_head->AppendVertex(x, y);
			}
		}
		else {
			if (shape_head->CountVertex() == 2 && Distance(shape_head->GetV(), x, y) <= point_dist) {
				return;
			}
			if (OtherCross(x, y)) {
				shape_head->AppendVertex(x, y);
			}
		}
	}
	else if (Distance(shape_head->GetV(), x, y) <= 0.1) {
		float sx = shape_head->GetV()->GetX();
		float sy = shape_head->GetV()->GetY();

		if (OtherCross(sx, sy) && Cross(sx, sy) && Wrap()) {
			shape_head->AppendVertex(sx, sy);
			AppendShape();
		}
		else if (OtherCross(x, y) && Cross(x, y)) {
			shape_head->AppendVertex(x, y);
		}
	}
	else {
		if (OtherCross(x, y) && Cross(x, y)) {
			shape_head->AppendVertex(x, y);
		}
	}
}
