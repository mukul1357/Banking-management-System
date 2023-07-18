#include "database.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "constants.h"

bool modifysimpleUser(simpleUser modUser)
{
	printf("id mod user = %d\n", modUser.ID);
	int i = modUser.ID - 1;
	int fd = open(simpleUser_DB_PATH, O_RDWR, 0744);
	bool success = false;
	simpleUser currentUsr;
	lseek(fd, (i) * sizeof(simpleUser), SEEK_SET);
	struct flock lck;
	lck.l_type = F_WRLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = (i) * sizeof(simpleUser);
	lck.l_len = sizeof(simpleUser);
	lck.l_pid = getpid();
	fcntl(fd, F_SETLKW, &lck);
	read(fd, &currentUsr, sizeof(simpleUser));
	if (!strcmp(currentUsr.status, "ACTIVE") && (modUser.accNum == currentUsr.accNum))
	{
		strcpy(modUser.status, "ACTIVE");
		lseek(fd, -sizeof(simpleUser), SEEK_CUR);
		success = write(fd, &modUser, sizeof(simpleUser));
	}
	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);
	close(fd);
	return success;
}

bool modifySharedUser(sharedUser modUser)
{
	int i = modUser.ID - 1;
	int fd = open(SHAREDUSER_DB_PATH, O_RDWR, 0744);
	bool success = false;
	struct flock lck;
	lck.l_type = F_WRLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = (i) * sizeof(sharedUser);
	lck.l_len = sizeof(sharedUser);
	lck.l_pid = getpid();
	fcntl(fd, F_SETLKW, &lck);
	sharedUser currentUsr;
	lseek(fd, (i) * sizeof(sharedUser), SEEK_SET);
	read(fd, &currentUsr, sizeof(sharedUser));
	if (!strcmp(currentUsr.status, "ACTIVE") && (modUser.accNum == currentUsr.accNum))
	{
		strcpy(modUser.status, "ACTIVE");
		lseek(fd, -sizeof(sharedUser), SEEK_CUR);
		success = write(fd, &modUser, sizeof(sharedUser));
	}
	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);
	close(fd);
	return success;
}

simpleUser getsimpleUser(int ID)
{
	int i = ID - 1;
	simpleUser currentUsr;
	int fd = open(simpleUser_DB_PATH, O_RDONLY, 0744);
	lseek(fd, (i) * sizeof(simpleUser), SEEK_SET);
	struct flock lck;
	lck.l_type = F_RDLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = (i) * sizeof(simpleUser);
	lck.l_len = sizeof(simpleUser);
	lck.l_pid = getpid();
	fcntl(fd, F_SETLKW, &lck);
	read(fd, &currentUsr, sizeof(simpleUser));
	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);
	close(fd);
	return currentUsr;
}

bool alterPassword(int accType, int ID, char newPwd[10])
{
	int i = ID - 1;
	if (accType == 1)
	{
		int fd = open(simpleUser_DB_PATH, O_RDWR, 0744);
		bool success;
		simpleUser currentUsr;
		lseek(fd, (i) * sizeof(simpleUser), SEEK_SET);
		struct flock lck;
		lck.l_type = F_WRLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = (i) * sizeof(simpleUser);
		lck.l_len = sizeof(simpleUser);
		lck.l_pid = getpid();
		fcntl(fd, F_SETLKW, &lck);
		read(fd, &currentUsr, sizeof(simpleUser));
		if (!strcmp(currentUsr.status, "ACTIVE"))
		{
			strcpy(currentUsr.password, newPwd);
			lseek(fd, -sizeof(simpleUser), SEEK_CUR);
			write(fd, &currentUsr, sizeof(simpleUser));
			success = true;
		}
		else
			success = false;
		lck.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lck);
		close(fd);
		return success;
	}
	else if (accType == 2)
	{
		int fd = open(SHAREDUSER_DB_PATH, O_RDWR, 0744);
		bool success;
		sharedUser currentUsr;
		lseek(fd, (i) * sizeof(sharedUser), SEEK_SET);
		struct flock lck;
		lck.l_type = F_WRLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = (i) * sizeof(sharedUser);
		lck.l_len = sizeof(sharedUser);
		lck.l_pid = getpid();
		fcntl(fd, F_SETLKW, &lck);
		read(fd, &currentUsr, sizeof(sharedUser));
		if (!strcmp(currentUsr.status, "ACTIVE"))
		{
			strcpy(currentUsr.password, newPwd);
			lseek(fd, -sizeof(sharedUser), SEEK_CUR);
			write(fd, &currentUsr, sizeof(sharedUser));
			success = true;
		}
		else
			success = false;
		lck.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lck);
		close(fd);
		return success;
	}
	return false;
}

bool withdrawMoney(int accType, int ID, float monetaryamount)
{
	int i = ID - 1;
	if (accType == 1)
	{
		int fd = open(simpleUser_DB_PATH, O_RDWR, 0744);
		bool success;
		simpleUser currentUsr;
		lseek(fd, (i) * sizeof(simpleUser), SEEK_SET);
		struct flock lck;
		lck.l_type = F_WRLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = (i) * sizeof(simpleUser);
		lck.l_len = sizeof(simpleUser);
		lck.l_pid = getpid();
		fcntl(fd, F_SETLKW, &lck);
		read(fd, &currentUsr, sizeof(simpleUser));
		if (!strcmp(currentUsr.status, "ACTIVE") && currentUsr.balance >= monetaryamount)
		{
			currentUsr.balance -= monetaryamount;
			lseek(fd, -sizeof(simpleUser), SEEK_CUR);
			write(fd, &currentUsr, sizeof(simpleUser));
			success = true;
		}
		else
			success = false;
		lck.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lck);
		close(fd);
		return success;
	}
	else if (accType == 2)
	{
		int fd = open(SHAREDUSER_DB_PATH, O_RDWR, 0744);
		bool success;
		sharedUser currentUsr;
		lseek(fd, (i) * sizeof(sharedUser), SEEK_SET);
		struct flock lck;
		lck.l_type = F_WRLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = (i) * sizeof(sharedUser);
		lck.l_len = sizeof(sharedUser);
		lck.l_pid = getpid();
		fcntl(fd, F_SETLKW, &lck);
		read(fd, &currentUsr, sizeof(sharedUser));
		if (!strcmp(currentUsr.status, "ACTIVE") && currentUsr.balance >= monetaryamount)
		{
			currentUsr.balance -= monetaryamount;
			lseek(fd, -sizeof(sharedUser), SEEK_CUR);
			write(fd, &currentUsr, sizeof(sharedUser));
			success = true;
		}
		else
			success = false;
		lck.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lck);
		close(fd);
		return success;
	}
	return false;
}
bool addsimpleUser(simpleUser record)
{
	int fd = open(simpleUser_DB_PATH, O_RDWR, 0744);
	bool success;
	simpleUser endUser;
	lseek(fd, -sizeof(simpleUser), SEEK_END);
	struct flock lck;
	lck.l_type = F_WRLCK;
	lck.l_whence = SEEK_END;
	lck.l_start = -sizeof(simpleUser);
	lck.l_len = sizeof(simpleUser);
	lck.l_pid = getpid();
	fcntl(fd, F_SETLKW, &lck);
	read(fd, &endUser, sizeof(simpleUser));
	record.ID = endUser.ID + 1;
	record.accNum = endUser.accNum + 1;
	strcpy(record.status, "ACTIVE");
	success = write(fd, &record, sizeof(simpleUser));
	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);
	close(fd);
	return success;
}

bool addSharedUser(sharedUser record)
{
	int fd = open(SHAREDUSER_DB_PATH, O_RDWR, 0744);
	bool success;

	sharedUser endUser;
	lseek(fd, -sizeof(sharedUser), SEEK_END);
	struct flock lck;
	lck.l_type = F_WRLCK;
	lck.l_whence = SEEK_END;
	lck.l_start = -sizeof(sharedUser);
	lck.l_len = sizeof(sharedUser);
	lck.l_pid = getpid();
	fcntl(fd, F_SETLKW, &lck);
	read(fd, &endUser, sizeof(sharedUser));
	record.ID = endUser.ID + 1;
	record.accNum = endUser.accNum + 1;
	strcpy(record.status, "ACTIVE");
	success = write(fd, &record, sizeof(sharedUser));
	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);
	close(fd);
	return success;
}

bool deletesimpleUser(int ID)
{
	int i = ID - 1;
	int fd = open(simpleUser_DB_PATH, O_RDWR, 0744);
	bool success;

	simpleUser currentUsr;
	lseek(fd, (i) * sizeof(simpleUser), SEEK_SET);
	struct flock lck;
	lck.l_type = F_WRLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = (i) * sizeof(simpleUser);
	lck.l_len = sizeof(simpleUser);
	lck.l_pid = getpid();
	fcntl(fd, F_SETLKW, &lck);
	read(fd, &currentUsr, sizeof(simpleUser));
	if (!strcmp(currentUsr.status, "ACTIVE"))
	{
		strcpy(currentUsr.status, "CLOSED");
		currentUsr.balance = 0;
		lseek(fd, -sizeof(simpleUser), SEEK_CUR);
		success = write(fd, &currentUsr, sizeof(simpleUser));
	}
	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);
	close(fd);
	return success;
}

bool deleteSharedUser(int ID)
{
	int i = ID - 1;
	int fd = open(SHAREDUSER_DB_PATH, O_RDWR, 0744);
	struct flock lck;
	lck.l_type = F_WRLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = (i) * sizeof(sharedUser);
	lck.l_len = sizeof(sharedUser);
	lck.l_pid = getpid();
	fcntl(fd, F_SETLKW, &lck);
	sharedUser currentUsr;
	lseek(fd, (i) * sizeof(sharedUser), SEEK_SET);
	read(fd, &currentUsr, sizeof(sharedUser));
	bool success;
	if (!strcmp(currentUsr.status, "ACTIVE"))
	{
		strcpy(currentUsr.status, "CLOSED");
		currentUsr.balance = 0;
		lseek(fd, -sizeof(sharedUser), SEEK_CUR);
		success = write(fd, &currentUsr, sizeof(sharedUser));
	}
	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);
	close(fd);
	return success;
}

float getBalance(int accType, int ID)
{
	int i = ID - 1;
	float success;
	if (accType == 1)
	{
		int i = ID - 1;
		int fd = open(simpleUser_DB_PATH, O_RDONLY, 0744);
		simpleUser temp;
		lseek(fd, (i) * sizeof(simpleUser), SEEK_SET);
		struct flock lck;
		lck.l_type = F_RDLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = (i) * sizeof(simpleUser);
		lck.l_len = sizeof(simpleUser);
		lck.l_pid = getpid();
		fcntl(fd, F_SETLKW, &lck);
		read(fd, &temp, sizeof(simpleUser));
		if (!strcmp(temp.status, "ACTIVE"))
			success = temp.balance;
		else
			success = 0;
		lck.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lck);
		close(fd);
		return success;
	}
	else if (accType == 2)
	{
		int i = ID - 1;
		int fd = open(SHAREDUSER_DB_PATH, O_RDONLY, 0744);
		sharedUser temp;
		struct flock lck;
		lck.l_type = F_RDLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = (i) * sizeof(sharedUser);
		lck.l_len = sizeof(sharedUser);
		lck.l_pid = getpid();
		fcntl(fd, F_SETLKW, &lck);
		lseek(fd, (i) * sizeof(sharedUser), SEEK_SET);
		read(fd, &temp, sizeof(sharedUser));
		if (!strcmp(temp.status, "ACTIVE"))
			success = temp.balance;
		else
			success = 0;
		lck.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lck);
		close(fd);
		return success;
	}
	return 0;
}

sharedUser getSharedUser(int ID)
{
	int i = ID - 1;
	sharedUser currentUsr;
	int fd = open(SHAREDUSER_DB_PATH, O_RDONLY, 0744);
	lseek(fd, (i) * sizeof(sharedUser), SEEK_SET);
	struct flock lck;
	lck.l_type = F_RDLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = (i) * sizeof(sharedUser);
	lck.l_len = sizeof(sharedUser);
	lck.l_pid = getpid();
	fcntl(fd, F_SETLKW, &lck);
	read(fd, &currentUsr, sizeof(sharedUser));
	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);
	close(fd);
	return currentUsr;
}

sudo getSudo(int ID)
{
	int i = ID - 1;
	sudo currentUsr;
	int fd = open(SUDO_DB_PATH, O_RDONLY, 0744);
	lseek(fd, (i) * sizeof(sudo), SEEK_SET);
	struct flock lck;
	lck.l_type = F_RDLCK;
	lck.l_whence = SEEK_SET;
	lck.l_start = (i) * sizeof(sudo);
	lck.l_len = sizeof(sudo);
	lck.l_pid = getpid();
	fcntl(fd, F_SETLKW, &lck);
	read(fd, &currentUsr, sizeof(sudo));
	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);
	close(fd);
	return currentUsr;
}

bool depositMoney(int accType, int ID, float monetaryamount)
{
	int i = ID - 1;
	if (accType == 1)
	{
		int fd = open(simpleUser_DB_PATH, O_RDWR, 0744);
		bool success;
		simpleUser currentUsr;
		lseek(fd, (i) * sizeof(simpleUser), SEEK_SET);
		struct flock lck;
		lck.l_type = F_WRLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = (i) * sizeof(simpleUser);
		lck.l_len = sizeof(simpleUser);
		lck.l_pid = getpid();
		fcntl(fd, F_SETLKW, &lck);
		read(fd, &currentUsr, sizeof(simpleUser));
		if (!strcmp(currentUsr.status, "ACTIVE"))
		{
			currentUsr.balance += monetaryamount;
			lseek(fd, -sizeof(simpleUser), SEEK_CUR);
			write(fd, &currentUsr, sizeof(simpleUser));
			success = true;
		}
		else
			success = false;
		lck.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lck);
		close(fd);
		return success;
	}
	else if (accType == 2)
	{
		int fd = open(SHAREDUSER_DB_PATH, O_RDWR, 0744);
		bool success;
		sharedUser currentUsr;
		lseek(fd, (i) * sizeof(sharedUser), SEEK_SET);
		struct flock lck;
		lck.l_type = F_WRLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = (i) * sizeof(sharedUser);
		lck.l_len = sizeof(sharedUser);
		lck.l_pid = getpid();
		fcntl(fd, F_SETLKW, &lck);
		read(fd, &currentUsr, sizeof(sharedUser));
		if (!strcmp(currentUsr.status, "ACTIVE"))
		{
			currentUsr.balance += monetaryamount;
			lseek(fd, -sizeof(sharedUser), SEEK_CUR);
			write(fd, &currentUsr, sizeof(sharedUser));
			getchar();
			success = true;
		}
		else
			success = false;
		lck.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lck);
		close(fd);
		return success;
	}
	return false;
}