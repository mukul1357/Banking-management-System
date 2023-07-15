#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../include/database.h"

int main()
{
    int fd = open("../database/normal_user_db.txt", O_RDWR | O_CREAT, 0744);
    NormalUser u1 = {1, "normal_user", "1234", 10, 500.00, "ACTIVE"};

    write(fd, &u1, sizeof(u1));

    printf("\nIntialising Normal User\n");
    printf("ID= %d\tName= %s\tPassword= %s\n", u1.userID, u1.holderName, u1.password);
    close(fd);

    int fd1 = open("../database/joint_user_db.txt", O_RDWR | O_CREAT, 0744);
    JointUser u2 = {1, "user1", "user2", "12345", 20, 10000.00, "ACTIVE"};

    write(fd1, &u2, sizeof(u2));

    printf("\nIntialising Joint User\n");
    printf("ID= %d\nName1= %s\tName2= %s\tPassword=%s\n", u2.userID, u2.holder1, u2.holder2, u2.password);
    close(fd1);

    int fd3 = open("../database/admin_db.txt", O_RDWR | O_CREAT, 0744);
    Admin u3 = {1, "admin", "95674"};

    write(fd3, &u3, sizeof(u3));

    printf("\nInitialising Admin\n");
    printf("ID= %d\tUsername= %s\tPassword= %s\n", u3.userID, u3.username, u3.password);
    close(fd3);

    return 0;
}