#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>

#include "../include/client.h"
#include "../include/dataStructure.h"

#define PORT 9000

int choice, option, currUserID;

void chooseOption(int sd)
{
    int valid = 1;
    while (valid)
    {
        printf("Login as:\n");
        printf("1 Normal User\n");
        printf("2 Joint Account User\n");
        printf("3 Admin\n");

        printf("\nYour Choice: ");
        scanf("%d", &choice);
        printf("\nChoice: %d\n\n", choice);

        switch (choice)
        {
        case 1:
            NormalUserLogin(sd);
            valid = 0;
            break;
        case 2:
            JointUserLogin(sd);
            valid = 0;
            break;
        case 3:
            AdminLogin(sd);
            valid = 0;
            break;
        default:
            printf("Invalid choice!\n\n");
            break;
        }
    }
    return;
}

void displayMenu(int sd)
{
    int select;
    int valid = 1;

    while (valid)
    {
        if (choice == 1 || choice == 2)
        {
            printf("1: Deposit\n");
            printf("2: Withdraw\n");
            printf("3: Check your Balance\n");
            printf("4: Change your password\n");
            printf("5: View Details\n");
            printf("6: Exit\n");

            printf("Your Choice: ");
            scanf("%d", &select);
            printf("Choice: %d\n", select);

            switch (select)
            {
            case 1:
                deposit(sd);
                valid = 0;
                break;

            case 2:
                withdraw(sd);
                valid = 0;
                break;

            case 3:
                checkBalance(sd);
                valid = 0;
                break;

            case 4:
                updatePassword(sd);
                valid = 0;
                break;

            case 5:
                viewDetails(sd);
                valid = 0;
                break;

            case 6:
                write(sd, &select, sizeof(int));
                printf("Thank You!!\n");
                exit(0);

            default:
                printf("Invalid Choice!\n");
                break;
            }
        }

        else if (choice == 3)
        {
            printf("1: Add Account\n");
            printf("2: Delete Account\n");
            printf("3: Modify Account\n");
            printf("4: Search Account\n");
            printf("5: Exit\n\n");

            printf("Your Choice: ");
            scanf("%d", &select);
            printf("Choice: %d\n", select);

            switch (select)
            {
            case 1:
                addAccount(sd);
                valid = 0;
                break;

            case 2:
                deleteAccount(sd);
                valid = 0;
                break;

            case 3:
                updateAccount(sd);
                valid = 0;
                break;

            case 4:
                searchAccount(sd);
                valid = 0;
                break;

            case 5:
                write(sd, &select, sizeof(int));
                printf("Thank you!!\n");
                exit(0);

            default:
                printf("Invalid Choice!\n");
                break;
            }
        }
    }
}

void NormalUserLogin(int sd)
{
    bool result;
    NormalUser currUser;

    printf("User ID: ");
    scanf("%d", &currUser.userID);
    currUserID = currUser.userID;

    printf("Password: ");
    scanf("%s", currUser.password);

    // to the server
    write(sd, &option, sizeof(int));
    write(sd, &currUser, sizeof(NormalUser));

    read(sd, &result, sizeof(result)); // from the server

    if (!result)
    {
        printf("\nInvalid login!\n\n");
        chooseOption(sd);
    }
    else
    {
        printf("Successfully logged in!\n");
    }
}

void JointUserLogin(int sd)
{
    bool result;
    JointUser currUser;
    printf("User ID: ");
    scanf("%d", &currUser.userID);

    currUserID = currUser.userID;

    printf("Password: ");
    scanf("%s", currUser.password);

    // to the server
    write(sd, &choice, sizeof(int));
    write(sd, &currUser, sizeof(JointUser));

    read(sd, &result, sizeof(result)); // from the server

    if (!result)
    {
        printf("\nInvalid login!\n\n");
        chooseOption(sd);
    }

    else
    {
        printf("Successfully logged in!\n");
    }
}

void AdminLogin(int sd)
{
    bool result;
    Admin currUser;

    printf("User ID: ");
    scanf("%d", &currUser.userID);
    currUserID = currUser.userID;

    printf("Password: ");
    scanf("%s", currUser.password);

    // to the server
    write(sd, &choice, sizeof(int));
    write(sd, &currUser, sizeof(Admin));

    read(sd, &result, sizeof(result)); // from the server

    if (!result)
    {
        printf("\nInvalid login!\n\n");
        chooseOption(sd);
    }

    else
    {
        printf("Successfully logged in!\n");
    }
}

void deposit(int sd)
{
    int select = 1;
    bool result;
    float amount;

    printf("Amount to Deposit (in Rs): ");
    scanf("%f", &amount);

    while (amount <= 0)
    {
        printf("Invalid amount! Enter a valid amount!!\n");

        printf("Amount to Deposit (in Rs): ");
        scanf("%f", &amount);
    }

    // to the server
    write(sd, &select, sizeof(int));
    write(sd, &amount, sizeof(float));

    read(sd, &result, sizeof(result)); // from the server

    if (!result)
    {
        printf("ERROR: Depositing money\n");
    }
    else
    {
        printf("Successfully deposited!\n");
    }

    displayMenu(sd);
}

void withdraw(int sd)
{
    int select = 2;
    bool result;
    float amount;

    printf("Amount to withdraw (in Rs): ");
    scanf("%f", &amount);

    while (amount <= 0)
    {
        printf("Invalid amount! Enter a valid amount!!\n");
        printf("Amount to withdraw (in Rs): ");
        scanf("%f", &amount);
    }

    // to the server
    write(sd, &select, sizeof(int));
    write(sd, &amount, sizeof(float));

    read(sd, &result, sizeof(result)); // from the server

    if (!result)
    {
        printf("ERROR: Withdrawing money\n");
    }
    else
    {
        printf("Money winthdrew Successfully\n");
    }

    displayMenu(sd);
    return;
}

void checkBalance(int sd)
{
    int select = 3;
    int buf;
    float amount;

    write(sd, &select, sizeof(int));
    buf = read(sd, &amount, sizeof(float));

    printf("Current Balance (in Rs): %0.2f\n", amount);

    displayMenu(sd);
}

void updatePassword(int sd)
{
    int select = 4;
    char password[10];
    char password2[10];
    bool result;

    printf("Enter the new password: ");
    scanf("%s", password);

    // to the server
    write(sd, &select, sizeof(int));
    write(sd, password, sizeof(password));

    read(sd, &result, sizeof(result)); // from the server

    if (!result)
    {
        printf("Error updating the password!\n");
    }
    else
    {
        printf("Password updated successfully!\n");
    }

    displayMenu(sd);
}

void viewDetails(int sd)
{
    int select = 5;

    // to the server
    write(sd, &select, sizeof(int));

    if (option == 1)
    {
        NormalUser currUser;
        read(sd, &currUser, sizeof(NormalUser));

        printf("User ID: %d\n", currUser.userID);
        printf("Name: %s\n", currUser.holderName);
        printf("Account Number: %d\n", currUser.accountNumber);
        printf("Available Balance(in Rs): %0.2f\n", currUser.balance);
        printf("Status: %s\n\n", currUser.status);
    }

    else if (option == 2)
    {
        JointUser currUser;
        read(sd, &currUser, sizeof(JointUser));

        printf("User ID: %d\n", currUser.userID);
        printf("Main Account Holder's Name : %s\n", currUser.holder1);
        printf("Other Account Holder's Name : %s\n", currUser.holder2);
        printf("Account Number : %d\n", currUser.accountNumber);
        printf("Available Balance : Rs.%0.2f\n", currUser.balance);
        printf("Status : %s\n\n", currUser.status);
    }

    displayMenu(sd);
}

void addAccount(int sd)
{
    int select = 1;
    int Accounttype;
    bool result;

    write(sd, &select, sizeof(int));

    printf("Which account you want to create: \n");
    printf("1: Normal Account\n");
    printf("2: Joint Account\n");

    scanf("%d", &Accounttype);

    write(sd, &Accounttype, sizeof(int));

    // Add a Normal account
    if (Accounttype == 1)
    {
        NormalUser addUser;

        printf("Account holder Name: ");
        scanf("%[^\n]", addUser.holderName);

        printf("Password (max 10 characters): ");
        scanf("%s", addUser.password);

        printf("Deposit Money: ");
        scanf("%f", &addUser.balance);

        write(sd, &addUser, sizeof(NormalUser));
    }

    // Add a joint account
    if (Accounttype == 2)
    {
        JointUser addUser;

        printf("Enter Account holder name:\n");

        printf("Primary holder name: \n");
        scanf("%[^\n]", addUser.holder1);

        printf("Secondary holder name: \n");
        scanf("%[^\n]", addUser.holder2);

        printf("Password (max 10 characters): ");
        scanf("%s", addUser.password);

        printf("Deposit Money: ");
        scanf("%f", &addUser.balance);

        write(sd, &addUser, sizeof(JointUser));
    }

    read(sd, &result, sizeof(result)); // from the server

    if (!result)
    {
        printf("Error adding the account!\n");
    }
    else
    {
        printf("Account Successfully added!\n");
    }

    displayMenu(sd);
}

void deleteAccount(int sd)
{
    int select = 2;
    int Accounttype, userID;
    bool result;

    write(sd, &select, sizeof(int));

    printf("Enter Account type: \n");
    printf("1: Normal User\n");
    printf("2: Joint User\n");
    scanf("%d", &Accounttype);

    write(sd, &Accounttype, sizeof(int));

    printf("Enter UserID: ");
    scanf("%d", &userID);

    write(sd, &userID, sizeof(int));
    read(sd, &result, sizeof(result)); // from the server

    if (!result)
    {
        printf("ERROR: Deleting the Account!\n");
    }
    else
    {
        printf("SUCCESS: Account deleted!\n");
    }

    displayMenu(sd);
}

void updateAccount(int sd)
{
    int select = 3;
    int Accounttype;
    bool result;

    write(sd, &select, sizeof(int));

    printf("Enter the Account type: \n");
    printf("1: Normal Account\n");
    printf("1: Joint Account\n");
    scanf("%d", &Accounttype);

    write(sd, &Accounttype, sizeof(int));

    if (Accounttype == 1)
    {
        NormalUser updateUser;

        printf("Enter UserID: ");
        scanf("%d", &updateUser.userID);

        printf("Enter Account Number: ");
        scanf("%d", &updateUser.accountNumber);

        printf("Enter name of new Account holder: ");
        scanf(" %[^\n]", updateUser.holderName);

        printf("New Password (max 10 characters): ");
        scanf("%s", updateUser.password);

        printf("Balance: ");
        scanf("%f", &updateUser.balance);

        write(sd, &updateUser, sizeof(NormalUser));
    }

    if (Accounttype == 2)
    {
        JointUser updateUser;

        printf("Enter UserID: ");
        scanf("%d", &updateUser.userID);

        printf("Enter Account Number: ");
        scanf("%d", &updateUser.accountNumber);

        printf("Details of new Account holders-\n");
        printf("Enter name of primary account holder: ");
        scanf("%[^\n]", updateUser.holder1);

        printf("Enter name of secondary account holder: ");
        scanf("%[^\n]", updateUser.holder2);

        printf("New Password (max 10 characters): ");
        scanf("%s", updateUser.password);

        printf("Balance: ");
        scanf("%f", &updateUser.balance);
    }

    read(sd, &result, sizeof(result)); // from the server

    if (!result)
    {
        printf("ERROR: Modifying the account!\n");
    }
    else
    {
        printf("SUCCESS: Account modified!");
    }

    displayMenu(sd);
}

void searchAccount(int sd)
{
    int select = 4;
    int Accounttype, len;
    bool result;

    write(sd, &select, sizeof(int));

    printf("Enter the Account type:\n");
    printf("1: Normal Account\n");
    printf("2: Joint Account\n");
    scanf("%d", &Accounttype);

    write(sd, &Accounttype, sizeof(int));

    if (Accounttype == 1)
    {
        NormalUser searchUser;
        int userID;

        printf("Enter User ID: ");
        scanf("%d", &userID);

        write(sd, &userID, sizeof(int));

        len = read(sd, &searchUser, sizeof(NormalUser));

        if (len == 0)
        {
            printf("UserID doesn't exist!\n");
        }
        else
        {
            printf("User ID : %d\n", searchUser.userID);
            printf("Holder Name : %s\n", searchUser.holderName);
            printf("Account Number : %d\n", searchUser.accountNumber);
            printf("Available Balance (in Rs) : %0.2f\n", searchUser.balance);
            printf("Status : %s\n\n", searchUser.status);
        }
    }

    if (Accounttype == 2)
    {
        JointUser searchUser;
        int userID;

        printf("Enter User ID: ");
        scanf("%d", &userID);

        write(sd, &userID, sizeof(int));

        len = read(sd, &searchUser, sizeof(JointUser));

        if (len == 0)
        {
            printf("UserID doesn't exist\n");
        }
        else
        {
            printf("User ID : %d\n", searchUser.userID);
            printf("Primary Account Holder's Name : %s\n", searchUser.holder1);
            printf("Secondary Account Holder's Name : %s\n", searchUser.holder2);
            printf("Account Number : %d\n", searchUser.accountNumber);
            printf("Available Balance : Rs.%0.2f\n", searchUser.balance);
            printf("Status : %s\n\n", searchUser.status);
        }
    }
    displayMenu(sd);
}

int main()
{
    struct sockaddr_in server;

    // connection establishment
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    // server.sin_addr.s_addr=INADDR_ANY; //same machine
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // same machine
    server.sin_port = htons(PORT);

    connect(sd, (struct sockaddr *)&server, sizeof(server));

    chooseOption(sd);
    displayMenu(sd);

    close(sd);

    return 0;
}