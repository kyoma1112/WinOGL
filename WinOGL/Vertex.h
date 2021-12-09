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
	//�@���_��X���W
	float x;
	//�@���_��Y���W
	float y;
	//  ���_��Z���W
	float z = 0;
	//�@���̒��_���X�g���w���|�C���^
	CVertex* next_vertex;
public:
	//�@���_��X���W����������
	void SetX(float new_x);
	//�@���_��Y���W����������
	void SetY(float new_y);
	//�@���_��Z���W����������
	void SetZ(float new_z);
	//�@���_��X�EY���W�̗�������������
	void SetXY(float new_x, float new_y);
	//�@���_��X���W��ǂݍ��ލ���
	float GetX();
	//�@���_��Y���W��ǂݍ���
	float GetY();
	//  ���_��Z���W��ǂݍ���
	float GetZ();
	//�@���_��X�EY���W�̗�����ǂݍ���
	void GetXY(float* get_x, float* get_y);
	//�@���̒��_���X�g���w���|�C���^����������
	void SetNext(CVertex* new_next);
	//�@���̒��_���X�g���w���|�C���^��ǂݍ���
	CVertex* GetNext();

	void FreeVertex();
};

