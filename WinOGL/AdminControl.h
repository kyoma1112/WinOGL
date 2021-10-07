#pragma once
#include <gl/GL.h>
#include "Vertex.h"

class CAdminControl
{
	
public:
	CAdminControl();
	~CAdminControl();

	void Draw();

	CVertex* AppendVertex(float x, float y);
	
	CVertex CV;
private:
	CVertex* vertex_head = NULL;
};

