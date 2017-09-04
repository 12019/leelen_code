#ifndef ADDRESSLIST_H
#define ADDRESSLIST_H

#define USERADDRMAX 10000
#define USERADDR_CONFIG_FILE  "/mnt/disk/config/UserAddrInfo"

typedef struct UserAddrInfo_
{
    char userName[11];
    char userIP[15];
    char type;
} UserAddrInfo;

void loadUserAddr();

void sortUserName();

int recurbinary(char strPeerName[],int header,int tail);

void saveUserAddrData();

void writeIpAndName();
#endif // ADDRESSLIST_H
