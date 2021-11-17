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
void CAdminControl::DrawArea(CShape* printS) {
	CVertex* nowV = printS->GetV();

	//control_shapeの複製
	CShape* copyShape = new CShape();
	while (nowV != NULL) {
		copyShape->AppendVertex(nowV->GetX(), nowV->GetY());
		nowV = nowV->GetNext();
	}

	CVertex* v1 = copyShape->GetV();
	CVertex* v2 = v1->GetNext();
	CVertex* v3 = v2->GetNext();
	float centerX, centerY;

	while (copyShape->CountVertex() >= 4) {
		centerX = (v1->GetX() + v2->GetX() + v3->GetX()) / 3;
		centerY = (v1->GetY() + v2->GetY() + v3->GetY()) / 3;
		if (!Inclusion(copyShape, centerX, centerY) && TriangleInclusion(copyShape,v1, v2, v3)) {
			glColor3f(0, 1.0, 0);
			glBegin(GL_TRIANGLES);
			glVertex2f(v1->GetX(), v1->GetY());
			glVertex2f(v2->GetX(), v2->GetY());
			glVertex2f(v3->GetX(), v3->GetY());
			glEnd();
			DeleteTriangle(copyShape,v2);
		}
		else {
			v1 = v2;
		}

		v2 = v3;
		v3 = v3->GetNext();
		if (v3->GetNext() == NULL) {
			v3 = copyShape->GetV();
		}
	}

	//複製した形状とその中の頂点の削除
	copyShape->GetV()->FreeVertex();
	copyShape->InitVHead();
	copyShape->FreeShape();
}

//面以外の表示
void CAdminControl::Draw()
{
	CShape* nowS = shape_head;
	CVertex* nowV;
	// TODO: ここに実装コードを追加します.
	while (nowS != NULL) {
		//頂点の表示
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

		//辺の表示
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
	//選択された頂点の表示
	if (control_point != NULL) {
		glColor3f(0, 1.0, 0);
		glPointSize(10);
		glBegin(GL_POINTS);

		glVertex2f(control_point->GetX(), control_point->GetY());

		glEnd();
	}
	//選択された辺の表示
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
void CAdminControl::DrawCursor(CRect rect) {
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

//選択の解除
void CAdminControl::InitSelect()
{
	control_point = NULL;
	control_edge = NULL;
	control_shape = NULL;
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

//点と線分の距離
float CAdminControl::PointLineDistance(CVertex* nowV, CVertex* nextV, float x, float y) {

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

	if (0 <= s && s <= 1) {
		float d = sqrt(pow(baX, 2) + pow(baY, 2));
		gaiseki = apX * baY - baX * apY;
		distance = fabs(gaiseki / d);
	}
	else if (s > 1) {
		distance = Distance(nowV, x, y);
	}
	else {
		distance = Distance(nextV, x, y);
	}

	return distance;
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

//形状の三角形部分の中に形状内の他の頂点が存在するかの判定 trueなら三角形内部に頂点無し、falseなら三角形内部に頂点有り
boolean CAdminControl::TriangleInclusion(CShape* nowS, CVertex* v1, CVertex* v2, CVertex* v3)
{
	float ax, ay, bx, by, cx, cy;
	float sum = 0, gaiseki, naiseki, tan;
	
	CVertex* nowV = nowS->GetV();
	if (nowV == v1) {
		nowV = nowV->GetNext();
	}
	if (nowV == v2) {
		nowV = nowV->GetNext();
	}
	if (nowV == v3) {
		nowV = nowV->GetNext();
	}
	//形状の終点の一つ前の点まで見る(始点と終点が同じ位置であるため)
	while (nowV->GetNext() != NULL) {
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

		if (2 * pi - fabs(sum) < 0.0001) {
			return false;
		}

		nowV = nowV->GetNext();
		//三角形の三点は見ないのでとばす
		if (nowV == v1) {
			nowV = nowV->GetNext();
		}
		if (nowV == v2) {
			nowV = nowV->GetNext();
		}
		if (nowV == v3) {
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

	while (nowS != NULL) {
		nowV = nowS->GetV();
		if (nowV == v1) {
			nowV = nowV->GetNext();
		}
		if (nowV == v2) {
			nowV = nowV->GetNext();
		}
		if (nowV == v3) {
			nowV = nowV->GetNext();
		}
		//形状の終点の一つ前の点まで見る(始点と終点が同じ位置であるため)
		while (nowV->GetNext() != NULL) {
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

			if (2 * pi - fabs(sum) < 0.0001) {
				return false;
			}

			nowV = nowV->GetNext();
			//三角形の三点は見ないのでとばす
			if (nowV == v1) {
				nowV = nowV->GetNext();
			}
			if (nowV == v2) {
				nowV = nowV->GetNext();
			}
			if (nowV == v3) {
				nowV = nowV->GetNext();
			}
		}
		nowS = nowS->GetNextShape();
	}

	return true;
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

//頂点の追加
void CAdminControl::CreateShape(float x, float y)
{
	//一番最初クリックされたときにshape生成
	if (shape_head == NULL) {
		AppendShape();
	}

	//一つ前の点の位置とクリックした位置を比較する
	if (shape_head->CountVertex() >= 1) {
		CVertex* endV = shape_head->GetV();
		while (endV->GetNext() != NULL) {
			endV = endV->GetNext();
		}
		//全く同じ場所に打った場合return
		if (endV->GetX() == x && endV->GetY() == y) {
			return;
		}
	}
	//点が3点未満の場合
	if (shape_head->CountVertex() < 3) {
		//一点目かどうか
		if (shape_head->CountVertex() == 0) {
			if (Inclusion(x, y)) {
				shape_head->AppendVertex(x, y);
			}
		}
		else {
			//一点目と今回打つ三点目が近い場合、三点目を追加せずにreturn
			if (shape_head->CountVertex() == 2 && Distance(shape_head->GetV(), x, y) <= point_dist) {
				return;
			}
			//他交差していなければ点の追加
			if (OtherCross(x, y)) {
				shape_head->AppendVertex(x, y);
			}
		}
	}
	//4点目以降でクリックした位置と始点が近い場合
	else if (Distance(shape_head->GetV(), x, y) <= 0.1) {
		float sx = shape_head->GetV()->GetX();
		float sy = shape_head->GetV()->GetY();
		
		//始点に向けて線を引いたとき、自交差＆他交差＆形状の内包をしていなければ始点の位置に点を追加して形状を閉じる
		if (OtherCross(sx, sy) && Cross(sx, sy) && Wrap()) {
			shape_head->AppendVertex(sx, sy);
			AppendShape();
		}
		//自交差＆他交差してなければ点を追加
		else if (OtherCross(x, y) && Cross(x, y)) {
			shape_head->AppendVertex(x, y);
		}
	}
	//4点目以降
	else {
		//自交差＆他交差してなければ点を追加
		if (OtherCross(x, y) && Cross(x, y)) {
			shape_head->AppendVertex(x, y);
		}
	}
}

//点の削除
void CAdminControl::DeletePoint(CVertex* deleteV)
{
	if (deleteV == NULL && control_point == NULL) {
		return;
	}
	CShape* nowS = shape_head->GetNextShape();
	CVertex* nowV = NULL;
	CVertex* preV = NULL;

	if (deleteV == NULL) {
		deleteV = control_point;
	}

	while (nowS != NULL) {
		nowV = nowS->GetV();
		while (nowV != NULL) {
			//形状の頂点が4点以下の場合削除しない
			if (nowS->CountVertex() <= 4) {
				break;
			}
			if (nowV == deleteV) {
				//選択された点が形状の始点の場合
				if (preV == NULL) {
					while (nowV->GetNext() != NULL) {
						preV = nowV;
						nowV = nowV->GetNext();
					}
					//選択された点を削除してよいかの判定
					if (TriangleInclusion(preV, deleteV, deleteV->GetNext())) {
						nowS->ChangeVHead(deleteV->GetNext());
						delete nowV;
						delete deleteV;
						preV->SetNext(NULL);
						nowS->AppendVertex(nowS->GetV()->GetX(), nowS->GetV()->GetY());
						control_point = NULL;
					}
				}
				//終点一つ前の点が選択されている場合
				else if (deleteV->GetNext()->GetNext() == NULL && TriangleInclusion(preV, deleteV, nowS->GetV())) {
					preV->SetNext(deleteV->GetNext());
					delete deleteV;
					control_point = NULL;
				}
				//それ以外の点が選択されている場合
				else if (TriangleInclusion(preV, deleteV, deleteV->GetNext())) {
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

	while (nowV != NULL) {
		if (nowV == deleteV) {
			//選択された点が形状の始点の場合
			if (preV == NULL) {
				while (nowV->GetNext() != NULL) {
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
			else {
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

//形状の選択をして良いかの判定する　できるときはtrue
boolean CAdminControl::CanSelect() 
{
	//形状が一つ以上存在していて、全ての形状が閉じているとき
	if (shape_head != NULL && shape_head->GetV() == NULL) {
		return true;
	}

	return false;
}

//形状の選択(形状の最終点が選ばれることはない)
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

	while (nowS != NULL) {
		nowV = nowS->GetV();
		nextV = nowV->GetNext();

		//始点だけ近いか見る
		nowPdist = Distance(nowV, x, y);
		if (nowPdist < minPdist) {
			selectPoint = nowV;
			minPdist = nowPdist;
		}
		while (nextV != NULL) {
			//select_distより近い点の中で一番近い点を格納
			nowPdist = Distance(nextV, x, y);
			nowEdist = PointLineDistance(nowV, nextV, x, y);
			if (nowPdist < minPdist) {
				selectPoint = nextV;
				minPdist = nowPdist;
			}
			//select_distより近い稜線の中で一番近い辺を格納
			if (nowEdist < minEdist) {
				selectEdge = nowV;
				minEdist = nowEdist;
			}
			nowV = nextV;
			nextV = nextV->GetNext();
		}
		//クリックした位置を内包している形状を格納
		if (!Inclusion(nowS, x, y)) {
			selectShape = nowS;
		}

		nowS = nowS->GetNextShape();
	}
	//頂点の選択
	if (selectPoint != NULL) {
		InitSelect();
		control_point = selectPoint;
		return;
	}
	//稜線の選択
	if (selectEdge != NULL) {
		InitSelect();
		control_edge = selectEdge;
		return;
	}
	//形状の選択
	if (selectShape != NULL) {
		InitSelect();
		control_shape = selectShape;
		return;
	}
	InitSelect();
}

