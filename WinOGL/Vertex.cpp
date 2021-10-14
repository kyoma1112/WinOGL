#include "pch.h"
#include "Vertex.h"

CVertex::CVertex()
{
	x = 0;
	y = 0;
	next_vertex = NULL;
}

CVertex::CVertex(float new_x, float new_y)
{
	SetXY(new_x, new_y);
	next_vertex = NULL;
}

CVertex::CVertex(float new_x, float new_y, CVertex* new_next)
{
	SetXY(new_x, new_y);
	SetNext(new_next);
}

CVertex::~CVertex()
{

}

//　頂点のX座標を書き込む
void CVertex::SetX(float new_x)
{
	x = new_x;
}

//　頂点のY座標を書き込む
void CVertex::SetY(float new_y)
{
	y = new_y;
}

//　頂点のX・Y座標の両方を書き込む
void CVertex::SetXY(float new_x, float new_y)
{
	x = new_x;
	y = new_y;
}

//　頂点のX座標を読み込む込む
float CVertex::GetX()
{
	return x;
}

//　頂点のY座標を読み込む
float CVertex::GetY()
{
	return y;
}

//　頂点のX・Y座標の両方を読み込む
void CVertex::GetXY(float* get_x, float* get_y)
{
	get_x = &x;
	get_y = &y;
}

//　次の頂点リストを指すポインタを書き込む
void CVertex::SetNext(CVertex* new_next)
{
	next_vertex = new_next;
}

//　次の頂点リストを指すポインタを読み込む
CVertex* CVertex::GetNext()
{
	return next_vertex;
}

void CVertex::FreeVertex()
{
	CVertex* nowV = this;
	while (nowV != NULL)
	{
		CVertex* del_cell = nowV;
		nowV = nowV->GetNext();
		delete del_cell;
	}
}
