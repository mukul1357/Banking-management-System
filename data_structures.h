#ifndef DATASTRCUTURES
#define DATASTRCUTURES
typedef struct simpleUser
{
	int ID;
	char name[30];
	char password[10];
	int accNum;
	float balance;
	char status[20];
} simpleUser;
typedef struct sharedUser
{
	int ID;
	char name1[30];
	char name2[30];
	char password[10];
	int accNum;
	float balance;
	char status[20];
} sharedUser;
typedef struct sudo
{
	int ID;
	char username[30];
	char password[10];
} sudo;
#endif