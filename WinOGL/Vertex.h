#pragma once
#include <stdio.h> 

class CVertex
{
public:
	CVertex();
	CVertex(float new_x, float new_y);
	CVertex(float new_x, float new_y, CVertex* new_next);
	~CVertex();
private:
	//　頂点のX座標
	float x;
	//　頂点のY座標
	float y;
	//  頂点のZ座標
	float z = 0;
	//　次の頂点リストを指すポインタ
	CVertex* next_vertex;
public:
	//　頂点のX座標を書き込む
	void SetX(float new_x);
	//　頂点のY座標を書き込む
	void SetY(float new_y);
	//　頂点のZ座標を書き込む
	void SetZ(float new_z);
	//　頂点のX・Y座標の両方を書き込む
	void SetXY(float new_x, float new_y);
	//　頂点のX座標を読み込む込む
	float GetX();
	//　頂点のY座標を読み込む
	float GetY();
	//  頂点のZ座標を読み込む
	float GetZ();
	//　頂点のX・Y座標の両方を読み込む
	void GetXY(float* get_x, float* get_y);
	//　次の頂点リストを指すポインタを書き込む
	void SetNext(CVertex* new_next);
	//　次の頂点リストを指すポインタを読み込む
	CVertex* GetNext();

	void FreeVertex();
};

