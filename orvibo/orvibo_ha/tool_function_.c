#include"tool_function_.h"

int enable_showlog=1;
static char bug_mem[1024];

/*开启打印信息*/
int  open_log_()
{
    enable_showlog=1;
    return 0;
}

/*关闭打印信息*/
int  close_log_()
{
    enable_showlog=0;
    return 0;
}

/*将hex数据转换成字符, 例如: 将0x0a转换成字符'A'*/
char  hex_to_char_(unsigned char sbyte)
{
    sbyte &= 0x0F;
    if (sbyte < 0x0A)
    {
        return (char)(sbyte + '0');
    }
    else
    {
        return (char)(sbyte - 0x0A + 'A');
    }
}

/*将缓冲区中的数据按hex格式输出*/
void  show_log_hex_(char *str, char *ptr, int len)
{
    int i;
    char ch;

    if(enable_showlog==0)
    {
        return;
    }

    i = strlen(str);
    if (i > sizeof(bug_mem))
    {
        return;
    }

    memcpy(&bug_mem[0], str, i);

    for(; len > 0; len--)
    {
        ch = *ptr++;
        if ((i + 3) > sizeof(bug_mem))
        {
            break;
        }
        bug_mem[i++] = hex_to_char_((char)(ch >> 4));
        bug_mem[i++] = hex_to_char_(ch);
        bug_mem[i++] = ' ';
    }

    bug_mem[i] = '\0';

    if(enable_showlog==1) {

        printf("%s\n\n", bug_mem);
    }
}

/*打印字符串*/
void  show_log_char_(char *str, char *ptr)
{
    if(enable_showlog==1) {

        printf("%s:%s\n\n", str,ptr);
    }
}

/*打印 int 数据*/
void  show_log_int_(char *str, int ptr)
{
    if(enable_showlog==1) {

        printf("%s:%d\n\n", str,ptr);
    }
}

/*获取无线网卡wlan0 ip地址*/
int  get_wlan0_ip_addr_(unsigned char *addr)
{
    int sock_get_ip;
    char ipaddr[50];

    struct sockaddr_in *sin;
    struct ifreq ifr_ip;

    if((sock_get_ip=socket(AF_INET, SOCK_STREAM, 0)) == -1) {

        printf("socket create failse...GetLocalIp!\n");
        return -1;
    }

    memset(&ifr_ip, 0, sizeof(ifr_ip));
    strncpy(ifr_ip.ifr_name, "wlan0", sizeof(ifr_ip.ifr_name) - 1);

    if( ioctl( sock_get_ip, SIOCGIFADDR, &ifr_ip) < 0 ) {

        printf("ip ioctl error\n");
        close(sock_get_ip);
        return -1;
    }

    sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;
    memset(ipaddr,0,50*sizeof(char));
    strcpy(ipaddr,inet_ntoa(sin->sin_addr));

    memcpy(addr,ipaddr,strlen(ipaddr)*sizeof(char));
    addr[strlen(ipaddr)] ='\0';
    close( sock_get_ip);
    return 0;
}

/*将指定个数的数值型16进制的数据转换成字符表示的数据*/
int  hex_to_str_(char *src,char *dst,int len)
{
    int i;
    char  ch;
    uint8_t tmp[1024] = {'\0'};

    i = len*2;

    if (i > sizeof(tmp)) {

        return -1;
    }

    i=0;
    for (; len > 0; len--) {

        ch = *src++;

        if ((i + 2) > sizeof(tmp)) {

            break;
        }

        tmp[i++] = tcp_hex_to_char((char)(ch >> 4));
        tmp[i++] = tcp_hex_to_char(ch);
    }

    memcpy(dst,tmp,i);
    return 0;
}
