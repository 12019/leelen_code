/*
 * =====================================================================================
 *
 *       Filename:  global.h
 *
 *    Description:  For calculating of the rate of screen plugin position 
 *
 *        Version:  1.0
 *        Created:  2014年07月22日 10时49分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  GLOBAL_INC
#define  GLOBAL_INC

#define KEVINDEBUG

namespace Kevin
{
	class Global
	{
		public:
			Global ();                             /* constructor */
			~Global ();                            /* constructor */

			/*-----------------------------------------------------------------------------
			 *  Most of the time used these functions is enough
			 *-----------------------------------------------------------------------------*/
			void init_rate(int w,int h);
			int get_w_value(int);
			int get_h_value(int);
			
			/*-----------------------------------------------------------------------------
			 *  special function sets
			 *-----------------------------------------------------------------------------*/
			int get_screen_version();

            int system_button_fonts();
			int main_maindialog_button_fonts(); 
			int systemset_device_button_fonts();
            int get_monlist_tableViewAddressList_fonts();
            int get_addressbook_tableViewAddressList_fonts();

			int photo_maindialog_button_fonts(); 
			int main_maindialog_tableViewNotifier();

			void get_maindialog_calendar_geometry(int&,int&,int&,int&);
			void get_maindialog_clock_geometry(int&,int&,int&,int&);

			void get_maindialog_tableViewNotifier_geometry(int&,int&,int&,int&);
			int get_maindialog_tableViewNotifier_lineheight();
			void get_maindialog_tableViewNotifier_iconsize(int&,int&);

            		void get_videoshow_geometry(int&,int&,int&,int&);
			void get_tabviewinfolist_geometry(int&,int&,int&,int&);
			void get_info_framepanel_geometry(int&,int&,int&,int&);
			void get_info_label_geometry(int&,int&,int&,int&);
			void get_info_tabviewinfolist_width(int&,int&,int&);

			void get_info_tabviewSecurityList_width(int&,int&,int&,int&);

			void get_info_tableViewAbnormalList_width(int&,int&,int&,int&);

			int get_messagebox_label_font_size();

			void get_photo_tabviewinfolist_width(int&,int&,int&);

			void get_info_tableViewMediaList_width(int& ,int& ,int& );

			int get_voip_ict_label_font_size();

			int get_pwdinfo_label_font_size();

			int get_ringvalue_font_size();

			int get_sysset_info_font_size();

			int get_infowrite_font_size();

			void get_info_frameinfobox(int&,int&,int&,int&);
			void get_info_labelinfotime(int&,int&,int&,int&);
			void get_info_labelinfofrom(int&,int&,int&,int&);
			void get_info_frameattachment(int&,int&,int&,int&);


			void get_info_texteditinfobody(int&,int&,int&,int&);

			void get_home_control_labelstatus_geometry(int&,int&,int&,int&);
			void get_home_control_labelroomname_geometry(int&,int&,int&,int&);

			void get_home_control_plugin_position(int&x,int&y);

			void get_voip_tableViewMst_Listwidth(int&,int&,int&,int&);

			int get_passwordset_font_size();

            int get_timeset_font_size();

			void get_calender_fonts_size(int &,int &,int &,int &,int &);

			int get_input_method_pinyin_offset();

			int get_input_method_pinyin_line_offset();

            		int get_public_tableviewlist_row_height();

			int get_all_title_lable_font();

			/*-----------------------------------------------------------------------------
			 *  Backup config file
			 *-----------------------------------------------------------------------------*/
			void Backup_Config_File();

			static const float RATIO;			// 播放音量的衰减系数

			/*-----------------------------------------------------------------------------
			 *  Doorbell 二次门铃操作
			 *-----------------------------------------------------------------------------*/
			void doorbell_power(bool flag);
			void doorbell_lock(bool flag);
			void process_doorbell_functions();
			void set_doorbell_ring_value(int value);
			void Write_fm1188(char* cmd);
		protected:

		private:
			const int m_default_screen_width;
			const int m_default_screen_height;
			double    m_w_rate;
			double    m_h_rate;

			/*-----------------------------------------------------------------------------
			 *  1:	800x480 	(default)
			 *  2:	1024x600
			 *  3:  1280x800
			 *-----------------------------------------------------------------------------*/
			int       m_screen_version;

	}; /* -----  end of class Global  ----- */
}

#define RESCALESCREEN(myuio)	myuio->setGeometry(g_pScaleSrc->get_w_value(myuio->geometry().x()), g_pScaleSrc->get_h_value(myuio->geometry().y()), g_pScaleSrc->get_w_value(myuio->geometry().width()), g_pScaleSrc->get_h_value(myuio->geometry().height()));

#define CALL_RECORD_COUNT 	   45			// 在xml文件中记录的最大信息条目数
#define CALL_RECORD_AVI_COUNT 	   10			// 在磁盘中记录的最多AVI文件数量

#define SECURITY_RECORD_COUNT  	   45			// 报警记录条目数
#define SECURITY_AVI_RECORD_COUNT  10			// 报警记录的视频条目数

#define LIMIT_CONFIG_LENGTH        500                  // config文件最低有效长度

#endif   /* ----- #ifndef GLOBAL_INC  ----- */
