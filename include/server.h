#include "dataStructure.h"

#ifndef _SERVER_H
#define _SERVER_H

bool checkNormalUser(NormalUser currUser);
bool checkJointUser(JointUser currUser);
bool checkAdmin(Admin currUser);
void serverTask(int nsd);

#endif 