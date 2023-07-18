#include "dataStructure.h"
#include <stdbool.h>

#ifndef _DATABASE_H
#define _DATABASE_H

bool addNormalUser(NormalUser record);
bool addJointUser(JointUser record);
bool deleteNormalUser(int ID);
bool deleteJointUser(int ID);
bool modifyNormalUser(NormalUser modUser);
bool modifyJointUser(JointUser modUser);
NormalUser getNormalUser(int ID);
JointUser getJointUser(int ID);
Admin getAdmin(int ID);
bool depositMoney(int accountType, int ID, float amount);
bool withdrawMoney(int accountType, int ID, float amount);
float getBalance(int accountType, int ID);
bool alterPassword(int accountType, int ID, char newPassword[10]);

#endif