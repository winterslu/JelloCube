//
//  lighting.h
//  JelloCube
//
//  Created by Ke Lu on 12-2-7.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef JelloCube_lighting_h
#define JelloCube_lighting_h

// global ambient light
GLfloat aGa[] = { 0.0, 0.0, 0.0, 0.0 };

// light 's ambient, diffuse, specular
GLfloat lKa0[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat lKd0[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lKs0[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat lKa1[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat lKd1[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lKs1[] = { 1.0, 0.0, 0.0, 1.0 };

GLfloat lKa2[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat lKd2[] = { 1.0, 1.0, 0.0, 1.0 };
GLfloat lKs2[] = { 1.0, 1.0, 0.0, 1.0 };

GLfloat lKa3[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat lKd3[] = { 0.0, 1.0, 1.0, 1.0 };
GLfloat lKs3[] = { 0.0, 1.0, 1.0, 1.0 };

GLfloat lKa4[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat lKd4[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat lKs4[] = { 0.0, 0.0, 1.0, 1.0 };

GLfloat lKa5[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat lKd5[] = { 1.0, 0.0, 1.0, 1.0 };
GLfloat lKs5[] = { 1.0, 0.0, 1.0, 1.0 };

GLfloat lKa6[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat lKd6[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lKs6[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat lKa7[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat lKd7[] = { 0.0, 1.0, 1.0, 1.0 };
GLfloat lKs7[] = { 0.0, 1.0, 1.0, 1.0 };

// light positions and directions
GLfloat lP0[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat lP1[] = { 1.5, -1.5, 1.5, 1.0 };
GLfloat lP2[] = { 1.999, 1.999, -1.999, 1.0 };
GLfloat lP3[] = { -1.999, 1.999, -1.999, 1.0 };
GLfloat lP4[] = { -1.999, -1.999, 1.999, 1.0 };
GLfloat lP5[] = { 1.999, -1.999, 1.999, 1.0 };
GLfloat lP6[] = { 1.999, 1.999, 1.999, 1.0 };
GLfloat lP7[] = { -1.999, 1.999, 1.999, 1.0 };

// Bouding box material color
GLfloat mBa[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mBd[] = { 0.6, 0.6, 0.6, 1.0 };
GLfloat mBs[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mBe[] = { 0.0, 0.0, 0.0, 1.0 };


// jelly material color
GLfloat mKa[] = { 0.05, 0.05, 0.05, 1.0 };
GLfloat mKd[] = { 0.866, 0.552, 0.357, 1.0 };
GLfloat mKs[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mKe[] = { 0.349, 0.124, 0.068, 1.0 };

#endif
