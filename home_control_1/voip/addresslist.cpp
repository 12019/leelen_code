#include "addresslist.h"
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

UserAddrInfo g_userAddr[USERADDRMAX];

int g_iValidAddrCnt;

bool ConfirmIP(const char* cTrueIP);

int compare(const void *a,const  void *b)
{
    return strcmp(((UserAddrInfo *)a)->userName,((UserAddrInfo *)b)->userName);
}

void loadUserAddr()
{
    g_iValidAddrCnt = 0;

    FILE *fp = fopen(USERADDR_CONFIG_FILE,"rb");

    if(fp == NULL)
    {
        printf("userAddr config file open error\n");
        return ;
    }

    memset(g_userAddr,0,sizeof(UserAddrInfo) * USERADDRMAX);

    fread(g_userAddr,sizeof(UserAddrInfo),USERADDRMAX,fp);

    fclose(fp);

    char tmpBuf[16];

    memset(tmpBuf,0,16);

    for(int i = 0;i < USERADDRMAX;i++)
    {
        memcpy(tmpBuf,g_userAddr[i].userIP,15);

        if(ConfirmIP((const char *)tmpBuf))
        {
                g_iValidAddrCnt++;
        }
        else
        {
            break;
        }
    }

    qsort(g_userAddr,g_iValidAddrCnt,sizeof(UserAddrInfo),compare);
}


void sortUserName()
{
    qsort(g_userAddr,USERADDRMAX,sizeof(UserAddrInfo),compare);

    saveUserAddrData();
}

int recurbinary(char strPeerName[],int header,int tail)
{
    int mid = 0;

    bool bRun = true;

    tail = g_iValidAddrCnt;

    while(bRun)
    {
        if(header > tail)
        {
            return -1;
        }
        else
        {
            mid = (tail + header)/2;
            if(strncmp(strPeerName,g_userAddr[mid].userName,strlen(strPeerName)) == 0)
            {
                return mid;
            }
            else if(strncmp(strPeerName,g_userAddr[mid].userName,strlen(strPeerName)) > 0)
            {
                header = mid + 1;
                continue;
            }
            else if(strncmp(strPeerName,g_userAddr[mid].userName,strlen(strPeerName)) < 0)
            {
                tail = mid - 1;
            }
        }
    }

    return -1;
}

void saveUserAddrData()
{
    FILE *fp = fopen(USERADDR_CONFIG_FILE,"wb+");

    if(fp == NULL)
    {
        printf("useraddr config file open error!\n");
        return;
    }

    fwrite(g_userAddr,sizeof(UserAddrInfo),USERADDRMAX,fp);

    fclose(fp);
}

void writeIpAndName()
{
    FILE *fp1 = fopen(USERADDR_CONFIG_FILE,"wb");

    if(fp1 == NULL)
    {
        printf("useraddr config file fp1 open error!\n");
        return;
    }

    UserAddrInfo ua;

    int m=0,n=1;

    for(int i=1;i<1000;i++)
     {
         for(int a=1;a<=8;a++)
         {
             memset(&ua,0,sizeof(ua));
             sprintf(ua.userName,"%s-%04d-%d","1111",i,a);
             sprintf (ua.userIP, "%s%d.%d","192.168.",m,n);
             sprintf(&ua.type,"%d",1);
             fwrite(&ua, sizeof(ua),1, fp1);
             n++;
             if(n>255)
              {
                  m++;
                  n = 1;
             }
         }
     }

    for(int i=1;i<1000;i++)
    {
        memset(&ua,0,sizeof(ua));

        sprintf(ua.userName,"%s-%04d","1111",i);

        sprintf (ua.userIP, "%s%d.%d","192.168.",m,n);

        sprintf(&ua.type,"%d",1);

        fwrite(&ua, sizeof(ua),1, fp1);

        n++;

        if(n>255)
         {
             m++;
             n = 1;
        }
    }

    fclose(fp1);
}
