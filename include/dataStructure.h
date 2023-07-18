typedef struct NormalUser{
	int userID;
	char holderName[40];
	char password[10];
	int accountNumber;
	float balance;
	char status[20];
}NormalUser;


typedef struct JointUser{
	int userID;
	char holder1[40];
	char holder2[40];
	char password[10];
	int accountNumber;
	float balance;
	char status[20];
}JointUser;


typedef struct Admin{
	int userID;
	char username[30];
	char password[10];
}Admin;
