/*
 * =====================================================================================
 *
 *       Filename:  global.cpp
 *
 *    Description:  Define of the global class 
 *
 *        Version:  1.0
 *        Created:  2014年07月22日 10时51分40秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/user.h>

#include <unistd.h>
#include <cstdio>
#include <cstdlib>

#include "global.h"

#include <QFileInfo>
#include <QSettings>

#define CONFIG_FILE "/mnt/disk/config/config"

extern int g_door_bell_type;				// 二次门铃的类型的全局变量
extern int psystem(const char *cmd);

namespace Kevin
{

const float  Global::RATIO = 0.439;

Global::Global (): m_default_screen_width(800) , m_default_screen_height(480) , m_w_rate(1) , m_h_rate(1) , m_screen_version(1)
{
}

Global::~Global ()
{
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init_rate
 *  Description:  init the rate of screen 
 * =====================================================================================
 */
void Global::init_rate(int w,int h)
{
    if ( w == 800 && h == 480 )
        m_screen_version = 1;
	if ( w == 1024 && h == 600 )
		m_screen_version = 2;
	else if ( w == 1280 && h == 800 )
		m_screen_version = 3;

	m_w_rate = static_cast<double>(w) / m_default_screen_width; 
	m_h_rate = static_cast<double>(h) / m_default_screen_height; 
}

int Global::get_w_value(int v)
{
	return static_cast<int>(v * m_w_rate);
}

int Global::get_h_value(int v)
{
	return static_cast<int>(v * m_h_rate);
}

int Global::system_button_fonts()
{
	switch ( m_screen_version ) 
	{
		case 1: 
            return 19;
			break;

		case 2: 
            return 25;
			break;

		case 3:
            return 27;  // 27
			break;

		default:	
            return 19;
			break;
	}				/* -----  end switch  ----- */
}

int Global::main_maindialog_button_fonts()
{
    switch ( m_screen_version )
    {
        case 1:
            return 20;  // 21
            break;

        case 2:
            return 28;
            break;

        case 3:
            return 37;
            break;

        default:
            return 20;
            break;
    }				/* -----  end switch  ----- */
}

int Global::systemset_device_button_fonts()
{
	switch ( m_screen_version ) 
	{
		case 1: 
			return 23;
			break;

		case 2: 
			return 19;
			break;

		case 3:
			return 23;

		default:	
			return 23;
			break;
	}				/* -----  end switch  ----- */
}

int Global::get_monlist_tableViewAddressList_fonts()
{
    switch ( m_screen_version )
    {
        case 1:
            return 20;
            break;

        case 2:
            return 22;
            break;

        case 3:
            return 26;
            break;

        default:
            return 20;
            break;
    }				/* -----  end switch  ----- */
}

int Global::get_addressbook_tableViewAddressList_fonts()
{
    switch ( m_screen_version )
    {
        case 1:
            return 20;
            break;

        case 2:
            return 22;
            break;

        case 3:
            return 26;
            break;

        default:
            return 20;
            break;
    }				/* -----  end switch  ----- */
}

int Global::photo_maindialog_button_fonts()
{
	switch ( m_screen_version ) 
	{
		case 1: 
			return 20;
			break;

		case 2: 
			return 25;
			break;

		case 3: 
			return 32;
			break;

		default:	
			return 20;
			break;
	}				/* -----  end switch  ----- */
}

int Global::main_maindialog_tableViewNotifier()
{
	switch ( m_screen_version ) 
	{
		case 1: 
			return 20;
			break;

		case 2: 
			return 25;
			break;

		case 3: 
			return 29;
			break;

		default:	
			return 20;
			break;
	}				/* -----  end switch  ----- */
}

void Global::get_maindialog_calendar_geometry(int& x,int& y,int& w,int& h)
{
	switch ( m_screen_version ) 
	{
		case 1: 
            { x = 231; y = 60; w = 210; h = 216; }
			break;

		case 2: 
            { x = 308; y = 53; w = 265; h = 288; }
			break;

		case 3: 
            { x = 392; y = 78; w = 306; h = 360; }
			break;

		default:	
            { x = 231; y = 60; w = 210; h = 216; }
			break;
	}				/* -----  end switch  ----- */
}

void Global::get_maindialog_clock_geometry(int& x,int& y,int& w,int& h)
{
	switch ( m_screen_version ) 
	{
		case 1: 
            { x = 76; y = 88; w = 162; h = 162; }
			break;

		case 2:
            { x = 110; y = 90; w = 200; h = 200; }
			break;

		case 3:
            { x = 135; y = 126; w = 260; h = 255; }
			break;

		default:
            x = 76; y = 88; w = 162; h = 162;
			break;
	}				/* -----  end switch  ----- */
}

void Global::get_info_framepanel_geometry(int& x,int& y,int& w,int& h)
{
	switch ( m_screen_version )
	{
		case 1: 
            x = 34; y = 18; w = 538; h = 450;
			break;

		case 2: 
			x = 38; y = 17; w = 700; h = 580;
			break;

		case 3:
            x = 56; y = 30; w = 858; h = 745;
			break;

		default:
			x = 40; y = 20; w = 550; h = 440;
			break;
	}				/* -----  end switch  ----- */
}

void Global::get_videoshow_geometry(int &x, int &y, int &w, int &h)
{
	switch ( m_screen_version )
	{
		case 1:
            x = 44; y = 77; w = 518; h = 377;
			break;

		case 2:
			x =53; y = 87; w = 670; h = 490;
			break;

		case 3:
            x =70; y = 119; w = 831; h = 645;
			break;

		default:
			x = 75; y = 90; w = 480; h = 345;
			break;
	}				/* -----  end switch  ----- */
}
void Global::get_tabviewinfolist_geometry(int& x,int& y,int& w,int& h)
{
	switch ( m_screen_version )
	{
		case 1: 
            x = 44; y = 77; w = 518; h = 377;
			break;

		case 2: 
			x = 58; y = 97; w = 660; h = 481;
			break;

		case 3:
            x = 80; y = 139; w = 811; h = 611;
			break;

		default:	
			x = 75; y = 90; w = 480; h = 345;
			break;
	}				/* -----  end switch  ----- */
}

void Global::get_info_label_geometry(int& x,int& y,int& w,int& h)
{
	switch ( m_screen_version )
	{
		case 1:
			x = 60; y = 20; w = 511; h = 51;
			break;

		case 2:
			x = 60; y = 25+5; w = 611+19; h = 51;
			break;

		case 3:
            x = 95; y = 40; w = 791; h = 71;
			break;

		default:	
			x = 60; y = 20; w = 511; h = 51;
			break;
	}				/* -----  end switch  ----- */
}
			
void Global::get_info_tabviewinfolist_width(int& x1,int& x2,int& x3)
{
	switch ( m_screen_version )
	{
		case 1: 
            x1 = 174; x2 = 172; x3 = 172;
			break;

		case 2: 
			x1 = 219; x2 = 219; x3 = 218;
			break;

		case 3: 
            x1 = 270; x2 = 270; x3 = 271;
			break;

		default:	
            x1 = 174; x2 = 172; x3 = 172;
			break;
	}				/* -----  end switch  ----- */
}

void Global::get_info_tabviewSecurityList_width(int& x1,int& x2,int& x3,int& x4)
{
	switch ( m_screen_version )
	{
		case 1: 
            x1 = 158+20; x2 = 120-20; x3 = 120; x4 = 120;
			break;

		case 2: 
			x1 = 206; x2 = 150; x3 = 150; x4 = 150;//206=175+25+6
			break;

		case 3:
            x1 = 202; x2 = 203; x3 = 203; x4 = 203;
			break;

		default:	
            x1 = 158; x2 = 120; x3 = 120; x4 = 120;
			break;
	}				/* -----  end switch  ----- */
}

void Global::get_info_tableViewAbnormalList_width(int& x1,int& x2,int& x3,int& x4)
{
	switch ( m_screen_version )
	{
		case 1: 
            x1 = 131; x2 = 129; x3 = 129; x4 = 129;
			break;

		case 2: 
			x1 = 165; x2 = 165; x3 = 165; x4 = 165;
			break;

		case 3:
            x1 = 202; x2 = 203; x3 = 203; x4 = 203;
			break;

		default:	
            x1 = 131; x2 = 129; x3 = 129; x4 = 129;
			break;
	}				/* -----  end switch  ----- */
}
			
int Global::get_messagebox_label_font_size()
{
	switch ( m_screen_version )
	{
		case 1: 
			return 20;
			break;

		case 2: 
			return 20;
			break;

		case 3:
			return 23;
			break;

		default:	
			return 20;
			break;
	}				/* -----  end switch  ----- */
}

int Global::get_voip_ict_label_font_size()
{
	switch ( m_screen_version )
	{
		case 1: 
            return 26;
			break;

		case 2: 
            		return 29;//和E70风格靠近，加大字体 lidh 20141018
			break;
		case 3:
			return 32;
			break;

		default:	
			return 26;
			break;
	}				/* -----  end switch  ----- */
}
			
int Global::get_pwdinfo_label_font_size()
{
	switch ( m_screen_version )
	{
		case 1: 
			return 21;
			break;

		case 2: 
            		return 24; 
			break;
		case 3:
			return 24;
			break;

		default:	
			return 21;
			break;
	}				/* -----  end switch  ----- */
}

void Global::get_photo_tabviewinfolist_width(int& x1,int& x2,int& x3)
{
	switch ( m_screen_version )
	{
		case 1:
            x1 = 174; x2 = 172; x3 = 172;
			break;

		case 2:
			x1 = 220; x2 = 220; x3 = 220;
			break;

		case 3:
            x1 = 270; x2 = 270; x3 = 271;
			break;

		default:
            x1 = 174; x2 = 172; x3 = 172;
			break;
	}				/* -----  end switch  ----- */
}

int Global::get_screen_version()
{
	return m_screen_version;
}

void Global::get_info_tableViewMediaList_width(int& x,int& y,int& w)
{
	switch ( m_screen_version )
	{
		case 1:
            x = 110; y = 210; w = 198;
			break;

		case 2:
			x = 219; y = 219; w = 218;
			break;

		case 3:
            x = 270; y = 270; w = 271;
			break;

		default:
            x = 110; y = 210; w = 198;
			break;
	}				/* -----  end switch  ----- */
}

void Global::get_maindialog_tableViewNotifier_geometry(int& x,int& y,int& w,int& h)
{
	switch ( m_screen_version )
	{
		case 1: 
            x = 441; y = 64; w = 264; h = 210;
			break;

		case 2: 
            x = 574; y = 60; w = 337; h = 267;
			break;

		case 3:
            x = 724; y = 94; w = 426; h = 326;
			break;

		default:
            x = 441; y = 64; w = 264; h = 210;
			break;
	}				/* -----  end switch  ----- */
}

int Global::get_maindialog_tableViewNotifier_lineheight()
{
	switch ( m_screen_version )
	{
		case 1:
            return 34;
			break;

		case 2:
            return 44;
			break;

		case 3:
            return 53;
			break;

		default:
            return 34;
			break;
	}				/* -----  end switch  ----- */
}

void Global::get_maindialog_tableViewNotifier_iconsize(int& x,int& y)
{
	switch ( m_screen_version )
	{
		case 1:
            x = 35; y = 35;
			break;

		case 2:
			x = 45; y = 45;
			break;

		case 3:
            x = 55; y = 45;
			break;

		default:
            x = 35; y = 35;
			break;
	}				/* -----  end switch  ----- */
}

int Global::get_ringvalue_font_size()
{
	switch ( m_screen_version )
	{
		case 1:
			return 21;
			break;

		case 2:
			return 24;  // 25
			break;

		default:
			return 21;
			break;
	}				/* -----  end switch  ----- */
}

int Global::get_sysset_info_font_size()
{
	switch ( m_screen_version )
	{
		case 1:
			return 21;
			break;

		case 2:
			return 28;
			break;

		case 3:
			return 29;
			break;

		default:
			return 21;
			break;
	}				/* -----  end switch  ----- */
}

int Global::get_infowrite_font_size()
{
	switch ( m_screen_version )
	{
		case 1:
			return 21;
			break;

		case 2:
			return 25;
			break;

		case 3:
			return 27;
			break;

		default:
			return 21;
			break;
	}				/* -----  end switch  ----- */
}
			
void Global::get_info_frameinfobox(int& x,int& y,int& w,int& h)
{
	get_info_framepanel_geometry(x,y,w,h);

	switch ( m_screen_version )
	{
		case 1: 
			x = x + 29; y = y + 69; w = w - 57; h = h - 87;
			break;

		case 2: 
			x = x + 29; y = y + 100; w = w - 57; h = h - 150;
			break;

		default:
			x = x + 29; y = y + 69; w = w - 57; h = h - 87;
			break;
	}				/* -----  end switch  ----- */
}

void Global::get_info_labelinfotime(int& x,int& y,int& w,int& h)
{
	get_info_frameinfobox(x,y,w,h);

	switch ( m_screen_version )
	{
		case 1: 
			x = x; y = y; w = 181; h = 41;
			break;

		case 2: 
			x = x; y = y; w = 225; h = 51;
			break;

		default:
			x = x; y = y; w = 181; h = 41;
			break;
	}				/* -----  end switch  ----- */
}

void Global::get_info_labelinfofrom(int& x,int& y,int& w,int& h)
{
	get_info_labelinfotime(x,y,w,h);

	switch ( m_screen_version )
	{
		case 1:
			x = x + w; y = y; w = w - 20; h = 41;
			break;

		case 2:
			x = x + w; y = y; w = w - 40; h = 51;
			break;

		default:
			x = x + w; y = y; w = w - 20; h = 41;
			break;
	}				/* -----  end switch  ----- */
}
			
void Global::get_info_frameattachment(int& x,int& y,int& w,int& h)
{
	get_info_frameinfobox(x,y,w,h);

	switch ( m_screen_version )
	{
		case 1:
			x = x; y = y + h - 101; w = w ; h = 100;
			break;

		case 2:
			x = x; y = y + h - 123; w = w ; h = 125;
			break;

		default:
			x = x; y = y + h - 101; w = w ; h = 100;
			break;
	}				/* -----  end switch  ----- */
}

void Global::get_home_control_labelstatus_geometry(int& x,int& y,int& w,int& h)
{
	switch ( m_screen_version )
	{
		case 1:
			x = 730; y = 60; w = 25 ; h = 25;
			break;

		case 2:
			x = 950; y = 70; w = 25 ; h = 25;
			break;

		default:
			x = 730; y = 60; w = 25 ; h = 25;
			break;
	}				/* -----  end switch  ----- */
}
			
void Global::get_home_control_labelroomname_geometry(int& x,int& y,int& w,int& h)
{
	switch ( m_screen_version )
	{
		case 1:
			x = 70; y = 50; w = 60 ; h = 32;
			break;

		case 2:
			x = 90; y = 70; w = 90 ; h = 40;
			break;

		default:
			x = 70; y = 50; w = 60 ; h = 32;
			break;
	}				/* -----  end switch  ----- */
}
			
void Global::get_home_control_plugin_position(int&x,int&y)
{
	switch ( m_screen_version )
	{
		case 1:
			x = 145; y = 103;
			break;

		case 2:
			x = 280; y = 170;
			break;

		default:
			x = 145; y = 103;
			break;
	}				/* -----  end switch  ----- */
}

void Global::get_voip_tableViewMst_Listwidth(int& w1,int& w2,int& w3,int& w4)
{
	switch ( m_screen_version )
	{
		case 1:
			w1 = 80; w2 = 156; w3 = 65 ; w4 = 175;
			break;

		case 2:
			w1 = 100; w2 = 190; w3 = 60 ; w4 = 210;
			break;

		default:
			w1 = 80; w2 = 156; w3 = 65 ; w4 = 175;
			break;
	}				/* -----  end switch  ----- */
}

int Global::get_passwordset_font_size()
{
	switch ( m_screen_version )
	{
		case 1:
			return 24;
			break;

		case 2:
			return 24; // 30 22
			break;

		case 3:
            return 24;
			break;

		default:
			return 24;
			break;
	}				/* -----  end switch  ----- */
}

int Global::get_timeset_font_size()
{
    switch ( m_screen_version )
    {
        case 1:
            return 22;
            break;

        case 2:
            return 27; // 30 22
            break;

        case 3:
            return 35;
            break;

        default:
            return 22;
            break;
    }				/* -----  end switch  ----- */
}

int Global::get_all_title_lable_font()
{
	switch ( m_screen_version )
	{
		case 1:
			return 26;
			break;

		case 2:
			return 29;
			break;

		case 3:
			return 32;
			break;

		default:
			return 26;
			break;
	}				/* -----  end switch  ----- */
}

void Global::get_calender_fonts_size(int &ymf,int &df,int &wf,int &llf,int &jjf)
{
	switch ( m_screen_version )
	{
		case 1:
            ymf = 28; df = 85; wf = 21; llf = 21; jjf = 21;
			break;

		case 2:
            ymf = 32; df = 105; wf = 25; llf = 25; jjf = 25;
			break;

		case 3:
            ymf = 42; df = 130; wf = 32; llf = 32; jjf = 32;
			break;

		default:
            ymf = 25; df = 78; wf = 19; llf = 19; jjf = 19;
			break;
	}				/* -----  end switch  ----- */
}

int Global::get_input_method_pinyin_offset()
{
	switch ( m_screen_version )
	{
		case 1:
			return 53;
			break;

		case 2:
			return 63;
			break;

		case 3:
			return 93;
			break;

		default:
			return 53;
			break;
	}				/* -----  end switch  ----- */
}
			
int Global::get_input_method_pinyin_line_offset()
{
	switch ( m_screen_version )
	{
		case 1:
			return 80;
			break;

		case 2:
			return 103;
			break;

		case 3:
			return 130;
			break;

		default:
			return 80;
			break;
	}				/* -----  end switch  ----- */
}

/************************************************************
描述：获取表格行高度
参数：
返回：无
************************************************************/
int Global::get_public_tableviewlist_row_height()
{
	switch ( m_screen_version )
	{
		case 1:
			return 34;
			break;

		case 2:
			return 34;
			break;

		case 3:
			return 52;
			break;

		default:
			return 34;
			break;
	}				/* -----  end switch  ----- */
}

/************************************************************
描述：配置文件备份函数，当config文件发生修改的时候，
      将文件/mnt/disk/config/config备份到/mnt/config
参数：
返回：无
************************************************************/
void Global::Backup_Config_File()
{
	psystem("cp -p /mnt/disk/config/config /mnt/config");	// 进行文件备份
	sync();
	printf ( "A20 backup config file to /mnt/config\n" );
}

/************************************************************
描述：打开二次门铃电源
参数： flag:true  打开电源  false：关闭电源
返回：
************************************************************/
void Global::doorbell_power(bool flag)
{
	QFileInfo file_gpio194("/sys/class/gpio/gpio194");
	bool exist= file_gpio194.isDir();

	if (!exist)
		system("echo '194'>/sys/class/gpio/export");
	system("echo 'out'>/sys/class/gpio/gpio194/direction");//设为输出

	if ( flag == true )
		system("echo '1'>/sys/class/gpio/gpio194/value");
	else
		system("echo '0'>/sys/class/gpio/gpio194/value");
}

/************************************************************
描述：二次门铃开关锁
参数： flag:true  打开锁  false：关闭锁
返回：
************************************************************/
void Global::doorbell_lock(bool flag)
{
	QFileInfo file_gpio193("/sys/class/gpio/gpio193");
	bool exist= file_gpio193.isDir();

	if (!exist)
		system("echo '193'>/sys/class/gpio/export");
	system("echo 'out'>/sys/class/gpio/gpio193/direction");//设为输出

	if ( flag == true )
		system("echo '0'>/sys/class/gpio/gpio193/value");
	else
		system("echo '1'>/sys/class/gpio/gpio193/value");

}

/************************************************************
描述：处理二次门铃的事件
      程序启动的时候调用，根据门铃类型参数进行初始化
参数：
返回：
************************************************************/
void Global::process_doorbell_functions()
{
	int iSpecialDefense;
	QSettings settings(CONFIG_FILE, QSettings::IniFormat);
    //iSpecialDefense  = settings.value("alert_param/special_defense","0").toInt();
    iSpecialDefense  = settings.value("alert_param/doorbell_type","-1").toInt();

    if(iSpecialDefense == -1)
    {
        iSpecialDefense  = settings.value("alert_param/special_defense","0").toInt();
    }

	g_door_bell_type = iSpecialDefense;

	switch ( iSpecialDefense ) 
	{
		case 0:
			doorbell_power(false);
			doorbell_lock(false);
			break;

		case 1:
			doorbell_power(true);
			doorbell_lock(false);
			break;

		case 2:
			doorbell_power(false);
			doorbell_lock(false);
			break;

		case 3:
			doorbell_power(true);
			doorbell_lock(false);
			break;

		default:
			break;
	}				/* -----  end switch  ----- */
}

void Global::Write_fm1188(char* cmd)
{
	int fd = open("/sys/kernel/debug/fm1188", O_RDWR, 0);
	if (fd<0)
		printf("open fm1188 fail\n");

	write(fd,cmd,sizeof(cmd));
	close(fd);
}

/************************************************************
描述：设置二次门铃中通话的音量
参数： value 输入的音量
返回：
************************************************************/
void Global::set_doorbell_ring_value(int value)
{
	char *cmd = new char[100];

	sprintf(cmd,"1");
	Write_fm1188(cmd);

	sprintf(cmd,"0xff01 %x",value);
	Write_fm1188(cmd);

	sprintf(cmd,"0");
	Write_fm1188(cmd);

	delete[] cmd;
	cmd = NULL;
}

}
