#include "database.h"
#include "constants.h"
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
    mkdir(BASE_PATH, 0744);
    int fd1 = open(simpleUser_DB_PATH, O_RDWR | O_CREAT, 0744);
    simpleUser u1 = {
        .ID = 1,
        .name = "Satya",
        .password = "1234",
        .accNum = 1000,
        .balance = 1000.0,
        .status = "ACTIVE"};
    printf("Simple User initialised with the following data\n");
    printf("id=%d\tname=%s\tpassword=%s\n", u1.ID, u1.name, u1.password);
    write(fd1, &u1, sizeof(u1));
    close(fd1);
    int fd2 = open(SHAREDUSER_DB_PATH, O_RDWR | O_CREAT, 0744);
    sharedUser u2 = {
        .ID = 1,
        .name1 = "Nandu",
        .name2 = "Kaushik",
        .password = "1234",
        .accNum = 2000,
        .balance = 1000.0,
        .status = "ACTIVE"};
    printf("Shared user initialised with the following data\n");
    printf("id=%d\tname1=%s\tname2=%s\tpassword=%s\n", u2.ID, u2.name1, u2.name2, u2.password);
    write(fd2, &u2, sizeof(u2));
    close(fd2);
    int fd3 = open(SUDO_DB_PATH, O_RDWR | O_CREAT, 0744);
    sudo u3 = {
        .ID = 1,
        .username = "Sudo",
        .password = "1234\n"};
    printf("Sudo user initialised with following data\n");
    printf("id=%d\tusername=%s\tpassword=%s\n", u3.ID, u3.username, u3.password);
    write(fd3, &u3, sizeof(u3));
    close(fd3);
    return 0;
}