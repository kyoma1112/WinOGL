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
	//�J�[�\����\�����郂�[�h
	bool cursorMode = false;
	//�ҏW���[�h
	bool editMode = false;

	CAdminControl();
	~CAdminControl();

	//�ʂ̕\��
	void DrawArea(CShape* printS);
	//�ʈȊO�̕\��
	void Draw();
	//�J�[�\���̕\��
	void DrawCursor(CRect rect);

	//�V�����`��̒ǉ�
	void AppendShape();
	//�I���̉���
	void InitSelect();
	//�_�Ɠ_�̋����̌v�Z
	float Distance(CVertex* s, float x, float y);
	//�_�ƒ����̋����̌v�Z
	float PointLineDistance(CVertex* nowV, CVertex* nextV, float x, float y);
	//�_�̒ǉ�
	void CreateShape(float x, float y);
	//�_�̍폜
	void DeletePoint(CVertex* deleteV);
	//�O�p�`�������̎O�p�`�̍폜
	void DeleteTriangle(CShape* printS, CVertex* deleteV);
	//�`��̑I��
	void SelectShape(float x, float y);

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
	//�`��̎O�p�`�����̒��Ɍ`����̑��̒��_�����݂��邩�̔��� true�Ȃ�O�p�`�����ɒ��_�����Afalse�Ȃ�O�p�`�����ɒ��_�L��
	boolean TriangleInclusion(CShape* nowS, CVertex* v1, CVertex* v2, CVertex* v3);
	//�`��̎O�p�`�̕����̒��ɎO�p�`�̎O�_�������S�Ă̌`��̒��_�����݂��Ă��邩�̔���@true�Ȃ�O�p�`�����ɒ��_�����Afalse�Ȃ�O�p�`�����ɒ��_�L��
	boolean TriangleInclusion(CVertex* v1, CVertex* v2, CVertex* v3);
	//�`��̑I�������ėǂ����̔��肷��
	boolean CanSelect();
	
private:
	CShape* shape_head = NULL;
	CVertex* control_point = NULL;
	CVertex* control_edge = NULL;
	CShape* control_shape = NULL;
	
};

