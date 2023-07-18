#ifndef DATABASE
#define DATABASE
#include "data_structures.h"
#include <stdbool.h>
bool addsimpleUser(simpleUser record);
bool addSharedUser(sharedUser record);
bool deletesimpleUser(int ID);
bool deleteSharedUser(int ID);
bool modifysimpleUser(simpleUser modUser);
bool modifySharedUser(sharedUser modUser);
simpleUser getsimpleUser(int ID);
sharedUser getSharedUser(int ID);
sudo getSudo(int ID);
bool depositMoney(int accType, int ID, float amt);
bool withdrawMoney(int accType, int ID, float amt);
float getBalance(int accType, int ID);
bool alterPassword(int accType, int ID, char newPwd[10]);
#endif