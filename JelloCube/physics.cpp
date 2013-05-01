/*

  USC/Viterbi/Computer Science
  "Jello Cube" Assignment 1 starter code

*/

#include "jello.h"
#include "physics.h"



void COMPUTE_FORCE(int i, int j, int k, int di, int dj, int dk, struct point * force, struct world * jello)
{
    int ip,jp,kp,spring;
    double length,L;
    point HookForce,DampingForce,tempVelocity;
    pMAKE(0.0, 0.0, 0.0, HookForce);
    pMAKE(0.0, 0.0, 0.0, DampingForce);
    ip=i+(di);
    jp=j+(dj);
    kp=k+(dk);
    if((abs(di)+abs(dj)+abs(dk))==1)
        spring = STRUCTURAL;
    else if((abs(di)+abs(dj)+abs(dk))==3)
        spring = LONG_SHEAR;
    else if((abs(di)+abs(dj)+abs(dk))==2 && (abs(di)==1 || abs(dj)==1 || abs(dk)==1))
        spring = SHORT_SHEAR;
    else if((abs(di)+abs(dj)+abs(dk))==2 && (abs(di)==2 || abs(dj)==2 || abs(dk)==2))
        spring = BEND;
    else spring = NOSPRING;
    
    if(!((ip>7) || (ip<0) || (jp>7) || (jp<0) || (kp>7) || (kp<0)))
    {
        /*Computes elastic force using Hook's Law on each point*/
        pDIFFERENCE(jello->p[ip][jp][kp],jello->p[i][j][k],HookForce);
        pLENGTH(HookForce,L);
        pNORMALIZE(HookForce);
        switch (spring) {
            case STRUCTURAL:
                pMULTIPLY(HookForce, (L-JELLOSIZE/7.0), HookForce);
                break;
            case SHORT_SHEAR:
                pMULTIPLY(HookForce, (L-sqrt(2 * (JELLOSIZE/7.0) * (JELLOSIZE/7))), HookForce);
                break;
            case LONG_SHEAR:
                pMULTIPLY(HookForce, (L-sqrt(3 * (JELLOSIZE/7.0) * (JELLOSIZE/7))), HookForce);
                break;
            case BEND:
                pMULTIPLY(HookForce, (L-2 * JELLOSIZE/7.0), HookForce);
                break;
            default:
                exit(0);
                break;
        }
        pMULTIPLY(HookForce,jello->kElastic,HookForce);
        /*Computes damping force on each point*/        
        pDIFFERENCE(jello->v[i][j][k],jello->v[ip][jp][kp],tempVelocity);
        pDIFFERENCE(jello->p[ip][jp][kp],jello->p[i][j][k],DampingForce);
        pNORMALIZE(DampingForce);
        DOTPRODUCT(tempVelocity,DampingForce,L);
        pMULTIPLY(DampingForce,L,DampingForce);
        pMULTIPLY(DampingForce,-jello->dElastic,DampingForce);
        
        pSUM(*force,DampingForce,*force);
        pSUM(*force,HookForce,*force);
    }
}

void COMPUTE_FORCEFIELD(int i, int j, int k, struct point * force, struct world * jello)
{
    int ip,jp,kp;
    double di,dj,dk;
    point p000,p001,p010,p011,p100,p101,p110,p111,tempF1,tempF2,tempF3;
    ip=(jello->p[i][j][k].x+BOXSIZE/2)*jello->resolution/BOXSIZE;
    jp=(jello->p[i][j][k].y+BOXSIZE/2)*jello->resolution/BOXSIZE;
    kp=(jello->p[i][j][k].z+BOXSIZE/2)*jello->resolution/BOXSIZE;
    di=((jello->p[i][j][k].x+BOXSIZE/2)-BOXSIZE/jello->resolution*ip)/(BOXSIZE/jello->resolution);
    dj=((jello->p[i][j][k].y+BOXSIZE/2)-BOXSIZE/jello->resolution*jp)/(BOXSIZE/jello->resolution);
    dk=((jello->p[i][j][k].z+BOXSIZE/2)-BOXSIZE/jello->resolution*kp)/(BOXSIZE/jello->resolution);
    
    p000 = jello->forceField[ip * jello->resolution * jello->resolution + jp * jello->resolution + kp];
    p001 = jello->forceField[ip * jello->resolution * jello->resolution + jp * jello->resolution + kp + 1];
    p010 = jello->forceField[ip * jello->resolution * jello->resolution + (jp + 1) * jello->resolution + kp];
    p011 = jello->forceField[ip * jello->resolution * jello->resolution + (jp + 1) * jello->resolution + kp + 1];
    p100 = jello->forceField[(ip + 1) * jello->resolution * jello->resolution + jp * jello->resolution + kp];
    p101 = jello->forceField[(ip + 1) * jello->resolution * jello->resolution + jp * jello->resolution + kp + 1];
    p110 = jello->forceField[(ip + 1) * jello->resolution * jello->resolution + (jp + 1) * jello->resolution + kp];
    p111 = jello->forceField[(ip + 1) * jello->resolution * jello->resolution + (jp + 1) * jello->resolution + kp + 1];
    pDIFFERENCE(p100,p000,tempF1);pMULTIPLY(tempF1,di,tempF1);pSUM(p000,tempF1,tempF1);
    pDIFFERENCE(p101,p001,tempF2);pMULTIPLY(tempF2,di,tempF2);pSUM(p001,tempF2,tempF2);
    pDIFFERENCE(tempF2, tempF1, tempF3);pMULTIPLY(tempF3, dk, tempF3);pSUM(tempF1,tempF3,tempF3);
    pDIFFERENCE(p110,p010,tempF1);pMULTIPLY(tempF1,di,tempF1);pSUM(p010,tempF1,tempF1);
    pDIFFERENCE(p111,p011,tempF2);pMULTIPLY(tempF2,di,tempF2);pSUM(p011,tempF2,tempF2);
    pDIFFERENCE(tempF2,tempF1,tempF2);pMULTIPLY(tempF2,dk,tempF2);pSUM(tempF1,tempF2,tempF2);
    pDIFFERENCE(tempF2,tempF3,tempF2);pMULTIPLY(tempF2,dj,tempF2);pSUM(tempF2,tempF3,tempF1);
    pSUM(*force,tempF1,*force);
}

void COMPUTE_COLLISION(int i, int j, int k, struct point * force, struct world * jello)
{
    double length,L,lx=0.0,ly=0.0,lz=0.0;
    point HookCollision,DampingCollision,tempVelocity;
    pMAKE(0.0,0.0,0.0,HookCollision);
    pMAKE(0.0,0.0,0.0,DampingCollision);
    
    double tempx=0.0,tempy=0.0,tempz=0.0;
    double tempa=0.0,tempb=0.0,tempc=0.0;
    if(jello->p[i][j][k].x>BOXSIZE/2)
    {
        lx = jello->p[i][j][k].x-BOXSIZE/2;
        tempa = -1.0;
        tempx = BOXSIZE/2-jello->p[i][j][k].x;
    }
    if(jello->p[i][j][k].x<-BOXSIZE/2)
    {
        lx = (-jello->p[i][j][k].x)-BOXSIZE/2;
        tempa = 1.0;
        tempx = BOXSIZE/2-jello->p[i][j][k].x;
    }
    if(jello->p[i][j][k].y>BOXSIZE/2)
    {
        ly = jello->p[i][j][k].y-BOXSIZE/2;
        tempb = -1.0;
        tempy = BOXSIZE/2-jello->p[i][j][k].y;
    }
    if(jello->p[i][j][k].y<-BOXSIZE/2)
    {
        ly = (-jello->p[i][j][k].y)-BOXSIZE/2;
        tempb = 1.0;
        tempy = BOXSIZE/2-jello->p[i][j][k].y;
    }
    if(jello->p[i][j][k].z>BOXSIZE/2)
    {
        lz = jello->p[i][j][k].z-BOXSIZE/2;
        tempc = -1.0;
        tempz = BOXSIZE/2-jello->p[i][j][k].z;
    }
    if(jello->p[i][j][k].z<-BOXSIZE/2)
    {
        lz = (-jello->p[i][j][k].z)-BOXSIZE/2;
        tempc = 1.0;
        tempz = BOXSIZE/2-jello->p[i][j][k].z;
    }
    
    /*Computes elastic force using Hook's Law on out-boudingbox points */
    L=sqrt(lx*lx+ly*ly+lz*lz);
    pMAKE(tempa,tempb,tempc,HookCollision);
    pNORMALIZE(HookCollision);
    pMULTIPLY(HookCollision, L, HookCollision);
    pMULTIPLY(HookCollision, jello->kCollision, HookCollision);
    /*Computes damping force on out-boudingbox points */    
    pMAKE(jello->v[i][j][k].x,jello->v[i][j][k].y,jello->v[i][j][k].z,tempVelocity);
    pMAKE(tempx,tempy,tempz,DampingCollision);
    pNORMALIZE(tempVelocity);
    DOTPRODUCT(tempVelocity,DampingCollision,L);
    pMULTIPLY(DampingCollision,L,DampingCollision);
    pMULTIPLY(DampingCollision,-jello->dCollision,DampingCollision);
    
    pSUM(*force,DampingCollision,*force);
    pSUM(*force,HookCollision,*force);
    
}

void COMPUTE_PLANE(int i, int j, int k, struct point * force, struct world * jello)
{
    double length,L;
    point HookCollision,DampingCollision,tempVelocity;
    
    if(jello->a*jello->p[i][j][k].x + jello->b*jello->p[i][j][k].y + jello->c*jello->p[i][j][k].z + jello->d < 0)
    {
        /*Computes elastic force using Hook's Law on plane points */
        L= jello->a*jello->p[i][j][k].x + jello->b*jello->p[i][j][k].y + jello->c*jello->p[i][j][k].z + jello->d;
        if(L<0)L = -L;
        L=L/sqrt(jello->a*jello->a + jello->b*jello->b + jello->c*jello->c);
        pMAKE(jello->a,jello->b,jello->c,HookCollision);
        pNORMALIZE(HookCollision);
        pMULTIPLY(HookCollision, L, HookCollision);
        pMULTIPLY(HookCollision, jello->kCollision, HookCollision);
        /*Computes damping force on plane points */
        pMAKE(jello->v[i][j][k].x,jello->v[i][j][k].y,jello->v[i][j][k].z,tempVelocity);
        pMAKE(jello->a,jello->b,jello->c,DampingCollision);
        pNORMALIZE(DampingCollision);
        DOTPRODUCT(tempVelocity,DampingCollision,L);
        pMULTIPLY(DampingCollision,L,DampingCollision);
        pMULTIPLY(DampingCollision,-jello->dCollision,DampingCollision);
    
        pSUM(*force,DampingCollision,*force);
        pSUM(*force,HookCollision,*force);
    }
}

void COMPUTE_USERFORCE(double x, double y, double z, struct point * force)
{
    point outforce;
    pMAKE(x, y, z, outforce);
    pSUM(*force, outforce, *force);
    
}

/* Computes acceleration to every control point of the jello cube, 
   which is in state given by 'jello'.
   Returns result in array 'a'. */
void computeAcceleration(struct world * jello, struct point a[8][8][8])
{
    int i,j,k;

    for (i=0; i<=7; i++)
        for (j=0; j<=7; j++)
            for (k=0; k<=7; k++)
            {
                point accumForce;
                accumForce.x = 0.0;
                accumForce.y = 0.0;
                accumForce.z = 0.0;
                
                /* Computes structural distance of each node */
                COMPUTE_FORCE(i,j,k,1,0,0,&accumForce,jello);
                COMPUTE_FORCE(i,j,k,0,1,0,&accumForce,jello);
                COMPUTE_FORCE(i,j,k,0,0,1,&accumForce,jello);
                COMPUTE_FORCE(i,j,k,-1,0,0,&accumForce,jello);
                COMPUTE_FORCE(i,j,k,0,-1,0,&accumForce,jello);
                COMPUTE_FORCE(i,j,k,0,0,-1,&accumForce,jello);
                
                /* Computes shear distance of each node */
                COMPUTE_FORCE(i,j,k,1,1,0,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,-1,1,0,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,-1,-1,0,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,1,-1,0,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,0,1,1,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,0,-1,1,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,0,-1,-1,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,0,1,-1,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,1,0,1,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,-1,0,1,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,-1,0,-1,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,1,0,-1,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,1,1,1,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,-1,1,1,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,-1,-1,1,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,1,-1,1,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,1,1,-1,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,-1,1,-1,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,-1,-1,-1,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,1,-1,-1,&accumForce, jello);
                
                /* Computes bend distance of each node */
                COMPUTE_FORCE(i,j,k,2,0,0,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,0,2,0,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,0,0,2,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,-2,0,0,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,0,-2,0,&accumForce, jello);
                COMPUTE_FORCE(i,j,k,0,0,-2,&accumForce, jello);
                
                /* Computes the collision effects of each node */
                COMPUTE_COLLISION(i,j,k,&accumForce, jello);
                
                /* Computes the force field on each node */
                COMPUTE_FORCEFIELD(i,j,k,&accumForce, jello);
                
                
                COMPUTE_PLANE(i,j,k,&accumForce, jello);
                
                if(pushUp)
                    COMPUTE_USERFORCE(0.0, 0.0, 10.0,&accumForce);
                if(pushDown)
                    COMPUTE_USERFORCE(0.0, 0.0, -10.0,&accumForce);
                if(pushLeft)
                    COMPUTE_USERFORCE(0.0, -10.0, 0.0,&accumForce);
                if(pushRight)
                    COMPUTE_USERFORCE(0.0, 10.0, 0.0,&accumForce);
                
                a[i][j][k].x = accumForce.x/jello->mass;
                a[i][j][k].y = accumForce.y/jello->mass;
                a[i][j][k].z = accumForce.z/jello->mass;
            }
    
}

/* performs one step of Euler Integration */
/* as a result, updates the jello structure */
void Euler(struct world * jello)
{
  int i,j,k;
  point a[8][8][8];

  computeAcceleration(jello, a);
  
  for (i=0; i<=7; i++)
    for (j=0; j<=7; j++)
      for (k=0; k<=7; k++)
      {
        jello->p[i][j][k].x += jello->dt * jello->v[i][j][k].x;
        jello->p[i][j][k].y += jello->dt * jello->v[i][j][k].y;
        jello->p[i][j][k].z += jello->dt * jello->v[i][j][k].z;
        jello->v[i][j][k].x += jello->dt * a[i][j][k].x;
        jello->v[i][j][k].y += jello->dt * a[i][j][k].y;
        jello->v[i][j][k].z += jello->dt * a[i][j][k].z;
      }
}

/* performs one step of RK4 Integration */
/* as a result, updates the jello structure */
void RK4(struct world * jello)
{
  point F1p[8][8][8], F1v[8][8][8], 
        F2p[8][8][8], F2v[8][8][8],
        F3p[8][8][8], F3v[8][8][8],
        F4p[8][8][8], F4v[8][8][8];

  point a[8][8][8];


  struct world buffer;

  int i,j,k;

  buffer = *jello; // make a copy of jello

  computeAcceleration(jello, a);

  for (i=0; i<=7; i++)
    for (j=0; j<=7; j++)
      for (k=0; k<=7; k++)
      {
         pMULTIPLY(jello->v[i][j][k],jello->dt,F1p[i][j][k]);
         pMULTIPLY(a[i][j][k],jello->dt,F1v[i][j][k]);
         pMULTIPLY(F1p[i][j][k],0.5,buffer.p[i][j][k]);
         pMULTIPLY(F1v[i][j][k],0.5,buffer.v[i][j][k]);
         pSUM(jello->p[i][j][k],buffer.p[i][j][k],buffer.p[i][j][k]);
         pSUM(jello->v[i][j][k],buffer.v[i][j][k],buffer.v[i][j][k]);
      }

  computeAcceleration(&buffer, a);

  for (i=0; i<=7; i++)
    for (j=0; j<=7; j++)
      for (k=0; k<=7; k++)
      {
         // F2p = dt * buffer.v;
         pMULTIPLY(buffer.v[i][j][k],jello->dt,F2p[i][j][k]);
         // F2v = dt * a(buffer.p,buffer.v);     
         pMULTIPLY(a[i][j][k],jello->dt,F2v[i][j][k]);
         pMULTIPLY(F2p[i][j][k],0.5,buffer.p[i][j][k]);
         pMULTIPLY(F2v[i][j][k],0.5,buffer.v[i][j][k]);
         pSUM(jello->p[i][j][k],buffer.p[i][j][k],buffer.p[i][j][k]);
         pSUM(jello->v[i][j][k],buffer.v[i][j][k],buffer.v[i][j][k]);
      }

  computeAcceleration(&buffer, a);

  for (i=0; i<=7; i++)
    for (j=0; j<=7; j++)
      for (k=0; k<=7; k++)
      {
         // F3p = dt * buffer.v;
         pMULTIPLY(buffer.v[i][j][k],jello->dt,F3p[i][j][k]);
         // F3v = dt * a(buffer.p,buffer.v);     
         pMULTIPLY(a[i][j][k],jello->dt,F3v[i][j][k]);
         pMULTIPLY(F3p[i][j][k],0.5,buffer.p[i][j][k]);
         pMULTIPLY(F3v[i][j][k],0.5,buffer.v[i][j][k]);
         pSUM(jello->p[i][j][k],buffer.p[i][j][k],buffer.p[i][j][k]);
         pSUM(jello->v[i][j][k],buffer.v[i][j][k],buffer.v[i][j][k]);
      }
         
  computeAcceleration(&buffer, a);


  for (i=0; i<=7; i++)
    for (j=0; j<=7; j++)
      for (k=0; k<=7; k++)
      {
         // F3p = dt * buffer.v;
         pMULTIPLY(buffer.v[i][j][k],jello->dt,F4p[i][j][k]);
         // F3v = dt * a(buffer.p,buffer.v);     
         pMULTIPLY(a[i][j][k],jello->dt,F4v[i][j][k]);

         pMULTIPLY(F2p[i][j][k],2,buffer.p[i][j][k]);
         pMULTIPLY(F3p[i][j][k],2,buffer.v[i][j][k]);
         pSUM(buffer.p[i][j][k],buffer.v[i][j][k],buffer.p[i][j][k]);
         pSUM(buffer.p[i][j][k],F1p[i][j][k],buffer.p[i][j][k]);
         pSUM(buffer.p[i][j][k],F4p[i][j][k],buffer.p[i][j][k]);
         pMULTIPLY(buffer.p[i][j][k],1.0 / 6,buffer.p[i][j][k]);
         pSUM(buffer.p[i][j][k],jello->p[i][j][k],jello->p[i][j][k]);

         pMULTIPLY(F2v[i][j][k],2,buffer.p[i][j][k]);
         pMULTIPLY(F3v[i][j][k],2,buffer.v[i][j][k]);
         pSUM(buffer.p[i][j][k],buffer.v[i][j][k],buffer.p[i][j][k]);
         pSUM(buffer.p[i][j][k],F1v[i][j][k],buffer.p[i][j][k]);
         pSUM(buffer.p[i][j][k],F4v[i][j][k],buffer.p[i][j][k]);
         pMULTIPLY(buffer.p[i][j][k],1.0 / 6,buffer.p[i][j][k]);
         pSUM(buffer.p[i][j][k],jello->v[i][j][k],jello->v[i][j][k]);
      }

  return;  
}
