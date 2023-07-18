#ifndef _CLIENT_H
#define _CLIENT_H

void NormalUserLogin(int sd);
void JointUserLogin(int sd);
void AdminLogin(int sd);
void displayMenu(int sd);
void deposit(int sd);
void withdraw(int sd);
void checkBalance(int sd);
void updatePassword(int sd);
void viewDetails(int sd);
void addAccount(int sd);
void deleteAccount(int sd);
void updateAccount(int sd);
void searchAccount(int sd);

#endif