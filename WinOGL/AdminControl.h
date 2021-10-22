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

	//����������@true�Ȃ�������ĂȂ��Afalse�Ȃ�������Ă���
	boolean Cross(float mx, float my);
	//����������@true�Ȃ�������ĂȂ��Afalse�Ȃ�������Ă���
	boolean OtherCross(float mx, float my);
	//�����v�Z  true�Ȃ�������Ă�Afalse�Ȃ�������Ă��Ȃ�
	boolean CrossCalc(CVertex* As, CVertex* Ae, CVertex* Bs, float mx, float my);
	//�_�̓����@true�Ȃ�O�Afalse�Ȃ��
	boolean inclusion(float x, float y);
	//�`��̓���� true�Ȃ�O�Afalse�Ȃ��
	boolean wrap();
	
private:
	CShape* shape_head = NULL;
};

