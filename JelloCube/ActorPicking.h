//
//  ActorPicking.h
//  JelloCube
//
//  Created by Winters Lu on 12-2-6.
//  Copyright (c) 2012 Winters Lu. All rights reserved.
//

#ifndef JelloCube_ActorPicking_h
#define JelloCube_ActorPicking_h

void LetGoActor();
bool Pick(int x, int y);
bool PickActor(int x, int y);
void MoveActor(int x, int y);
void ViewProject(point * v, int &xi, int &yi, float &depth);
void ViewUnProject(int xi, int yi, float depth, point * v);
void Project(bool isPick);

#endif
