#include <cstdlib>
#include <cstdio>
#include <QString>

/************************************************************
  该文件为生成styleSheet主程序
************************************************************/

/************************************************************
  系统背景图片路径
************************************************************/
extern char g_appBackImagePath[];

#include <global.h>
extern Kevin::Global  *g_pScaleSrc;
/************************************************************
描述：生成系统界面的styleSheet
参数：strLang - "en":语言为英文
			   其它: 语言为其它语言
返回：生成的styleSheet
************************************************************/
QString getStyleSheet(QString strLang)
{
    Q_UNUSED(strLang);

    QString strPath;
    QString strStyleSheet;
    QString strNumFontSize;
    QString strNumFontFocusSize;
    QString strNumFontPressSize;
    QString strFontSize = "16";
    QString strBtnColor = "#582F00";
    QString strBtnFocusColor = "#582F00";//#C00000";
    QString strBtnPressColor = "#582F00";//#C00000";

    QString strBtnFontSize;
    strBtnFontSize.sprintf("%d",g_pScaleSrc->system_button_fonts());

    //QDialog
    /*strStyleSheet = "  QDialog {"
                "      background-image: url(" + QString(g_appBackImagePath) + ");"
                "  }";
    */

    strStyleSheet = "  QDialog#MainDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    /*strStyleSheet += "  QDialog#VoipMainDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#InfoManMainDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#InfoManDetailDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#InfoWriteDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#InfoManDetailDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#RingFileSetDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#SecurityMainDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#DatetimeSetDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#DeviceSetMainDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#LanguageSetDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#PasswordSetDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#IstDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#AddDefenseDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#SysSetPasswordDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#PhotoUDiskDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#MediaPlayerDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#IctRcvHookDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#InfoManViewAttachmentDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#RingAndVolumeDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#SecurityParamSetDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#SecurityAbnormalDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#SecurityPasswordDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#SecurityPlayerDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#SecurityAlertDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#MonDialog{"
            "      background-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";
    */

    //QPushButton //默认按键大小和颜色style在这里统一设置，其他按键需要特别设置的在按键处单独设置  add by lidh 20141101
    strStyleSheet += " QPushButton {"
            "      background-image: url(:/images/main/num-1.png);"
            "      border-radius: 7px;"
            "      color: " + strBtnColor + ";"
            "      font: " + strBtnFontSize + "px;"
            "	   background-repeat: no-repeat;"
            "      background-position: center;"
            "  }";

    strStyleSheet += " QPushButton:focus {"
            "      background-image: url(:/images/main/num-2.png);"
            "      border-radius: 7px;"
            "      color: " + strBtnFocusColor + ";"
            "      font: " + strBtnFontSize + "px;"
            "	   background-repeat: no-repeat;"
            "      background-position: center;"
            "  }";
    strStyleSheet += " QPushButton:pressed {"
            "      background-image: url(:/images/main/num-2.png);"
            "      border-radius: 7px;"
            "      color: " + strBtnPressColor + ";"
            "      font: " + strBtnFontSize + "px;"
            "	   background-repeat: no-repeat;"
            "      background-position: center;"
            "  }";
    strStyleSheet += " QPushButton:disabled {"
            "      border-radius: 7px;"
            "      color: " + strBtnColor + ";"
            "      font: " + strBtnFontSize + "px;"
            "	   background-repeat: no-repeat;"
            "      background-position: center;"
            "  }";
    strStyleSheet += " QPushButton:checked {"
            "      border-radius: 7px;"
            "      color: #604010;"
            "      font: " + strBtnFontSize + "px;"
            "	   background-repeat: no-repeat;"
            "      background-position: center;"
            "  }";
    strStyleSheet += " QPushButton:flat {"
            "      border-radius: 7px;"
            "      color: #604010;"
            "      font: " + strBtnFontSize + "px;"
            "	   background-repeat: no-repeat;"
            "      background-position: center;"
            "  }";

    //QLabel
    strStyleSheet += "  QLabel {"
            "      color: #582F00;"
            "      font: 20px;"
            "      border: 0px solid #C0C0C0;"
            "	   font-family: \"Gurmukhi MN\";" //\"Century Gothic\";"
            "  }";

    //QLineEdit
    strStyleSheet += "  QLineEdit {"
            "      border-radius: 7px;"
            "      font: 20px;"
            "	   color: #582F00;"
            "      border: 5px solid #E0E0E0;"
            "  }";

    //输入法默认按键属性在这里统一设置  add by lidh 20141125
    strStyleSheet += "  QToolButton {"
            "      border-image: url(:/images/keyboard/anniu1-1.png);"
            "      border-radius: 4px;"
            "      color: #604010;"
            "      font: 38px;"
            "  }";
    strStyleSheet += "  QToolButton:pressed {"
            "      border-image: url(:/images/keyboard/anniu1-2.png);"
            "      border-radius: 4px;"
            "      color: #604010;"
            "      font: 38px;"
            "  }";
    strStyleSheet += "  QToolButton:checked {"
            "      border-image: url(:/images/keyboard/anniu1-2.png);"
            "      border-radius: 4px;"
            "      color: #604010;"
            "      font: 38px;"
            "  }";

    //framePanel
    strStyleSheet += "  QFrame#framePanel {"
            "     background-image: url(:/images/main/panel.png);"
            "     border: 0px;"
            "  }";


    /************************************************************
    描述：main界面style
    ************************************************************/
    QString cf;

    int ymf,df,wf,llf,jjf;
    g_pScaleSrc->get_calender_fonts_size(ymf,df,wf,llf,jjf);

    cf.sprintf("%d",ymf);
    strStyleSheet += "QLabel#labelYearMonth {"
            "    font: "+cf+"px bold;"
            "    font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "    color: #FFFFFF;"
#endif
#ifdef STYLE_YELLOW
            "    color: #000000;"
#endif
            "}";

    cf.sprintf("%d",df);
    strStyleSheet += "QLabel#labelDay {"
            "    font: "+cf+"px bold;"
            "    font-family: \"Tahoma\";"
#ifdef STYLE_BLUE
            "    color: #FFFFFF;"
#endif
#ifdef STYLE_YELLOW
            "    color: #000000;"
#endif
            "}";

    cf.sprintf("%d",wf);
    strStyleSheet += "QLabel#labelWeek {"
            "    font: "+cf+"px bold;"
            "    font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "    color: #FFFFFF;"
#endif
#ifdef STYLE_YELLOW
            "    color: #000000;"
#endif
            "}";

    cf.sprintf("%d",llf);
    strStyleSheet += "QLabel#labelNongli {"
            "    font: "+cf+"px bold;"
            "    font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "    color: #FFFFFF;"
#endif
#ifdef STYLE_YELLOW
            "    color: #000000;"
#endif
            "}";

    cf.sprintf("%d",jjf);
    strStyleSheet += "QLabel#labelJieriJieqi {"
            "    font: "+cf+"px bold;"
            "    font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "    color: #FFFFFF;"
#endif
#ifdef STYLE_YELLOW
            "    color: #000000;"
#endif
            "}";

    QString s;
    s.sprintf("%d",g_pScaleSrc->main_maindialog_tableViewNotifier());

    strStyleSheet += "  QTableView#tableViewNotifier {"
            "      background-image: url(:/images/main/paperbox.png);"
            "      border-top: 1px;"
            "      border-bottom: 1px;"
            "      border-left: 1px;"
            "      border-right: 1px;"
            "      color: #604010;"
            "      font: " + s + "px bold;"
            "  }";

    strStyleSheet += "  QTableView#tableViewNotifier::item {"
            "      border: 0;"
            "      border-top: 20px;"
            "  }";

    QString strmaindialogBtnFontSize;
    strmaindialogBtnFontSize.sprintf("%d",g_pScaleSrc->main_maindialog_button_fonts());
    strStyleSheet += " QPushButton#pushButtonVoip {"
            "      background-image: url(:/images/main/voip-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonVoip:focus {"
            "      background-image: url(:/images/main/voip-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #FF0000 ;"
//  	    "      font: bold;"
#endif
            "	}";
    strStyleSheet += " QPushButton#pushButtonVoip:pressed {"
            "      background-image: url(:/images/main/voip-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";

    strStyleSheet += " QPushButton#pushButtonTerminalHelp {"
            "      background-image: url(:/images/main/terminal_help-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonTerminalHelp:focus {"
            "      background-image: url(:/images/main/terminal_help-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #FF0000 ;"
//  	    "      font: bold;"
#endif
            "	}";
    strStyleSheet += " QPushButton#pushButtonTerminalHelp:pressed {"
            "      background-image: url(:/images/main/terminal_help-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";

    strStyleSheet += " QPushButton#pushButtonIntercomMan {"
            "      background-image: url(:/images/main/intercom_man-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonIntercomMan:focus {"
            "      background-image: url(:/images/main/intercom_man-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #FF0000 ;"
//  	    "      font: bold;"
#endif
            "	}";
    strStyleSheet += " QPushButton#pushButtonIntercomMan:pressed {"
            "      background-image: url(:/images/main/intercom_man-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";

    strStyleSheet += " QPushButton#pushButtonCallCenter {"
            "      background-image: url(:/images/main/call_center-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenter:focus {"
            "      background-image: url(:/images/main/call_center-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #FF0000 ;"
//  	    "      font: bold;"
#endif
            "	}";
    strStyleSheet += " QPushButton#pushButtonCallCenter:pressed {"
            "      background-image: url(:/images/main/call_center-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";

    strStyleSheet += " QPushButton#pushButtonInterFreeSetting {"
            "      background-image: url(:/images/main/interfree_setting-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonInterFreeSetting:focus {"
            "      background-image: url(:/images/main/interfree_setting-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #FF0000 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonInterFreeSetting:pressed {"
            "      background-image: url(:/images/main/interfree_setting-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";

    strStyleSheet += " QPushButton#pushButtonSystemSetting {"
            "      background-image: url(:/images/main/system_setting-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonSystemSetting:focus {"
            "      background-image: url(:/images/main/system_setting-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #FF0000 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonSystemSetting:pressed {"
            "      background-image: url(:/images/main/system_setting-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";

    strStyleSheet += " QPushButton#pushButtonVideoMon {"
            "      background-image: url(:/images/main/video_mon-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonVideoMon:focus {"
            "      background-image: url(:/images/main/video_mon-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #FF0000 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonVideoMon:pressed {"
            "      background-image: url(:/images/main/video_mon-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";

    strStyleSheet += " QPushButton#pushButtonInfoMan {"
            "      background-image: url(:/images/main/info_man-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoMan:focus {"
            "      background-image: url(:/images/main/info_man-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #FF0000 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoMan:pressed {"
            "      background-image: url(:/images/main/info_man-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";

    strStyleSheet += " QPushButton#pushButtonSecurityMan {"
            "      background-image: url(:/images/main/security_man-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecurityMan:focus {"
            "      background-image: url(:/images/main/security_man-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #FF0000 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecurityMan:pressed {"
            "      background-image: url(:/images/main/security_man-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";

    strStyleSheet += " QPushButton#pushButtonCallElevator {"
            "      background-image: url(:/images/main/call_elevator-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevator:focus {"
            "      background-image: url(:/images/main/call_elevator-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #FF0000 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevator:pressed {"
            "      background-image: url(:/images/main/call_elevator-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";

    strStyleSheet += " QPushButton#pushButtonPhoto {"
            "      background-image: url(:/images/main/photo-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhoto:focus {"
            "      background-image: url(:/images/main/photo-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #FF0000 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhoto:pressed {"
            "      background-image: url(:/images/main/photo-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";

    strStyleSheet += " QPushButton#pushButtonHomeControl {"
            "      background-image: url(:/images/main/home_control-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonHomeControl:focus {"
            "      background-image: url(:/images/main/home_control-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #FF0000 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonHomeControl:pressed {"
            "      background-image: url(:/images/main/home_control-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";

    strStyleSheet += " QPushButton#pushButtonSendImg {"
            "      background-image: url(:/images/main/sendImg-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonSendImg:focus {"
            "      background-image: url(:/images/main/sendImg-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #FF0000 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonSendImg:pressed {"
            "      background-image: url(:/images/main/sendImg-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";

    strStyleSheet += " QPushButton#pushButtonOther {"
            "      background-image: url(:/images/main/other-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonOther:focus {"
            "      background-image: url(:/images/main/other-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #FF0000 ;"
//  	    "      font: bold;"
#endif
            "  }";
    strStyleSheet += " QPushButton#pushButtonOther:pressed {"
            "      background-image: url(:/images/main/other-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
#ifdef STYLE_BLUE
            "      color: #FFFFFF ;"
            "      font: bold;"
#endif
#ifdef STYLE_YELLOW
            "      color: #604010 ;"
//  	    "      font: bold;"
#endif
            "  }";


    /************************************************************
    描述：common/mymessagebox界面style
    ************************************************************/
    QString ft;
    ft.sprintf("%d",g_pScaleSrc->get_messagebox_label_font_size());
    strPath = "images/common/";
    //MyMessageBox
    strStyleSheet += "  QDialog#MyMessageBox {"
            "      border-image: url(:/" + strPath + "messagebox.png) 47 87 20 87 repeat;" //25 50 20 50 repeat;"//24 59 10 59;"
            "      border-top: 47px transparent;"
            "      border-bottom: 20px transparent;"
            "	   border-right: 87px transparent;"
            "	   border-left: 87px transparent;"
            "      background-image: url();"
            "  }";

    //labelMyMessageBox
    strStyleSheet += "  QLabel#labelMyMessageBox {"
            "      font: " + ft + "px;"
            "      border: 0px;"// solid #C0C0C0;"
            "  }";

    //pushButtonMyMessageBoxOK
    strStyleSheet += " QPushButton#pushButtonMyMessageBoxOK {"
            "      background-image: url(:/" + strPath + "ok-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMyMessageBoxOK:focus {"
            "      background-image: url(:/" + strPath + "ok-1.png);"
            "	}";
    strStyleSheet += " QPushButton#pushButtonMyMessageBoxOK:pressed {"
            "      background-image: url(:/" + strPath + "ok-2.png);"
            "  }";

    //pushButtonMyMessageBoxCancel
    strStyleSheet += " QPushButton#pushButtonMyMessageBoxCancel {"
            "      background-image: url(:/" + strPath + "cancel-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMyMessageBoxCancel:focus {"
            "      background-image: url(:/" + strPath + "cancel-1.png);"
            "	}";
    strStyleSheet += " QPushButton#pushButtonMyMessageBoxCancel:pressed {"
            "      background-image: url(:/" + strPath + "cancel-2.png);"
            "  }";

    /************************************************************
    描述：info_man/Infomanmaindialog界面style
    ************************************************************/
    strPath = "images/info_man/";
    //pushButtonInfoManMainWrite
    strStyleSheet += " QPushButton#pushButtonInfoManMainWrite {"
            "      background-image: url(:/" + strPath + "write-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainWrite:focus {"
            "      background-image: url(:/" + strPath + "write-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainWrite:pressed {"
            "      background-image: url(:/" + strPath + "write-2.png);"
            "  }";

    //pushButtonInfoManMainDel
    strStyleSheet += " QPushButton#pushButtonInfoManMainDel {"
            "      background-image: url(:/" + strPath + "delete-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainDel:focus {"
            "      background-image: url(:/" + strPath + "delete-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainDel:pressed {"
            "      background-image: url(:/" + strPath + "delete-2.png);"
            "  }";

    //pushButtonInfoManMainDetail
    strStyleSheet += " QPushButton#pushButtonInfoManMainDetail {"
            "      background-image: url(:/" + strPath + "detail-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainDetail:focus {"
            "      background-image: url(:/" + strPath + "detail-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainDetail:pressed {"
            "      background-image: url(:/" + strPath + "detail-2.png);"
            "  }";

    //pushButtonInfoManMainDelAll
    strStyleSheet += " QPushButton#pushButtonInfoManMainDelAll {"
            "      background-image: url(:/" + strPath + "deleteall-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainDelAll:focus {"
            "      background-image: url(:/" + strPath + "deleteall-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainDelAll:pressed {"
            "      background-image: url(:/" + strPath + "deleteall-2.png);"
            "  }";

    //pushButtonInfoManMainPrev
    strStyleSheet += " QPushButton#pushButtonInfoManMainPrev {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainPrev:focus {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainPrev:pressed {"
            "      background-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonInfoManMainNext
    strStyleSheet += " QPushButton#pushButtonInfoManMainNext {"
            "      background-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainNext:focus {"
            "      background-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainNext:pressed {"
            "      background-image: url(:/" + strPath + "next-2.png);"
            "  }";

    //pushButtonInfoManMainReturn
    strStyleSheet += " QPushButton#pushButtonInfoManMainReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：info_man/Infowritedialog界面style
    ************************************************************/
    strPath = "images/info_man/";
    //pushButtonInfoWriteAddress
    strStyleSheet += " QPushButton#pushButtonInfoWriteAddress {"
            "      background-image: url(:/" + strPath + "address-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoWriteAddress:focus {"
            "      background-image: url(:/" + strPath + "address-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoWriteAddress:pressed {"
            "      background-image: url(:/" + strPath + "address-2.png);"
            "  }";

    //pushButtonInfoWriteSend
    strStyleSheet += " QPushButton#pushButtonInfoWriteSend {"
            "      background-image: url(:/" + strPath + "send-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoWriteSend:focus {"
            "      background-image: url(:/" + strPath + "send-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoWriteSend:pressed {"
            "      background-image: url(:/" + strPath + "send-2.png);"
            "  }";

    //pushButtonInfoWriteReturn
    strStyleSheet += " QPushButton#pushButtonInfoWriteReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoWriteReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoWriteReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：security/securitymaindialog界面style
    ************************************************************/
    strPath = "images/security/";
    //pushButtonSecMainDel
    strStyleSheet += " QPushButton#pushButtonSecMainDel {"
            "      background-image: url(:/" + strPath + "delete-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainDel:focus {"
            "      background-image: url(:/" + strPath + "delete-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainDel:pressed {"
            "      background-image: url(:/" + strPath + "delete-2.png);"
            "  }";

    //pushButtonSecMainDetail
    strStyleSheet += " QPushButton#pushButtonSecMainDetail {"
            "      background-image: url(:/" + strPath + "detail-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainDetail:focus {"
            "      background-image: url(:/" + strPath + "detail-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainDetail:pressed {"
            "      background-image: url(:/" + strPath + "detail-2.png);"
            "  }";

    //pushButtonSecMainDelAll
    strStyleSheet += " QPushButton#pushButtonSecMainDelAll {"
            "      background-image: url(:/" + strPath + "deleteall-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainDelAll:focus {"
            "      background-image: url(:/" + strPath + "deleteall-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainDelAll:pressed {"
            "      background-image: url(:/" + strPath + "deleteall-2.png);"
            "  }";

    //pushButtonSecMainSetArm
    strStyleSheet += " QPushButton#pushButtonSecMainSetArm {"
            "      background-image: url(:/" + strPath + "setarm-1.png);"
            "      border-radius: 7px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainSetArm:focus {"
            "      background-image: url(:/" + strPath + "setarm-1.png);"
            "      border-radius: 7px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainSetArm:pressed {"
            "      background-image: url(:/" + strPath + "setarm-2.png);"
            "      border-radius: 7px;"
            "  }";

    //pushButtonSecMainReturn
    strStyleSheet += " QPushButton#pushButtonSecMainReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonSecMainPrev
    strStyleSheet += " QPushButton#pushButtonSecMainPrev {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainPrev:focus {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainPrev:pressed {"
            "      background-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonSecMainNext
    strStyleSheet += " QPushButton#pushButtonSecMainNext {"
            "      background-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainNext:focus {"
            "      background-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainNext:pressed {"
            "      background-image: url(:/" + strPath + "next-2.png);"
            "  }";

    /************************************************************
    描述：security/securityabnormaldialog界面style
    ************************************************************/
    strPath = "images/security/";
    //pushButtonSecAbnormalHome
    strStyleSheet += " QPushButton#pushButtonSecAbnormalHome {"
            "      background-image: url(:/" + strPath + "home-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalHome:focus {"
            "      background-image: url(:/" + strPath + "home-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalHome:pressed {"
            "      background-image: url(:/" + strPath + "home-2.png);"
            "  }";

    //pushButtonSecAbnormalOut
    strStyleSheet += " QPushButton#pushButtonSecAbnormalOut {"
            "      background-image: url(:/" + strPath + "out-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalOut:focus {"
            "      background-image: url(:/" + strPath + "out-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalOut:pressed {"
            "      background-image: url(:/" + strPath + "out-2.png);"
            "  }";

    //pushButtonSecAbnormalPrev
    strStyleSheet += " QPushButton#pushButtonSecAbnormalPrev {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalPrev:focus {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalPrev:pressed {"
            "      background-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonSecAbnormalNext
    strStyleSheet += " QPushButton#pushButtonSecAbnormalNext {"
            "      background-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalNext:focus {"
            "      background-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalNext:pressed {"
            "      background-image: url(:/" + strPath + "next-2.png);"
            "  }";

    //pushButtonSecAbnormalReturn(返回键)
    strStyleSheet += " QPushButton#pushButtonSecAbnormalReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonSecAbnormalDisArm
    strStyleSheet += " QPushButton#pushButtonSecAbnormalDisArm {"
            "      background-image: url(:/" + strPath + "disarm-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalDisArm:focus {"
            "      background-image: url(:/" + strPath + "disarm-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalDisArm:pressed {"
            "      background-image: url(:/" + strPath + "disarm-2.png);"
            "  }";

    /************************************************************
    描述：security/securitypasswddialog界面style
    ************************************************************/
    if (g_pScaleSrc->get_screen_version() == 1) {
            strNumFontSize = "35";
            strNumFontFocusSize = "35";
            strNumFontPressSize = "35";
    }
    else if (g_pScaleSrc->get_screen_version() == 2) {
        strNumFontSize = "42";
        strNumFontFocusSize = "42";
        strNumFontPressSize = "42";
    }
    else if (g_pScaleSrc->get_screen_version() == 3) {
        strNumFontSize = "50";
        strNumFontFocusSize = "50";
        strNumFontPressSize = "50";
    }

    strPath = "images/security/";

    //pushButtonSecPwdBack
    strStyleSheet += " QPushButton#pushButtonSecPwdBack {"
            "      background-image: url(:/" + strPath + "back-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdBack:focus {"
            "      background-image: url(:/" + strPath + "back-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPasswdBack:pressed {"
            "      background-image: url(:/" + strPath + "back-2.png);"
            "  }";

    //pushButtonSecPwdOK
    strStyleSheet += " QPushButton#pushButtonSecPwdOK {"
            "      background-image: url(:/" + strPath + "ok-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdOK:focus {"
            "      background-image: url(:/" + strPath + "ok-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdOK:pressed {"
            "      background-image: url(:/" + strPath + "ok-2.png);"
            "  }";

    //pushButtonSecPwdReturn
    strStyleSheet += " QPushButton#pushButtonSecPwdReturn {"
            "      background-image: url(:/" + strPath + "return2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdReturn:focus {"
            "      background-image: url(:/" + strPath + "return2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdReturn:pressed {"
            "      background-image: url(:/" + strPath + "return2-2.png);"
            "  }";
    //pushButtonSecPwdClear
    strStyleSheet += " QPushButton#pushButtonSecPwdClear {"
            "      background-image: url(:/" + strPath + "clear-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdClear:focus {"
            "      background-image: url(:/" + strPath + "clear-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdClear:pressed {"
            "      background-image: url(:/" + strPath + "clear-2.png);"
            "  }";

    //pushButtonSecPwdNum
    strStyleSheet += " QPushButton#pushButtonSecPwdNum0 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum0:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum0:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum1 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum1:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum1:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum2 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum2:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum2:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum3 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum3:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum3:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum4 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum4:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum4:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum5 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum5:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum5:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum6 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum6:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum6:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum7 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum7:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum7:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonSecPwdNum8 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum8:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum8:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonSecPwdNum9 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum9:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum9:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdDot {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdDot:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdDot:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdShop {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdShop:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdShop:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";

    /************************************************************
    描述：voip/Mondialog界面style
    ************************************************************/
    strPath = "images/voip/";
    //pushButtonMonList
    strStyleSheet += " QPushButton#pushButtonMonList {"
            "      background-image: url(:/" + strPath + "detail2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonList:focus {"
            "      background-image: url(:/" + strPath + "detail2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonList:pressed {"
            "      background-image: url(:/" + strPath + "detail2-2.png);"
            "  }";

    //pushButtonMonOpen
    strStyleSheet += " QPushButton#pushButtonMonOpen {"
            "      background-image: url(:/" + strPath + "open2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonOpen:focus {"
            "      background-image: url(:/" + strPath + "open2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonOpen:pressed {"
            "      background-image: url(:/" + strPath + "open2-2.png);"
            "  }";

    //pushButtonMonHangUp
    strStyleSheet += " QPushButton#pushButtonMonHangUp {"
            "      background-image: url(:/" + strPath + "hangup2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonHangUp:focus {"
            "      background-image: url(:/" + strPath + "hangup2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonHangUp:pressed {"
            "      background-image: url(:/" + strPath + "hangup2-2.png);"
            "  }";

    //pushButtonMonDial
    strStyleSheet += " QPushButton#pushButtonMonDial {"
            "      background-image: url(:/" + strPath + "connect2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonDial:focus {"
            "      background-image: url(:/" + strPath + "connect2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonDial:pressed {"
            "      background-image: url(:/" + strPath + "connect2-2.png);"
            "  }";

    //pushButtonMonReturn
    strStyleSheet += " QPushButton#pushButtonMonReturn {"
            "      background-image: url(:/" + strPath + "return2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonReturn:focus {"
            "      background-image: url(:/" + strPath + "return2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonReturn:pressed {"
            "      background-image: url(:/" + strPath + "return2-2.png);"
            "  }";

    /************************************************************
    描述：intercom_man/mediamandialog界面style
    ************************************************************/
    strPath = "images/intercom_man/";
    strStyleSheet += " QPushButton#pushButtonMediaManDelAll {"
            "      background-image: url(:/" + strPath + "deleteall-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManDelAll:focus {"
            "      background-image: url(:/" + strPath + "deleteall-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManDelAll:pressed {"
            "      background-image: url(:/" + strPath + "deleteall-2.png);"
            "  }";

    //pushButtonMediaManDetail
    strStyleSheet += " QPushButton#pushButtonMediaManDetail {"
            "      background-image: url(:/" + strPath + "detail-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManDetail:focus{"
            "      background-image: url(:/" + strPath + "detail-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManDetail:pressed {"
            "      background-image: url(:/" + strPath + "detail-2.png);"
            "  }";

    //pushButtonMediaManDel
    strStyleSheet += " QPushButton#pushButtonMediaManDel {"
            "      background-image: url(:/" + strPath + "delete-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManDel:focus {"
            "      background-image: url(:/" + strPath + "delete-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManDel:pressed {"
            "      background-image: url(:/" + strPath + "delete-2.png);"
            "  }";

    //pushButtonMediaManPrev
    strStyleSheet += " QPushButton#pushButtonMediaManPrev {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManPrev:focus {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManPrev:pressed {"
            "      background-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonMediaManNext
    strStyleSheet += " QPushButton#pushButtonMediaManNext {"
            "      background-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManNext:focus {"
            "      background-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManNext:pressed {"
            "      background-image: url(:/" + strPath + "next-2.png);"
            "  }";

    //pushButtonMediaManReturn
    strStyleSheet += " QPushButton#pushButtonMediaManReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：call_center/callcenterdialog界面style
    ************************************************************/
    strPath = "images/call_center/";
    //pushButtonCallCenterAnswer
    strStyleSheet += " QPushButton#pushButtonCallCenterAnswer {"
            "      background-image: url(:/" + strPath + "answer-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterAnswer:focus {"
            "      background-image: url(:/" + strPath + "answer-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterAnswer:pressed {"
            "      background-image: url(:/" + strPath + "answer-2.png);"
            "  }";

    //pushButtonCallCenterHangUp
    strStyleSheet += " QPushButton#pushButtonCallCenterHangUp {"
            "      background-image: url(:/" + strPath + "hangup-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterHangUp:focus {"
            "      background-image: url(:/" + strPath + "hangup-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterHangUp:pressed {"
            "      background-image: url(:/" + strPath + "hangup-2.png);"
            "  }";

    //pushButtonCallCenterVideoOpen
    strStyleSheet += " QPushButton#pushButtonCallCenterVideoOpen {"
            "      background-image: url(:/" + strPath + "videoopen-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterVideoOpen:focus {"
            "      background-image: url(:/" + strPath + "videoopen-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterVideoOpen:pressed {"
            "      background-image: url(:/" + strPath + "videoopen-2.png);"
            "  }";

    //pushButtonCallCenterVideoClose
    strStyleSheet += " QPushButton#pushButtonCallCenterVideoClose {"
            "      background-image: url(:/" + strPath + "videoclose-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterVideoClose:focus {"
            "      background-image: url(:/" + strPath + "videoclose-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterVideoClose:pressed {"
            "      background-image: url(:/" + strPath + "videoclose-2.png);"
            "  }";

    //pushButtonCallCenterDial
    strStyleSheet += " QPushButton#pushButtonCallCenterDial {"
            "      background-image: url(:/" + strPath + "dial-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterDial:focus {"
            "      background-image: url(:/" + strPath + "dial-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterDial:pressed {"
            "      background-image: url(:/" + strPath + "dial-2.png);"
            "  }";

    //pushButtonCallCenterRecord
    strStyleSheet += " QPushButton#pushButtonCallCenterRecord {"
            "      background-image: url(:/" + strPath + "record-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterRecord:focus {"
            "      background-image: url(:/" + strPath + "record-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterRecord:pressed {"
            "      background-image: url(:/" + strPath + "record-2.png);"
            "  }";

    //pushButtonCallCenterReturn
    strStyleSheet += " QPushButton#pushButtonCallCenterReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：photo/photomaindialog界面style
    ************************************************************/
    strPath = "images/photo/";
    //pushButtonPhotoMainNext
    strStyleSheet += " QPushButton#pushButtonPhotoMainNext {"
            "      background-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainNext:focus {"
            "      background-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainNext:pressed {"
            "      background-image: url(:/" + strPath + "next-2.png);"
            "  }";

    //pushButtonPhotoMainPrev
    strStyleSheet += " QPushButton#pushButtonPhotoMainPrev {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainPrev:focus {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainPrev:pressed {"
            "      background-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonPhotoMainImport
    strStyleSheet += " QPushButton#pushButtonPhotoMainImport {"
            "      background-image: url(:/" + strPath + "import-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainImport:focus {"
            "      background-image: url(:/" + strPath + "import-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainImport:pressed {"
            "      background-image: url(:/" + strPath + "import-2.png);"
            "  }";

    //pushButtonPhotoMainExport
    strStyleSheet += " QPushButton#pushButtonPhotoMainExport {"
            "      background-image: url(:/" + strPath + "export-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainExport:focus {"
            "      background-image: url(:/" + strPath + "export-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainExport:pressed {"
            "      background-image: url(:/" + strPath + "export-2.png);"
            "  }";

    //pushButtonPhotoMainDel
    strStyleSheet += " QPushButton#pushButtonPhotoMainDel {"
            "      background-image: url(:/" + strPath + "delete-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainDel:focus {"
            "      background-image: url(:/" + strPath + "delete-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainDel:pressed {"
            "      background-image: url(:/" + strPath + "delete-2.png);"
            "  }";

    //pushButtonPhotoMainCheck
    strStyleSheet += " QPushButton#pushButtonPhotoMainCheck {"
            "      background-image: url(:/" + strPath + "check-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainCheck:focus {"
            "      background-image: url(:/" + strPath + "check-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainCheck:pressed {"
            "      background-image: url(:/" + strPath + "check-2.png);"
            "  }";

    //pushButtonPhotoMainReturn
    strStyleSheet += " QPushButton#pushButtonPhotoMainReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：photo/photoudiskdialog界面style
    ************************************************************/
    strPath = "images/photo/";
    //pushButtonPhotoUDiskNext
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskNext {"
            "      background-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskNext:focus {"
            "      background-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskNext:pressed {"
            "      background-image: url(:/" + strPath + "next-2.png);"
            "  }";

    //pushButtonPhotoUDiskPrev
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskPrev {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskPrev:focus {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskPrev:pressed {"
            "      background-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonPhotoUDiskImport
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskImport {"
            "      background-image: url(:/" + strPath + "import-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskImport:focus {"
            "      background-image: url(:/" + strPath + "import-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskImport:pressed {"
            "      background-image: url(:/" + strPath + "import-2.png);"
            "  }";

    //pushButtonPhotoUDiskCheck
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskCheck {"
            "      background-image: url(:/" + strPath + "check-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskCheck:focus {"
            "      background-image: url(:/" + strPath + "check-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskCheck:pressed {"
            "      background-image: url(:/" + strPath + "check-2.png);"
            "  }";

    //pushButtonPhotoUDiskReturn
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：system_setting/syssetpassworddialog界面style
    ************************************************************/
    if (g_pScaleSrc->get_screen_version() == 1) {
        strNumFontSize = "35";
        strNumFontFocusSize = "35";
        strNumFontPressSize = "35";
    }
    else if (g_pScaleSrc->get_screen_version() == 2) {
        strNumFontSize = "42";
        strNumFontFocusSize = "42";
        strNumFontPressSize = "42";
    }
    else if (g_pScaleSrc->get_screen_version() == 3) {
        strNumFontSize = "50";
        strNumFontFocusSize = "50";
        strNumFontPressSize = "50";
    }

    strPath = "images/system_setting/";
    //pushButtonSysPwdBack
    strStyleSheet += " QPushButton#pushButtonSysPwdBack {"
            "      background-image: url(:/" + strPath + "back-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdBack:focus {"
            "      background-image: url(:/" + strPath + "back-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdBack:pressed {"
            "      background-image: url(:/" + strPath + "back-2.png);"
            "  }";

    //pushButtonSysPwdClear
    strStyleSheet += " QPushButton#pushButtonSysPwdClear {"
            "      background-image: url(:/" + strPath + "clear-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdClear:focus {"
            "      background-image: url(:/" + strPath + "clear-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdClear:pressed {"
            "      background-image: url(:/" + strPath + "clear-2.png);"
            "  }";

    //pushButtonSysPwdReturn
    strStyleSheet += " QPushButton#pushButtonSysPwdReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonSysPwdOK
    strStyleSheet += " QPushButton#pushButtonSysPwdOK {"
            "      background-image: url(:/" + strPath + "ok-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdOK:focus {"
            "      background-image: url(:/" + strPath + "ok-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdOK:pressed {"
            "      background-image: url(:/" + strPath + "ok-2.png);"
            "  }";

    //pushButtonSysPwdNum
    strStyleSheet += " QPushButton#pushButtonSysPwdNum0 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum0:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum0:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum1 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum1:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum1:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum2 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum2:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum2:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum3 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum3:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum3:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum4 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum4:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum4:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum5 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum5:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum5:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum6 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum6:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum6:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum7 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum7:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum7:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonSysPwdNum8 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum8:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum8:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonSysPwdNum9 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum9:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum9:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdDot {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdDot:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdDot:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdShop {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdShop:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdShop:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";

    /************************************************************
    描述：system_setting/password/passwordsetdialog界面style
    ************************************************************/
    if (g_pScaleSrc->get_screen_version() == 1) {
        strNumFontSize = "35";
        strNumFontFocusSize = "35";
        strNumFontPressSize = "35";
    }
    else if (g_pScaleSrc->get_screen_version() == 2) {
        strNumFontSize = "42";
        strNumFontFocusSize = "42";
        strNumFontPressSize = "42";
    }
    else if (g_pScaleSrc->get_screen_version() == 3) {
        strNumFontSize = "50";
        strNumFontFocusSize = "50";
        strNumFontPressSize = "50";
    }

    strPath = "images/system_setting/";
    //pushButtonPwdSetBack
    strStyleSheet += " QPushButton#pushButtonPwdSetBack {"
            "      background-image: url(:/" + strPath + "back2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetBack:focus {"
            "      background-image: url(:/" + strPath + "back2-2.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetBack:pressed {"
            "      background-image: url(:/" + strPath + "back2-2.png);"
            "  }";

    //pushButtonPwdSetSave
    strStyleSheet += " QPushButton#pushButtonPwdSetSave {"
            "      background-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetSave:focus {"
            "      background-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetSave:pressed {"
            "      background-image: url(:/" + strPath + "save-2.png);"
            "  }";

    //pushButtonPwdSetReturn
    strStyleSheet += " QPushButton#pushButtonPwdSetReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonPwdSetClear
    strStyleSheet += " QPushButton#pushButtonPwdSetClear {"
            "      background-image: url(:/" + strPath + "clear-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetClear:focus {"
            "      background-image: url(:/" + strPath + "clear-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetClear:pressed {"
            "      background-image: url(:/" + strPath + "clear-2.png);"
            "  }";

    //pushButtonPwdSetNum
    strStyleSheet += " QPushButton#pushButtonPwdSetNum0 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum0:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum0:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum1 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum1:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum1:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum2 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum2:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum2:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum3 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum3:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum3:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum4 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum4:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum4:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum5 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum5:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum5:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum6 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum6:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum6:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum7 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum7:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum7:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonPwdSetNum8 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum8:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum8:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonPwdSetNum9 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum9:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum9:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetDot {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetDot:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetDot:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetShop {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetShop:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetShop:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";

    /************************************************************
    描述：system_setting/language/languagesetdialog界面style
    ************************************************************/
    QString strLagSetFont;
    QString strLagBtnColor= "#adacac";
    QString strLagBtnFocusColor= "#582f00";
    strLagSetFont.sprintf("%d",26);

    strPath = "images/system_setting/";
    //pushButtonLagSetEn
    strStyleSheet += " QPushButton#pushButtonLagSetEn {"
            "      background-image: url(:/" + strPath + "language-1.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnColor + ";"
            "  }";
    strStyleSheet += " QPushButton#pushButtonLagSetEn:focus {"
            "      background-image: url(:/" + strPath + "language-2.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnFocusColor + ";"
            "  }";
    strStyleSheet += " QPushButton#pushButtonLagSetEn:pressed {"
            "      background-image: url(:/" + strPath + "language-2.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnFocusColor + ";"
            "  }";

    //pushButtonLagSetSimpCh
    strStyleSheet += " QPushButton#pushButtonLagSetSimpCh {"
            "      background-image: url(:/" + strPath + "language-1.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnColor + ";"
            "  }";
    strStyleSheet += " QPushButton#pushButtonLagSetSimpCh:focus {"
            "      background-image: url(:/" + strPath + "language-2.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnFocusColor + ";"
            "  }";
    strStyleSheet += " QPushButton#pushButtonLagSetSimpCh:pressed {"
            "      background-image: url(:/" + strPath + "language-2.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnFocusColor + ";"
            "  }";

    //pushButtonLagSetTraCh
    strStyleSheet += " QPushButton#pushButtonLagSetTraCh {"
            "      background-image: url(:/" + strPath + "language-1.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnColor + ";"
            "  }";
    strStyleSheet += " QPushButton#pushButtonLagSetTraCh:focus {"
            "      background-image: url(:/" + strPath + "language-2.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnFocusColor + ";"
            "  }";
    strStyleSheet += " QPushButton#pushButtonLagSetTraCh:pressed {"
            "      background-image: url(:/" + strPath + "language-2.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnFocusColor + ";"
            "  }";

    //pushButtonLagSetReturn
    strStyleSheet += " QPushButton#pushButtonLagSetReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "      font: " + strBtnFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonLagSetReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "      font: " + strBtnFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonLagSetReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "      font: " + strBtnFontSize + "px;"
            "  }";

    /************************************************************
    描述：system_setting/datetime/datetimesetdialog界面style
    ************************************************************/
    if (g_pScaleSrc->get_screen_version() == 1) {
            strNumFontSize = "35";
            strNumFontFocusSize = "35";
            strNumFontPressSize = "35";
    }
    else if (g_pScaleSrc->get_screen_version() == 2) {
        strNumFontSize = "42";
        strNumFontFocusSize = "42";
        strNumFontPressSize = "42";
    }
    else if (g_pScaleSrc->get_screen_version() == 3) {
        strNumFontSize = "50";
        strNumFontFocusSize = "50";
        strNumFontPressSize = "50";
    }
    strPath = "images/system_setting/";

    QString sf;
    sf.sprintf("%d",g_pScaleSrc->get_timeset_font_size());
    //labelDateTimeSetYear
    strStyleSheet += "  QLabel#labelDateTimeSetYear {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";

    //labelDateTimeSetMonth
    strStyleSheet += "  QLabel#labelDateTimeSetMonth {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";

    //labelDateTimeSetDay
    strStyleSheet += "  QLabel#labelDateTimeSetDay {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";

    //labelDateTimeSetHour
    strStyleSheet += "  QLabel#labelDateTimeSetHour {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";

    //labelDateTimeSetMinute
    strStyleSheet += "  QLabel#labelDateTimeSetMinute {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";

    //labelDateTimeSetSecond
    strStyleSheet += "  QLabel#labelDateTimeSetSecond {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";

    //lineEditYear
    strStyleSheet += "  QLineEdit#lineEditYear {"
        "      font: " + sf + "px;"
        "	   border: 0px;"
        "      background-color: rgba(0, 0, 0, 0);"
        "  }";

    //lineEditMonth
    strStyleSheet += "  QLineEdit#lineEditMonth {"
        "      font: " + sf + "px;"
        "	   border: 0px;"
        "      background-color: rgba(0, 0, 0, 0);"
        "  }";

    //lineEditDay
    strStyleSheet += "  QLineEdit#lineEditDay {"
        "      font: " + sf + "px;"
        "	   border: 0px;"
        "      background-color: rgba(0, 0, 0, 0);"
        "  }";

    //lineEditHour
    strStyleSheet += "  QLineEdit#lineEditHour {"
        "      font: " + sf + "px;"
        "	   border: 0px;"
        "      background-color: rgba(0, 0, 0, 0);"
        "  }";

    //lineEditMinute
    strStyleSheet += "  QLineEdit#lineEditMinute {"
        "      font: " + sf + "px;"
        "	   border: 0px;"
        "      background-color: rgba(0, 0, 0, 0);"
        "  }";

    //lineEditSecond
    strStyleSheet += "  QLineEdit#lineEditSecond {"
        "      font: " + sf + "px;"
        "	   border: 0px;"
        "      background-color: rgba(0, 0, 0, 0);"
        "  }";

    //pushButtonDateTimeSetTime
    strStyleSheet += " QPushButton#pushButtonDateTimeSetTime {"
            "      background-image: url(:/" + strPath + "time-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetTime:focus {"
            "      background-image: url(:/" + strPath + "time-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetTime:pressed {"
            "      background-image: url(:/" + strPath + "time-2.png);"
            "  }";

    //pushButtonDateTimeSetBack
    strStyleSheet += " QPushButton#pushButtonDateTimeSetBack {"
            "      background-image: url(:/" + strPath + "back2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetBack:focus {"
            "      background-image: url(:/" + strPath + "back2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetBack:pressed {"
            "      background-image: url(:/" + strPath + "back2-2.png);"
            "  }";

    //pushButtonDateTimeSetClear
    strStyleSheet += " QPushButton#pushButtonDateTimeSetClear {"
            "      background-image: url(:/" + strPath + "clear-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetClear:focus {"
            "      background-image: url(:/" + strPath + "clear-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetClear:pressed {"
            "      background-image: url(:/" + strPath + "clear-2.png);"
            "  }";

    //pushButtonDateTimeSetSave
    strStyleSheet += " QPushButton#pushButtonDateTimeSetSave {"
            "      background-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetSave:focus {"
            "      background-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetSave:pressed {"
            "      background-image: url(:/" + strPath + "save-2.png);"
            "  }";

    //pushButtonDateTimeSetReturn
    strStyleSheet += " QPushButton#pushButtonDateTimeSetReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonDateTimeSetNum
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum0 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum0:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum0:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum1 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum1:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum1:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum2 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum2:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum2:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum3 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum3:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum3:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum4 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum4:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum4:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum5 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum5:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum5:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum6 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum6:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum6:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum7 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum7:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum7:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum8 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum8:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum8:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum9 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum9:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum9:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetDot {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetDot:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetDot:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetShop {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetShop:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetShop:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";

    /************************************************************
    描述：system_setting/device/adddefense界面style
    ************************************************************/
    QString adf;
    adf.sprintf("%d",g_pScaleSrc->get_ringvalue_font_size());

    strPath = "images/system_setting/";
    //pushButtonAddDefCalibrate
    strStyleSheet += " QPushButton#pushButtonAddDefCalibrate {"
            "      background-image: url(:/" + strPath + "calibrate2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefCalibrate:focus {"
            "      background-image: url(:/" + strPath + "calibrate2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefCalibrate:pressed {"
            "      background-image: url(:/" + strPath + "calibrate2-2.png);"
            "  }";

    //pushButtonAddDefReset
    strStyleSheet += " QPushButton#pushButtonAddDefReset {"
            "      background-image: url(:/" + strPath + "reset2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefReset:focus {"
            "      background-image: url(:/" + strPath + "reset2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefReset:pressed {"
            "      background-image: url(:/" + strPath + "reset2-2.png);"
            "  }";

    //pushButtonAddDefReboot
    strStyleSheet += " QPushButton#pushButtonAddDefReboot {"
            "      background-image: url(:/" + strPath + "reboot2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefReboot:focus {"
            "      background-image: url(:/" + strPath + "reboot2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefReboot:pressed {"
            "      background-image: url(:/" + strPath + "reboot2-2.png);"
            "  }";

    //pushButtonAddDefNext
    strStyleSheet += " QPushButton#pushButtonAddDefNext {"
            "      background-image: url(:/" + strPath + "next2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefNext:focus {"
            "      background-image: url(:/" + strPath + "next2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefNext:pressed {"
            "      background-image: url(:/" + strPath + "next2-2.png);"
            "  }";

    //pushButtonAddDefPrev
    strStyleSheet += " QPushButton#pushButtonAddDefPrev {"
            "      background-image: url(:/" + strPath + "prev2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefPrev:focus {"
            "      background-image: url(:/" + strPath + "prev2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefPrev:pressed {"
            "      background-image: url(:/" + strPath + "prev2-2.png);"
            "  }";

    //pushButtonAddDefDel
    strStyleSheet += " QPushButton#pushButtonAddDefDel {"
            "      background-image: url(:/" + strPath + "delete2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefDel:focus {"
            "      background-image: url(:/" + strPath + "delete2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefDel:pressed {"
            "      background-image: url(:/" + strPath + "delete2-2.png);"
            "  }";

    //pushButtonAddDefSave
    strStyleSheet += " QPushButton#pushButtonAddDefSave {"
            "      background-image: url(:/" + strPath + "save2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefSave:focus {"
            "      background-image: url(:/" + strPath + "save2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefSave:pressed {"
            "      background-image: url(:/" + strPath + "save2-2.png);"
            "  }";

    //pushButtonAddDefReturn
    strStyleSheet += " QPushButton#pushButtonAddDefReturn {"
            "      background-image: url(:/" + strPath + "return4-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefReturn:focus {"
            "      background-image: url(:/" + strPath + "return4-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefReturn:pressed {"
            "      background-image: url(:/" + strPath + "return4-2.png);"
            "  }";

    //pushButtonAddDefType
    strStyleSheet += " QPushButton#pushButtonAddDefType {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefType:focus {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefType:pressed {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";

    //pushButtonAddDefChannel
    strStyleSheet += " QPushButton#pushButtonAddDefChannel {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefChannel:focus {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefChannel:pressed {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";

    //pushButtonAddDefProperty
    strStyleSheet += " QPushButton#pushButtonAddDefProperty {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefProperty:focus {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefProperty:pressed {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";

    //pushButtonAddDefSubType
    strStyleSheet += " QPushButton#pushButtonAddDefSubType {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefSubType:focus {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefSubType:pressed {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";

    //pushButtonAddDefDelayAlarm
    strStyleSheet += " QPushButton#pushButtonAddDefDelayAlarm {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefDelayAlarm:focus {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefDelayAlarm:pressed {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonAddDefBypass {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefBypass:focus {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefBypass:pressed {"
            "      background-image: url();"
            "      font: " + adf + "px;"
            "  }";

    /************************************************************
    描述：common/addressbookdialog界面style
    ************************************************************/
    strPath = "images/common/";
    //pushButtonAddressBookCancel
    strStyleSheet += " QPushButton#pushButtonAddressBookCancel {"
            "      background-image: url(:/" + strPath + "cancel-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddressBookCancel:focus {"
            "      background-image: url(:/" + strPath + "cancel-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddressBookCancel:pressed {"
            "      background-image: url(:/" + strPath + "cancel-2.png);"
            "  }";

    /************************************************************
    描述：voip/ictdialog界面style
    ************************************************************/
    if (g_pScaleSrc->get_screen_version() == 1) {
        strNumFontSize = "35";
        strNumFontFocusSize = "35";
        strNumFontPressSize = "35";
    }
    else if (g_pScaleSrc->get_screen_version() == 2) {
        strNumFontSize = "42";
        strNumFontFocusSize = "42";
        strNumFontPressSize = "42";
    }
    else if (g_pScaleSrc->get_screen_version() == 3) {
        strNumFontSize = "50";
        strNumFontFocusSize = "50";
        strNumFontPressSize = "50";
    }

    strPath = "images/voip/";
    //pushButtonIctDial
    strStyleSheet += " QPushButton#pushButtonIctDial {"
            "      background-image: url(:/" + strPath + "dial-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctDial:focus {"
            "      background-image: url(:/" + strPath + "dial-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctDial:pressed {"
            "      background-image: url(:/" + strPath + "dial-2.png);"
            "  }";

    //pushButtonIctHangUp
    strStyleSheet += " QPushButton#pushButtonIctHangUp {"
        "      background-image: url(:/" + strPath + "hangup-1.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonIctHangUp:focus {"
        "      background-image: url(:/" + strPath + "hangup-1.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonIctHangUp:pressed {"
        "      background-image: url(:/" + strPath + "hangup-2.png);"
        "  }";

    //pushButtonIctAddressBook
    strStyleSheet += " QPushButton#pushButtonIctAddressBook {"
            "      background-image: url(:/" + strPath + "addressbook-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctAddressBook:focus {"
            "      background-image: url(:/" + strPath + "addressbook-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctAddressBook:pressed {"
            "      background-image: url(:/" + strPath + "addressbook-2.png);"
            "  }";

    //pushButtonIctVideoClose
    strStyleSheet += " QPushButton#pushButtonIctVideoClose {"
            "      background-image: url(:/" + strPath + "videoclose-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctVideoClose:focus {"
            "      background-image: url(:/" + strPath + "videoclose-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctVideoClose:pressed {"
            "      background-image: url(:/" + strPath + "videoclose-2.png);"
            "  }";

    //pushButtonIctVideoOpen
    strStyleSheet += " QPushButton#pushButtonIctVideoOpen {"
            "      background-image: url(:/" + strPath + "videoopen-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctVideoOpen:focus {"
            "      background-image: url(:/" + strPath + "videoopen-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctVideoOpen:pressed {"
            "      background-image: url(:/" + strPath + "videoopen-2.png);"
            "  }";

    //pushButtonIctRecord
    strStyleSheet += " QPushButton#pushButtonIctRecord {"
            "      background-image: url(:/" + strPath + "record-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRecord:focus {"
            "      background-image: url(:/" + strPath + "record-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRecord:pressed {"
            "      background-image: url(:/" + strPath + "record-2.png);"
            "  }";

    //pushButtonIctReturn
    strStyleSheet += " QPushButton#pushButtonIctReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonIctBack
    strStyleSheet += " QPushButton#pushButtonIctBack {"
            "      background-image: url(:/" + strPath + "back-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctBack:focus {"
            "      background-image: url(:/" + strPath + "back-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctBack:pressed {"
            "      background-image: url(:/" + strPath + "back-2.png);"
            "  }";

    //pushButtonIctNum
    strStyleSheet += " QPushButton#pushButtonIctNum0 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum0:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum0:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum1 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum1:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum1:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum2 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum2:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum2:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum3 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum3:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum3:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum4 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum4:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum4:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum5 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum5:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum5:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum6 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum6:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum6:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum7 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum7:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum7:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonIctNum8 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum8:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum8:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonIctNum9 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum9:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum9:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctDot {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctDot:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctDot:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctShop {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctShop:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctShop:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";

    /************************************************************
    描述：voip/ictrcvhookdialog界面style
    ************************************************************/
    strPath = "images/voip/";

    //pushButtonIctRcvHookHangUp
    strStyleSheet += " QPushButton#pushButtonIctRcvHookHangUp {"
            "      background-image: url(:/" + strPath + "hangup2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookHangUp:focus {"
            "      background-image: url(:/" + strPath + "hangup2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookHangUp:pressed {"
            "      background-image: url(:/" + strPath + "hangup2-2.png);"
            "  }";

    //pushButtonIctRcvHookVideoClose
    strStyleSheet += " QPushButton#pushButtonIctRcvHookVideoClose {"
            "      background-image: url(:/" + strPath + "videoclose2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookVideoClose:focus {"
            "      background-image: url(:/" + strPath + "videoclose2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookVideoClose:pressed {"
            "      background-image: url(:/" + strPath + "videoclose2-2.png);"
            "  }";

    //pushButtonIctRcvHookOpen
    strStyleSheet += " QPushButton#pushButtonIctRcvHookOpen {"
            "      background-image: url(:/" + strPath + "open2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookOpen:focus {"
            "      background-image: url(:/" + strPath + "open2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookOpen:pressed {"
            "      background-image: url(:/" + strPath + "open2-2.png);"
            "  }";

    //pushButtonIctRcvHookReturn
    strStyleSheet += " QPushButton#pushButtonIctRcvHookReturn {"
            "      background-image: url(:/" + strPath + "return2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookReturn:focus {"
            "      background-image: url(:/" + strPath + "return2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookReturn:pressed {"
            "      background-image: url(:/" + strPath + "return2-2.png);"
            "  }";

    //pushButtonIctRcvHookVideoOpen
    strStyleSheet += " QPushButton#pushButtonIctRcvHookVideoOpen {"
            "      background-image: url(:/" + strPath + "videoopen2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookVideoOpen:focus {"
            "      background-image: url(:/" + strPath + "videoopen2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookVideoOpen:pressed {"
            "      background-image: url(:/" + strPath + "videoopen2-2.png);"
            "  }";

    /************************************************************
    描述：voip/istdialog界面style
    ************************************************************/
    strPath = "images/voip/";
    //pushButtonIstAnswer
    strStyleSheet += " QPushButton#pushButtonIstAnswer {"
            "      background-image: url(:/" + strPath + "answer-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstAnswer:focus {"
            "      background-image: url(:/" + strPath + "answer-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstAnswer:pressed {"
            "      background-image: url(:/" + strPath + "answer-2.png);"
            "  }";

    //pushButtonIstVideoClose
    strStyleSheet += " QPushButton#pushButtonIstVideoClose {"
            "      background-image: url(:/" + strPath + "videoclose2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstVideoClose:focus {"
            "      background-image: url(:/" + strPath + "videoclose2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstVideoClose:pressed {"
            "      background-image: url(:/" + strPath + "videoclose2-2.png);"
            "  }";

    //pushButtonIstVideoOpen
    strStyleSheet += " QPushButton#pushButtonIstVideoOpen {"
            "      background-image: url(:/" + strPath + "videoopen2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstVideoOpen:focus {"
            "      background-image: url(:/" + strPath + "videoopen2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstVideoOpen:pressed {"
            "      background-image: url(:/" + strPath + "videoopen2-2.png);"
            "  }";

    //pushButtonIstOpen
    strStyleSheet += " QPushButton#pushButtonIstOpen {"
            "      background-image: url(:/" + strPath + "open2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstOpen:focus {"
            "      background-image: url(:/" + strPath + "open2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstOpen:pressed {"
            "      background-image: url(:/" + strPath + "open2-2.png);"
            "  }";

    //pushButtonIstHangUp
    strStyleSheet += " QPushButton#pushButtonIstHangUp {"
            "      background-image: url(:/" + strPath + "hangup2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstHangUp:focus {"
            "      background-image: url(:/" + strPath + "hangup2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstHangUp:pressed {"
            "      background-image: url(:/" + strPath + "hangup2-2.png);"
            "  }";

    /************************************************************
    描述：voip/monlistdialog界面style
    ************************************************************/
    strPath = "images/voip/";
    //pushButtonMonListCancel
    strStyleSheet += " QPushButton#pushButtonMonListCancel {"
            "      background-image: url(:/" + strPath + "cancel-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonListCancel:focus {"
            "      background-image: url(:/" + strPath + "cancel-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonListCancel:pressed {"
            "      background-image: url(:/" + strPath + "cancel-2.png);"
            "  }";

    /************************************************************
    描述：info_man/infomandetaildialog界面style
    ************************************************************/
    strPath = "images/info_man/";
    //pushButtonInfoManDetailReply
    strStyleSheet += " QPushButton#pushButtonInfoManDetailReply {"
            "      background-image: url(:/" + strPath + "reply-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailReply:focus {"
            "      background-image: url(:/" + strPath + "reply-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailReply:pressed {"
            "      background-image: url(:/" + strPath + "reply-2.png);"
            "  }";

    //pushButtonInfoManDetailAttach
    strStyleSheet += " QPushButton#pushButtonInfoManDetailAttach {"
            "      background-image: url(:/" + strPath + "attach-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailAttach:focus {"
            "      background-image: url(:/" + strPath + "attach-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailAttach:pressed {"
            "      background-image: url(:/" + strPath + "attach-2.png);"
            "  }";

    //pushButtonInfoManDetailReturn
    strStyleSheet += " QPushButton#pushButtonInfoManDetailReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonInfoManDetailPrev
    strStyleSheet += " QPushButton#pushButtonInfoManDetailPrev {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailPrev:focus {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailPrev:pressed {"
            "      background-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonInfoManDetailNext
    strStyleSheet += " QPushButton#pushButtonInfoManDetailNext {"
            "      background-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailNext:focus {"
            "      background-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailNext:pressed {"
            "      background-image: url(:/" + strPath + "next-2.png);"
            "  }";

    /************************************************************
    描述：info_man/infomanviewattachmentdialog界面style
    ************************************************************/
    strPath = "images/info_man/";
    //pushButtonInfoManViewZoomIn
    strStyleSheet += " QPushButton#pushButtonInfoManViewZoomIn {"
            "      background-image: url(:/" + strPath + "zoomin-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewZoomIn:focus {"
            "      background-image: url(:/" + strPath + "zoomin-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewZoomIn:pressed {"
            "      background-image: url(:/" + strPath + "zoomin-2.png);"
            "  }";

    //pushButtonInfoManViewZoomOut
    strStyleSheet += " QPushButton#pushButtonInfoManViewZoomOut {"
            "      background-image: url(:/" + strPath + "zoomout-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewZoomOut:focus {"
            "      background-image: url(:/" + strPath + "zoomout-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewZoomOut:pressed {"
            "      background-image: url(:/" + strPath + "zoomout-2.png);"
            "  }";

    //pushButtonInfoManViewNext
    strStyleSheet += " QPushButton#pushButtonInfoManViewNext {"
            "      background-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewNext:focus {"
            "      background-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewNext:pressed {"
            "      background-image: url(:/" + strPath + "next-2.png);"
            "  }";

    //pushButtonInfoManViewPrev
    strStyleSheet += " QPushButton#pushButtonInfoManViewPrev {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewPrev:focus {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewPrev:pressed {"
            "      background-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonInfoManViewReturn
    strStyleSheet += " QPushButton#pushButtonInfoManViewReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：security/securityplayerdialog界面style
    ************************************************************/
    strPath = "images/security/";
    //pushButtonSecPlayerPause
    strStyleSheet += " QPushButton#pushButtonSecPlayerPause {"
            "      background-image: url(:/" + strPath + "pause-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPlayerPause:focus {"
            "      background-image: url(:/" + strPath + "pause-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPlayerPause:pressed {"
            "      background-image: url(:/" + strPath + "pause-2.png);"
            "  }";

    //pushButtonSecPlayerPlay
    strStyleSheet += " QPushButton#pushButtonSecPlayerPlay {"
            "      background-image: url(:/" + strPath + "play-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPlayerPlay:focus {"
            "      background-image: url(:/" + strPath + "play-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPlayerPlay:pressed {"
            "      background-image: url(:/" + strPath + "play-2.png);"
            "  }";

    //pushButtonSecPlayerStop
    strStyleSheet += " QPushButton#pushButtonSecPlayerStop {"
            "      background-image: url(:/" + strPath + "stop-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPlayerStop:focus {"
            "      background-image: url(:/" + strPath + "stop-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPlayerStop:pressed {"
            "      background-image: url(:/" + strPath + "stop-2.png);"
            "  }";

    //pushButtonSecPlayerReturn
    strStyleSheet += " QPushButton#pushButtonSecPlayerReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPlayerReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPlayerReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：security/securityalertdialog界面style
    ************************************************************/
    strPath = "images/security/";

    //pushButtonSecAlertHome
    strStyleSheet += " QPushButton#pushButtonSecAlertHome {"
            "      background-image: url(:/" + strPath + "home-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAlertHome:focus {"
            "      background-image: url(:/" + strPath + "home-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAlertHome:pressed {"
            "      background-image: url(:/" + strPath + "home-2.png);"
            "  }";

    //pushButtonSecAlertOut
    strStyleSheet += " QPushButton#pushButtonSecAlertOut {"
            "      background-image: url(:/" + strPath + "out-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAlertOut:focus {"
            "      background-image: url(:/" + strPath + "out-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAlertOut:pressed {"
            "      background-image: url(:/" + strPath + "out-2.png);"
            "  }";

    //pushButtonSecAlertDisArm
    strStyleSheet += " QPushButton#pushButtonSecAlertDisArm {"
            "      background-image: url(:/" + strPath + "disarm-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAlertDisArm:focus {"
            "      background-image: url(:/" + strPath + "disarm-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAlertDisArm:pressed {"
            "      background-image: url(:/" + strPath + "disarm-2.png);"
            "  }";

    //pushButtonSecAlertReturn
    strStyleSheet += " QPushButton#pushButtonSecAlertReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAlertReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAlertReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：system_setting/security/securityparamsetdialog界面style
    ************************************************************/
    strPath = "images/system_setting/";

    //pushButtonSecParaSetDefense
    strStyleSheet += " QPushButton#pushButtonSecParaSetDefense {"
            "      background-image: url();"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecParaSetDefense:focus {"
            "      background-image: url();"
            "      color: rgb(158, 106, 26);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecParaSetDefense:pressed {"
            "      background-image: url();"
            "      color: #FF0000;"
            "  }";

    //pushButtonSecParaSetSendPic
    strStyleSheet += "QPushButton#pushButtonSecParaSetSendPic {"
            "       background-image:url();"
            "}";
    strStyleSheet += "QPushButton#pushButtonSecParaSetSendPic:focus {"
            "       background-image:url();"
            "}";
    strStyleSheet += "QPushButton#pushButtonSecParaSetSendPic:pressed {"
            "       background-image:url();"
            "}";

    //pushButtonSecParaSetSave
    strStyleSheet += " QPushButton#pushButtonSecParaSetSave {"
            "      background-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecParaSetSave:focus {"
            "      background-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecParaSetSave:pressed {"
            "      background-image: url(:/" + strPath + "save-2.png);"
            "  }";

    //pushButtonSecParaSetReturn
    strStyleSheet += " QPushButton#pushButtonSecParaSetReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecParaSetReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecParaSetReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：intercom_man/mediaplayerdialog界面style
    ************************************************************/
    strPath = "images/intercom_man/";
    //pushButtonMediaPlayerPlay
    strStyleSheet += " QPushButton#pushButtonMediaPlayerPlay {"
            "      background-image: url(:/" + strPath + "play-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerPlay:focus {"
            "      background-image: url(:/" + strPath + "play-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerPlay:pressed {"
            "      background-image: url(:/" + strPath + "play-2.png);"
            "  }";

    //pushButtonMediaPlayerStop
    strStyleSheet += " QPushButton#pushButtonMediaPlayerStop {"
            "      background-image: url(:/" + strPath + "stop-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerStop:focus {"
            "      background-image: url(:/" + strPath + "stop-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerStop:pressed {"
            "      background-image: url(:/" + strPath + "stop-2.png);"
            "  }";

    //pushButtonMediaPlayerPause
    strStyleSheet += " QPushButton#pushButtonMediaPlayerPause {"
            "      background-image: url(:/" + strPath + "pause-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerPause:focus {"
            "      background-image: url(:/" + strPath + "pause-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerPause:pressed {"
            "      background-image: url(:/" + strPath + "pause-2.png);"
            "  }";

    //pushButtonMediaPlayerReturn
    strStyleSheet += " QPushButton#pushButtonMediaPlayerReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonMediaPlayerCall
    strStyleSheet += " QPushButton#pushButtonMediaPlayerCall {"
            "      background-image: url(:/" + strPath + "call-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerCall:focus {"
            "      background-image: url(:/" + strPath + "call-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerCall:pressed {"
            "      background-image: url(:/" + strPath + "call-2.png);"
            "  }";

    /************************************************************
    描述：system_setting/ring_and_volume/ringandvolumedialog界面style
    ************************************************************/
    strPath = "images/system_setting/";
    QString rf;
    rf.sprintf("%d",g_pScaleSrc->get_ringvalue_font_size());

    //pushButtonRingVolumeDail
    strStyleSheet += " QPushButton#pushButtonRingVolumeDail {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeDail:focus {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      color: rgb(158, 106, 26);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeDail:pressed {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      color: #FF0000;"
            "  }";

    //pushButtonRingVolumeCallIn
    strStyleSheet += " QPushButton#pushButtonRingVolumeCallIn {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeCallIn:focus {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      color: rgb(158, 106, 26);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeCallIn:pressed {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      color: #FF0000;"
            "  }";

    //pushButtonRingVolumeDoor
    strStyleSheet += " QPushButton#pushButtonRingVolumeDoor {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeDoor:focus {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      color: rgb(158, 106, 26);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeDoor:pressed {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      color: #FF0000;"
            "  }";

    //pushButtonRingVolumeCenter1
    strStyleSheet += " QPushButton#pushButtonRingVolumeCenter1 {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeCenter1:focus {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      color: rgb(158, 106, 26);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeCenter1:pressed {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      color: #FF0000;"
            "  }";

    //pushButtonRingVolumeCenter2
    strStyleSheet += " QPushButton#pushButtonRingVolumeCenter2 {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeCenter2:focus {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      color: rgb(158, 106, 26);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeCenter2:pressed {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      color: #FF0000;"
            "  }";

    //pushButtonRingVolumeSave
    strStyleSheet += " QPushButton#pushButtonRingVolumeSave {"
            "      background-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeSave:focus {"
            "      background-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeSave:pressed {"
            "      background-image: url(:/" + strPath + "save-2.png);"
            "  }";

    //pushButtonRingVolumeReturn
    strStyleSheet += " QPushButton#pushButtonRingVolumeReturn {"
        "      background-image: url(:/" + strPath + "return-1.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeReturn:focus {"
        "      background-image: url(:/" + strPath + "return-1.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeReturn:pressed {"
        "      background-image: url(:/" + strPath + "return-2.png);"
        "  }";


    /************************************************************
    描述：system_setting/ring_and_volume/ringfilesetdialog界面style
    ************************************************************/
    strPath = "images/system_setting/";

    //pushButtonRingFileSetCancel
    strStyleSheet += " QPushButton#pushButtonRingFileSetCancel {"
            "      background-image: url(:/" + strPath + "cancel-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingFileSetCancel:focus {"
            "      background-image: url(:/" + strPath + "cancel-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingFileSetCancel:pressed {"
            "      background-image: url(:/" + strPath + "cancel-2.png);"
            "  }";

    //pushButtonRingFileSetSave
    strStyleSheet += " QPushButton#pushButtonRingFileSetSave {"
            "      background-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingFileSetSave:focus {"
            "      background-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingFileSetSave:pressed {"
            "      background-image: url(:/" + strPath + "save-2.png);"
            "  }";


    /************************************************************
    描述：voip/voipmaindialog界面style
    ************************************************************/
    //pushButtonVoipMainMon
    strStyleSheet += " QPushButton#pushButtonVoipMainMon {"
            "      border-image: url(:/images/voip_dialog/videoopen-1.png);"
            "      color: " + strBtnColor + ";"
            "      font: " + strBtnFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonVoipMainMon:focus {"
            "      border-image: url(:/images/voip_dialog/videoopen-1.png);"
            "      color: " + strBtnFocusColor + ";"
            "      font: " + strBtnFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonVoipMainMon:pressed {"
            "      border-image: url(:/images/voip_dialog/videoopen-2.png);"
            "      color: " + strBtnPressColor + ";"
            "      font: " + strBtnFontSize + "px;"
            "  }";

    //pushButtonVoipMainDial
    strStyleSheet += " QPushButton#pushButtonVoipMainDial {"
            "      background-image: url(:/images/voip_dialog/answer-1.png);"
            "      color: " + strBtnColor + ";"
            "      font: " + strBtnFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonVoipMainDial:focus {"
            "      background-image: url(:/images/voip_dialog/answer-1.png);"
            "      color: " + strBtnFocusColor + ";"
            "      font: " + strBtnFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonVoipMainDial:pressed {"
            "      background-image: url(:/images/voip_dialog/answer-2.png);"
            "      color: " + strBtnPressColor + ";"
            "      font: " + strBtnFontSize + "px;"
            "  }";

    //pushButtonVoipMainReturn
    strStyleSheet += " QPushButton#pushButtonVoipMainReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonVoipMainReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonVoipMainReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：voip/mstdialog界面style
    ************************************************************/
    //pushButtonMstPrev
    strStyleSheet += " QPushButton#pushButtonMstPrev {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMstPrev:focus {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMstPrev:pressed {"
            "      background-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonMstNext
    strStyleSheet += " QPushButton#pushButtonMstNext {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMstNext:focus {"
            "      background-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMstNext:pressed {"
            "      background-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonMstHangUp
    strStyleSheet += " QPushButton#pushButtonMstHangUp {"
            "      background-image: url(:/images/voip_dialog/hangup-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMstHangUp:focus {"
            "      background-image: url(:/images/voip_dialog/hangup-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMstHangUp:pressed {"
            "      background-image: url(:/images/voip_dialog/hangup-2.png);"
            "  }";

    //pushButtonMstReturn
    strStyleSheet += " QPushButton#pushButtonMstReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMstReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMstReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";


    /************************************************************
    描述：system_setting/device/devicesetmaindialog界面style
    ************************************************************/
    QString strDeviceSetMainFontSize;
    strDeviceSetMainFontSize.sprintf("%d",g_pScaleSrc->systemset_device_button_fonts());

#if 0
    //pushButtonCalibration
    strStyleSheet += " QPushButton#pushButtonCalibration {"
        "      background-image: url(:/images/system_setting/blank-1.png);"
        "      color: " + strBtnColor + ";"
        "      font: " + strDeviceSetMainFontSize + "px;"
        "  }";
    strStyleSheet += " QPushButton#pushButtonCalibration:focus {"
        "      background-image: url(:/images/system_setting/blank-1.png);"
        "      color: " + strBtnFocusColor + ";"
        "      font: " + strDeviceSetMainFontSize + "px;"
        "  }";
    strStyleSheet += " QPushButton#pushButtonCalibration:pressed {"
        "      background-image: url(:/images/system_setting/blank-2.png);"
        "      color: " + strBtnPressColor + ";"
        "      font: " + strDeviceSetMainFontSize + "px;"
        "  }";

    //pushButtonSystemReset
    strStyleSheet += " QPushButton#pushButtonSystemReset {"
        "      background-image: url(:/images/system_setting/systemreset-1.png);"
        "      color: " + strBtnColor + ";"
        "      font: " + strBtnFontSize + "px;"
        "  }";
    strStyleSheet += " QPushButton#pushButtonSystemReset:focus {"
        "      background-image: url(:/images/system_setting/systemreset-1.png);"
        "      color: " + strBtnFocusColor + ";"
        "      font: " + strBtnFontSize + "px;"
        "  }";
    strStyleSheet += " QPushButton#pushButtonSystemReset:pressed {"
        "      background-image: url(:/images/system_setting/systemreset-2.png);"
        "      color: " + strBtnPressColor + ";"
        "      font: " + strBtnFontSize + "px;"
        "  }";
#endif

    /************************************************************
    描述：door_bell/doorbelldialog界面style
    ************************************************************/
    strPath = "images/door_bell/";

    //pushButtonDoorBellAnswer
    strStyleSheet += " QPushButton#pushButtonDoorBellAnswer {"
        "      background-image: url(:/" + strPath + "answer-1.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonDoorBellAnswer:focus {"
        "      background-image: url(:/" + strPath + "answer-1.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonDoorBellAnswer:pressed {"
        "      background-image: url(:/" + strPath + "answer-2.png);"
        "  }";

    //pushButtonDoorBellOpen
    strStyleSheet += " QPushButton#pushButtonDoorBellOpen {"
            "      background-image: url(:/" + strPath + "open-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDoorBellOpen:focus {"
            "      background-image: url(:/" + strPath + "open-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDoorBellOpen:pressed {"
            "      background-image: url(:/" + strPath + "open-2.png);"
            "  }";

    //pushButtonDoorBellReturn
    strStyleSheet += " QPushButton#pushButtonDoorBellReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDoorBellReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDoorBellReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：call_elevator/callelevatordialog界面style
    ************************************************************/
    if (g_pScaleSrc->get_screen_version() == 2) {
        strNumFontSize = "42";
        strNumFontFocusSize = "42";
        strNumFontPressSize = "42";
    }
    else {
        strNumFontSize = "50";
        strNumFontFocusSize = "50";
        strNumFontPressSize = "50";
    }

    strPath = "images/call_elevator/";
    //pushButtonCallElevatorElevator
    strStyleSheet += " QPushButton#pushButtonCallElevatorElevator {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorElevator {"
            "      border-image: url(:/" + strPath + "prev-2.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorElevator {"
            "      border-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonCallElevatorClear
    strStyleSheet += " QPushButton#pushButtonCallElevatorClear {"
            "      border-image: url(:/" + strPath + "delete-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorClear {"
            "      border-image: url(:/" + strPath + "delete-2.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorClear {"
            "      border-image: url(:/" + strPath + "delete-2.png);"
            "  }";

    //pushButtonCallElevatorReturn
    strStyleSheet += " QPushButton#pushButtonCallElevatorReturn {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorReturn:focus {"
            "      background-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorReturn:pressed {"
            "      background-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonCallElevatorNum
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum0 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum0:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum0:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum1 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum1:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum1:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum2 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum2:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum2:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum3 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum3:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum3:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum4 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum4:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum4:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum5 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum5:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum5:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum6 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum6:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum6:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum7 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum7:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum7:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonCallElevatorNum8 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum8:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum8:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonCallElevatorNum9 {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum9:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum9:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorDot {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorDot:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorDot:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorShop {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorShop:focus {"
            "      background-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorShop:pressed {"
            "      background-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";

#if 0
    /************************************************************
    描述：system_setting/door_machine/doormachinedialog界面style
    ************************************************************/
    strPath = "images/system_setting/door_machine/doormachine/";

    //pushButtonDoorMachineSave
    strStyleSheet += " QPushButton#pushButtonDoorMachineSave {"
            "      background-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDoorMachineSave:focus {"
            "      background-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDoorMachineSave:pressed {"
            "      background-image: url(:/" + strPath + "save-2.png);"
            "  }";
#endif

    /************************************************************
    描述：system_setting/syssetmaindialog界面style
    ************************************************************/
    strPath = "images/system_setting/";

    //pushButtonSysSetMainVersion
    strStyleSheet += " QPushButton#pushButtonSysSetMainVersion {"
            "      background-image: url(:/" + strPath + "version2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainVersion:focus {"
            "      background-image: url(:/" + strPath + "version2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainVersion:pressed {"
            "      background-image: url(:/" + strPath + "version2-2.png);"
            "  }";

    //pushButtonSysSetMainLag
    strStyleSheet += " QPushButton#pushButtonSysSetMainLag {"
            "      background-image: url(:/" + strPath + "language2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainLag:focus {"
            "      background-image: url(:/" + strPath + "language2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainLag:pressed {"
            "      background-image: url(:/" + strPath + "language2-2.png);"
            "  }";

    //pushButtonSysSetMainPwd
    strStyleSheet += " QPushButton#pushButtonSysSetMainPwd {"
            "      background-image: url(:/" + strPath + "password2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainPwd:focus {"
            "      background-image: url(:/" + strPath + "password2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainPwd:pressed {"
            "      background-image: url(:/" + strPath + "password2-2.png);"
            "  }";

    //pushButtonSysSetMainTime
    strStyleSheet += " QPushButton#pushButtonSysSetMainTime {"
            "      background-image: url(:/" + strPath + "time2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainTime:focus {"
            "      background-image: url(:/" + strPath + "time2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainTime:pressed {"
            "      background-image: url(:/" + strPath + "time2-2.png);"
            "  }";

    //pushButtonSysSetMainRing
    strStyleSheet += " QPushButton#pushButtonSysSetMainRing {"
            "      background-image: url(:/" + strPath + "ring2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainRing:focus {"
            "      background-image: url(:/" + strPath + "ring2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainRing:pressed {"
            "      background-image: url(:/" + strPath + "ring2-2.png);"
            "  }";

    //pushButtonSysSetMainDevice
    strStyleSheet += " QPushButton#pushButtonSysSetMainDevice {"
            "      background-image: url(:/" + strPath + "device2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainDevice:focus {"
            "      background-image: url(:/" + strPath + "device2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainDevice:pressed {"
            "      background-image: url(:/" + strPath + "device2-2.png);"
            "  }";

    //pushButtonSysSetMainSecurity
    strStyleSheet += " QPushButton#pushButtonSysSetMainSecurity {"
            "      background-image: url(:/" + strPath + "security2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainSecurity:focus {"
            "      background-image: url(:/" + strPath + "security2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainSecurity:pressed {"
            "      background-image: url(:/" + strPath + "security2-2.png);"
            "  }";

    //pushButtonSysSetMainReturn
    strStyleSheet += " QPushButton#pushButtonSysSetMainReturn {"
        "      background-image: url(:/" + strPath + "return2-1.png);"
        "      font: " + strBtnFontSize + "px;"
        "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainReturn:focus {"
        "      background-image: url(:/" + strPath + "return2-1.png);"
        "      font: " + strBtnFontSize + "px;"
        "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainReturn:pressed {"
        "      background-image: url(:/" + strPath + "return2-2.png);"
        "      font: " + strBtnFontSize + "px;"
        "  }";


    /************************************************************
    描述：system_setting/syssetengineerdialog界面style
    ************************************************************/
    strPath = "images/system_setting/";

    //pushButtonSysSetEngineerNetAddress
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerNetAddress {"
            "      background-image: url(:/" + strPath + "netaddress-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerNetAddress:focus {"
            "      background-image: url(:/" + strPath + "netaddress-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerNetAddress:pressed {"
            "      background-image: url(:/" + strPath + "netaddress-2.png);"
            "  }";

    //pushButtonSysSetEngineerDoorMachine
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerDoorMachine {"
            "      background-image: url(:/" + strPath + "doormachine-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerDoorMachine:focus {"
            "      background-image: url(:/" + strPath + "doormachine-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerDoorMachine:pressed {"
            "      background-image: url(:/" + strPath + "doormachine-2.png);"
            "  }";

    //pushButtonSysSetEngineerIpCamera
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerIpCamera {"
            "      background-image: url(:/" + strPath + "ipcamera-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerIpCamera:focus {"
            "      background-image: url(:/" + strPath + "ipcamera-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerIpCamera:pressed {"
            "      background-image: url(:/" + strPath + "ipcamera-2.png);"
            "  }";

    //pushButtonSysSetEngineerPwd
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerPwd {"
            "      background-image: url(:/" + strPath + "password-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerPwd:focus {"
            "      background-image: url(:/" + strPath + "password-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerPwd:pressed {"
            "      background-image: url(:/" + strPath + "password-2.png);"
            "  }";

    //pushButtonSysSetEngineerReturn
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerReturn {"
            "      background-image: url(:/" + strPath + "return3-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerReturn:focus {"
            "      background-image: url(:/" + strPath + "return3-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerReturn:pressed {"
            "      background-image: url(:/" + strPath + "return3-2.png);"
            "  }";

    //pushButtonSysSetEngineerSave
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerSave {"
            "      background-image: url(:/" + strPath + "save3-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerSave:focus {"
            "      background-image: url(:/" + strPath + "save3-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerSave:pressed {"
            "      background-image: url(:/" + strPath + "save3-2.png);"
            "  }";

    //pushButtonSysSetEngineerRemoteCheck
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerRemoteCheck {"
        "      background-image: url(:/images/system_setting/page-unselected.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerRemoteCheck:focus {"
        "      background-image: url(:/images/system_setting/page-unselected.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerRemoteCheck:pressed {"
        "      background-image: url(:/images/system_setting/page-selected.png);"
        "  }";

    //pushButtonSysSetEngineerOther
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerOther {"
        "      background-image: url(:/images/system_setting/other-1.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerOther:focus {"
        "      background-image: url(:/images/system_setting/other-1.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerOther:pressed {"
        "      background-image: url(:/images/system_setting/other-2.png);"
        "  }";


	//Time mode 1..3 dialogs
	strStyleSheet += "  QDialog#TimeMode1Dialog {"
		"      background-color: #D0E8FF;"
		"      background-image: url();"
		"      border: 2px solid #CCCCCC;"
		"  }";
	strStyleSheet += "  QDialog#TimeMode2Dialog {"
		"      background-color: #D0E8FF;"
		"      background-image: url();"
		"      border: 2px solid #CCCCCC;"
		"  }";
	strStyleSheet += "  QDialog#TimeMode3Dialog {"
		"      background-color: #D0E8FF;"
		"      background-image: url();"
		"      border: 2px solid #CCCCCC;"
		"  }";

	strStyleSheet += "  QDialog#MonListDialog {"
        "      border-image: url(:/images/main/messagebox.png) 47 87 20 87 repeat;"//24 59 10 59;"
		"      border-top: 47px transparent;"
		"      border-bottom: 20px transparent;"
		"	   border-right: 87px transparent;"
		"	   border-left: 87px transparent;"
		"      background-image: url();"
		"  }";

	strStyleSheet += "  QDialog#AddressBookDialog {"
        "      border-image: url(:/images/main/messagebox.png) 47 87 20 87 repeat;"//24 59 10 59;"
		"      border-top: 47px transparent;"
		"      border-bottom: 20px transparent;"
		"	   border-right: 87px transparent;"
		"	   border-left: 87px transparent;"
		"      background-image: url();"
		"  }";

    strStyleSheet += "	QLabel#doorbell_label {"
        "      background-color: #F6F4F4;" //#D0E8FF;"
        "      color: #604010;"
        "      border: 3px solid #404040;"
        "      font: 20px"
        "  }";

	strStyleSheet += "  QLabel#labelMediaPlayerInfo {"
        "      font: 20px;"
		"  }";

	strStyleSheet += "  QComboBox {"
		"      border: 1px solid gray;"
		"      border-radius: 3px;"
		"      font: 18px;"
		"  }";
	strStyleSheet += "  QComboBox::drop-down {"
		"      width: 30px;"
		"      "
		"  }";
	strStyleSheet += "  QComboBox::down-arrow {"
        "	   border-image: url(:/images/main/scrollbar-down-1.bmp);"
		"  }";
	strStyleSheet += "  QComboBox::down-arrow:pressed {"
        "	   border-image: url(:/images/main/scrollbar-down-2.bmp);"
		"  }";
	strStyleSheet += "  QComboBox QAbstractItemView {"
		"      border: 2px solid rgb(255, 128, 128);"
		//"      selection-background-color: rgb(0, 255, 0);"
		//	"      height:80px;"
		"  }";

    sf.sprintf("%d",g_pScaleSrc->get_voip_ict_label_font_size());

    strStyleSheet += "  QLineEdit#lineEditInfo {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";

    strStyleSheet += "  QLineEdit#lineEditDisplay {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "      background-color: rgba(0, 0, 0, 0);"
        "  }";

	sf.sprintf("%d",g_pScaleSrc->get_pwdinfo_label_font_size());
	strStyleSheet += "  QLabel#labelPwdInfo {"
		"      font: " + sf + "px;"
        "      border: 0px;"
		"  }";

	sf.sprintf("%d",g_pScaleSrc->get_passwordset_font_size());

    strStyleSheet += "  QLabel#label {"
        "      font: " + sf + "px;"
		"      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_2 {"
		"      font: " + sf + "px;"
		"      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_3 {"
		"      font: " + sf + "px;"
		"      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_4 {"
		"      font: " + sf + "px;"
		"      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_5 {"
		"      font: " + sf + "px;"
		"      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_6 {"
		"      font: " + sf + "px;"
		"      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_info_title {"
		"      font: " + sf + "px;"
		"      border: 0px;"
		"  }";

	strStyleSheet += "  QLineEdit#lineEditUseName {"
		"      font: " + sf + "px;"
		"      border: 0px;"
		"  }";
	strStyleSheet += "  QLineEdit#lineEditPassword {"
		"      font: " + sf + "px;"
		"      border: 0px;"
        "  }";

    sf.sprintf("%d",g_pScaleSrc->get_ringvalue_font_size());
    strStyleSheet += "  QLabel#label_ringvalue_1 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLabel#label_ringvalue_2 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLabel#label_ringvalue_3 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLabel#label_ringvalue_4 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLabel#label_ringvalue_5 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLabel#label_ringvalue_6 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLabel#label_ringvalue_7 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";

	strStyleSheet += "  QLCDNumber {"
		"      color: rgb(255, 0, 0);"
		"      background-color: rgb(0, 0, 0);"
		"  }";

	strStyleSheet += "  QLabel#label_adddefense_1 {"
		"      font: " + sf + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_adddefense_2 {"
		"      font: " + sf + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_adddefense_3 {"
		"      font: " + sf + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_adddefense_4 {"
		"      font: " + sf + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_adddefense_5 {"
		"      font: " + sf + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_adddefense_6 {"
		"      font: " + sf + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_adddefense_7 {"
		"      font: " + sf + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_adddefense_8 {"
		"      font: " + sf + "px;"
        "      border: 0px;"
		"  }";

	strStyleSheet += "  QLabel#label_DoorMachine_1 {"
		"      font: " + sf + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_DoorMachine_2 {"
		"      font: " + sf + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_DoorMachine_3 {"
		"      font: " + sf + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_DoorMachine_4 {"
		"      font: " + sf + "px;"
        "	   color: #604010;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_DoorMachine_5 {"
		"      font: " + sf + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_DoorMachine_6 {"
		"      font: " + sf + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_DoorMachine_7 {"
		"      font: " + sf + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_DoorMachine_8 {"
		"      font: " + sf + "px;"
        "      border: 0px;"
		"  }";

    strStyleSheet += "  QLineEdit#lineEditName1 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditName2 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditName3 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditName4 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditName5 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditName6 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";

    strStyleSheet += "  QLineEdit#lineEditUserName1 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditUserName2 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditUserName3 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditUserName4 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditUserName5 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditUserName6 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";

    strStyleSheet += "  QLineEdit#lineEditPwd1 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditPwd2 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditPwd3 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditPwd4 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditPwd5 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditPwd6 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";

    strStyleSheet += "  QLineEdit#lineEditNetName1 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditNetName2 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditNetName3 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditNetName4 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditNetName5 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditNetName6 {"
        "      font: " + sf + "px;"
        "      border: 0px;"
        "  }";

	s.sprintf("%d",g_pScaleSrc->get_sysset_info_font_size());

	strStyleSheet += "  QLabel#label_securityset_1 {"
		"      font: " + s + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_securityset_2 {"
		"      font: " + s + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_securityset_3 {"
		"      font: " + s + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_securityset_4 {"
		"      font: " + s + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#label_securityset_5 {"
		"      font: " + s + "px;"
        "      border: 0px;"
		"  }";

	strStyleSheet += "	QLabel#label_floor_num {"
		"      font: " + s + "px;"
        "      border: 0px;"
		"  }";


	s.sprintf("%d",g_pScaleSrc->get_infowrite_font_size());
	strStyleSheet += "  QLabel#labelInfoReceiver {"
		"      font: " + s + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#labelInfoTitle {"
		"      font: " + s + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#labelInfoBody {"
		"      font: " + s + "px;"
        "      border: 0px;"
		"  }";
	strStyleSheet += "  QLabel#labelInfoAttachment {"
		"      font: " + s + "px;"
        "      border: 0px;"
		"  }";

    strStyleSheet += "  QLabel#labelInfoAttachment {"
        "      font: " + s + "px;"
        "      border: 0px;"
        "  }";

	strStyleSheet += "  MyLabel#labelExtension1 {"
		"      font: " + s + "px;"
		"      color: #604010;"
		"      border: 0px;"
		"  }";
	strStyleSheet += "  MyLabel#labelExtension2 {"
		"      font: " + s + "px;"
		"      color: #604010;"
		"      border: 0px;"
        "  }";

    strStyleSheet += "  MyTextEdit#textEditInfoBody {"
        "      font: " + s + "px;"
        "      color: #604010;"
        "      border: 0px;"
        "  }";
	
	s.sprintf("%d",g_pScaleSrc->get_all_title_lable_font());
	
	strStyleSheet += "  QLabel#label_title {"
		"      font: " + s + "px;"
        "      border: 0px;"
		"  }";

    strStyleSheet += "  QLabel#labelInfo {"
        "      font: " + s + "px;"
        "      border: 0px;"
        "  }";

	strStyleSheet += "  MyLineEdit#lineEditReceiverName {"
		//"      border-radius: 7px;"
		"      font: 26px;"
		"	   border: 0px;"
        "	   color: #582F00;"
		"      background-color: rgba(0, 0, 0, 0);"
		//"      border: 5px solid #E0E0E0;"
		"  }";
	strStyleSheet += "  MyLineEdit#lineEditInfoTitle {"
		//"      border-radius: 7px;"
		"      font: 26px;"
		"	   border: 0px;"
        "	   color: #582F00;"
		"      background-color: rgba(0, 0, 0, 0);"
		//"      border: 5px solid #E0E0E0;"
        "  }";
    QString sft;
    sft.sprintf("%d",g_pScaleSrc->get_passwordset_font_size());

    strStyleSheet += "  QLineEdit#lineEditPassword {"
        "      font: "+sft+"px;"
        "	   border: 0px;"
        "      background-color: rgba(0, 0, 0, 0);"
        "  }";
    strStyleSheet += "  QLineEdit#lineEditCheckPassword {"
        "      font: "+sft+"px;"
        "	   border: 0px;"
        "      background-color: rgba(0, 0, 0, 0);"
        "  }";

    strStyleSheet += "  QTextEdit {"
        "      background-color: transparent;"
        "      color: #604010;"
        "      font: 19px;"
        "      border: 0px solid #C0C0C0;"
        "  }";

    //NEW INPUTMETHOD
	strStyleSheet += "  QLCDNumber {"
		"      color: rgb(255, 0, 0);"
		"      background-color: rgb(0, 0, 0);"
		"  }";

    strStyleSheet += " QPushButton#pushButtonSave {"
            "      background-image: url(:/images/system_setting/save-1.png);"
        "      color: " + strBtnColor + ";"
        "      font: " + strBtnFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSave:focus {"
            "      background-image: url(:/images/system_setting/save-1.png);"
        "      color: " + strBtnFocusColor + ";"
        "      font: " + strBtnFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSave:pressed {"
            "      background-image: url(:/images/system_setting/save-2.png);"
        "      color: " + strBtnPressColor + ";"
        "      font: " + strBtnFontSize + "px;"
            "  }";

	strStyleSheet += " QPushButton#pushButtonCancel {"
            "      background-image: url(:/images/voip_dialog/return-1.png);"
		"      color: " + strBtnColor + ";"
		"      font: " + strBtnFontSize + "px;"
			"  }";
	strStyleSheet += " QPushButton#pushButtonCancel:focus {"
            "      background-image: url(:/images/voip_dialog/return-1.png);"
		"      color: " + strBtnFocusColor + ";"
		"      font: " + strBtnFontSize + "px;"
			"  }";
	strStyleSheet += " QPushButton#pushButtonCancel:pressed {"
            "      background-image: url(:/images/voip_dialog/return-2.png);"
		"      color: " + strBtnPressColor + ";"
		"      font: " + strBtnFontSize + "px;"
			"  }";

	//Home control
	strStyleSheet += " QPushButton#pushButtonTypeLight {"
        "      background-image: url(:/images/home_control/light.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTypeSwitch {"
        "      background-image: url(:/images/home_control/switch.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTypeCurtain {"
        "      background-image: url(:/images/home_control/curtain.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTypeAC {"
        "      background-image: url(:/images/home_control/ac.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTypeTV {"
        "      background-image: url(:/images/home_control/tv.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTypeMusic {"
        "      background-image: url(:/images/home_control/music.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTypeCustom {"
        "      background-image: url(:/images/home_control/user.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonHCReturn {"
        "      background-image: url(:/images/voip_dialog/return-1.png);"
		"      color: " + strBtnColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCReturn:focus {"
        "      background-image: url(:/images/voip_dialog/return-1.png);"
		"      color: " + strBtnFocusColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCReturn:pressed {"
        "      background-image: url(:/images/voip_dialog/return-2.png);"
		"      color: " + strBtnPressColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";

	strStyleSheet += " QPushButton#pushButtonHCEnterRoom {"
        "      background-image: url(:/images/home_control/enter_room-1.png);"
		"      color: " + strBtnColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCEnterRoom:focus {"
        "      background-image: url(:/images/home_control/enter_room-1.png);"
		"      color: " + strBtnFocusColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCEnterRoom:pressed {"
        "      background-image: url(:/images/home_control/enter_room-2.png);"
		"      color: " + strBtnPressColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";

	strStyleSheet += " QPushButton#pushButtonHCExitRoom {"
        "      background-image: url(:/images/home_control/exit_room-1.png);"
		"      color: " + strBtnColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCExitRoom:focus {"
        "      background-image: url(:/images/home_control/exit_room-1.png);"
		"      color: " + strBtnFocusColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCExitRoom:pressed {"
        "      background-image: url(:/images/home_control/exit_room-2.png);"
		"      color: " + strBtnPressColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";

	strStyleSheet += " QPushButton#pushButtonHCSceneReturn {"
        "      background-image: url(:/images/home_control/scene_return-1.png);"
		"      border-radius: 7px;"
		"      color: " + strBtnColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCSceneReturn:focus {"
        "      background-image: url(:/images/home_control/scene_return-1.png);"
		"      border-radius: 7px;"
		"      color: " + strBtnFocusColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"	}";
	strStyleSheet += " QPushButton#pushButtonHCSceneReturn:pressed {"
        "      background-image: url(:/images/home_control/scene_return-3.png);"
		"      border-radius: 7px;"
		"      color: " + strBtnPressColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";

	strStyleSheet += " QPushButton#pushButtonEnterHCScene {"
        "      background-image: url(:/images/main/home_control-1.png);"
		"      border-radius: 7px;"
		"      color: " + strBtnColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";
	strStyleSheet += " QPushButton#pushButtonEnterHCScene:focus {"
        "      background-image: url(:/images/main/home_control-1.png);"
		"      border-radius: 7px;"
		"      color: " + strBtnFocusColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"	}";
	strStyleSheet += " QPushButton#pushButtonEnterHCScene:pressed {"
        "      background-image: url(:/images/main/home_control-3.png);"
		"      border-radius: 7px;"
		"      color: " + strBtnPressColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";

	strStyleSheet += " QPushButton#pushButtonFloor {"
            "      background-image: url(:/images/home_control/blank-1.png);"
		"      color: " + strBtnColor + ";"
		"      font: " + strBtnFontSize + "px;"
			"  }";
	strStyleSheet += " QPushButton#pushButtonFloor:focus {"
            "      background-image: url(:/images/home_control/blank-1.png);"
		"      color: " + strBtnFocusColor + ";"
		"      font: " + strBtnFontSize + "px;"
			"  }";
	strStyleSheet += " QPushButton#pushButtonFloor:pressed {"
            "      background-image: url(:/images/home_control/blank-2.png);"
		"      color: " + strBtnPressColor + ";"
		"      font: " + strBtnFontSize + "px;"
			"  }";

	strStyleSheet += " QPushButton#pushButtonPowerOn {"
        "      background-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonPowerOn:focus {"
        "      background-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonPowerOn:pressed {"
        "      background-image: url(:/images/home_control/power-off.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonPowerOff {"
        "      background-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonPowerOff:focus {"
        "      background-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonPowerOff:pressed {"
        "      background-image: url(:/images/home_control/power-on.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBrightnessAdd {"
        "      background-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBrightnessAdd:focus {"
        "      background-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBrightnessAdd:pressed {"
        "      background-image: url(:/images/home_control/add-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBrightnessSub {"
        "      background-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBrightnessSub:focus {"
        "      background-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBrightnessSub:pressed {"
        "      background-image: url(:/images/home_control/sub-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonHCSubControlReturn {"
        "      background-image: url(:/images/home_control/return-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCSubControlReturn:focus {"
        "      background-image: url(:/images/home_control/return-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCSubControlReturn:pressed {"
        "      background-image: url(:/images/home_control/return-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonCurtainOpen {"
        "      background-image: url(:/images/home_control/curtain-open-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonCurtainOpen:focus {"
        "      background-image: url(:/images/home_control/curtain-open-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonCurtainOpen:pressed {"
        "      background-image: url(:/images/home_control/curtain-open-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonCurtainStop {"
        "      background-image: url(:/images/home_control/pause-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonCurtainStop:focus {"
        "      background-image: url(:/images/home_control/pause-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonCurtainStop:pressed {"
        "      background-image: url(:/images/home_control/pause-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonCurtainHalfOpen {"
        "      background-image: url(:/images/home_control/curtain-half-open-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonCurtainHalfOpen:focus {"
        "      background-image: url(:/images/home_control/curtain-half-open-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonCurtainHalfOpen:pressed {"
        "      background-image: url(:/images/home_control/curtain-half-open-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonCurtainClose {"
        "      background-image: url(:/images/home_control/curtain-close-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonCurtainClose:focus {"
        "      background-image: url(:/images/home_control/curtain-close-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonCurtainClose:pressed {"
        "      background-image: url(:/images/home_control/curtain-close-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonTVStop {"
        "      background-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVStop:focus {"
        "      background-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVStop:pressed {"
        "      background-image: url(:/images/home_control/power-on.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonTVStart {"
        "      background-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVStart:focus {"
        "      background-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVStart:pressed {"
        "      background-image: url(:/images/home_control/power-off.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonTVMode {"
        "      background-image: url(:/images/home_control/tv-mode-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVMode:focus {"
        "      background-image: url(:/images/home_control/tv-mode-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVMode:pressed {"
        "      background-image: url(:/images/home_control/tv-mode-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonTVNextChannel {"
        "      background-image: url(:/images/home_control/up-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVNextChannel:focus {"
        "      background-image: url(:/images/home_control/up-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVNextChannel:pressed {"
        "      background-image: url(:/images/home_control/up-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonTVPrevChannel {"
        "      background-image: url(:/images/home_control/down-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVPrevChannel:focus {"
        "      background-image: url(:/images/home_control/down-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVPrevChannel:pressed {"
        "      background-image: url(:/images/home_control/down-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonTVIncVolume {"
        "      background-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVIncVolume:focus {"
        "      background-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVIncVolume:pressed {"
        "      background-image: url(:/images/home_control/add-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonTVDecVolume {"
        "      background-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVDecVolume:focus {"
        "      background-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVDecVolume:pressed {"
        "      background-image: url(:/images/home_control/sub-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonACStart {"
        "      background-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACStart:focus {"
        "      background-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACStart:pressed {"
        "      background-image: url(:/images/home_control/power-off.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonACStop {"
        "      background-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACStop:focus {"
        "      background-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACStop:pressed {"
        "      background-image: url(:/images/home_control/power-on.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonACTemperatureAdd {"
        "      background-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACTemperatureAdd:focus {"
        "      background-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACTemperatureAdd:pressed {"
        "      background-image: url(:/images/home_control/add-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonACTemperatureSub {"
        "      background-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACTemperatureSub:focus {"
        "      background-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACTemperatureSub:pressed {"
        "      background-image: url(:/images/home_control/sub-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonACSwing {"
        "      background-image: url(:/images/home_control/ac-swing-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACSwing:focus {"
        "      background-image: url(:/images/home_control/ac-swing-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACSwing:pressed {"
        "      background-image: url(:/images/home_control/ac-swing-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonACWind {"
        "      background-image: url(:/images/home_control/ac-wind-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACWind:focus {"
        "      background-image: url(:/images/home_control/ac-wind-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACWind:pressed {"
        "      background-image: url(:/images/home_control/ac-wind-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonACMode {"
        "      background-image: url(:/images/home_control/ac-mode-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACMode:focus {"
        "      background-image: url(:/images/home_control/ac-mode-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACMode:pressed {"
        "      background-image: url(:/images/home_control/ac-mode-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMOpen {"
        "      background-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMOpen:focus {"
        "      background-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMOpen:pressed {"
        "      background-image: url(:/images/home_control/power-off.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMClose {"
        "      background-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMClose:focus {"
        "      background-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMClose:pressed {"
        "      background-image: url(:/images/home_control/power-on.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMMute {"
        "      background-image: url(:/images/home_control/mute-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMMute:focus {"
        "      background-image: url(:/images/home_control/mute-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMMute:pressed {"
        "      background-image: url(:/images/home_control/mute-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMUnmute {"
        "      background-image: url(:/images/home_control/unmute-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMUnmute:focus {"
        "      background-image: url(:/images/home_control/unmute-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMUnmute:pressed {"
        "      background-image: url(:/images/home_control/unmute-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMSource {"
        "      background-image: url(:/images/home_control/source-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMSource:focus {"
        "      background-image: url(:/images/home_control/source-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMSource:pressed {"
        "      background-image: url(:/images/home_control/source-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMPrevMusic {"
        "      background-image: url(:/images/home_control/prev-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMPrevMusic:focus {"
        "      background-image: url(:/images/home_control/prev-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMPrevMusic:pressed {"
        "      background-image: url(:/images/home_control/prev-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMNextMusic {"
        "      background-image: url(:/images/home_control/next-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMNextMusic:focus {"
        "      background-image: url(:/images/home_control/next-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMNextMusic:pressed {"
        "      background-image: url(:/images/home_control/next-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMPlay {"
        "      background-image: url(:/images/home_control/play-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMPlay:focus {"
        "      background-image: url(:/images/home_control/play-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMPlay:pressed {"
        "      background-image: url(:/images/home_control/play-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMPause {"
        "      background-image: url(:/images/home_control/pause-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMPause:focus {"
        "      background-image: url(:/images/home_control/pause-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMPause:pressed {"
        "      background-image: url(:/images/home_control/pause-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMVolumeAdd {"
        "      background-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMVolumeAdd:focus {"
        "      background-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMVolumeAdd:pressed {"
        "      background-image: url(:/images/home_control/add-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMVolumeSub {"
        "      background-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMVolumeSub:focus {"
        "      background-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMVolumeSub:pressed {"
        "      background-image: url(:/images/home_control/sub-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCOpen {"
        "      background-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCOpen:focus {"
        "      background-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCOpen:pressed {"
        "      background-image: url(:/images/home_control/power-off.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCClose {"
        "      background-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCClose:focus {"
        "      background-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCClose:pressed {"
        "      background-image: url(:/images/home_control/power-on.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCUp {"
        "      background-image: url(:/images/home_control/up-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCUp:focus {"
        "      background-image: url(:/images/home_control/up-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCUp:pressed {"
        "      background-image: url(:/images/home_control/up-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCDown {"
        "      background-image: url(:/images/home_control/down-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCDown:focus {"
        "      background-image: url(:/images/home_control/down-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCDown:pressed {"
        "      background-image: url(:/images/home_control/down-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCLeft {"
        "      background-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCLeft:focus {"
        "      background-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCLeft:pressed {"
        "      background-image: url(:/images/home_control/sub-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCRight {"
        "      background-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCRight:focus {"
        "      background-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCRight:pressed {"
        "      background-image: url(:/images/home_control/add-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCOK {"
        "      background-image: url(:/images/home_control/ok-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCOK:focus {"
        "      background-image: url(:/images/home_control/ok-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCOK:pressed {"
        "      background-image: url(:/images/home_control/ok-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCPlay {"
        "      background-image: url(:/images/home_control/play-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCPlay:focus {"
        "      background-image: url(:/images/home_control/play-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCPlay:pressed {"
        "      background-image: url(:/images/home_control/play-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCPause {"
        "      background-image: url(:/images/home_control/pause-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCPause:focus {"
        "      background-image: url(:/images/home_control/pause-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCPause:pressed {"
        "      background-image: url(:/images/home_control/pause-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCPrev {"
        "      background-image: url(:/images/home_control/prev-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCPrev:focus {"
        "      background-image: url(:/images/home_control/prev-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCPrev:pressed {"
        "      background-image: url(:/images/home_control/prev-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCNext {"
        "      background-image: url(:/images/home_control/next-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCNext:focus {"
        "      background-image: url(:/images/home_control/next-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCNext:pressed {"
        "      background-image: url(:/images/home_control/next-2.png);"
		"  }";

	/*strStyleSheet += "  QDialog#LightControlDialog {"
        "      background-image: url(:/images/home_control/background-1.png);"
		"  }";
	*/
	strStyleSheet += "  QFrame#frameHC1 {"
        "     background-image: url(:/images/home_control/panel-1.png);"
		"	  border: 0px;"
		"  }";

	strStyleSheet += "  QFrame#frameHC2 {"
        "     background-image: url(:/images/home_control/panel-2.png);"
		"	  border: 0px;"
		"  }";

	strStyleSheet += "  QFrame#frameHCCurtain {"
        "     background-image: url(:/images/home_control/panel-curtain.png);"
		"	  border: 0px;"
		"  }";

	strStyleSheet += "  QFrame#frameHCDeviceType {"
        "     background-image: url(:/images/home_control/panel-device_type.png);"
		"	  border: 0px;"
		"  }";

	//Graphics view

    strStyleSheet += "  QFrame#framePanel_video {"
        "     background-image: url(:/images/main/panel_small.png);"
        "     border: 0px;"
        "  }";

	strStyleSheet += "  QFrame#frameVideoShow {"
		"      background-color: #000000;"
		"      border: 0px;"
		"  }";

	strStyleSheet += "  QFrame#frameFullshow {"
		"      background-color: #000000;"
		"      border: 16px solid #E0E0E0;"//#C0E0F0;"
		"  }";

	strStyleSheet += "  QFrame#frameVersionSetting {"
        "      background-image:  url(:/images/system_setting/version_table.png);"
		"  }";

	strStyleSheet += "  QFrame#framePasswordSetting {"
        "      background-image:  url(:/images/system_setting/password_table.png);"
		"  }";

	strStyleSheet += "  QFrame#frameSecuritySetting {"
        "      background-image:  url(:/images/system_setting/security_table.png);"
		"  }";

	strStyleSheet += "  QFrame#frameDateTimeSetting {"
        "      background-image:  url(:/images/system_setting/datetime_table.png);"
		"  }";

	strStyleSheet += "  QFrame#frameDoorMachineSetting {"
        "      background-image:  url(:/images/system_setting/doormachine_table.png);"
		"  }";

	strStyleSheet += "  QFrame#frameIpCameraSetting {"
        "      background-image:  url(:/images/system_setting/ipcamera_table.png);"
		"  }";

	strStyleSheet += "  QFrame#frameRingAmdVolumeSetting {"
        "      background-image:  url(:/images/system_setting/address_table.png);"
		"  }";

	strStyleSheet += "  QFrame#frameOtherSetting {"
        "      background-image:  url(:/images/system_setting/address_table.png);"
		"  }";

	strStyleSheet += "  QFrame#frameAddressSetting {"
        "      background-image:  url(:/images/system_setting/address_table.png);"
		"  }";

    strStyleSheet += "  QFrame#frameDefenseSetting {"
        "      background-image:  url(:/images/system_setting/defense_table.png);"
        "  }";


	//Scroll bar
	strStyleSheet += "QScrollBar:vertical {"
		"      border:0.5px solid #dadbde;"
		"      background-color:#c0c0c0;"
		"      width: 26px;"
		"      margin: 22px 0 22px 0;"
		"  }"
		"  QScrollBar::handle:vertical {"
		"      border: 1px solid #808080;"
		"	   background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,"
		"          stop: 0 #F0FFFF, stop: 1 #C6E2FF);"
		"      min-height: 20px;"
		"  }"
		"  QScrollBar::add-line:vertical {"
		"      border: 1px solid #808080;"
		"      background: #C6E2FF;"
		"      height: 26px;"
		"      subcontrol-position: bottom;"
		"      subcontrol-origin: margin;"
		"  }"
		"  QScrollBar::sub-line:vertical {"
		"      border: 1px solid #808080;"
		"      background: #C6E2FF 1029890972;"
		"      height: 26px;"
		"      subcontrol-position: top;"
		"      subcontrol-origin: margin;"
		"  }"
		"  QScrollBar:up-arrow:vertical {"
		"	     border: 1px solid grey;"
		"	     width: 26px;"
		"	     height: 26px;"
        "		 border-image: url(:/images/main/scrollbar-up-1.bmp);"
		"  }"
		"  QScrollBar:up-arrow:vertical:pressed {"
		"	     border: 1px solid grey;"
		"	     width: 26px;"
		"	     height: 26px;"
        "		 border-image: url(:/images/main/scrollbar-up-2.bmp);"
		"  }"
		"  QScrollBar:down-arrow:vertical {"
		"	     border: 1px solid grey;"
		"	     width: 26px;"
		"	     height: 26px;"
        "		 border-image: url(:/images/main/scrollbar-down-1.bmp);"
		"  }"
		"  QScrollBar:down-arrow:vertical:pressed {"
		"	     border: 1px solid grey;"
		"	     width: 26px;"
		"	     height: 26px;"
        "		 border-image: url(:/images/main/scrollbar-down-2.bmp);"
		"  }"
		"  QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
		"      background: none;"
		"  }";


	strStyleSheet += "QScrollBar:horizontal {"
		"      border:0.5px solid #dadbde;"
		"      background-color:#c0c0c0;"
		"      height: 26px;"
		"      margin: 0 22px 0 22px;"
		"  }"
		"  QScrollBar::handle:horizontal {"
		"      border: 1px solid #808080;"
		"	   background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,"
		"          stop: 0 #F0FFFF, stop: 1 #C6E2FF);"
		"      min-width: 20px;"
		"  }"
		"  QScrollBar::add-line:horizontal {"
		"      border: 1px solid #808080;"
		"      background: #C6E2FF;"
		"      width: 26px;"
		"      subcontrol-position: right;"
		"      subcontrol-origin: margin;"
		"  }"
		"  QScrollBar::sub-line:horizontal {"
		"      border: 1px solid #808080;"
		"      background: #C6E2FF;"
		"      width: 26px;"
		"      subcontrol-position: left;"
		"      subcontrol-origin: margin;"
		"  }"
		"  QScrollBar:left-arrow:horizontal {"
		"	     border: 1px solid grey;"
		"	     width: 26px;"
		"	     height: 26px;"
        "		 border-image: url(:/images/main/scrollbar-left-1.bmp);"
		"  }"
		"  QScrollBar:left-arrow:horizontal:pressed {"
		"	     border: 1px solid grey;"
		"	     width: 26px;"
		"	     height: 26px;"
        "		 border-image: url(:/images/main/scrollbar-left-2.bmp);"
		"  }"
		"  QScrollBar:right-arrow:horizontal {"
		"	     border: 1px solid grey;"
		"	     width: 26px;"
		"	     height: 26px;"
        "		 border-image: url(:/images/main/scrollbar-right-1.bmp);"
		"  }"
		"  QScrollBar:right-arrow:horizontal:pressed {"
		"	     border: 1px solid grey;"
		"	     width: 26px;"
		"	     height: 26px;"
        "		 border-image: url(:/images/main/scrollbar-right-2.bmp);"
		"  }"
		"  QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {"
		"      background: none;"
		"  }";

	strStyleSheet += "  QHeaderView::section {"
		"      background-color: #FEF9F3;"
		"      border: 1px solid #C0C0C0;"
		"      border-left-width: 1px;"
		"      border-right-width: 0px;"
		"      border-top-width: 0px;"
		"      border-bottom-width: 1px;"
		"      height: 34px;"
		"      font: 20px;"
		"      color: #604010;"
		"  }";

	strStyleSheet += "  QHeaderView#tvRingFileHeader::section {"
		"      background-color: #F6F4F4;"
		"      border: 1px solid #C0C0C0;"
		"      border-left-width: 1px;"
		"      border-right-width: 0px;"
		"      border-top-width: 0px;"
		"      border-bottom-width: 1px;"
		"      height: 34px;"
		"      font: 20px;"
		"      color: #604010;"
		"  }";

	/*strStyleSheet += "  QHeaderView::up-arrow {"
        "      image: url(:/imnormal ages/main/scrollbar-up-1.bmp);"
		"  }";

	strStyleSheet += "  QHeaderView::down-arrow {"
        "      image: url(:/images/main/scrollbar-down-1.bmp);"
		"  }";
	*/
	strStyleSheet += "  QTableView {"
		"      background-color: transparent;"
		"      selection-color: #FF0000;"
		"      selection-background-color: transparent;"
		"	   border: 2px solid #C0C0C0;"
		"      border-left-width: 1px;"
		"      border-right-width: 2px;"
		//"      border-top-width: 0px;"
		//"      border-bottom-width: 1px;"
		"      color: #604010;"
		"      font: 20px;"
		"  }";

	strStyleSheet += "  QTableView::item {"
		"      border: 1px solid #C0C0C0;"
		"      border-left-width: 1px;"
		"      border-right-width: 0px;"
		"      border-top-width: 0px;"
		"      border-bottom-width: 1px;"
		//"      height: 34px;"
		"  }";

    QString tableViewMonListFontSize;
    tableViewMonListFontSize.sprintf("%d",g_pScaleSrc->get_monlist_tableViewAddressList_fonts());
    strStyleSheet += "  QTableView#tableViewMonList {"
        "      background-color: transparent;"
        "      selection-color: #FF0000;"
        "      selection-background-color: transparent;"
        "	   border: none;"//2px solid #C0C0C0;"
        "      color: #604010;"
        "      font: " + tableViewMonListFontSize + "px;"
        "  }";

    strStyleSheet += "  QTableView#tableViewMonList::item {"
        "      border: none;"
        "  }";

    QString tableViewAddressListFontSize;
    tableViewAddressListFontSize.sprintf("%d",g_pScaleSrc->get_addressbook_tableViewAddressList_fonts());
	strStyleSheet += "  QTableView#tableViewAddressList {"
		"      background-color: transparent;"
		"      selection-color: #FF0000;"
		"      selection-background-color: transparent;"
		"	   border: none;"//2px solid #C0C0C0;"
		"      color: #604010;"
        "      font: " + tableViewAddressListFontSize + "px;"
		"  }";

	strStyleSheet += "  QTableView#tableViewAddressList::item {"
		"      border: none;"
		"  }";

	strStyleSheet += "  QListView {"
		"      background-color: transparent;"
		"      selection-color: #FF0000;"
		"      selection-background-color: transparent;"
		"      font: 18px;"
		"      border: 0px solid #C0C0C0;"
		"      color: #604010;"
		"  }";

	strStyleSheet += "  QListView::item {"
		"      font: 18px;"
		"      height: 30px;"
		"  }";

	strStyleSheet += "  QSlider::groove:horizontal {"
		"      border: 1px solid #404040;"//#999999;"
		"      height: 7px;"
		"	   background-color: #C0C0C0;"
		//"      background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
		"      margin: 9px 0;"
		"  }";
	strStyleSheet += "  QSlider::handle:horizontal {"
        "      border-image: url(:/images/system_setting/slider-handle-horizontal.png);"//qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #00b4b4, stop:1 #008f8f);"
		"      border: 0px;"//5c5c5c;"
		"      width: 25px;"
		//"      width: 30px;"
		//"	   height: 25px;"
		"      margin: -9px 0;"
		//"      border-radius: 3px;"
		"  }";

	strStyleSheet += "  QSlider::groove:vertical {"
		"      border: 1px solid #404040;"//#999999;"
		"      width: 8px;"
		"      background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #B1B1B1, stop:1 #c4c4c4);"
		"      margin: 0 2px;"
		"  }";
	strStyleSheet += "  QSlider::handle:vertical {"
        "      background: url(:/images/voip_dialog/slider-handle-vertical.jpg);"//qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
		"      border: 1px solid #404040;"//#5c5c5c;"
		"      height: 18px;"
		"      margin: 0 -6px;"
		"      border-radius: 3px;"
		"  }";

	//Home control
	strStyleSheet += "  QSlider#horizontalSliderBrightnessAdjustment::groove:horizontal {"
		"	   background-color: transparent;"
		"      height: 30px;"
		"	   border: 0px;"
		"  }";
	strStyleSheet += "  QSlider#horizontalSliderBrightnessAdjustment::handle:horizontal {"
        "      background-image: url(:/images/home_control/slider-handle.png);"
		"      border-image: url();"
		"      border: 0px;"
		"      width: 30px;"
		"      margin: 0px 0px;"
		"  }";

	strStyleSheet += "  QSlider#horizontalSliderBrightnessAdjustment::add-page:horizontal {"
        "      border-image: url(:/images/home_control/slider-add.png) -8 12 0 2 repeat;"
		"      border-top: 0px transparent;"
		"      border-bottom: 0px transparent;"
		"	   border-right: 12px transparent;"
		"	   border-left: 2px transparent;"
		"  }";

	strStyleSheet += "  QSlider#horizontalSliderBrightnessAdjustment::sub-page:horizontal {"
        "      border-image: url(:/images/home_control/slider-sub.png) -8 2 0 12 repeat;"
		"      border-top: 0px transparent;"
		"      border-bottom: 0px transparent;"
		"	   border-right: 2px transparent;"
		"	   border-left: 12px transparent;"
		"  }";

    //**************input method************************************/
    //按键
    strStyleSheet += "  QToolButton#btnchangeCHINESE{"
            "      border-image: url(:/images/keyboard/qiehuan-1.png);"
            "  }";
    strStyleSheet += "  QToolButton#btnchangeCHINESE:pressed {"
            "      border-image: url(:/images/keyboard/qiehuan-2.png);"
            "  }";

    strStyleSheet += "  QToolButton#btnchangeENGLISH{"
            "      border-image: url(:/images/keyboard/qiehuan-1.png);"
            "  }";
    strStyleSheet += "  QToolButton#btnchangeENGLISH:pressed {"
            "      border-image: url(:/images/keyboard/qiehuan-2.png);"
            "  }";
    strStyleSheet += "  QToolButton#btnCaps{"
            "      border-image: url(:/images/keyboard/daxie-1.png);"
            "  }";
    strStyleSheet += "  QToolButton#btnCaps:pressed {"
            "      border-image: url(:/images/keyboard/daxie-2.png);"
            "  }";
    strStyleSheet += "  QToolButton#btn_space{"
            "      border-image: url(:/images/keyboard/anniu3-1.png);"
            "  }";
    strStyleSheet += "  QToolButton#btn_space:pressed {"
            "      border-image: url(:/images/keyboard/anniu3-2.png);"
            "  }";
    strStyleSheet += "  QToolButton#btnEnter{"
            "      border-image: url(:/images/keyboard/huanhang-1.png);"
            "  }";
    strStyleSheet += "  QToolButton#btnEnter:pressed {"
            "      border-image: url(:/images/keyboard/huanhang-2.png);"
            "  }";
    strStyleSheet += "  QToolButton#btnBackspace{"
            "      border-image: url(:/images/keyboard/delete-1.png);"
            "  }";
    strStyleSheet += "  QToolButton#btnBackspace:pressed {"
            "      border-image: url(:/images/keyboard/delete-2.png);"
            "  }";

    strStyleSheet += "  QToolButton#btnPeriod {"
            "      border-image: url(:/images/keyboard/-1.png);"
            "  }";
    strStyleSheet += "  QToolButton#btnPeriod:pressed {"
            "      border-image: url(:/images/keyboard/-2.png);"
            "  }";

    strStyleSheet += "  QToolButton#btn_delete{"
            "      border-image: url(:/images/keyboard/delete-1.png);"
            "  }";
    strStyleSheet += "  QToolButton#btn_delete:pressed {"
            "      border-image: url(:/images/keyboard/delete-2.png);"
            "  }";
    strStyleSheet += "  QToolButton#btn_caps{"
            "      border-image: url(:/images/keyboard/daxie-1.png);"
            "  }";
    strStyleSheet += "  QToolButton#btn_caps:pressed {"
            "      border-image: url(:/images/keyboard/daxie-2.png);"
            "  }";
    strStyleSheet += "  QToolButton#btn_hide{"
            "      border-image: url(:/images/keyboard/4-1.png);"
            "  }";
    strStyleSheet += "  QToolButton#btn_hide:pressed {"
            "      border-image: url(:/images/keyboard/4-2.png);"
            "  }";
    strStyleSheet += "  QLabel#labelCHINESE {"
            "      border-image: url(:/images/keyboard/jian.png);"
            "  }";
    strStyleSheet += "  QLabel#labelENGLISH {"
            "      border-image: url(:/images/keyboard/ying.png);"
            "  }";
    /*****************************输入法面板  end  *********************************/

    //**************inputinfodialog************************************/
    strStyleSheet += "  QDialog#InputInfoDialog {"
            "     border-image: url(:/images/keyboard/bg.png);"
            "  }";
    strStyleSheet += "  QLabel#PicLabel {"
            "      border-image: url(:/images/keyboard/shurukuang.png);"
            "  }";

    strStyleSheet += "  QPushButton#InputInfoButtonConfirm {"
            "      background-image: url(:/images/keyboard/confirm1-1.png);"
            "      background-color: transparent;"
            "      font:bold 21 px;"
            "      color: #582F00;"
            "  }";
    strStyleSheet += "  QPushButton#InputInfoButtonConfirm:pressed {"
            "      background-image: url(:/images/keyboard/confirm1-2.png);"
            "      background-color: transparent;"
            "      font:bold 21 px;"
            "      color: #AB977F;"
            "  }";
    strStyleSheet += "  QPushButton#InputInfoButtonCancel {"
            "      background-image: url(:/images/keyboard/cancel1-1.png);"
            "      background-color: transparent;"
            "      font:bold 21 px;"
            "      color: #582F00;"
            "  }";
    strStyleSheet += "  QPushButton#InputInfoButtonCancel:pressed {"
            "      background-image: url(:/images/keyboard/cancel1-2.png);"
            "      background-color: transparent;"
            "      font:bold 21 px;"
            "      color: #AB977F;"
            "  }";

    /*****************************inputinfodialog  end  *********************************/
	return strStyleSheet;
}
