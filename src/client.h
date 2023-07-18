#ifndef _CLIENT_H
#define _CLIENT_H
void userSimpleLogin(int sd);
void userSharedLogin(int sd);
void sudoLogin(int sd);
void showMenu(int sd);
void deposit(int sd);
void withdraw(int sd);
void balanceEnquiry(int sd);
void changePassword(int sd);
void viewDetails(int sd);
void addAccount(int sd);
void deleteAccount(int sd);
void modifyAccount(int sd);
void searchAccount(int sd);
#endif