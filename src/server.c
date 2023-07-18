#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include "database.h"
#include "constants.h"

bool checksimpleUser(simpleUser currUser)
{
	int i = currUser.ID - 1;
	int fd = open(simpleUser_DB_PATH, O_RDONLY, 0744);
	bool success;
	simpleUser tmp;
	struct flock lck;
	lck.l_start = (i) * sizeof(simpleUser);
	lck.l_len = sizeof(simpleUser);
	lck.l_pid = getpid();
	lck.l_type = F_RDLCK;
	lck.l_whence = SEEK_SET;
	lseek(fd, (i) * sizeof(simpleUser), SEEK_SET);
	fcntl(fd, F_SETLKW, &lck);
	read(fd, &tmp, sizeof(simpleUser));
	success = !strcmp(tmp.password, currUser.password) && !strcmp(tmp.status, "ACTIVE");
	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);
	close(fd);
	return success;
}

bool checkSharedUser(sharedUser currUser)
{
	int i = currUser.ID - 1;
	int fd = open(SHAREDUSER_DB_PATH, O_RDONLY, 0744);
	bool success;
	sharedUser tmp;
	struct flock lck;
	lck.l_start = (i) * sizeof(sharedUser);
	lck.l_len = sizeof(sharedUser);
	lck.l_pid = getpid();
	lck.l_type = F_RDLCK;
	lck.l_whence = SEEK_SET;
	fcntl(fd, F_SETLKW, &lck);
	lseek(fd, (i) * sizeof(sharedUser), SEEK_SET);
	read(fd, &tmp, sizeof(sharedUser));
	success = !strcmp(tmp.password, currUser.password) && !strcmp(tmp.status, "ACTIVE");
	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);
	close(fd);
	return success;
}

bool checkSudo(sudo currUser)
{
	int i = currUser.ID - 1;
	int fd = open(SUDO_DB_PATH, O_RDONLY, 0744);
	bool success;
	sudo tmp;
	struct flock lck;
	lck.l_start = (i) * sizeof(sudo);
	lck.l_len = sizeof(sudo);
	lck.l_pid = getpid();
	lck.l_type = F_RDLCK;
	lck.l_whence = SEEK_SET;
	fcntl(fd, F_SETLKW, &lck);
	lseek(fd, (i) * sizeof(sudo), SEEK_SET);
	read(fd, &tmp, sizeof(sudo));
	success = !strcmp(tmp.password, currUser.password);
	lck.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lck);
	close(fd);
	return success;
}

void serverTask(int nsd)
{
	int message_len, choice1, type, optn, accType, currID;
	bool success;
	while (1)
	{
		read(nsd, &optn, sizeof(optn));
		printf("Option : %d\n", optn);
		simpleUser susr;
		sharedUser jusr;
		sudo ausr;
		switch (optn)
		{
		case 1:
			accType = 1;
			message_len = read(nsd, &susr, sizeof(simpleUser));
			printf("Username : %d\n", susr.ID);
			printf("Password : %s\n", susr.password);
			currID = susr.ID;
			success = checksimpleUser(susr);
			write(nsd, &success, sizeof(success));
			break;
		case 2:
			accType = 2;
			message_len = read(nsd, &jusr, sizeof(sharedUser));
			currID = jusr.ID;
			printf("Username : %d\n", jusr.ID);
			printf("Password : %s\n", jusr.password);
			success = checkSharedUser(jusr);
			write(nsd, &success, sizeof(success));
			break;
		case 3:
			accType = 3;
			message_len = read(nsd, &ausr, sizeof(sudo));
			currID = ausr.ID;
			printf("Username : %d\n", ausr.ID);
			printf("Password : %s\n", ausr.password);
			success = checkSudo(ausr);
			write(nsd, &success, sizeof(success));
			break;
		case 4:
			accType = 3;
			message_len = read(nsd, &ausr, sizeof(sudo));
			currID = ausr.ID;
			printf("Username : %d\n", ausr.ID);
			printf("Password : %s\n", ausr.password);
			success = checkSudo(ausr);
			write(nsd, &success, sizeof(success));
			break;
		default:
			success = false;
			write(nsd, &success, sizeof(success));
		}
		if (success)
			break;
	}

	while (1)
	{
		read(nsd, &choice1, sizeof(int));
		if (optn == 1 || optn == 2)
		{
			float monetaryAmount;
			char pwd[10];
			simpleUser susr;
			sharedUser jusr;
			if (optn == 1)
			{
				susr = getsimpleUser(currID);
			}
			else
			{
				sharedUser jusr = getSharedUser(currID);
			}
			switch (choice1)
			{
			case 1:
				read(nsd, &monetaryAmount, sizeof(float));
				success = depositMoney(accType, currID, monetaryAmount);
				write(nsd, &success, sizeof(success));
				break;
			case 2:
				read(nsd, &monetaryAmount, sizeof(float));
				success = withdrawMoney(accType, currID, monetaryAmount);
				write(nsd, &success, sizeof(success));
				break;
			case 3:
				monetaryAmount = getBalance(accType, currID);
				write(nsd, &monetaryAmount, sizeof(float));
				break;
			case 4:
				read(nsd, pwd, sizeof(pwd));
				success = alterPassword(accType, currID, pwd);
				write(nsd, &success, sizeof(success));
				break;
			case 5:
				if (optn == 1)
				{
					write(nsd, &susr, sizeof(simpleUser));
				}
				else if (optn == 2)
				{
					write(nsd, &jusr, sizeof(sharedUser));
				}
				break;
			}
		}
		else if (optn == 3)
		{
			read(nsd, &type, sizeof(int));
			simpleUser susr;
			sharedUser jusr;
			int ID;
			switch (choice1)
			{
			case 1:
				if (type == 1)
				{
					read(nsd, &susr, sizeof(simpleUser));
					success = addsimpleUser(susr);
				}
				else if (type == 2)
				{
					read(nsd, &jusr, sizeof(sharedUser));
					success = addSharedUser(jusr);
				}
				write(nsd, &success, sizeof(success));
				break;
			case 2:
				if (type == 1)
				{
					read(nsd, &ID, sizeof(int));
					success = deletesimpleUser(ID);
				}
				else if (type == 2)
				{
					read(nsd, &ID, sizeof(int));
					success = deleteSharedUser(ID);
				}
				write(nsd, &success, sizeof(success));
				break;
			case 3:
				if (type == 1)
				{
					read(nsd, &susr, sizeof(simpleUser));
					success = modifysimpleUser(susr);
				}
				else if (type == 2)
				{
					read(nsd, &jusr, sizeof(sharedUser));
					success = modifySharedUser(jusr);
				}
				write(nsd, &success, sizeof(success));
				break;
			case 4:
				if (type == 1)
				{
					read(nsd, &ID, sizeof(int));
					susr = getsimpleUser(ID);
				}
				else if (type == 2)
				{
					read(nsd, &ID, sizeof(int));
					jusr = getSharedUser(ID);
				}
				write(nsd, &success, sizeof(success));
				break;
			}
		}
	}
	close(nsd);
	printf("Client ended.\n");
	return;
}

void *connection_handler(void *nsd)
{
	int nsfd = *(int *)nsd;
	serverTask(nsfd);
}

int main()
{
	struct sockaddr_in server, client;
	int sd, nsd, clnt_length;
	bool success;
	pthread_t thrds;
	sd = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORTNO);
	bind(sd, (struct sockaddr *)&server, sizeof(server));
	listen(sd, 5);
	printf("Waiting.....\n");
	while (1)
	{
		clnt_length = sizeof(client);
		nsd = accept(sd, (struct sockaddr *)&client, &clnt_length);
		printf("Connected\n");
		if (pthread_create(&thrds, NULL, connection_handler, (void *)&nsd) < 0)
		{
			perror("could not create thread");
			return 1;
		}
	}
	pthread_exit(NULL);
	close(sd);
	return 0;
}