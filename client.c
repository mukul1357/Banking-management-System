#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include "data_structures.h"
#include "constants.h"
#include "client.h"

int Choice;

void validChoice(int n, int *out)
{
	do
	{
		scanf("%d", out);
	} while (*out <= 0 || *out >= n);
}

void choose(int servfd)
{
	int i = 1;
	printf("%d : Normal-User Login\n", i++);
	printf("%d : Shared Account-User Login\n", i++);
	printf("%d : Sudo Login\n", i++);
	printf("Pick a choice : ");
	validChoice(i, &Choice);
	switch (Choice)
	{
	case 1:
		userSimpleLogin(servfd);
		break;
	case 2:
		userSharedLogin(servfd);
		break;
	case 3:
		sudoLogin(servfd);
		break;
	}
	return;
}

void menu(int servfd)
{
	int choice1;
	int i = 1;
	switch (Choice)
	{
	case 1:
	case 2:
		printf("%d : Deposit\n", i++);
		printf("%d : Withdraw\n", i++);
		printf("%d : Balance Enquiry\n", i++);
		printf("%d : Password Change\n", i++);
		printf("%d : View Details\n", i++);
		printf("%d : Exit\n", i++);

		printf("Choose an Choice : ");
		scanf("%d", &choice1);
		printf("Choice : %d\n", choice1);

		switch (choice1)
		{
		case 1:
			deposit(servfd);
			break;
		case 2:
			withdraw(servfd);
			break;
		case 3:
			balanceEnquiry(servfd);
			break;
		case 4:
			changePassword(servfd);
			break;
		case 5:
			viewDetails(servfd);
			break;
		case 6:
			write(servfd, &choice1, sizeof(int));
			printf("Bye\n");
			exit(0);
		}
		break;
	case 3:
		printf("%d : Add Account\n", i++);
		printf("%d : Delete Account\n", i++);
		printf("%d : Modify Account\n", i++);
		printf("%d : Search Account\n", i++);
		printf("%d : Exit\n", i++);

		printf("Choose an Choice : ");
		validChoice(i, &choice1);

		switch (choice1)
		{
		case 1:
			addAccount(servfd);
			break;
		case 2:
			deleteAccount(servfd);
			break;
		case 3:
			modifyAccount(servfd);
			break;
		case 4:
			searchAccount(servfd);
			break;
		case 5:
			write(servfd, &choice1, sizeof(int));
			printf("Bye\n");
			exit(0);
		}
		break;
	}
}

void userSimpleLogin(int servfd)
{
	simpleUser usr;
	printf("Username : ");
	scanf("%d", &usr.ID);
	printf("Password : ");
	scanf("%s", usr.password);
	write(servfd, &Choice, sizeof(int));
	write(servfd, &usr, sizeof(simpleUser));
	bool success;
	read(servfd, &success, sizeof(success));
	if (success)
	{
		printf("Succesful Login\n");
	}
	else
	{
		printf("Invalid Credentials\n");
		choose(servfd);
	}
}

void modifyAccount(int servfd)
{
	int choice1 = 3;
	int type;
	write(servfd, &choice1, sizeof(int));
	printf("Enter the type account\n1: Normal Account\n2: Shared Account : ");
	validChoice(3, &type);
	write(servfd, &type, sizeof(int));
	if (type == 1)
	{
		simpleUser changedUser;
		printf("User ID : ");
		scanf("%d", &changedUser.ID);
		printf("Account Number : ");
		scanf("%d", &changedUser.accNum);
		printf("New Name of the account holder : ");
		scanf(" %[^\n]", changedUser.name);
		printf("New Password(max 10 characters) : ");
		scanf("%s", changedUser.password);
		printf("New Balance : ");
		scanf("%f", &changedUser.balance);
		write(servfd, &changedUser, sizeof(simpleUser));
	}
	else
	{
		sharedUser changedUser;
		printf("User ID : ");
		scanf("%d", &changedUser.ID);
		printf("Account Number : ");
		scanf("%d", &changedUser.accNum);
		printf("New Name of the primary account holder : ");
		scanf(" %[^\n]", changedUser.name1);
		printf("New Name of the other account holder : ");
		scanf(" %[^\n]", changedUser.name2);
		printf("New Password(max 10 characters) : ");
		scanf("%s", changedUser.password);
		printf("New Balance : ");
		scanf("%f", &changedUser.balance);
		write(servfd, &changedUser, sizeof(sharedUser));
	}
	bool success;
	read(servfd, &success, sizeof(success)); // from the server
	if (success)
	{
		printf("Account modification success\n");
	}
	else
	{
		printf("Account modification failed, username possibly incorrect\n");
	}
	menu(servfd);
}

void userSharedLogin(int servfd)
{
	sharedUser usr;
	printf("User ID : ");
	scanf("%d", &usr.ID);
	printf("Password : ");
	scanf("%s", usr.password);
	write(servfd, &Choice, sizeof(int));
	write(servfd, &usr, sizeof(sharedUser));
	bool success;
	read(servfd, &success, sizeof(success));
	if (success)
	{
		printf("Succesful Login\n");
	}
	else
	{
		printf("Invalid Credentials\n");
		choose(servfd);
	}
}

void sudoLogin(int servfd)
{
	sudo usr;
	printf("User ID : ");
	scanf("%d", &usr.ID);
	printf("Password : ");
	scanf("%s", usr.password);
	write(servfd, &Choice, sizeof(int));
	write(servfd, &usr, sizeof(sudo));
	bool success;
	read(servfd, &success, sizeof(success));
	if (!success)
	{
		printf("Succesful Login\n");
	}
	else
	{
		printf("Invalid Credentials\n");
		choose(servfd);
	}
}

void viewDetails(int servfd)
{
	int choice1 = 5;
	write(servfd, &choice1, sizeof(int));
	if (Choice == 1)
	{
		simpleUser usr1;
		read(servfd, &usr1, sizeof(simpleUser));
		printf("User ID : %d\n", usr1.ID);
		printf("Name : %s\n", usr1.name);
		printf("Account Number : %d\n", usr1.accNum);
		printf("Available Balance : Rs.%0.2f\n", usr1.balance);
		printf("Status : %s\n", usr1.status);
	}
	else if (Choice == 2)
	{
		sharedUser usr2;
		read(servfd, &usr2, sizeof(sharedUser));
		printf("User ID : %d\n", usr2.ID);
		printf("Main Account Holder's Name : %s\n", usr2.name1);
		printf("Other Account Holder's Name : %s\n", usr2.name2);
		printf("Account Number : %d\n", usr2.accNum);
		printf("Available Balance : Rs.%0.2f\n", usr2.balance);
		printf("Status : %s\n", usr2.status);
	}
	menu(servfd);
}

void addAccount(int servfd)
{
	int choice1 = 1;
	int type;
	bool success;
	write(servfd, &choice1, sizeof(int));
	printf("Enter the type account\n1: Normal Account\n2: Shared Account) : ");
	validChoice(3, &type);
	write(servfd, &type, sizeof(int));
	if (type == 1)
	{
		simpleUser newUser;
		printf("Name of the account holder : ");
		scanf(" %[^\n]", newUser.name);
		printf("Password(max 10 characters) : ");
		scanf("%s", newUser.password);
		printf("Initial Deposit : Rs.");
		scanf("%f", &newUser.balance);
		write(servfd, &newUser, sizeof(simpleUser));
	}
	else
	{
		sharedUser newUser;
		printf("Name of the primary account holder : ");
		scanf(" %[^\n]", newUser.name1);
		printf("Name of the other account holder : ");
		scanf(" %[^\n]", newUser.name2);
		printf("Password(max 10 characters) : ");
		scanf("%s", newUser.password);
		printf("Initial Deposit : Rs.");
		scanf("%f", &newUser.balance);
		write(servfd, &newUser, sizeof(sharedUser));
	}
	read(servfd, &success, sizeof(success));
	if (success)
	{
		printf("Succesfully added the account\n");
	}
	else
	{
		printf("Error adding the account\n");
	}
	menu(servfd);
}

void deleteAccount(int servfd)
{
	int choice1 = 2;
	write(servfd, &choice1, sizeof(int));
	printf("Enter the type account\n1: Normal Account\n2: Shared Account) : ");
	int type, ID;
	validChoice(3, &type);
	write(servfd, &type, sizeof(int));
	printf("User ID : ");
	scanf("%d", &ID);
	write(servfd, &ID, sizeof(int));
	bool success;
	read(servfd, &success, sizeof(success));
	if (!success)
	{
		printf("Account deletion successful\n");
	}
	else
	{
		printf("Account deletion failed, username possibly incorrect\n");
	}
	menu(servfd);
}

void deposit(int servfd)
{
	double monetary_amount;
	do
	{
		printf("Amount to Deposit : Rs.");
		scanf("%f", &monetary_amount);
		if (monetary_amount <= 0)
		{
			printf("Amount must be greater than 0\n");
			continue;
		}
	} while (false);
	int choice1 = 1;
	write(servfd, &choice1, sizeof(int));
	write(servfd, &monetary_amount, sizeof(float));
	bool success;
	read(servfd, &success, sizeof(success));
	if (success)
	{
		printf("Deposition Successful\n");
	}
	else
	{
		printf("Something went wrong\n");
	}
	menu(servfd);
}

void withdraw(int servfd)
{
	float monetary_amount;
	int choice1 = 2;
	do
	{
		printf("Amount to withdraw : Rs.");
		scanf("%f", &monetary_amount);
		if (monetary_amount <= 0)
		{
			printf("Amount must be greater than 0\n");
			continue;
		}
	} while (false);
	write(servfd, &choice1, sizeof(int));
	write(servfd, &monetary_amount, sizeof(float));
	bool success;
	read(servfd, &success, sizeof(success));
	if (success)
	{
		printf("Withdrawl Successful\n");
	}
	else
	{
		printf("Error withdrawing your money, please check your balance!!\n");
	}
	menu(servfd);
}

void balanceEnquiry(int servfd)
{
	float monetary_amount;
	int choice1 = 3;
	write(servfd, &choice1, sizeof(int));
	int len;
	len = read(servfd, &monetary_amount, sizeof(float));
	printf("Your Balance: Rs. ");
	printf("%0.2f\n", monetary_amount);
	menu(servfd);
}

void changePassword(int servfd)
{
	bool success;
	printf("Enter the new password(lessar than 11 characters): ");
	char newpAss[10];
	scanf("%s", newpAss);
	int choice1 = 4;
	write(servfd, &choice1, sizeof(int));
	write(servfd, newpAss, sizeof(newpAss));
	read(servfd, &success, sizeof(success));
	if (success)
	{
		printf("Password successfully updated\n");
	}
	else
	{
		printf("Password updation failed\n");
	}
	menu(servfd);
}

void searchAccount(int servfd)
{
	int choice1 = 4;
	int type, len;
	bool success;
	write(servfd, &choice1, sizeof(int));
	printf("Enter the type account\n1: Normal Account\n2: Shared Account) : ");
	validChoice(3, &type);
	write(servfd, &type, sizeof(int));
	if (type == 1)
	{
		simpleUser searchUser1;
		int ID;
		printf("User ID : ");
		scanf("%d", &ID);
		write(servfd, &ID, sizeof(int));
		len = read(servfd, &searchUser1, sizeof(simpleUser));
		if (len == 0)
		{
			printf("Username must be greater than 0 characters\n");
		}
		else
		{
			printf("User ID : %d\n", searchUser1.ID);
			printf("Name : %s\n", searchUser1.name);
			printf("Account Number : %d\n", searchUser1.accNum);
			printf("Available Balance : Rs.%0.2f\n", searchUser1.balance);
			printf("Status : %s\n", searchUser1.status);
		}
	}
	else
	{
		sharedUser searchUser2;
		int ID1;
		printf("User ID : ");
		scanf("%d", &ID1);
		write(servfd, &ID1, sizeof(int));
		len = read(servfd, &searchUser2, sizeof(sharedUser));
		if (len == 0)
		{
			printf("Username must be greater than 0 characters\n");
		}
		else
		{
			printf("User ID : %d\n", searchUser2.ID);
			printf("Main Account Holder's Name : %s\n", searchUser2.name1);
			printf("Other Account Holder's Name : %s\n", searchUser2.name2);
			printf("Account Number : %d\n", searchUser2.accNum);
			printf("Available Balance : Rs.%0.2f\n", searchUser2.balance);
			printf("Status : %s\n", searchUser2.status);
		}
	}
	menu(servfd);
}

int main()
{
	struct sockaddr_in server;
	int servfd;
	char success;
	servfd = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(PORTNO);
	connect(servfd, (struct sockaddr *)&server, sizeof(server));
	choose(servfd);
	menu(servfd);
	close(servfd);
	return 0;
}