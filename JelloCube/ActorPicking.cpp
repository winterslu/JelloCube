//
//  ActorPicking.cpp
//  JelloCube
//
//  Created by Ke Lu on 12-2-6.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "jello.h"
#include "ActorPicking.h"


void Project(bool isPick)
{
    if(!isPick)
        return;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	if(isPick)
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT,viewport);
		gluPickMatrix(g_vMousePos[0], viewport[3]-g_vMousePos[1], 10, 10, viewport);
	}
}

void ViewProject(point * v, int &xi, int &yi, float &depth)
{
	GLint viewPort[4];
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	glGetIntegerv(GL_VIEWPORT, viewPort);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	GLdouble winX, winY, winZ;
	gluProject((GLdouble) v->x, (GLdouble) v->y, (GLdouble) v->z,
               modelMatrix, projMatrix, viewPort, &winX, &winY, &winZ);
	xi = (int)winX; yi = viewPort[3] - (int)winY - 1; depth = (float)winZ;
}

void ViewUnProject(int xi, int yi, float depth, point * v)
{
	GLint viewPort[4];
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	glGetIntegerv(GL_VIEWPORT, viewPort);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	yi = viewPort[3] - yi - 1;
	GLdouble wx, wy, wz;
	gluUnProject((GLdouble) xi, (GLdouble) yi, (GLdouble) depth,
                 modelMatrix, projMatrix, viewPort, &wx, &wy, &wz);
    pMAKE(wx, wy, wz, *v);
}


bool Pick(int x, int y)
{
    double length;
    point ray_orig, ray_dir;
    ViewUnProject(x, y, 0.0f, &ray_orig);
    ViewUnProject(x, y, 1.0f, &ray_dir);
    pDIFFERENCE(ray_dir, ray_orig, ray_dir);
    pNORMALIZE(ray_dir);
    
}



void MoveActor(int x, int y)
{
//	if (!gMouseSphere) return;
//	NxVec3 pos;
//	ViewUnProject(x,y, gMouseDepth, pos);
	//gMouseSphere->setGlobalPosition(pos);
}