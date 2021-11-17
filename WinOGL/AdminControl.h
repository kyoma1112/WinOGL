#pragma once
#include <gl/GL.h>
#include "Vertex.h"
#include "pch.h"
#include "Shape.h"

#define pi 3.141592
#define point_dist 0.005
#define select_dist 0.018

class CAdminControl
{
	
public:
	//カーソルを表示するモード
	bool cursorMode = false;
	//編集モード
	bool editMode = false;

	CAdminControl();
	~CAdminControl();

	//面の表示
	void DrawArea(CShape* printS);
	//面以外の表示
	void Draw();
	//カーソルの表示
	void DrawCursor(CRect rect);

	//新しい形状の追加
	void AppendShape();
	//選択の解除
	void InitSelect();
	//点と点の距離の計算
	float Distance(CVertex* s, float x, float y);
	//点と直線の距離の計算
	float PointLineDistance(CVertex* nowV, CVertex* nextV, float x, float y);
	//点の追加
	void CreateShape(float x, float y);
	//点の削除
	void DeletePoint(CVertex* deleteV);
	//三角形分割中の三角形の削除
	void DeleteTriangle(CShape* printS, CVertex* deleteV);
	//形状の選択
	void SelectShape(float x, float y);

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
	//形状の三角形部分の中に形状内の他の頂点が存在するかの判定 trueなら三角形内部に頂点無し、falseなら三角形内部に頂点有り
	boolean TriangleInclusion(CShape* nowS, CVertex* v1, CVertex* v2, CVertex* v3);
	//形状の三角形の部分の中に三角形の三点を除く全ての形状の頂点が存在しているかの判定　trueなら三角形内部に頂点無し、falseなら三角形内部に頂点有り
	boolean TriangleInclusion(CVertex* v1, CVertex* v2, CVertex* v3);
	//形状の選択をして良いかの判定する
	boolean CanSelect();
	
private:
	CShape* shape_head = NULL;
	CVertex* control_point = NULL;
	CVertex* control_edge = NULL;
	CShape* control_shape = NULL;
	
};

