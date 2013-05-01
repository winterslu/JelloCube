/*
 
 USC/Viterbi/Computer Science
 "Jello Cube" Assignment 1 starter code
 
 */

#include "jello.h"
#include "showCube.h"

int pointMap(int side, int i, int j)
{
    int r;
    
    switch (side)
    {
        case 1: //[i][j][0] bottom face
            r = 64 * i + 8 * j;
            break;
        case 6: //[i][j][7] top face
            r = 64 * i + 8 * j + 7;
            break;
        case 2: //[i][0][j] front face
            r = 64 * i + j;
            break;
        case 5: //[i][7][j] back face
            r = 64 * i + 56 + j;
            break;
        case 3: //[0][i][j] left face
            r = 8 * i + j;
            break;
        case 4: //[7][i][j] right face
            r = 448 + 8 * i + j;
            break;
    }
    
    return r;
}

void showCube(struct world * jello)
{
    glInitNames();
    glEnable(GL_LIGHTING);    
    glEnable(GL_DEPTH_TEST);    
    
    
    int i,j,k,ip,jp,kp;
    point r1,r2,r3; // aux variables
    
    /* normals buffer and counter for Gourad shading*/
    struct point normal[8][8];
    int counter[8][8];
    
    int face;
    double faceFactor, length;
    
    if (fabs(jello->p[0][0][0].x) > 10)
    {
        printf ("Your cube somehow escaped way out of the box.\n");
        exit(0);
    }
    
    
#define NODE(face,i,j) (*((struct point * )(jello->p) + pointMap((face),(i),(j))))
    
    
#define PROCESS_NEIGHBOUR(di,dj,dk) \
ip=i+(di);\
jp=j+(dj);\
kp=k+(dk);\
if\
(!( (ip>7) || (ip<0) ||\
(jp>7) || (jp<0) ||\
(kp>7) || (kp<0) ) && ((i==0) || (i==7) || (j==0) || (j==7) || (k==0) || (k==7))\
&& ((ip==0) || (ip==7) || (jp==0) || (jp==7) || (kp==0) || (kp==7))) \
{\
glVertex3f(jello->p[i][j][k].x,jello->p[i][j][k].y,jello->p[i][j][k].z);\
glVertex3f(jello->p[ip][jp][kp].x,jello->p[ip][jp][kp].y,jello->p[ip][jp][kp].z);\
}\

    
    if (viewingMode==0) // render wireframe
    {
        glLineWidth(1);
        glPointSize(5);
        glDisable(GL_LIGHTING);
        for (i=0; i<=7; i++)
            for (j=0; j<=7; j++)
                for (k=0; k<=7; k++)
                {
                    if (i*j*k*(7-i)*(7-j)*(7-k) != 0) // not surface point
                        continue;
                    
                    glBegin(GL_POINTS); // draw point
                    glColor4f(0,0,0,0);  
                    glVertex3f(jello->p[i][j][k].x,jello->p[i][j][k].y,jello->p[i][j][k].z);        
                    glEnd();
                    
                    //
                    //if ((i!=7) || (j!=7) || (k!=7))
                    //  continue;
                    
                    glBegin(GL_LINES);      
                    // structural
                    if (structural == 1)
                    {
                        glColor4f(0,0,1,1);
                        PROCESS_NEIGHBOUR(1,0,0);
                        PROCESS_NEIGHBOUR(0,1,0);
                        PROCESS_NEIGHBOUR(0,0,1);
                        PROCESS_NEIGHBOUR(-1,0,0);
                        PROCESS_NEIGHBOUR(0,-1,0);
                        PROCESS_NEIGHBOUR(0,0,-1);
                    }
                    
                    // shear
                    if (shear == 1)
                    {
                        glColor4f(0,1,0,1);
                        PROCESS_NEIGHBOUR(1,1,0);
                        PROCESS_NEIGHBOUR(-1,1,0);
                        PROCESS_NEIGHBOUR(-1,-1,0);
                        PROCESS_NEIGHBOUR(1,-1,0);
                        PROCESS_NEIGHBOUR(0,1,1);
                        PROCESS_NEIGHBOUR(0,-1,1);
                        PROCESS_NEIGHBOUR(0,-1,-1);
                        PROCESS_NEIGHBOUR(0,1,-1);
                        PROCESS_NEIGHBOUR(1,0,1);
                        PROCESS_NEIGHBOUR(-1,0,1);
                        PROCESS_NEIGHBOUR(-1,0,-1);
                        PROCESS_NEIGHBOUR(1,0,-1);
                        
                        PROCESS_NEIGHBOUR(1,1,1)
                        PROCESS_NEIGHBOUR(-1,1,1)
                        PROCESS_NEIGHBOUR(-1,-1,1)
                        PROCESS_NEIGHBOUR(1,-1,1)
                        PROCESS_NEIGHBOUR(1,1,-1)
                        PROCESS_NEIGHBOUR(-1,1,-1)
                        PROCESS_NEIGHBOUR(-1,-1,-1)
                        PROCESS_NEIGHBOUR(1,-1,-1)
                    }
                    
                    // bend
                    if (bend == 1)
                    {
                        glColor4f(1,0,0,1);
                        PROCESS_NEIGHBOUR(2,0,0);
                        PROCESS_NEIGHBOUR(0,2,0);
                        PROCESS_NEIGHBOUR(0,0,2);
                        PROCESS_NEIGHBOUR(-2,0,0);
                        PROCESS_NEIGHBOUR(0,-2,0);
                        PROCESS_NEIGHBOUR(0,0,-2);
                    }           
                    glEnd();
                }
        glEnable(GL_LIGHTING);
    }
    
    else
    {
        
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_COLOR);
        glPolygonMode(GL_FRONT, GL_FILL); 
        
        for (face=1; face <= 6; face++) 
            // face == face of a cube
            // 1 = bottom, 2 = front, 3 = left, 4 = right, 5 = far, 6 = top
        {
            glPushName(face);
            if ((face==1) || (face==3) || (face==5))
                faceFactor=-1; // flip orientation
            else
                faceFactor=1;
            
            
            for (i=0; i <= 7; i++) // reset buffers
                for (j=0; j <= 7; j++)
                {
                    normal[i][j].x=0;normal[i][j].y=0;normal[i][j].z=0;
                    counter[i][j]=0;
                }
            
            /* process triangles, accumulate normals for Gourad shading */
            
            for (i=0; i <= 6; i++)
                for (j=0; j <= 6; j++) // process block (i,j)
                {
                    pDIFFERENCE(NODE(face,i+1,j),NODE(face,i,j),r1); // first triangle
                    pDIFFERENCE(NODE(face,i,j+1),NODE(face,i,j),r2);
                    CROSSPRODUCTp(r1,r2,r3); pMULTIPLY(r3,faceFactor,r3);
                    pNORMALIZE(r3);
                    pSUM(normal[i+1][j],r3,normal[i+1][j]);
                    counter[i+1][j]++;
                    pSUM(normal[i][j+1],r3,normal[i][j+1]);
                    counter[i][j+1]++;
                    pSUM(normal[i][j],r3,normal[i][j]);
                    counter[i][j]++;
                    
                    pDIFFERENCE(NODE(face,i,j+1),NODE(face,i+1,j+1),r1); // second triangle
                    pDIFFERENCE(NODE(face,i+1,j),NODE(face,i+1,j+1),r2);
                    CROSSPRODUCTp(r1,r2,r3); pMULTIPLY(r3,faceFactor,r3);
                    pNORMALIZE(r3);
                    pSUM(normal[i+1][j],r3,normal[i+1][j]);
                    counter[i+1][j]++;
                    pSUM(normal[i][j+1],r3,normal[i][j+1]);
                    counter[i][j+1]++;
                    pSUM(normal[i+1][j+1],r3,normal[i+1][j+1]);
                    counter[i+1][j+1]++;
                }
            
            
            /* the actual rendering */
            for (j=1; j<=7; j++) 
            {
                
                if (faceFactor  > 0)
                    glFrontFace(GL_CCW); // the usual definition of front face
                else
                    glFrontFace(GL_CW); // flip definition of orientation
                
                glBegin(GL_TRIANGLE_STRIP);
                for (i=0; i<=7; i++)
                {
                    glNormal3f(normal[i][j].x / counter[i][j],normal[i][j].y / counter[i][j],
                               normal[i][j].z / counter[i][j]);
                    glVertex3f(NODE(face,i,j).x, NODE(face,i,j).y, NODE(face,i,j).z);
                    glNormal3f(normal[i][j-1].x / counter[i][j-1],normal[i][j-1].y/ counter[i][j-1],
                               normal[i][j-1].z / counter[i][j-1]);
                    glVertex3f(NODE(face,i,j-1).x, NODE(face,i,j-1).y, NODE(face,i,j-1).z);
                }
                glEnd();
            }
            
            
        }
        
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        
        
    } // end for loop over faces
    glFrontFace(GL_CCW);
    
    glDisable(GL_LIGHTING);
    glLoadName(-1);
}

void showBoundingBox()
{
    switch (viewingMode)
    {
        case 0:// render wireframe
            
            int i,j;
            
            glColor4f(0.6,0.6,0.6,0);
            
            glBegin(GL_LINES);
            
            // front face
            for(i=-2; i<=2; i++)
            {
                glVertex3f(i,-2,-2);
                glVertex3f(i,-2,2);
            }
            for(j=-2; j<=2; j++)
            {
                glVertex3f(-2,-2,j);
                glVertex3f(2,-2,j);
            }
            
            // back face
            for(i=-2; i<=2; i++)
            {
                glVertex3f(i,2,-2);
                glVertex3f(i,2,2);
            }
            for(j=-2; j<=2; j++)
            {
                glVertex3f(-2,2,j);
                glVertex3f(2,2,j);
            }
            
            // left face
            for(i=-2; i<=2; i++)
            {
                glVertex3f(-2,i,-2);
                glVertex3f(-2,i,2);
            }
            for(j=-2; j<=2; j++)
            {
                glVertex3f(-2,-2,j);
                glVertex3f(-2,2,j);
            }
            
            // right face
            for(i=-2; i<=2; i++)
            {
                glVertex3f(2,i,-2);
                glVertex3f(2,i,2);
            }
            for(j=-2; j<=2; j++)
            {
                glVertex3f(2,-2,j);
                glVertex3f(2,2,j);
            }
            
            glEnd();
            break;
        case 1:
            
            glEnable(GL_LIGHTING);
            glEnable(GL_DEPTH_TEST);
            //glDisable(GL_CULL_FACE);
            
            glBegin(GL_POLYGON);
            glNormal3f(1.0f,0.0f,0.0f);
            glVertex3f(-2.0f,-2.0f,-2.0f);
            glVertex3f(-2.0f,2.0f,-2.0f);
            glVertex3f(-2.0f,2.0f,2.0f);
            glVertex3f(-2.0f,-2.0f,2.0f);
            glEnd();
            
            glBegin(GL_POLYGON);
            glNormal3f(-1.0f,0.0f,0.0f);
            glVertex3f(2.0f,-2.0f,2.0f);
            glVertex3f(2.0f,2.0f,2.0f);
            glVertex3f(2.0f,2.0f,-2.0f);
            glVertex3f(2.0f,-2.0f,-2.0f);
            glEnd();
            
            glBegin(GL_POLYGON);
            glNormal3f(0.0f,0.0f,-1.0f);
            glVertex3f(2.0f,-2.0f,2.0f);
            glVertex3f(-2.0f,-2.0f,2.0f);
            glVertex3f(-2.0f,2.0f,2.0f);
            glVertex3f(2.0f,2.0f,2.0f);
            glEnd();
            
            glBegin(GL_POLYGON);
            glNormal3f(0.0f,0.0f,1.0f);
            glVertex3f(2.0f,2.0f,-2.0f);
            glVertex3f(-2.0f,2.0f,-2.0f);
            glVertex3f(-2.0f,-2.0f,-2.0f);
            glVertex3f(2.0f,-2.0f,-2.0f);
            glEnd();
            
            glBegin(GL_POLYGON);
            glNormal3f(0.0f,-1.0f,0.0f);
            glVertex3f(2.0f,2.0f,2.0f);
            glVertex3f(-2.0f,2.0f,2.0f);
            glVertex3f(-2.0f,2.0f,-2.0f);
            glVertex3f(2.0f,2.0f,-2.0f);
            glEnd();
            
            glBegin(GL_POLYGON);
            glNormal3f(0.0f,1.0f,0.0f);
            glVertex3f(2.0f,-2.0f,-2.0f);
            glVertex3f(-2.0f,-2.0f,-2.0f);
            glVertex3f(-2.0f,-2.0f,2.0f);
            glVertex3f(2.0f,-2.0f,2.0f);
            glEnd();
            
            //glEnable(GL_CULL_FACE);
            glDisable(GL_LIGHTING);
            break;
    }
    
    return;
}


void showInclinedPlane(struct world * jello)
{
    double a=jello->a,b=jello->b,c=jello->c,d=jello->d;
    double x1=-2,x2=2,y1=-2,y2=2,z1=-2,z2=2;
    
    point XYZ[12];
    for(int i=0;i<8;i++)
        XYZ[i].x=999;
    
    double n,length;
    n =(-d-a*x1-b*y1)/c;
    pMAKE(x1, y1, n, XYZ[0]);
    n =(-d-a*x1-b*y2)/c;
    pMAKE(x1, y2, n, XYZ[1]);
    n =(-d-a*x2-b*y1)/c;
    pMAKE(x2, y1, n, XYZ[2]);
    n =(-d-a*x2-b*y2)/c;
    pMAKE(x2, y2, n, XYZ[3]);
    
    n =(-d-a*x1-c*z1)/b;
    pMAKE(x1, n, z1, XYZ[4]);
    n =(-d-a*x1-c*z2)/b;
    pMAKE(x1, n, z2, XYZ[5]);
    n =(-d-a*x2-c*z1)/b;
    pMAKE(x2, n, z1, XYZ[6]);
    n =(-d-a*x2-c*z2)/b;
    pMAKE(x2, n, z2, XYZ[7]);
    
    n =(-d-c*z1-b*y1)/a;
    pMAKE(n, y1, z1, XYZ[8]);
    n =(-d-c*z2-b*y1)/a;
    pMAKE(n, y1, z2, XYZ[9]);
    n =(-d-c*z1-b*y2)/a;
    pMAKE(n, y2, z1, XYZ[10]);
    n =(-d-c*z2-b*y2)/a;
    pMAKE(n, y2, z2, XYZ[11]);

    
    point p;
    pMAKE(a,b,c,p);
    pNORMALIZE(p);
    
    switch (viewingMode)
    {
        case 1:
            glEnable(GL_LIGHTING);
            glEnable(GL_DEPTH_TEST);
            //glColor4f(0.3,0.4,0.2,0);
            glDisable(GL_CULL_FACE);
            
            glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(p.x, p.y, p.z);
            
            for(int i =0;i<12;i++)
                if(XYZ[i].x >=-2 &&XYZ[i].x <=2 &&XYZ[i].y >=-2 &&XYZ[i].y <=2 &&XYZ[i].z >=-2 &&XYZ[i].z <=2 )
                    glVertex3f(XYZ[i].x, XYZ[i].y, XYZ[i].z);
//            
            glEnd();
            
            glEnable(GL_CULL_FACE);
            glDisable(GL_LIGHTING);
            break;
        case 0:
            
            glBegin(GL_LINES);
            
            for(int i =0;i<12;i++)
                if(XYZ[i].x >=-2 &&XYZ[i].x <=2 &&XYZ[i].y >=-2 &&XYZ[i].y <=2 &&XYZ[i].z >=-2 &&XYZ[i].z <=2 )
                    for(int j =0;j<12;j++)
                    {
                        if(XYZ[j].x >=-2 &&XYZ[j].x <=2 &&XYZ[j].y >=-2 &&XYZ[j].y <=2 &&XYZ[j].z >=-2 &&XYZ[j].z <=2 )
                        {
                            int k=0;
                            if(XYZ[i].x==XYZ[j].x)
                                k++;
                            if(XYZ[i].y==XYZ[j].y)
                                k++;
                            if(XYZ[i].z==XYZ[j].z)
                                k++;
                            if(k==1)
                            {
                                glVertex3f(XYZ[i].x, XYZ[i].y, XYZ[i].z);
                                glVertex3f(XYZ[j].x, XYZ[j].y, XYZ[j].z);
                            }
                        }
                    }
            
            glEnd();
            
            break;
    }
    return;
}
