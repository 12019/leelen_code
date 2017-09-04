#include "defensegpioscanthread.h"
#include <fcntl.h>
#include "main/maindialog.h"

extern MainDialog *g_pMainDlg;

DefenseGPIOScanThread::DefenseGPIOScanThread(QObject *parent) :
    QThread(parent)
{
    m_pHeadGPIODefenseData = NULL;

    m_bRun  = -1;

    m_bCopyDataOneTime = false;
}

DefenseGPIOScanThread::~DefenseGPIOScanThread()
{
  if(m_pHeadGPIODefenseData) delete m_pHeadGPIODefenseData;
}

void DefenseGPIOScanThread::run()
{
    int fd;

    unsigned char FangQuStatus[2] = {0};
    int buf3[8];

    DeviceStatus.devicemove = 1;
    DeviceStatus.deviceopen = 1;

    memset(DefenseGpioStatus, 1, sizeof(DefenseGpioStatus));

    fd = open("/dev/YouXianFangQu",O_RDONLY);

    fd_set fset;

    struct timeval tv;

    int ret;

    m_bRun = 1;

    while(m_bRun)
    {
        if(m_bCopyDataOneTime)
        {
            m_bCopyDataOneTime = false;

            close(fd);

            fd = open("/dev/YouXianFangQu",O_RDONLY);

            tv.tv_sec  = 0;

            tv.tv_usec = 100000;

            FD_ZERO(&fset);

            FD_SET(fd,&fset);

            ret = select(fd + 1,&fset,NULL,NULL,&tv);

            if(ret <= 0)
            {
                continue;
            }

            ret = read(fd,FangQuStatus,2*sizeof(unsigned char));

            if(m_pHeadGPIODefenseData == NULL)
             {
                 m_pHeadGPIODefenseData = new GPIODefenseData;

                 m_pHeadGPIODefenseData->next = NULL;

                m_pHeadGPIODefenseData->type = 0;

                m_pHeadGPIODefenseData->number = FangQuStatus[0];
             }
             else
             {
                GPIODefenseData *pGPIODefenseData = new GPIODefenseData;

                 pGPIODefenseData->next = NULL;

                pGPIODefenseData->type = 0;

                m_pHeadGPIODefenseData->number = FangQuStatus[0];

                GPIODefenseData * tGPIODefenseData = m_pHeadGPIODefenseData;

                 while(tGPIODefenseData->next  != NULL)
                 {

                     tGPIODefenseData = tGPIODefenseData->next;
                 }

                 tGPIODefenseData->next = pGPIODefenseData;
            }

            if (!(FangQuStatus[1] & 0x08))//模拟二次门铃
            {
                if(m_pHeadGPIODefenseData == NULL)
                {
                    m_pHeadGPIODefenseData = new GPIODefenseData;

                    m_pHeadGPIODefenseData->next = NULL;

                    m_pHeadGPIODefenseData->type = 1;

                    m_pHeadGPIODefenseData->number = FangQuStatus[1];
                }
                else
                {
                    GPIODefenseData *pGPIODefenseData = new GPIODefenseData;

                    pGPIODefenseData->next = NULL;

                    pGPIODefenseData->type = 1;

                    pGPIODefenseData->number = FangQuStatus[1];

                    GPIODefenseData * tGPIODefenseData = m_pHeadGPIODefenseData;

                    while(tGPIODefenseData->next  != NULL)
                    {

                        tGPIODefenseData = tGPIODefenseData->next;
                    }

                    tGPIODefenseData->next = pGPIODefenseData;
                }
            }
        }

        tv.tv_sec  = 0;

        tv.tv_usec = 40000;

        FD_ZERO(&fset);

        FD_SET(fd,&fset);

        ret = select(fd + 1,&fset,NULL,NULL,&tv);

        if(ret <= 0)
        {
            continue;
        }

        ret = read(fd,FangQuStatus,2*sizeof(unsigned char));

        unsigned char FangQu = FangQuStatus[0];
        unsigned char DoorRing = FangQuStatus[1];

        for(int number = 0;number < 8;number++)
        {
            QSettings settings("/mnt/disk/config/defense", QSettings::IniFormat);

            buf3[number] = settings.value("wiredefense/defense" + QString::number(number+1)).toInt();
        }

        int count=0, m_sign1 = 0,
        m_sign2 = 0,
        m_sign3 = 0,
        m_sign4 = 0,
        m_sign5 = 0,
        m_sign6 = 0,
        m_sign7 = 0,
        m_sign8 = 0;

        int  m_signJumpout = 257;

       for(int i = 1; i < 9; i++)
       {
            FangQuStatus[0] = FangQu;

             count = 0;

            if(((FangQuStatus[0] & 0x01) == 0x00) && (m_sign1 == 0) && (buf3[0] ==1) && (DefenseGpioStatus[0] == 1))
            {
                if(count>1)
                {
                    FangQuStatus[0] =  FangQuStatus[0]|0x01;
                 }
                else
                {
                    m_sign1 = 1;
                    DefenseGpioStatus[0] = 0;
                }
                 count++;

            }

            if(((FangQuStatus[0] & 0x02) == 0x00) && (m_sign2 == 0) && (buf3[1] == 1) && (DefenseGpioStatus[1] == 1))
            {
                if(count > 0)
                {
                    FangQuStatus[0] =  FangQuStatus[0]|0x02;
                }
                else
                {
                    m_sign2 = 1;
                    DefenseGpioStatus[1] = 0;
                }
                count++;
            }

            if(((FangQuStatus[0] & 0x04) == 0x00) && (m_sign3 == 0) && (buf3[2] == 1) && (DefenseGpioStatus[2] == 1))
            {
                if(count > 0)
                {
                    FangQuStatus[0] =  FangQuStatus[0]|0x04;
                }
                else
                {
                    m_sign3 = 1;
                    DefenseGpioStatus[2] = 0;
                }
                 count++;

            }

            if(((FangQuStatus[0] & 0X08) == 0X00) && (m_sign4 == 0) && (buf3[3] == 1) && (DefenseGpioStatus[3] == 1))
            {
                if(count>0)
                {
                    FangQuStatus[0] =  FangQuStatus[0]|0x08;
                }
                else
                {
                    m_sign4 = 1;
                    DefenseGpioStatus[3] = 0;
                }
                count++;
            }

            if(((FangQuStatus[0] & 0x10)==0x00) && (m_sign5 == 0) && (buf3[4] == 1) && (DefenseGpioStatus[4] == 1))
            {
                if(count>0)
                {
                    FangQuStatus[0] =  FangQuStatus[0]|0x10;
                }
                else
                {
                    m_sign5 = 1;
                    DefenseGpioStatus[4] = 0;
                }
                count++;
            }

            if(((FangQuStatus[0] & 0x20)==0x00) && (m_sign6 == 0) && (buf3[5] == 1) && (DefenseGpioStatus[5] == 1))
            {
                if(count>0)
                {
                    FangQuStatus[0] =  FangQuStatus[0]|0x20;
                }
                else
                {
                    m_sign6 = 1;
                    DefenseGpioStatus[5] = 0;
                }
                 count++;
            }

            if(((FangQuStatus[0] & 0x40)==0x00) && (m_sign7 == 0) && (buf3[6] == 1) && (DefenseGpioStatus[6] == 1))
            {
                if(count>0)
                {
                    FangQuStatus[0] =  FangQuStatus[0]|0x40;
                }
                else
                {
                    m_sign7 = 1;
                    DefenseGpioStatus[6] = 0;
                }
                 count++;
            }

            if(((FangQuStatus[0] & 0x80)==0x00) && (m_sign8 == 0) && (buf3[7] == 1) && (DefenseGpioStatus[7] == 1))
            {
                if(count>0)
                {
                    FangQuStatus[0] =  FangQuStatus[0]|0x80;
                }
                else
                {
                    m_sign8 = 1;
                    DefenseGpioStatus[7] = 0;
                }
                 count++;
            }

            if(m_signJumpout == FangQuStatus[0])
            {
                break;
            }

            printf("m_signJumpout %x %x\n",m_signJumpout,FangQuStatus[0]);

            if(m_pHeadGPIODefenseData == NULL)
             {
                 m_pHeadGPIODefenseData = new GPIODefenseData;

                 m_pHeadGPIODefenseData->next = NULL;

                m_pHeadGPIODefenseData->type = 0;

                m_pHeadGPIODefenseData->number = FangQuStatus[0];

                m_signJumpout = FangQuStatus[0];
             }
             else
             {
                m_signJumpout = FangQuStatus[0];

                GPIODefenseData *pGPIODefenseData = new GPIODefenseData;

                 pGPIODefenseData->next = NULL;

                pGPIODefenseData->type = 0;

                pGPIODefenseData->number = FangQuStatus[0];

                 GPIODefenseData * tGPIODefenseData = m_pHeadGPIODefenseData;

                 while(tGPIODefenseData->next  != NULL)
                 {

                     tGPIODefenseData = tGPIODefenseData->next;
                 }

                 tGPIODefenseData->next = pGPIODefenseData;
            }
        }

        if (!(DoorRing & 0x08))//模拟二次门铃
        {
            if(m_pHeadGPIODefenseData == NULL)
            {
                m_pHeadGPIODefenseData = new GPIODefenseData;

                m_pHeadGPIODefenseData->next = NULL;

                m_pHeadGPIODefenseData->type = 1;

                m_pHeadGPIODefenseData->number = DoorRing;
            }
            else
            {
                GPIODefenseData *pGPIODefenseData = new GPIODefenseData;

                pGPIODefenseData->next = NULL;

                pGPIODefenseData->type = 1;

                pGPIODefenseData->number = DoorRing;

                GPIODefenseData * tGPIODefenseData = m_pHeadGPIODefenseData;

                while(tGPIODefenseData->next  != NULL)
                {

                    tGPIODefenseData = tGPIODefenseData->next;
                }

                tGPIODefenseData->next = pGPIODefenseData;
            }
        }

        if (DoorRing & 0x10) //防拆输入
        {
            if (!DeviceStatus.deviceopen)
            {
                emit g_pMainDlg->m_pSecurityMainDlg->emitSigPlayWav(0);
                g_pMainDlg->m_pSecurityMainDlg->emitUploadDeviceInfo(0);
                DeviceStatus.deviceopen = 1;
            }
        }
        else
            DeviceStatus.deviceopen = 0;

        if (DoorRing & 0x20) //防移输入
        {
            if (!DeviceStatus.devicemove)
            {
                emit g_pMainDlg->m_pSecurityMainDlg->emitSigPlayWav(1);
                g_pMainDlg->m_pSecurityMainDlg->emitUploadDeviceInfo(1);
                DeviceStatus.devicemove = 1;
            }
        }
        else
            DeviceStatus.devicemove = 0;
    }
   close(fd);
}

void DefenseGPIOScanThread::writeDefenseCharArray(char areaNumber)
{
    buf_char[0] = 0xf7;

    buf_char[1] = 0x00;

    buf_char[2] = 0x01;

    buf_char[3] = 0xff;

    buf_char[4] = 0x00;

    buf_char[5] = areaNumber;

    buf_char[6] = 0xff - areaNumber;

    buf_char[7] = 0x00;

    buf_char[8] = 0x00;

    buf_char[9] = 0x00;

    buf_char[10] = 0x00;

    buf_char[11] = 0x00;

    buf_char[12] = 0x00;

    int sum = 0;

    for(int i = 0;i < 13;i++)
    {
        sum += buf_char[i];
    }

    buf_char[13] = char(0 - sum) ;

}

void DefenseGPIOScanThread::copyArray(char buf[])
{
    GPIODefenseData *data = NULL;

    if (m_pHeadGPIODefenseData->type == 0)
    {
        data = getDataFromList();

        if(data == NULL)
        {
            return;
        }

        writeDefenseCharArray(data->number);
    }
    else
    {
        data = getDataFromList();

        if(data == NULL)
        {
            return;
        }

        writeDoorRingCharArray();
    }

    for(int i = 0;i < 14;i++)
    {
        buf[i] = buf_char[i];
    }

   delete data;
}

GPIODefenseData* DefenseGPIOScanThread::getDataFromList()
{
    GPIODefenseData *data = NULL;

    if(m_pHeadGPIODefenseData != NULL)
    {
        data = m_pHeadGPIODefenseData;

        m_pHeadGPIODefenseData = data->next;

    }
    else
    {
        return NULL;
    }
    return data;
}

void DefenseGPIOScanThread::writeDoorRingCharArray()
{
    buf_char[0] = 0xf7;

    buf_char[1] = 0xff;

    buf_char[2] = 0x20;//门铃

    buf_char[3] = 0xfd;

    buf_char[4] = 0x00;

    buf_char[5] = 0x00;

    buf_char[6] = 0x00;

    buf_char[7] = 0x00;

    buf_char[8] = 0x00;

    buf_char[9] = 0x00;

    buf_char[10] = 0x00;

    buf_char[11] = 0x00;

    buf_char[12] = 0x00;

    int sum = 0;

    for(int i = 0;i < 13;i++)
    {
        sum += buf_char[i];
    }

    buf_char[13] = char(0 - sum) ;
}
