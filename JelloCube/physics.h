/*

  USC/Viterbi/Computer Science
  "Jello Cube" Assignment 1 starter code

*/

#ifndef _PHYSICS_H_
#define _PHYSICS_H_


#define STRUCTURAL  1
#define SHORT_SHEAR 2
#define LONG_SHEAR  3
#define BEND        4
#define COLLISION   5
#define NOSPRING    6


void COMPUTE_FORCE(int i, int j, int k, int di, int dj, int dk, struct point * force, struct world * jello);

void COMPUTE_COLLISION(int i, int j, int k, struct point * force, struct world * jello);

void COMPUTE_FORCEFIELD(int i, int j, int k, struct point * force, struct world * jello);

void COMPUTE_PLANE(int i, int j, int k, struct point * force, struct world * jello);

void COMPUTE_USERFORCE(double x, double y, double z, struct point * force);

void computeAcceleration(struct world * jello, struct point a[8][8][8]);


// perform one step of Euler and Runge-Kutta-4th-order integrators
// updates the jello structure accordingly
void Euler(struct world * jello);
void RK4(struct world * jello);

#endif

