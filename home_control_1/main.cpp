#include <QtGui/QApplication>
#include "maindialog.h"
#include "lpcore.h"
#include "command.h"
#include "mediastreamer2/mediastream.h"
#include "data_types.h"
#include <QtXml>
#include "languagesetdialog.h"
#include "syssetmaindialog.h"
#include <QTableView>
#include <qstandarditemmodel.h>
#include "core/leelen_comdef.h"
#include "leelen_protocol.h"
#include "ui_maindialog.h"
#include "application.h"
#include "ui_languagesetdialog.h"
#include "ui_mediamandialog.h"
#include "ui_versiondialog.h"
#include <fcntl.h>
#include "ui_maindialog.h"
#include <signal.h>
#include <sys/vfs.h>
#include "addresslist.h"
#include <sys/stat.h>
#include <unistd.h>


#include <linux/rtc.h>

#include "global.h"
#ifdef CORTEXA7
#include "render.h"
#endif

extern "C"
{
#include "mediastreamer2/tvout.h"
}

/************************************************************
  该文件为main()函数主程序
************************************************************/

/************************************************************
  如果该宏为1则程序为NFS调试模式，为0则程序为发布模式
************************************************************/
#define NFS_DEBUG		0

#define PATHMAX			64

#define LED_PANEL_CTRL		1

QString strSoftVersion;
QString SoftVersion;
QString HardVersion;

static char configfile_name[PATHMAX];

Kevin::Global  *g_pScaleSrc = NULL;

LinphoneCore *lphone = NULL;

MainDialog *g_pMainDlg = NULL;

char *g_pDesktopImageData = NULL;
extern char g_strNativeName[50];
extern char g_strLphoneName[3][50];

extern int g_fb0_width;

extern int g_fb0_height;

extern int g_enable_tvout;

extern int g_iVideoCapSizeType;

extern int g_iRecordMaxTime;

char g_appBackImagePath[] = ":/images/main/background.jpg";//N70-10-80-7.jpg";

int g_iDeviceType;

int g_door_bell_type;	// 二次门铃的类型的全局变量

extern int psystem(const char *cmd);

int IsFileEXist(const char *fullname)
{
        struct stat buf;
        int err = stat(fullname,&buf);

        if(err == 0)
                return TOUCHR;
        if(errno == 2) //判断errno 是否为“ ENOENT         参数file_name指定的文件不存在”
        {
                perror("err");
                return TOUCHZ;
        }
        return TOUCHR;
}

/************************************************************
描述：将软件版本信息中的时间设置到当前的系统时间
参数：
返回：
************************************************************/
void Init_System_Data_Time(QString& ts)
{
	struct tm tm_time;
	struct timeval tv_time;
	memset(&tm_time, 0, sizeof(struct tm));
	QDateTime dt = QDateTime::fromString(ts.toLatin1().data(),"yyyyMMdd");

	tm_time.tm_year = dt.toUTC().date().year() - 1900;
	tm_time.tm_mon = dt.toUTC().date().month() - 1;
	tm_time.tm_mday = dt.toUTC().date().day();

	tm_time.tm_hour = 8;
	tm_time.tm_min  = 30;
	tm_time.tm_sec  = 0;

	QDateTime datetime = QDateTime::currentDateTime();
	QDateTime datetime_utc(QDate(datetime.toUTC().date().year(), datetime.toUTC().date().month(), datetime.toUTC().date().day()),
			QTime(datetime.toUTC().time().hour(), datetime.toUTC().time().minute(), datetime.toUTC().time().second()));
	int time_zone_sec_off = datetime_utc.secsTo(datetime);

	int rtc;
	if ((rtc = ::open("/dev/rtc0", O_WRONLY)) < 0)
	{
		printf("RTC open error\n");
	}

	tv_time.tv_sec = (time_t)(timegm(&tm_time) - time_zone_sec_off);
	tv_time.tv_usec = 0;

	settimeofday(&tv_time, NULL);

	if (rtc >= 0)
	{
		if (ioctl(rtc, RTC_SET_TIME, &tm_time) < 0)
		{
			printf("RTC write error\n");
		}

		::close(rtc);
	}
}

/************************************************************
描述：将系统的信息以文件的形式添加在程序当中
参数：
返回：
************************************************************/
bool gen_system_info(char** argv)
{
	char* cmd  = new char[200];
	char* name = new char[200];

	bool special_flag = false;

	SoftVersion = argv[0];					// 获得软件信息
	int split_pos = SoftVersion.lastIndexOf("_");
	if (split_pos != -1)
	{
		if ( SoftVersion.length() - split_pos < 4 )	// 含有特殊标志
		{
			split_pos = SoftVersion.lastIndexOf("_",split_pos-1);
			SoftVersion = SoftVersion.mid(split_pos + 1);
			SoftVersion.chop(2);			// 将尾巴的_S去掉
			special_flag = true;
		}
		else
			SoftVersion = SoftVersion.mid(split_pos + 1);
	}
	else
	{
		delete[] name;
		name = NULL;
		delete[] cmd;
		cmd = NULL;

		return false;
	}

	sprintf(cmd,"cat /proc/cmdline > /tmp/1");		// 获得硬件版本
	psystem(cmd);
	sync();
	QFile file("/tmp/1");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		delete name;
		name = NULL;
		delete cmd;
		cmd = NULL;

		return false;
	}
	while (!file.atEnd()) 
	{
		QByteArray line = file.readLine();
		int sp = line.indexOf("hardversion=")+12;
		int mp = line.indexOf("-",sp);
		int ep = line.indexOf("-",mp+1);
		
		QByteArray re1,re2;
		for (int i=sp;i<mp;i++)
			re1 += line[i];
		for (int i=mp+1;i<ep;i++)
			re2 += line[i];

		if ( re1.size() == 1 ) 
			HardVersion = "0"+re1;
		else
			HardVersion = re1;

		if ( re2.size() == 1 ) 
			HardVersion += "0"+re2;
		else
			HardVersion += re2;
	}
	file.close();

	QString KernelVersion;
	sprintf(cmd,"uname -v > /tmp/1");			// 获得kernel编译时间
	psystem(cmd);
	sync();
	QFile file1("/tmp/1");
	if (!file1.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		delete name;
		name = NULL;
		delete cmd;
		cmd = NULL;

		return false;
	}
	while (!file1.atEnd()) 
	{
		QByteArray line = file1.readLine();
		QString tt;
		int sp = line.indexOf("PREEMPT ")+8;
		QByteArray re;

		for ( int i=sp;i<line.length()-1;i++ )
			re += line[i];
		tt = re;
		KernelVersion = QDateTime::fromString(tt.toLatin1().data(),"ddd MMM d hh:mm:ss 'CST' yyyy").toString("yyyyMMdd");
	}
	file1.close();
								// 获得u-boot版本
	QString UbootVersion;
	sprintf(cmd,"cat /proc/cmdline > /tmp/1");
	psystem(cmd);
	sync();
	QFile file2("/tmp/1");
	if (!file2.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		delete name;
		name = NULL;
		delete cmd;
		cmd = NULL;

		return false;
	}
	while (!file2.atEnd()) 
	{
		QByteArray line = file2.readLine();
		int sp = line.indexOf("ubootversion=")+13;
		QString tt;
		QByteArray re;

		for ( int i=sp;i<sp+11;i++ )
			re += line[i];
		tt = re;

		UbootVersion = QDateTime::fromString(tt.toLatin1().data(),"MMM-d-yyyy").toString("yyyyMMdd");
	}
	file2.close();

	sprintf(cmd,"rm -f /tmp/1");				// 删除临时文件
	psystem(cmd);
	sync();

	sprintf(cmd,"rm -f /versions_*");			// 删除旧的版本信息
	psystem(cmd);
	sync();

	if ( special_flag )
		sprintf(name,"versions_H%s_U%s_K%s_M%s_S",HardVersion.toLatin1().data(),UbootVersion.toLatin1().data(),KernelVersion.toLatin1().data(),SoftVersion.toLatin1().data());
	else
		sprintf(name,"versions_H%s_U%s_K%s_M%s",HardVersion.toLatin1().data(),UbootVersion.toLatin1().data(),KernelVersion.toLatin1().data(),SoftVersion.toLatin1().data());

	sprintf(cmd,"touch /%s",name);
	psystem(cmd);
	sync();

	delete name;
	name = NULL;
	delete cmd;
	cmd = NULL;

	return true;
}

/************************************************************
描述： 获得文件的大小，如该文件不存在，则返回-1
参数：
返回： -1  error
       文件大小
************************************************************/
int get_file_size(const char *path)
{
	int filesize = -1;
	struct stat statbuff;
	if(stat(path, &statbuff) < 0)
	{
		return filesize;
	}
	else
	{
		filesize = statbuff.st_size;
	}
	return filesize;
}

/************************************************************
描述： 恢复/mnt/disk/configbackup/config到/mnt/disk/config/config
       与/mnt/config文件
参数： 已经确保/mnt/disk/configbackup/config正常
************************************************************/
void restore_config_from_last_file()
{
	QString ip, native, mc1, mc2, doorway, gate_way, net_mask, passwd;
	{
		QSettings settings_config("/mnt/disk/config/config", QSettings::IniFormat);
		ip   = settings_config.value("system/native_ip").toString();  			// 如果没有这个key,就保持/mnt/disk/configbackup/config中的数值
		native = settings_config.value("system/native_name").toString();
		mc1  = settings_config.value("system/manage_center1_name").toString();
		mc2  = settings_config.value("system/manage_center2_name").toString();
		doorway = settings_config.value("system/doorway_name").toString();
		gate_way = settings_config.value("system/gate_way").toString();
		net_mask = settings_config.value("system/net_mask").toString();
		passwd = settings_config.value("system/passwd").toString();
	}

	psystem("cp -p /mnt/disk/configbackup/config /mnt/disk/config/config");
	sync();
	{
		QSettings new_settings_config("/mnt/disk/config/config", QSettings::IniFormat);

		new_settings_config.setValue("system/native_ip",ip.toLatin1().data());          // 进行工程参数的回复
		new_settings_config.setValue("system/native_name",native.toLatin1().data());
		new_settings_config.setValue("system/manage_center1_name",mc1.toLatin1().data());
		new_settings_config.setValue("system/manage_center2_name",mc2.toLatin1().data());
		new_settings_config.setValue("system/doorway_name",doorway.toLatin1().data());
		new_settings_config.setValue("system/gate_way",gate_way.toLatin1().data());
		new_settings_config.setValue("system/net_mask",net_mask.toLatin1().data());
		new_settings_config.setValue("system/passwd",passwd.toLatin1().data());
	}

	psystem("cp -p /mnt/disk/config/config /mnt/config");
	sync();
}

/************************************************************
描述： 判断/mnt/disk/config/config与/mnt/config是否正常，
       如果有一个文件不正常，就将正常替换异常的文件，判断
       依据主要是文件长度>500
       如果两个文件都异常，那么使用第三个备份文件
       /mnt/disk/configbackup/config文件进行还原
参数：
************************************************************/
void check_and_restore_config_file()
{
	bool config_available_flag 	 = false;
	bool backupconfig_available_flag = false;

	int config_length = get_file_size(CONFIG_FILE);
	if( config_length > LIMIT_CONFIG_LENGTH )				   // config文件正常
		config_available_flag = true;
	config_length = get_file_size("/mnt/config");
	if( config_length > LIMIT_CONFIG_LENGTH )				   // /mnt/config文件正常
		backupconfig_available_flag = true;

	if ( backupconfig_available_flag == true && config_available_flag == true )// config文件与/mnt/config都正常
		return;
	else if ( backupconfig_available_flag == false && config_available_flag == true )
	{
		psystem("cp -p /mnt/disk/config/config /mnt/config");
		sync();
	}
	else if ( backupconfig_available_flag == true && config_available_flag == false )	// config文件异常，进行配置文件还原，并重启
	{
		psystem("cp -p /mnt/config /mnt/disk/config/config");
		sync();

		int n = getpid();
		char cmd[20];
		sprintf(cmd,"kill %d",n);
		system(cmd);
	}
	else									   // 两个config文件都异常
	{
		bool lastconfig_available_flag   = false;			   // /mnt/disk/configbackup/config
		config_length = get_file_size("/mnt/disk/configbackup/config");
		if( config_length > LIMIT_CONFIG_LENGTH )
			lastconfig_available_flag = true;

		if ( lastconfig_available_flag )				   // 第三方备份文件正常
		{
										   // 从/mnt/disk/backupconfig/config中回复
			psystem("cp -p /mnt/disk/configbackup/config /mnt/disk/config/config");
			psystem("cp -p /mnt/disk/config/config /mnt/config");
			sync();
			
			int n = getpid();
			char cmd[20];
			sprintf(cmd,"kill %d",n);
			system(cmd);
		}
		else
			printf ( "ALL config file is bad\n" );			   // 全部config损坏以默认方式启动
	}
}

/************************************************************
描述：向指定对象发事件，发事件后不等对象处理立即返回
参数：receiver - 接收事件的对象
	 type - 事件类型
	 message - 事件附带的消息
返回：无
************************************************************/
void PostEventWithMessage(QObject *receiver, QEvent::Type type, QString message)
{
	if (type - 5000 == NID_IDLE_RCVINV)
	{
		closeMessBox(g_pMainDlg);
	}

	qApp->postEvent(receiver, new Levent(type, message));
}

/************************************************************
描述：lphone库上发消息的函数，在lphone库中被调用
参数：lc - LinphoneCore对象
	 nid - 消息号
	 msg - 消息附带的文本信息
返回：无
************************************************************/
void lpc_notify(LinphoneCore *lc, notify_t nid, char *msg)
{
	Q_UNUSED(lc);

	ltype t = (QEvent::Type)(5000 + nid);

	PostEventWithMessage(g_pMainDlg->m_pVoipMainDlg, t, QString(msg));
}

/*************************************************************************
描述：如果没有record.xml文件进行文件的创建
      false:  创建文件失败
      true:   创建文件成功
*************************************************************************/
bool init_record_xml_file()
{
	psystem("/bin/rm -f /mnt/disk/media/record.xml");   // 强行删除record.xml文件下，如果不存在也不影响

	FILE *fp;

	char msg[] = "<?xml version='1.0' encoding='UTF-8'?>\n<sysrecord>\n<record duration= \"duration\" datetime= \"datetime\" aviurl= \"aviurl\" type=\"type\" ipaddress=\"ipaddress\" state=\"state\" />\n</sysrecord>";

	fp = fopen("/mnt/disk/media/record.xml","wb+");

	if(NULL == fp)
	{
		printf("open record.xml failed\n");
		return false;
	}

	fwrite(msg,strlen(msg),1,fp);

	fclose(fp);
	sync();
	return true;
}

/*************************************************************************
描述：之前的版本的对讲记录是45条，现在改为10条，当记录超过10条，
      全部清除过去的记录，包含avi文件以及xml文件
*************************************************************************/
void clean_call_record()
{
	QFile file("/mnt/disk/media/record.xml");
	QDomDocument doc;
	int loadcount = 1;

RELOAD:
	if (!file.open(QIODevice::ReadOnly))
	{
		psystem("/bin/rm -f /mnt/disk/media/*avi");

		if ( !init_record_xml_file() )
			printf ( "error %s:%d re-init media directory fail\n",__FILE__,__LINE__ );
	}

	if (!doc.setContent(&file))
	{
		file.close();

		psystem("/bin/rm -f /mnt/disk/media/*avi");

		if ( !init_record_xml_file() )
			printf ( "error %s:%d re-init media directory fail\n",__FILE__,__LINE__ );

		if ( loadcount != 0 )
		{
			loadcount --;
			goto RELOAD;
		}
		else
		{
			printf ( "error %s:%d can't load xml need to fix the bug\n",__FILE__,__LINE__ );
			return;
		}
	}
	else
		file.close();

	QDomElement root = doc.documentElement();

	QString recordNum = root.lastChild().toElement().attribute("recordNum");

	int idx = recordNum.toInt();

	if( idx > CALL_RECORD_COUNT )				// 如果超过记录数目，说明是旧有设备，进行数据清空
	{
		psystem("/bin/rm -f /mnt/disk/media/*avi");

		if ( !init_record_xml_file() )
			printf ( "%s:%d re-init media directory fail\n",__FILE__,__LINE__ );
	}
}

/************************************************************
描述： 当flash空间小于20MB的时候，按照时间顺序删除AVI文件 
参数： 
返回： 
************************************************************/
void remove_avi_files_over_20MB(QDomNodeList& list,QFile& file,QDomDocument& doc)
{
	struct statfs diskinfo;
	int freediskvolume;
	int times = CALL_RECORD_AVI_COUNT;
CHECKAGAIN:
	statfs("/",&diskinfo);
	freediskvolume = (diskinfo.f_bfree * diskinfo.f_bsize) >> 20;
	if ( freediskvolume >= 20 ) return;			// 如果还有超过20MB的磁盘空间，就返回

	QString url;

	for( int i = 1;i < list.count();i++ )			// 滤遍list中的全部有效url数据
	{
		url =  list.at(i).toElement().attribute(QString("aviurl")); 
		if ( url != "" )
		{
			QFile::remove(url);			// 清除最旧的AVI文件 
			sync();

			list.at(i).toElement().attributeNode("aviurl").setValue("");   
								 // 删除AVI文件后，将xml文件中某记录的aviurl清除

			if( !file.open(QIODevice::WriteOnly | QIODevice::Truncate) )     // 进行xml文件的保存
			{
				printf ( "error %s:%d can't open the record.xml need be fixed\n",__FILE__,__LINE__ );
				return ;
			}

			QTextStream out(&file);
			doc.save(out,4);			 //  将修改后的record.xml文件进行保存
			file.close();
			sync();

			break;
		}
	}
	times --;
	if ( times != 0 )					// 最多确认AVI记录的条目数字
		goto CHECKAGAIN;
}

/************************************************************
描述： 根据现在xml文件中的AVI文件名称，获得缺失的文件名称
参数： record.xml文件中的数据集合
返回： 返回avi中第一个缺失的编号
************************************************************/
int loop_record_xml_file_return_index(const QDomNodeList& list)
{
	int return_value = 999;					// 特殊数字表示没有找到合适的尾数，如有存在该数值说明有逻辑错误
	bool filled_avi_slot[CALL_RECORD_AVI_COUNT];
	for ( int i = 0; i < CALL_RECORD_AVI_COUNT; i += 1 ) 	// 初始化全部的可能的avi文件编号
		filled_avi_slot[i] = false;

	QString url;

	for( int i = 1;i < list.count();i++ )			// 滤遍list中的全部有效url数据
	{
		url =  list.at(i).toElement().attribute(QString("aviurl")); 
		if ( url != "" )
		{
			filled_avi_slot[url.right(7).left(3).toInt()] = true;
		}
	}


	for ( int i = 0; i < CALL_RECORD_AVI_COUNT; i += 1 ) 
	{
		if ( filled_avi_slot[i] == false )		// 返回第一个空闲的AVI编号
		{
			return_value = i; 
			break;
		}
	}

	return return_value;
}

/************************************************************
描述：lphone库录像时用该函数获取AVI文件名的序号，在lphone库中被调用
参数：lc - LinphoneCore对象
	 num - 存函数返回的AVI文件名的序号
	 先保存AVI文件，再写入xml文件
返回：无
************************************************************/
void lpc_log_get_calls_num(LinphoneCore *lc, int *num)
{
	printf("lpc_log_get_calls_num 0\n");

    g_pMainDlg->m_pSysSetMainDlg->m_pRingAndVolumeDlg->m_pDoorBellRingFileSetDlg->Switch_Channel_Sound(false);

    g_pMainDlg->stop_pwm_ring();

	Q_UNUSED(lc);

	QFile file("/mnt/disk/media/record.xml");

	QDomDocument doc;

	if (!file.open(QIODevice::ReadOnly)) return;
	printf("lpc_log_get_calls_num 1\n");
	if (!doc.setContent(&file))
	{
		file.close();
		//david 2014.5.12
		psystem("/bin/rm -f /mnt/disk/media/*avi");

		QStandardItemModel *model;

		model = (QStandardItemModel *)g_pMainDlg->m_pMediaManDlg->ui->tableViewMediaList->model();

		model->removeRows(0, model->rowCount());

		if ( !init_record_xml_file() )
			printf ( "error %s:%d re-init media directory fail\n",__FILE__,__LINE__ );
	}
	else
		file.close();

    	QDomElement root = doc.documentElement();
	QString aviurl = root.lastChild().toElement().attribute("aviurl");
	QString recordnum = root.lastChild().toElement().attribute("recordNum");

	int idx;

	if (aviurl == "aviurl")
	{
		idx = 0;
	}
	else
	{
		//"avi_rec_001.avi"

		idx = recordnum.toInt();
		int xml_record_count = idx;
		QDomNodeList list = doc.elementsByTagName("record");

		remove_avi_files_over_20MB(list,file,doc);					 // 当磁盘空间小于20MB,进行文件清理工作

		if ( ( idx >= CALL_RECORD_AVI_COUNT ) )       	 				 // idx 下标从1开始,问题是当磁盘已经小于20MB，但是记录数还没有达到10如何?
		{
			QString aviurl_del = list.at(idx-CALL_RECORD_AVI_COUNT+1).toElement().attribute(QString("aviurl"));

			if ( aviurl_del == "" )							 // 有发生删除有效AVI条目的情况
			{
				idx = loop_record_xml_file_return_index(list);			 // 获得AVI序号
			}
			else
			{
				QFile::remove(aviurl_del);					 // 清除 上溯第N条记录的avi文件名称
				sync();
				list.at(idx-CALL_RECORD_AVI_COUNT+1).toElement().attributeNode("aviurl").setValue("");   
												 // 删除AVI文件后，将xml文件中某记录的aviurl清除

				if( !file.open(QIODevice::WriteOnly | QIODevice::Truncate) )     // 进行xml文件的保存
				{
					printf ( "error %s:%d can't open the record.xml need be fixed\n",__FILE__,__LINE__ );
					return ;
				}

				QTextStream out(&file);
				doc.save(out,4);						 //  将修改后的record.xml文件进行保存
				file.close();
				sync();

				idx = aviurl_del.right(7).left(3).toInt();			 // 返回 被删除的avi文件的序号
			}
#ifdef ARM11
			psystem("/bin/echo 3 > /proc/sys/vm/drop_caches");		         // 功能确认，位置确认 ?????
#endif
		}
		else
		{
			idx = loop_record_xml_file_return_index(list);				 // 获得AVI序号
		}

		if ( xml_record_count >= CALL_RECORD_COUNT )
		{
			root.removeChild(list.at(1));

			QAbstractItemModel *model = g_pMainDlg->m_pMediaManDlg->getTableViewMediaList()->model();
			model->removeRow(model->rowCount() - 1);

			for( int i = 1;i < list.count();i++ )				 	 // 重新设置序号
			{
				list.at(i).toElement().attributeNode("recordNum").setValue(QString::number(i));
			}

			if( !file.open(QIODevice::WriteOnly | QIODevice::Truncate) )
			{
				printf ( "error %s:%d can't open the record.xml need be fixed\n",__FILE__,__LINE__ );
 				return ;
			}

			QTextStream out(&file);
			doc.save(out,4);							 //  将修改后的record.xml文件进行保存
			file.close();
			sync();
		}
	}
	*num = idx;
}

/************************************************************
描述：lphone库VoIP结束后调用该函数，该函数用于更新对讲记录xml文件，并
	 在对讲管理的对讲记录列表控件中添加一条新的对讲记录
参数：lc - LinphoneCore对象
	 msg - 文本信息
返回：无
************************************************************/
void lpc_calllog(LinphoneCore *lc, void *msg)
{
	Q_UNUSED(msg);

#if NFS_DEBUG == 1
	//lg 用nfs调试时不存储拨打方信息
	if (lc->calllog->dir == LinphoneCallOutgoing)
		return;
#endif
	//读xml文件
	QFile file("/mnt/disk/media/record.xml");
	if (!file.open(QIODevice::ReadOnly)) return;
	QDomDocument doc;
	if (!doc.setContent(&file))
	{
		file.close();
		return;
	}
	//读xml文件结束
	file.close();

	QDomElement root = doc.documentElement();
	//以下一个Element和七个Attr是独立创建的
	QDomElement record = doc.createElement("record");
	QDomAttr recordNum=doc.createAttribute("recordNum");
	QDomAttr datetime = doc.createAttribute("datetime");
	QDomAttr type = doc.createAttribute("type");
	QDomAttr address = doc.createAttribute("ipaddress");
	QDomAttr state = doc.createAttribute("state");
	QDomAttr duration = doc.createAttribute("duration");
	QDomAttr aviurl=doc.createAttribute("aviurl");
	QDomAttr unread=doc.createAttribute("unread");
	QString num = root.lastChild().toElement().attribute("recordNum");


	int count = num.toInt() + 1;

	recordNum.setValue(QString::number(count));
	datetime.setValue(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
	unread.setValue("0");
	if(lc->calllog->dir == LinphoneCallOutgoing)
	{
		type.setValue("send");
	}
	else
	{
		type.setValue("receive");

		if (lc->calllog->status == LinphoneCallRecord)
		{
			unread.setValue("2");
		}
		else
		{
			unread.setValue("1");
		}
	}

	address.setValue(lc->calllog->user);//lc->calllog->ip));

	switch(lc->calllog->status){
	case LinphoneCallSuccess:
		state.setValue("LinphoneCallSuccess");
		break;
	case LinphoneCallAborted:
		state.setValue("LinphoneCallAborted");
		break;
	case LinphoneCallMissed:
		state.setValue( "LinphoneCallMissed");
		break;
	case LinphoneCallDeclined:
		state.setValue("LinphoneCallDeclined");
		break;
	case LinphoneCallRecord:
		state.setValue("LinphoneCallRecord");
		break;
	}
	duration.setValue(QString::number(lc->calllog->duration));
	aviurl.setValue(QString(lc->calllog->aviurl));
	record.setAttributeNode(recordNum);
	record.setAttributeNode(datetime);
	record.setAttributeNode(type);
	record.setAttributeNode(address);
	record.setAttributeNode(state);
	record.setAttributeNode(duration);
	record.setAttributeNode(aviurl);
	record.setAttributeNode(unread);
	root.appendChild(record);

	if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return ;
	QTextStream out(&file);
	doc.save(out,4);
	file.close();
	//if (g_pMainDlg->m_pMediaManDlg->isVisible())
	{
		////QString dur;
		////dur = duration.value().append(QObject::tr("秒"));

		QString call_type;
		if (type.value() == "receive")
		{
			if (state.value() == "LinphoneCallSuccess")
			{
				call_type = ":/images/intercom_man/status_callin.png";
			}
			else
			{
				if (state.value() == "LinphoneCallRecord")
				{
					call_type = ":/images/intercom_man/status_callin_record_unread.png";//icon
				}
				else
				{
					call_type = ":/images/intercom_man/status_callin_failed_unread.png";
				}

				if (!g_pMainDlg->m_pMediaManDlg->isVisible())
				{
					g_pMainDlg->m_pMediaManDlg->m_newVisitorCnt++;

					QSettings settings_config(CONFIG_FILE, QSettings::IniFormat);
					settings_config.setValue("info/new_visitor_cnt", g_pMainDlg->m_pMediaManDlg->m_newVisitorCnt);
					g_pMainDlg->emitSigSetNotify(0);
				}
			}
		}
		else if (type.value() == "send")
		{
			if (state.value() == "LinphoneCallSuccess")
			{
				call_type = ":/images/intercom_man/status_callout.png";
			}
			else
			{
				call_type = ":/images/intercom_man/status_callout_failed.png";
			}
		}

		QString have_avifile;
		QFile avi_file(aviurl.value());
		if (avi_file.open(QIODevice::ReadOnly))
		{
			have_avifile = "yes";
			avi_file.close();
		}
		else
		{
			have_avifile = "no";
		}

		QString strTemp = lc->calllog->user;
		int pos = strTemp.indexOf("?");
		if (pos >= 0)
		{
			strTemp = strTemp.left(pos);
		}

		QStringList &addressList = g_pMainDlg->m_pAddressBookDlg->m_addressListNetName;
		int i;
		for (i = 0; i < addressList.count(); i++)
		{
			QString str = addressList.at(i);

			if (strTemp.length() > str.length())
			{
				if ((strTemp.left(str.length()) == str) && (strTemp.at(str.length()) == QChar('-')))
				{
					strTemp = g_pMainDlg->m_pAddressBookDlg->m_addressListName.at(i) + strTemp.mid(str.length());
					break;
				}
			}
			else if (strTemp == str)
			{
				strTemp = g_pMainDlg->m_pAddressBookDlg->m_addressListName.at(i);
				break;
			}
		}

		if (i >= addressList.count())
		{
			pos = strTemp.indexOf("-");
			if ((pos != -1) && ((pos != strTemp.length() - 2) || (strTemp.at(strTemp.length() - 1).isDigit())))
			{
				QString strTemp2 = g_strNativeName;//g_strLphoneName[0];
				int pos2;
				pos2 = strTemp2.indexOf("?");
				if (pos2 >= 0) strTemp2 = strTemp2.left(pos2);

				pos2 = strTemp2.indexOf("-");
				if ((pos2 != -1) && ((pos2 != strTemp2.length() - 2) || (strTemp2.at(strTemp2.length() - 1).isDigit())))
				{
					strTemp2 = strTemp2.left(pos2);
					if (strTemp.left(pos) == strTemp2)
					{
						strTemp = strTemp.mid(pos + strlen("-"));
					}
				}
			}
		}

		QString strDateTime;
		int year, month, day, hour, minute, second;
		sscanf(datetime.value().toLocal8Bit().data(), "%04d-%02d-%02d %02d:%02d:%02d", &year, &month, &day,
			   &hour, &minute, &second);
		strDateTime.sprintf("%d-%d-%d %d:%02d", year, month, day, hour, minute);

		g_pMainDlg->emitCallRecordInfoManager(call_type,strTemp,strDateTime,have_avifile,unread.value()); // 包含 g_pMainDlg->emitSigSetRowHeight(0, 0);
//		g_pMainDlg->emitSigSetRowHeight(0, 0);//修复对讲管管理信息框比较大问题 lidh 20141018

	}
}

/************************************************************
  lphone库通过这3个接口函数和QT界面程序交互
************************************************************/
LinphoneCoreVTable linphonec_vtable = {
	lpc_notify,
	lpc_log_get_calls_num,
	lpc_calllog
};

void sig_handler(int signo)
{
	printf("sig_handler:%d\n", signo);
	if ((signo == SIGTERM) || (signo == SIGINT))
	{
		if (g_pMainDlg)
		{
			if (g_pMainDlg->m_watchdog_fd >= 0)
			{
				g_pMainDlg->m_pWatchDogTimer->stop();
				write(g_pMainDlg->m_watchdog_fd, "V", 1);
				close(g_pMainDlg->m_watchdog_fd);
			}
		}
		exit(0);
	}
}

/************************************************************
描述：记录终端机主叫，被叫，监控，报警次数和重启次数
参数：nOption对应不同情况
返回：无
************************************************************/
void systemRunLog(int nOption)
{
    char filepath[70] = "/mnt/disk/config/UsingRecordFile";

    if((access(filepath,F_OK)) == -1){
        psystem("no /mnt/disk/config/usingRecordFile\n");
    }

    QSettings settings("/mnt/disk/config/UsingRecordFile", QSettings::IniFormat);

    int ictTimes, istTimers = 0,mstTimers = 0,Securitycount = 0,cardCount = 0,   unLock = 0;

    switch(nOption)
    {
        case  0:
        {
            ictTimes    = settings.value("log/ictTimes", "0").toInt();

            istTimers  = settings.value("log/istTimers", "0").toInt();

            mstTimers = settings.value("log/mstTimers", "0").toInt();

            Securitycount= settings.value("log/Securitycount", "0").toInt();

            cardCount  =settings.value("log/cardCount", "0").toInt();

            unLock       = settings.value("log/unLock", "0").toInt();

            FILE *fp = fopen("/mnt/disk/config/log_file.txt","a+");

            if(fp == NULL)
            {
                printf("log_file  file fp open error!\n");
                return;
            }

            fseek( fp, SEEK_SET,SEEK_END);

            char date[100];

            memset(date,0,100);

            sprintf(date,"\r\nInformation ictTimes:%d,istTimers:%d,mstTimers:%d,Securitycount:%d,cardCount:%d,unLock:%d.",
                    ictTimes,istTimers,mstTimers,Securitycount,cardCount,unLock);

            fwrite(date, sizeof(date),1, fp);

            settings.setValue("log/ictTimes",0);

            settings.setValue("log/istTimers",0);

            settings.setValue("log/mstTimers",0);

            settings.setValue("log/Securitycount",0);

            settings.setValue("log/cardCount",0);

            settings.setValue("log/unLock",0);

            int count = 0,size = 0;

            count  = settings.value("system/reset", "0").toInt();

            fseek( fp, SEEK_SET,SEEK_END);

            size = ftell(fp);

            if(size > 100000)
            {
                system("cat /dev/null > /mnt/disk/config/log_file.txt");
            }

            count++;

            if(count >32766)
            {
                count = 0;

                settings.setValue("system/reset",0);
            }

            settings.setValue("system/reset",count);

            QDateTime datetime = QDateTime::currentDateTime();

            memset(date,0,100);

            sprintf(date,"\r\nIt is  %d times:%d m%d d %d:%d:%d reset.",count,datetime.date().month(),
                    datetime.date().day(),datetime.time().hour(),datetime.time().minute(),datetime.time().second());

            fwrite(date, sizeof(date),1, fp);

            fclose(fp);

            break;
        }
        case  1://菜单重启
        {
            FILE *fp = fopen("/mnt/disk/config/log_file.txt","a+");

            if(fp == NULL)
            {
                printf("log_file  file fp open error!\n");
                return;
            }

            char flag[60];

            memset(flag,0,60);

            sprintf(flag,"\r\n It is restarted through the menu\n");

            fwrite(flag, sizeof(flag),1, fp);

            fclose(fp);

            break;
        }
        case  2://主叫
        {
            ictTimes = settings.value("log/ictTimes", "0").toInt();

            if(ictTimes > 32000)
            {
                settings.setValue("log/ictTimes",1);

                ictTimes= 0;

                FILE *fp = fopen("/mnt/disk/config/log_file.txt","a+");

                if(fp == NULL)
                {
                    printf("log_file  file fp open error!\n");
                    return;
                }

                fseek( fp, SEEK_SET,SEEK_END);

                char date[30];

                memset(date,0,30);

                sprintf(date,"\r\n ictTimes over 32000.");

                fwrite(date, sizeof(date),1, fp);

                fclose(fp);
            }
            else
            {
                ictTimes++;

                settings.setValue("log/ictTimes",ictTimes);
            }
            break;
        }
        case  3://被叫
        {
            istTimers = settings.value("log/istTimers", "0").toInt();

            if(istTimers > 32000)
            {
                settings.setValue("log/istTimers",1);

                istTimers=0;

                FILE *fp = fopen("/mnt/disk/config/log_file.txt","a+");

                if(fp == NULL)
                {
                    printf("log_file  file fp open error!\n");
                    return;
                }

                fseek( fp, SEEK_SET,SEEK_END);

                char date[30];

                memset(date,0,30);

                sprintf(date,"\r\n istTimers over 32000.");

                fwrite(date, sizeof(date),1, fp);

                fclose(fp);
            }
            else
            {
                istTimers++;

                settings.setValue("log/istTimers",istTimers);
            }
            break;
        }
        case  4://监控
        {
            mstTimers = settings.value("log/mstTimers", "0").toInt();

            if(mstTimers > 32000)
            {
                settings.setValue("log/mstTimers",1);

                mstTimers=0;

                FILE *fp = fopen("/mnt/disk/config/log_file.txt","a+");

                if(fp == NULL)
                {
                    printf("log_file  file fp open error!\n");
                    return;
                }

                fseek( fp, SEEK_SET,SEEK_END);
                char date[30];

                memset(date,0,30);

                sprintf(date,"\r\n mstTimers over 32000.");

                fwrite(date, sizeof(date),1, fp);

                fclose(fp);
            }
            else
            {
                mstTimers++;

                settings.setValue("log/mstTimers",mstTimers);
            }
            break;
        }
        case  5://安防报警
        {
            Securitycount = settings.value("log/Securitycount", "0").toInt();

            if(Securitycount > 32000)
            {
                settings.setValue("log/Securitycount",1);

                Securitycount=0;

                FILE *fp = fopen("/mnt/disk/config/log_file.txt","a+");

                if(fp == NULL)
                {
                    printf("log_file  file fp open error!\n");
                    return;
                }

                fseek( fp, SEEK_SET,SEEK_END);

                char date[30];

                memset(date,0,30);

                sprintf(date,"\r\n Securitycount over 32000.");

                fwrite(date, sizeof(date),1, fp);

                fclose(fp);
            }
            else
            {
                Securitycount++;

                settings.setValue("log/Securitycount",Securitycount);
            }
            break;
        }
        case  6://刷卡
        {
            cardCount = settings.value("log/cardCount", "0").toInt();

            if(cardCount > 32000)
            {
                settings.setValue("log/cardCount",1);

                cardCount=0;

                FILE *fp = fopen("/mnt/disk/config/log_file.txt","a+");

                if(fp == NULL)
                {
                    printf("log_file  file fp open error!\n");
                    return;
                }

                fseek( fp, SEEK_SET,SEEK_END);

                char date[30];

                memset(date,0,30);

                sprintf(date,"\r\n cardCount over 32000.");

                fwrite(date, sizeof(date),1, fp);

                fclose(fp);
            }
            else
            {
                cardCount++;

                settings.setValue("log/cardCount",cardCount);
            }
            break;
        }
        case  7://开锁
        {
            unLock = settings.value("log/unLock", "0").toInt();

            if(unLock > 32000)
            {
                settings.setValue("log/unLock",1);

                unLock=0;

                FILE *fp = fopen("/mnt/disk/config/log_file.txt","a+");

                if(fp == NULL)
                {
                    printf("log_file  file fp open error!\n");
                    return;
                }

                fseek( fp, SEEK_SET,SEEK_END);

                char date[30];

                memset(date,0,30);

                sprintf(date,"\r\n unLock over 32,000.");

                fwrite(date, sizeof(date),1, fp);

                fclose(fp);
            }
            else
            {
                unLock++;

                settings.setValue("log/unLock",unLock);
            }
            break;
        }
    default:
        break;
    }
}
/*******************************************************************
描述:   udp协议的注册回调函数
参数:   [in]type：  回调类型
        [in]param：  回调参数
返回:   无
********************************************************************/
void udp_msg_callback(UDP_CALLBACK_E type, unsigned long param)
{
    switch (type)
    {
        case UDP_CALLBACK_REMOTE_SET_SECURITY:
        {
            unsigned char state = (uint8_t)param;
            g_pMainDlg->m_pSecurityMainDlg->ReceiveUdpData(state);
            break;
        }
        case UDP_CALLBACK_INFO:
        {
            char *file_name = (char*)param;

            g_pMainDlg->m_pInfoManMainDlg->ReceiveUdpInfo(file_name);
        }
        default:
            break;
    }
}
void addLogFile()
{
    FILE *fp = fopen("/mnt/disk/config/log_file.txt","a+");

    if(fp == NULL)
    {
        printf("log_file  file fp open error!\n");
        return;
    }

    QSettings settings("/mnt/disk/config/config", QSettings::IniFormat);

    int count = 0,size = 0;

    count  = settings.value("system/reset", "0").toInt();

    fseek( fp, SEEK_SET,SEEK_END);

    size = ftell(fp);

    if(size > 100000)
    {
        psystem("cat /dev/null > /mnt/disk/config/log_file.txt");
    }

    count++;

    settings.setValue("system/reset",count);

    QDateTime datetime = QDateTime::currentDateTime();

    char date[55];

    memset(date,0,55);

    sprintf(date,"\r\nReset:%d times:%02d m%d d %d:%d:%d.",count,datetime.date().month(),
            datetime.date().day(),datetime.time().hour(),datetime.time().minute(),datetime.time().second());

    fwrite(date, sizeof(date),1, fp);

    fclose(fp);
}


QString getStyleSheet(QString strLang);
QString getStyleSheet_1280x800(QString strLang);

QTranslator *g_pTranslatorApp;
bool g_bTranslatorFound;
//bool g_bMainDlgInit = 0;

/************************************************************
描述：程序主函数
参数：argc - 程序命令行参数的个数
	 argv - 程序命令行参数
返回：0 - 正常
　　　非0 - 异常
************************************************************/
int main(int argc, char *argv[])
{
    system("/bin/echo 173 > /sys/class/gpio/export");			// 门铃Door_Bell_UNLOCK设为空闲状态
	system("/bin/echo out > /sys/class/gpio/gpio173/direction");
	system("/bin/echo 1 > /sys/class/gpio/gpio173/value");

	system("/bin/echo 193 > /sys/class/gpio/export");			// 初始化为闭锁状态 
	system("/bin/echo out > /sys/class/gpio/gpio193/direction");
	system("/bin/echo 1 > /sys/class/gpio/gpio193/value");

	gen_system_info(argv);							// 启动的时候生成系统信息

#ifdef CORTEXA7
    	render_init();

    	render_exit();
#endif
	//david 2014.5.15
	if(!IsFileEXist("/dev/dsp"))
	{
		psystem("reboot");
	}

	// 为了便于排除问题暂时先屏蔽该代码
	// 如果打开启该功能，必须保证config文件>=500个字节，另外首次启动必须成功以便生成备份文件
	check_and_restore_config_file();					// 根据config文件大小判断文件是否非法

	//addLogFile();
	systemRunLog(0);

//	Init_System_Data_Time(SoftVersion);

	loadUserAddr();

	clean_call_record();

	extern int g_iRingMaxTime;

	g_iRingMaxTime = 45;

	g_iRecordMaxTime = 30;

	g_iDeviceType = IsFileEXist("/dev/YouXianFangQu");

	//stop watchdog
	int watchdog_fd = -1;
	watchdog_fd = open("/dev/watchdog", O_WRONLY);
	if (watchdog_fd != -1)
	{
		write(watchdog_fd, "V", 1);
		close(watchdog_fd);
	}
	//end of stop watchdog

	signal(SIGTERM,sig_handler);
	signal(SIGINT, sig_handler);

	system("/bin/echo  'lock'>/sys/power/wake_lock");

#if LED_PANEL_CTRL == 1
	system("/bin/echo 172 > /sys/class/gpio/export");				// 创建两个led的设备节点
	system("/bin/echo out > /sys/class/gpio/gpio172/direction");
	system("/bin/echo 1 > /sys/class/gpio/gpio172/value");				// 关闭LED的灯

	system("/bin/echo 173 > /sys/class/gpio/export");
	system("/bin/echo out > /sys/class/gpio/gpio173/direction");
	system("/bin/echo 1 > /sys/class/gpio/gpio173/value");				// 关闭LED的灯

//	system("/bin/echo 196 > /sys/class/gpio/export");
//	system("/bin/echo out > /sys/class/gpio/gpio196/direction");
//	system("/bin/echo 1 > /sys/class/gpio/gpio196/value");
//	system("/bin/echo 196 > /sys/class/gpio/unexport");
//
//	system("/bin/echo 197 > /sys/class/gpio/export");
//	system("/bin/echo out > /sys/class/gpio/gpio197/direction");
//	system("/bin/echo 0 > /sys/class/gpio/gpio197/value");
//	system("/bin/echo 197 > /sys/class/gpio/unexport");
#endif
	{
		QSettings settings(CONFIG_FILE, QSettings::IniFormat);
		g_iVideoCapSizeType = VIDEO_SIZE_720P;
		g_enable_tvout = settings.value("system/enable_tvout", "0").toInt();
        }

	Application a(argc, argv);

	g_pScaleSrc = new Kevin::Global;
	g_pScaleSrc->init_rate(QApplication::desktop()->size().width(),QApplication::desktop()->size().height());
	g_pScaleSrc->process_doorbell_functions();					// 根据门铃类型进行初始化工作
	QImage *pAppBackImage;

	pAppBackImage = new QImage;
	if (pAppBackImage->load(g_appBackImagePath))
	{
		printf("set app background\n");
		QImage img2;
		img2 = pAppBackImage->scaled(QApplication::desktop()->size().width(),QApplication::desktop()->size().height());
		QBrush brush(img2);
		QWSServer::setBackground(brush);
		delete pAppBackImage;
		pAppBackImage = NULL;
	}
	else
	{
		printf("open app background failed\n");
		delete pAppBackImage;
		pAppBackImage = NULL;
	}

	QWSServer::setCursorVisible(false);

	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

	QFontDatabase::addApplicationFont("images/keyboard/arial.ttf");
	QFontDatabase::addApplicationFont("lib/fonts/gothic.ttf");
	QFontDatabase::addApplicationFont("lib/fonts/Gurmukhi MN.ttf");
	QFontDatabase::addApplicationFont("lib/fonts/fzltxhkm.ttf");
	QFontDatabase::addApplicationFont("lib/fonts/fzlth.ttf");
	QFontDatabase::addApplicationFont("lib/fonts/TAHOMA_1.ttf");

	QFont font("Gurmukhi MN", 18);//Century Gothic", 18);
	QApplication::setFont(font);

	int ret = 0;

	qRegisterMetaType <Qt::SortOrder> ("Qt::SortOrder");
	qRegisterMetaType <QItemSelection> ("QItemSelection");
	qRegisterMetaType <QDateTime> ("QDateTime");

	QSettings settings(CONFIG_FILE, QSettings::IniFormat);
	QString strLang = settings.value("language/lang", "zh_CN").toString();

	if ( g_pScaleSrc->get_screen_version() == 3 )			// 1280x800 的分辨率
		a.setStyleSheet(getStyleSheet_1280x800(strLang));
	else
		a.setStyleSheet(getStyleSheet(strLang));

	g_pTranslatorApp = new QTranslator;
	g_bTranslatorFound = g_pTranslatorApp->load(":/" + strLang + ".qm");

	if (g_bTranslatorFound)
	{
		a.installTranslator(g_pTranslatorApp);
	}

	MainDialog MainDlg;

	strSoftVersion = argv[0];

    int split_pos = strSoftVersion.indexOf("V");
    strSoftVersion = strSoftVersion.mid(split_pos + 1);
    split_pos = strSoftVersion.indexOf("_");
	if (split_pos != -1)
	{
		strSoftVersion = strSoftVersion.mid(split_pos + 1);
		MainDlg.m_pSysSetMainDlg->m_pVersionDlg->ui->lineEdit_SoftwareVersion->setText(strSoftVersion);
	}

	MainDlg.getClock()->setBackgroundImage(":/images/main/clock.png");
	MainDlg.getCalendar()->Generate(true);

	QSize screenSize = QApplication::desktop()->size();
	g_fb0_width = screenSize.width();
	g_fb0_height = screenSize.height();

	snprintf(configfile_name, PATHMAX, "linphonerc");
	//MainDialog初始化中有IP地址设置，所以必须MainDialog初始化完后再启动lphone
	lphone = linphone_core_init(&linphonec_vtable, configfile_name);

	if (pAppBackImage)
	{
		delete pAppBackImage;
		pAppBackImage = NULL;
	}
	MainDlg.show();

#if LED_PANEL_CTRL == 1
//	system("/bin/echo 196 > /sys/class/gpio/export");
//	system("/bin/echo 0 > /sys/class/gpio/gpio196/value");
//	system("/bin/echo 196 > /sys/class/gpio/unexport");
//
//	system("/bin/echo 197 > /sys/class/gpio/export");
//	system("/bin/echo 1 > /sys/class/gpio/gpio197/value");
//	system("/bin/echo 197 > /sys/class/gpio/unexport");
#endif

    QString str_version;
    QString hw_version = "1.0-20120101";
    QString device_type = "BASIC-TERMINAL";
    QString fw_version = strSoftVersion;
    QString str_protocol = "UDP3.01_TCP2.0";
    QString uw_version = "V2.6";

    settings.setIniCodec(QTextCodec::codecForName("UTF8"));
    hw_version = settings.value("system/systeminfo_hardware_version","").toString();

    str_version = str_protocol + "-" + device_type  + "-" + fw_version \
            + "-" + uw_version  + "-" + hw_version;

    init_protocol(str_version.toLatin1().data(),udp_msg_callback);


	ret = a.exec();

    linphone_core_uninit(lphone);

	if (g_bTranslatorFound)
    {
        a.removeTranslator(g_pTranslatorApp);
	}

    delete g_pTranslatorApp;

    delete g_pScaleSrc;

	return ret;
}
