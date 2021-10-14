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

//�@���_��X���W����������
void CVertex::SetX(float new_x)
{
	x = new_x;
}

//�@���_��Y���W����������
void CVertex::SetY(float new_y)
{
	y = new_y;
}

//�@���_��X�EY���W�̗�������������
void CVertex::SetXY(float new_x, float new_y)
{
	x = new_x;
	y = new_y;
}

//�@���_��X���W��ǂݍ��ލ���
float CVertex::GetX()
{
	return x;
}

//�@���_��Y���W��ǂݍ���
float CVertex::GetY()
{
	return y;
}

//�@���_��X�EY���W�̗�����ǂݍ���
void CVertex::GetXY(float* get_x, float* get_y)
{
	get_x = &x;
	get_y = &y;
}

//�@���̒��_���X�g���w���|�C���^����������
void CVertex::SetNext(CVertex* new_next)
{
	next_vertex = new_next;
}

//�@���̒��_���X�g���w���|�C���^��ǂݍ���
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
