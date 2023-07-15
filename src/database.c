#include "../include/database.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>


bool addNormalUser(NormalUser record)
{
    int fd = open("../database/normal_user_db.txt", O_RDWR, 0744);
    bool result;

    int fl;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start = (-1)*sizeof(NormalUser); // nth record
    lock.l_len = sizeof(NormalUser);          // sizeof(record)
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record

    NormalUser endUser;
    lseek(fd, (-1)*sizeof(NormalUser), SEEK_END); // changing the file pointer to the selected record
    read(fd, &endUser, sizeof(NormalUser));

    record.userID = endUser.userID + 1;
    record.accountNumber = endUser.accountNumber + 1;
    strcpy(record.status, "ACTIVE");

    int j = write(fd, &record, sizeof(NormalUser));
    if (j != 0)
        result = true;
    else
        result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool addJointUser(JointUser record)
{
    int fd = open("../database/joint_user_db.txt", O_RDWR, 0744);
    bool result;

    int fl;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start = (-1)*sizeof(JointUser); // nth record
    lock.l_len = sizeof(JointUser);          // sizeof(record)
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record

    JointUser endUser;
    lseek(fd, (-1)*sizeof(JointUser), SEEK_END); // changing the file pointer to the selected record
    read(fd, &endUser, sizeof(JointUser));

    record.userID = endUser.userID + 1;
    record.accountNumber = endUser.accountNumber + 1;
    strcpy(record.status, "ACTIVE");

    int j = write(fd, &record, sizeof(JointUser));
    if (j != 0)
        result = true;
    else
        result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool deleteNormalUser(int ID)
{
    int i = ID - 1;
    int fd = open("../database/normal_user_db.txt", O_RDWR, 0744);
    bool result;

    int fl;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(NormalUser); // nth record
    lock.l_len = sizeof(NormalUser);         // sizeof(record)
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record

    NormalUser currUser;
    lseek(fd, (i)*sizeof(NormalUser), SEEK_SET); // changing the file pointer to the selected record
    read(fd, &currUser, sizeof(NormalUser));

    if (!strcmp(currUser.status, "ACTIVE"))
    {
        strcpy(currUser.status, "CLOSED");
        currUser.balance = 0;

        lseek(fd, (-1)*sizeof(NormalUser), SEEK_CUR);
        int j = write(fd, &currUser, sizeof(NormalUser));
        if (j != 0)
            result = true;
        else
            result = false;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool deleteJointUser(int ID)
{
    int i = ID - 1;
    int fd = open("../database/joint_user_db.txt", O_RDWR, 0744);
    bool result;

    int fl;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(JointUser); // nth record
    lock.l_len = sizeof(JointUser);         // sizeof(record)
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record

    JointUser currUser;
    lseek(fd, (i)*sizeof(JointUser), SEEK_SET); // changing the file pointer to the selected record
    read(fd, &currUser, sizeof(JointUser));

    if (!strcmp(currUser.status, "ACTIVE"))
    {
        strcpy(currUser.status, "CLOSED");
        currUser.balance = 0;

        lseek(fd, (-1)*sizeof(JointUser), SEEK_CUR);
        int j = write(fd, &currUser, sizeof(JointUser));
        if (j != 0)
            result = true;
        else
            result = false;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool modifyNormalUser(NormalUser modUser)
{
    printf("id mod user = %d\n", modUser.userID);
    int i = modUser.userID - 1;
    int fd = open("../database/normal_user_db.txt", O_RDWR, 0744);
    bool result = false;

    int fl;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(NormalUser); // nth record
    lock.l_len = sizeof(NormalUser);         // sizeof(record)
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record

    NormalUser currUser;
    lseek(fd, (i)*sizeof(NormalUser), SEEK_SET); // changing the file pointer to the selected record
    read(fd, &currUser, sizeof(NormalUser));

    if (!strcmp(currUser.status, "ACTIVE") && (modUser.accountNumber == currUser.accountNumber))
    {
        strcpy(modUser.status, "ACTIVE");
        lseek(fd, (-1)*sizeof(NormalUser), SEEK_CUR);
        int j = write(fd, &modUser, sizeof(NormalUser));
        if (j != 0)
            result = true;
        else
            result = false;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool modifyJointUser(JointUser modUser)
{
    int i = modUser.userID - 1;
    int fd = open("../database/joint_user_db.txt", O_RDWR, 0744);
    bool result = false;

    int fl;
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(JointUser); // nth record
    lock.l_len = sizeof(JointUser);         // sizeof(record)
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record

    JointUser currUser;
    lseek(fd, (i)*sizeof(JointUser), SEEK_SET); // changing the file pointer to the selected record
    read(fd, &currUser, sizeof(JointUser));

    if (!strcmp(currUser.status, "ACTIVE") && (modUser.accountNumber == currUser.accountNumber))
    {
        strcpy(modUser.status, "ACTIVE");
        lseek(fd, (-1)*sizeof(JointUser), SEEK_CUR);
        int j = write(fd, &modUser, sizeof(JointUser));
        if (j != 0)
            result = true;
        else
            result = false;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

NormalUser getNormalUser(int ID)
{
    int i = ID - 1;
    NormalUser currUser;
    int fd = open("../database/normal_user_db.txt", O_RDONLY, 0744);

    int fl;
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(NormalUser); // nth record
    lock.l_len = sizeof(NormalUser);         // sizeof(record)
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record

    lseek(fd, (i)*sizeof(NormalUser), SEEK_SET); // changing the file pointer to the selected record
    read(fd, &currUser, sizeof(NormalUser));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return currUser;
}

JointUser getJointUser(int ID)
{
    int i = ID - 1;
    JointUser currUser;
    int fd = open("../database/joint_user_db.txt", O_RDONLY, 0744);

    int fl;
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(JointUser); // nth record
    lock.l_len = sizeof(JointUser);         // sizeof(record)
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record

    lseek(fd, (i)*sizeof(JointUser), SEEK_SET); // changing the file pointer to the selected record
    read(fd, &currUser, sizeof(JointUser));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return currUser;
}

Admin getAdmin(int ID)
{
    int i = ID - 1;
    Admin currUser;
    int fd = open("../database/admin_db.txt", O_RDONLY, 0744);
    int fl;
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(Admin); // nth record
    lock.l_len = sizeof(Admin);         // sizeof(record)
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record

    lseek(fd, (i)*sizeof(Admin), SEEK_SET); // changing the file pointer to the selected record
    read(fd, &currUser, sizeof(Admin));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return currUser;
}

bool depositMoney(int accType, int ID, float amount)
{
    int i = ID - 1;
    if (accType == 1)
    {
        int fd = open("../database/normal_user_db.txt", O_RDWR, 0744);
        bool result;
        int fl;
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i)*sizeof(NormalUser); // nth record
        lock.l_len = sizeof(NormalUser);         // sizeof(record)
        lock.l_pid = getpid();

        fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record

        NormalUser currUser;
        lseek(fd, (i)*sizeof(NormalUser), SEEK_SET); // changing the file pointer to the selected record
        read(fd, &currUser, sizeof(NormalUser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            currUser.balance += amount;
            lseek(fd, sizeof(NormalUser)*(-1), SEEK_CUR);
            write(fd, &currUser, sizeof(NormalUser));
            result = true;
        }
        else
            result = false;
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }
    else if (accType == 2)
    {
        int fd = open("../database/joint_user_db.txt", O_RDWR, 0744);
        bool result;
        int fl;
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i)*sizeof(JointUser); // nth record
        lock.l_len = sizeof(JointUser);         // sizeof(record)
        lock.l_pid = getpid();

        fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record

        JointUser currUser;
        lseek(fd, (i)*sizeof(JointUser), SEEK_SET); // changing the file pointer to the selected record
        read(fd, &currUser, sizeof(JointUser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            currUser.balance += amount;
           
            lseek(fd, sizeof(JointUser)*(-1), SEEK_CUR);
            write(fd, &currUser, sizeof(JointUser));
            getchar();
            result = true;
        }
        else
            result = false;
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }
    return false;
}

bool withdrawMoney(int accType, int ID, float amount)
{
    int i = ID - 1;
    if (accType == 1)
    {
        int fd = open("../database/normal_user_db.txt", O_RDWR, 0744);
        bool result;
        int fl;
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i)*sizeof(NormalUser); // nth record
        lock.l_len = sizeof(NormalUser);         // sizeof(record)
        lock.l_pid = getpid();

        fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record
       

        NormalUser currUser;
        lseek(fd, (i)*sizeof(NormalUser), SEEK_SET); // changing the file pointer to the selected record
        read(fd, &currUser, sizeof(NormalUser));

        if (!strcmp(currUser.status, "ACTIVE") && currUser.balance >= amount)
        {
            currUser.balance -= amount;
            lseek(fd, sizeof(NormalUser)*(-1), SEEK_CUR);
            write(fd, &currUser, sizeof(NormalUser));
            result = true;
        }
        else
            result = false;
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }
    else if (accType == 2)
    {
        int fd = open("../database/joint_user_db.txt", O_RDWR, 0744);
        bool result;
        int fl;
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i)*sizeof(JointUser); // nth record
        lock.l_len = sizeof(JointUser);         // sizeof(record)
        lock.l_pid = getpid();

        fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record
       

        JointUser currUser;
        lseek(fd, (i)*sizeof(JointUser), SEEK_SET); // changing the file pointer to the selected record
        read(fd, &currUser, sizeof(JointUser));

        if (!strcmp(currUser.status, "ACTIVE") && currUser.balance >= amount)
        {
            currUser.balance -= amount;
            lseek(fd, sizeof(JointUser)*(-1), SEEK_CUR);
            write(fd, &currUser, sizeof(JointUser));
            result = true;
        }
        else
            result = false;

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }
    return false;
}

float getBalance(int accType, int ID)
{
    int i = ID - 1;
    float result;
    
    if (accType == 1)
    {
        int i = ID - 1;
        int fd = open("../database/normal_user_db.txt", O_RDONLY, 0744);
        NormalUser temp;

        int fl;
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i)*sizeof(NormalUser); // nth record
        lock.l_len = sizeof(NormalUser);         // sizeof(record)
        lock.l_pid = getpid();

        fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record
       

        lseek(fd, (i)*sizeof(NormalUser), SEEK_SET); // changing the file pointer to the selected record
        read(fd, &temp, sizeof(NormalUser));
        if (!strcmp(temp.status, "ACTIVE"))
            result = temp.balance;
        else
            result = 0;

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }

    else if (accType == 2)
    {
        int i = ID - 1;
        int fd = open("../database/joint_user_db.txt", O_RDONLY, 0744);
        JointUser temp;

        int fl;
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i)*sizeof(JointUser); // nth record
        lock.l_len = sizeof(JointUser);         // sizeof(record)
        lock.l_pid = getpid();

        fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record
       

        lseek(fd, (i)*sizeof(JointUser), SEEK_SET); // changing the file pointer to the selected record
        read(fd, &temp, sizeof(JointUser));
        if (!strcmp(temp.status, "ACTIVE"))
            result = temp.balance;
        else
            result = 0;

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }
    return 0;
}

bool alterPassword(int accType, int ID, char newPassword[10])
{
    int i = ID - 1;
    if (accType == 1)
    {
        int fd = open("../database/normal_user_db.txt", O_RDWR, 0744);
        bool result;
        int fl;
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i)*sizeof(NormalUser); // nth record
        lock.l_len = sizeof(NormalUser);         // sizeof(record)
        lock.l_pid = getpid();

        fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record
       

        NormalUser currUser;
        lseek(fd, (i)*sizeof(NormalUser), SEEK_SET); // changing the file pointer to the selected record
        read(fd, &currUser, sizeof(NormalUser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            strcpy(currUser.password, newPassword);
            lseek(fd, sizeof(NormalUser)*(-1), SEEK_CUR);
            write(fd, &currUser, sizeof(NormalUser));
            result = true;
        }
        else
            result = false;
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }
    
    else if (accType == 2)
    {
        int fd = open("../database/joint_user_db.txt", O_RDWR, 0744);
        bool result;
        int fl;
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i)*sizeof(JointUser); // nth record
        lock.l_len = sizeof(JointUser);         // sizeof(record)
        lock.l_pid = getpid();

        fl = fcntl(fd, F_SETLKW, &lock); // lock the selected record
       

        JointUser currUser;
        lseek(fd, (i)*sizeof(JointUser), SEEK_SET); // changing the file pointer to the selected record
        read(fd, &currUser, sizeof(JointUser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            strcpy(currUser.password, newPassword);
            lseek(fd, sizeof(JointUser)*(-1), SEEK_CUR);
            write(fd, &currUser, sizeof(JointUser));
            result = true;
        }
        else
            result = false;
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }
 
    return false;
}