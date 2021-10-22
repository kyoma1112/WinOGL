#pragma once
#include <gl/GL.h>
#include "Vertex.h"
#include "pch.h"
#include "Shape.h"

#define pi 3.141592

class CAdminControl
{
	
public:
	CAdminControl();
	~CAdminControl();

	void Draw();

	void AppendShape();
	float Distance(CVertex* s, float x, float y);
	void CreateShape(float x, float y);

	//自交差判定　trueなら交差してない、falseなら交差している
	boolean Cross(float mx, float my);
	//他交差判定　trueなら交差してない、falseなら交差している
	boolean OtherCross(float mx, float my);
	//交差計算  trueなら交差してる、falseなら交差していない
	boolean CrossCalc(CVertex* As, CVertex* Ae, CVertex* Bs, float mx, float my);
	//点の内包判定　trueなら外、falseなら内
	boolean inclusion(float x, float y);
	//形状の内包判定 trueなら外、falseなら内
	boolean wrap();
	
private:
	CShape* shape_head = NULL;
};

