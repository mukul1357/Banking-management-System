#ifndef _SERVER_H
#define _SERVER_H
#include "data_structures.h"
bool checksimpleUser(simpleUser currUser);
bool checkSharedUser(sharedUser currUser);
bool checkSudo(sudo currUser);
void serverTask(int nsd);
#endif