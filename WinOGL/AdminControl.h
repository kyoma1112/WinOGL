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
	void DrawCursor(CRect rect, float x, float y);

	//�V�����`��̒ǉ�
	void AppendShape();
	//�I���̉���
	void InitSelect();
	//�_�Ɠ_�̋����̌v�Z
	float Distance(CVertex* s, float x, float y);
	//�_�ƒ����̋����̌v�Z
	float PointLineDistance(CVertex* nowV, CVertex* nextV, float x, float y);
	//�Ȃ��p�̌v�Z(����ab�ƒ���bc�̂Ȃ��p�̌v�Z)
	float AngleCalc(float ax, float ay, float bx, float by, float cx, float cy);
	//�_�̒ǉ�
	void CreateShape(float x, float y);
	//�_�̍폜
	void DeletePoint(CVertex* deleteV);
	//�O�p�`�������̎O�p�`�̍폜
	void DeleteTriangle(CShape* printS, CVertex* deleteV);
	//���_�̈ړ�
	void MovePoint(CVertex* moveV, float x, float y);
	//�`��̈ړ�(x, y���`��̏d�S�ɂȂ�)
	void MoveShape(CShape* moveS, float x, float y);
	//�ӏ�ɓ_��ǉ�����
	void DivideEdge(CVertex* v1, float x, float y);
	//��_��ԋp
	CVertex* GetBasePoint();
	//��_�̒ǉ�
	void AddBasePoint(float x, float y);
	//��_�̍폜
	void DeleteBasePoint();
	//�`��̉�]�ړ�
	void RotateShape(CShape* rotateS, float baseX, float baseY, float x, float y);
	//�`��̊g��k��
	void ResizeShape(CShape* resizeS, float baseX, float baseY, short zDelta);
	//�`��̑I��
	void SelectShape(float x, float y);

	//����������@true�Ȃ�������ĂȂ��Afalse�Ȃ�������Ă���
	boolean Cross(float mx, float my);
	//��_���Ȃ������Ƃ̎���������(Bs, Be�̍��W��ʂ̒l�ɒu��������)�@true�Ȃ�������ĂȂ��Afalse�Ȃ�������Ă���
	boolean Cross(CShape* nowS, CVertex* Bs, CVertex* Be, float Bs_x, float Bs_y, float Be_x, float Be_y);
	//��_���Ȃ������Ƃ̎���������@true�Ȃ�������ĂȂ��Afalse�Ȃ�������Ă���
	boolean Cross(CShape* nowS, CVertex* Bs, CVertex* Be);
	//����������@true�Ȃ�������ĂȂ��Afalse�Ȃ�������Ă���
	boolean OtherCross(float mx, float my);
	//��̍��W���Ȃ������Ƃ̑���������@true�Ȃ�������ĂȂ��Afalse�Ȃ�������Ă���
	boolean OtherCross(CShape* skipS, float Bs_x, float Bs_y, float Be_x, float Be_y);
	//��_���Ȃ������Ƃ̑���������@true�Ȃ�������ĂȂ��Afalse�Ȃ�������Ă���
	boolean OtherCross(CShape* skipS, CVertex* Bs, CVertex* Be);
	//�����v�Z  true�Ȃ�������Ă�Afalse�Ȃ�������Ă��Ȃ�
	boolean CrossCalc(CVertex* As, CVertex* Ae, CVertex* Bs, float mx, float my);
	//�����v�Z  true�Ȃ�������Ă�Afalse�Ȃ�������Ă��Ȃ�
	boolean CrossCalc(float As_x, float As_y, float Ae_x, float Ae_y, float Bs_x, float Bs_y, float Be_x, float Be_y);
	//�_�̓����(�S�Ă̌`��Ŕ��肷��)�@true�Ȃ�O�Afalse�Ȃ��
	boolean Inclusion(float x, float y);
	//�_�̓����(��̌`��Ŕ��肷��)�@true�Ȃ�O�Afalse�Ȃ��
	boolean Inclusion(CShape* nowS, float x, float y);
	//�`��̓���� true�Ȃ�O�Afalse�Ȃ��
	boolean Wrap();
	//�`��̓����(�_�̈ړ��Ŏg�p) true�Ȃ�O�Afalse�Ȃ��
	boolean Wrap(CShape* checkS, CVertex* v2, float mx, float my);
	//�`��̓����(�`����w��) true�Ȃ�O�Afalse�Ȃ��
	boolean Wrap(CShape* checkS);
	//�`��̎O�p�`�����̒��Ɍ`����̑��̒��_�����݂��邩�̔��� true�Ȃ�O�p�`�����ɒ��_�����Afalse�Ȃ�O�p�`�����ɒ��_�L��
	boolean TriangleInclusion(CShape* nowS, CVertex* v1, CVertex* v2, CVertex* v3);
	//�`��̎O�p�`�̕����̒��ɎO�p�`�̎O�_�������S�Ă̌`��̒��_�����݂��Ă��邩�̔���@true�Ȃ�O�p�`�����ɒ��_�����Afalse�Ȃ�O�p�`�����ɒ��_�L��
	boolean TriangleInclusion(CVertex* v1, CVertex* v2, CVertex* v3);
	//�`��̑I�������ėǂ����̔��肷��
	boolean CanSelect();
	//�`�󂪑I������Ă��邩�̔���(�_�A�ӁA�`�󂪑I������Ă���ꍇ�A���ꂼ��1,2,3��Ԃ��B)
	int NowSelect();
	
private:
	CShape* shape_head = NULL;
	CVertex* control_point = NULL;
	CVertex* control_edge = NULL;
	CShape* control_shape = NULL;
	CVertex* base_point = NULL;
};

