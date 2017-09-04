#include "tscalibratethread.h"
#include "maindialog.h"
#include "application.h"
#include <poll.h>

extern MainDialog *g_pMainDlg;

TSCalibrateThread::TSCalibrateThread(QObject *parent) :
    QThread(parent)
{
	connect(this, SIGNAL(sigOpenCalibration()), g_pMainDlg, SLOT(openCalibration()));
}

void TSCalibrateThread::run()
{
	int fd;
	int ret;
	unsigned char KeyValue, tmp;

	fd = open("/dev/EINT0_Driver", O_RDONLY);

	if (fd < 0) return;

	m_bRun = true;
	while (m_bRun)
	{
		struct pollfd fds;
		memset(&fds,0,sizeof(fds));
		fds.events = POLLIN;
		fds.fd = fd;
		ret = poll(&fds, 1, 1000);

		if (ret <= 0) continue;

        ret = read(fd, &KeyValue, sizeof(unsigned char));
        tmp = KeyValue;
		tmp = tmp & 0x80;
		KeyValue = KeyValue & 0x7f;

        if(tmp) continue;

        if (!g_pMainDlg->isVisible()) continue;

		if (g_pMainDlg->m_pSysSetMainDlg->m_pLanguageSetDlg->m_bIsSettingLanguage) continue;

		if (!((Application *)qApp)->m_bTimerOff) //bug
		{
			((Application *)qApp)->setLCDAndTimerOn();
		}

		if (g_pMainDlg->m_pCalibrationDlg->isVisible())
		{
			continue;
		}

		if (KeyValue == 9)
		{
			emit sigOpenCalibration();
		}
	}

	close(fd);
}
