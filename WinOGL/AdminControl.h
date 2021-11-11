#pragma once
#include <gl/GL.h>
#include "Vertex.h"
#include "pch.h"
#include "Shape.h"

#define pi 3.141592
#define point_dist 0.07
#define select_dist 0.018

class CAdminControl
{
	
public:
	CAdminControl();
	~CAdminControl();

	//面の表示
	//void AreaDraw(CShape* printS);
	//面以外の表示
	void Draw();
	//カーソルの表示
	void CursorDraw(CRect rect);

	//新しい形状の追加
	void AppendShape();
	//点と点の距離の計算
	float Distance(CVertex* s, float x, float y);
	//点と直線の距離の計算
	float PointLineDistance(CVertex* nowV, CVertex* nextV, float x, float y);
	//左クリックしたときの処理
	void CreateShape(float x, float y);
	//右クリックをしたときの処理

	//自交差判定　trueなら交差してない、falseなら交差している
	boolean Cross(float mx, float my);
	//他交差判定　trueなら交差してない、falseなら交差している
	boolean OtherCross(float mx, float my);
	//交差計算  trueなら交差してる、falseなら交差していない
	boolean CrossCalc(CVertex* As, CVertex* Ae, CVertex* Bs, float mx, float my);
	//点の内包判定(全ての形状で判定する)　trueなら外、falseなら内
	boolean Inclusion(float x, float y);
	//点の内包判定(一つの形状で判定する)　trueなら外、falseなら内
	boolean Inclusion(CShape* nowS, float x, float y);
	//形状の内包判定 trueなら外、falseなら内
	boolean Wrap();
	//形状の頂点の配置方向判定　trueなら時計周り、falseなら反時計周り
	boolean Direction(CShape* nowS);
	//三角形の配置方向判定 trueなら時計周り、falseなら半時計周り
	boolean TriangleDirection(CVertex* startV, CVertex* middleV, CVertex* endV);
	
private:
	CShape* shape_head = NULL;
	CVertex* control_point = NULL;
	CVertex* control_edge = NULL;
	CShape* control_shape = NULL;
	
};

