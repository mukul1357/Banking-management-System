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

#include "../include/database.h"

#define PORT 9000

bool checkNormalUser(NormalUser currUser)
{
    int i = currUser.userID - 1;
    int fd = open("../database/normal_user_db.txt", O_RDONLY, 0744);
    bool result;
    NormalUser temp;

    int fl;
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(NormalUser); // nth record
    lock.l_len = sizeof(NormalUser);         // sizeof(record)
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record

    lseek(fd, (i)*sizeof(NormalUser), SEEK_SET); 
    read(fd, &temp, sizeof(NormalUser));


    if (!strcmp(temp.password, currUser.password) && !strcmp(temp.status, "ACTIVE"))
        result = true;
    else
        result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool checkJointUser(JointUser currUser)
{
    int i = currUser.userID - 1;
    int fd = open("../database/joint_user_db.txt", O_RDONLY, 0744);
    bool result;
    JointUser temp;

    int fl;
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(JointUser); // nth record
    lock.l_len = sizeof(JointUser);         
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record

    lseek(fd, (i)*sizeof(JointUser), SEEK_SET); // changing the file pointer to the selected record
    read(fd, &temp, sizeof(JointUser));
    if (!strcmp(temp.password, currUser.password) && !strcmp(temp.status, "ACTIVE"))
        result = true;
    else
        result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool checkAdmin(Admin currUser)
{
    int i = currUser.userID - 1;
    int fd = open("../database/Admin_db.txt", O_RDONLY, 0744);
    bool result;
    Admin temp;

    int fl;
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(Admin); // nth record
    lock.l_len = sizeof(Admin);         // sizeof(record)
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record

    lseek(fd, (i)*sizeof(Admin), SEEK_SET); // changing the file pointer to the selected record
    read(fd, &temp, sizeof(Admin));
    if (!strcmp(temp.password, currUser.password))
        result = true;
    else
        result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

void serverTask(int nsd)
{
    int msgLength, select, type, option, accType, currUserID;
    bool result;
    while (1)
    {
        read(nsd, &option, sizeof(option));
        printf("Option : %d\n", option);

        if (option == 1)
        {
            NormalUser currUser;
            accType = 1;
            msgLength = read(nsd, &currUser, sizeof(NormalUser));
            printf("Username : %d\n", currUser.userID);
            printf("Password : %s\n", currUser.password);
            currUserID = currUser.userID;
            result = checkNormalUser(currUser);
            write(nsd, &result, sizeof(result));
        }
        else if (option == 2)
        {
            JointUser currUser;
            accType = 2;
            msgLength = read(nsd, &currUser, sizeof(JointUser));
            currUserID = currUser.userID;
            printf("Username : %d\n", currUser.userID);
            printf("Password : %s\n", currUser.password);
            result = checkJointUser(currUser);
            write(nsd, &result, sizeof(result));
        }
        else if (option == 3)
        {
            Admin currUser;
            accType = 3;
            msgLength = read(nsd, &currUser, sizeof(Admin));
            currUserID = currUser.userID;
            printf("Username : %d\n", currUser.userID);
            printf("Password : %s\n", currUser.password);
            result = checkAdmin(currUser);
            write(nsd, &result, sizeof(result));
        }
        else
        {
            result = false;
            write(nsd, &result, sizeof(result));
        }
        if (result)
            break;
    }

    while (true)
    {
        read(nsd, &select, sizeof(int));
        if (option == 1 || option == 2)
        {
            if (select == 1)
            {
                float amount;
                read(nsd, &amount, sizeof(float));
                result = depositMoney(accType, currUserID, amount);
                write(nsd, &result, sizeof(result));
            }
            else if (select == 2)
            {
                float amount;
                read(nsd, &amount, sizeof(float));
                result = withdrawMoney(accType, currUserID, amount);
                write(nsd, &result, sizeof(result));
            }
            else if (select == 3)
            {
                float amount;
                amount = getBalance(accType, currUserID);
                write(nsd, &amount, sizeof(float));
            }
            else if (select == 4)
            {
                char pwd[10];
                read(nsd, pwd, sizeof(pwd));
                result = alterPassword(accType, currUserID, pwd);
                write(nsd, &result, sizeof(result));
            }
            else if (select == 5)
            {
                if (option == 1)
                {
                    NormalUser user1 = getNormalUser(currUserID);
                    write(nsd, &user1, sizeof(NormalUser));
                }
                else if (option == 2)
                {
                    JointUser user2 = getJointUser(currUserID);
                    write(nsd, &user2, sizeof(JointUser));
                }
            }
            else if (select == 6)
                break;
        }
        else if (option == 3)
        {
            read(nsd, &type, sizeof(int));
            if (select == 1)
            {
                if (type == 1)
                {
                    NormalUser newUser1;
                    read(nsd, &newUser1, sizeof(NormalUser));
                    result = addNormalUser(newUser1);
                    write(nsd, &result, sizeof(result));
                }
                else if (type == 2)
                {
                    JointUser newUser2;
                    read(nsd, &newUser2, sizeof(JointUser));
                    result = addJointUser(newUser2);
                    write(nsd, &result, sizeof(result));
                }
            }
            else if (select == 2)
            {
                if (type == 1)
                {
                    int delUserID;
                    read(nsd, &delUserID, sizeof(int));
                    result = deleteNormalUser(delUserID);
                    write(nsd, &result, sizeof(result));
                }
                else if (type == 2)
                {
                    int delUserID;
                    read(nsd, &delUserID, sizeof(int));
                    result = deleteJointUser(delUserID);
                    write(nsd, &result, sizeof(result));
                }
            }
            else if (select == 3)
            {
                if (type == 1)
                {
                    NormalUser modUser;
                    read(nsd, &modUser, sizeof(NormalUser));
                    result = modifyNormalUser(modUser);
                    write(nsd, &result, sizeof(result));
                }
                else if (type == 2)
                {
                    JointUser modUser;
                    read(nsd, &modUser, sizeof(JointUser));
                    result = modifyJointUser(modUser);
                    write(nsd, &result, sizeof(result));
                }
            }
            else if (select == 4)
            {
                int userID;
                if (type == 1)
                {
                    NormalUser searchUser;
                    read(nsd, &userID, sizeof(int));
                    searchUser = getNormalUser(userID);
                    write(nsd, &searchUser, sizeof(NormalUser));
                }
                else if (type == 2)
                {
                    JointUser searchUser;
                    read(nsd, &userID, sizeof(int));
                    searchUser = getJointUser(userID);
                    write(nsd, &searchUser, sizeof(JointUser));
                }
            }
            else if (select == 5)
                break;
        }
    }

    close(nsd);
    printf("Client session ended!\n");
}

void *connection_handler(void *nsd)
{
    int nsfd = *(int *)nsd;
    serverTask(nsfd);
}

int main()
{
    struct sockaddr_in server, client;
    int sd, nsd, clientLen;
    bool result;
    pthread_t threads;
    sd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(sd, (struct sockaddr *)&server, sizeof(server));
    listen(sd, 10); // queue length : 10 clients per connection

    printf("Waiting for a client...\n");

    while (1)
    {
        clientLen = sizeof(client);
        nsd = accept(sd, (struct sockaddr *)&client, &clientLen);

        printf("Connected to the client...\n");

        if (pthread_create(&threads, NULL, connection_handler, (void *)&nsd) < 0)
        {
            perror("could not create thread");
            return 1;
        }
    }

    pthread_exit(NULL);
    close(sd);
    return 0;
}
