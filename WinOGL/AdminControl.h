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

	//�ʂ̕\��
	//void AreaDraw(CShape* printS);
	//�ʈȊO�̕\��
	void Draw();
	//�J�[�\���̕\��
	void CursorDraw(CRect rect);

	//�V�����`��̒ǉ�
	void AppendShape();
	//�_�Ɠ_�̋����̌v�Z
	float Distance(CVertex* s, float x, float y);
	//�_�ƒ����̋����̌v�Z
	float PointLineDistance(CVertex* nowV, CVertex* nextV, float x, float y);
	//���N���b�N�����Ƃ��̏���
	void CreateShape(float x, float y);
	//�E�N���b�N�������Ƃ��̏���

	//����������@true�Ȃ�������ĂȂ��Afalse�Ȃ�������Ă���
	boolean Cross(float mx, float my);
	//����������@true�Ȃ�������ĂȂ��Afalse�Ȃ�������Ă���
	boolean OtherCross(float mx, float my);
	//�����v�Z  true�Ȃ�������Ă�Afalse�Ȃ�������Ă��Ȃ�
	boolean CrossCalc(CVertex* As, CVertex* Ae, CVertex* Bs, float mx, float my);
	//�_�̓����(�S�Ă̌`��Ŕ��肷��)�@true�Ȃ�O�Afalse�Ȃ��
	boolean Inclusion(float x, float y);
	//�_�̓����(��̌`��Ŕ��肷��)�@true�Ȃ�O�Afalse�Ȃ��
	boolean Inclusion(CShape* nowS, float x, float y);
	//�`��̓���� true�Ȃ�O�Afalse�Ȃ��
	boolean Wrap();
	//�`��̒��_�̔z�u��������@true�Ȃ玞�v����Afalse�Ȃ甽���v����
	boolean Direction(CShape* nowS);
	//�O�p�`�̔z�u�������� true�Ȃ玞�v����Afalse�Ȃ甼���v����
	boolean TriangleDirection(CVertex* startV, CVertex* middleV, CVertex* endV);
	
private:
	CShape* shape_head = NULL;
	CVertex* control_point = NULL;
	CVertex* control_edge = NULL;
	CShape* control_shape = NULL;
	
};

