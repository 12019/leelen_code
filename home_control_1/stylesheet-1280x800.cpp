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
QString getStyleSheet_1280x800(QString strLang)
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
                "      border-image: url(" + QString(g_appBackImagePath) + ");"
                "  }";
    */

    strStyleSheet = "  QDialog#MainDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    /*strStyleSheet += "  QDialog#VoipMainDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#InfoManMainDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#InfoManDetailDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#InfoWriteDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#InfoManDetailDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#RingFileSetDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#SecurityMainDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#DatetimeSetDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#DeviceSetMainDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#LanguageSetDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#PasswordSetDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#IstDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#AddDefenseDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#SysSetPasswordDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#PhotoUDiskDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#MediaPlayerDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#IctRcvHookDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#InfoManViewAttachmentDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#RingAndVolumeDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#SecurityParamSetDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#SecurityAbnormalDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#SecurityPasswordDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#SecurityPlayerDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#SecurityAlertDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";

    strStyleSheet += "  QDialog#MonDialog{"
            "      border-image: url(" + QString(g_appBackImagePath) + ");"
            "  }";
    */

    //QPushButton //默认按键大小和颜色style在这里统一设置，其他按键需要特别设置的在按键处单独设置  add by lidh 20141101
    strStyleSheet += " QPushButton {"
            "      border-image: url(:/images/main/num-1.png);"
            "      border-radius: 7px;"
            "      color: " + strBtnColor + ";"
            "      font: " + strBtnFontSize + "px;"
            "	   background-repeat: no-repeat;"
            "      background-position: center;"
            "  }";
    strStyleSheet += " QPushButton:focus {"
            "      border-image: url(:/images/main/num-2.png);"
            "      border-radius: 7px;"
            "      color: " + strBtnFocusColor + ";"
            "      font: " + strBtnFontSize + "px;"
            "	   background-repeat: no-repeat;"
            "      background-position: center;"
            "  }";
    strStyleSheet += " QPushButton:pressed {"
            "      border-image: url(:/images/main/num-2.png);"
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
            "     border-image: url(:/images/main/panel.png);"
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
            "    color: #FFFFFF;"
            "}";

    cf.sprintf("%d",df);
    strStyleSheet += "QLabel#labelDay {"
            "    font: "+cf+"px bold;"
            "    font-family: \"Tahoma\";"
            "    color: #FFFFFF;"
            "}";

    cf.sprintf("%d",wf);
    strStyleSheet += "QLabel#labelWeek {"
            "    font: "+cf+"px bold;"
            "    font-family: \"FZLanTingHei-R-GBK\";"
            "    color: #FFFFFF;"
            "}";

    cf.sprintf("%d",llf);
    strStyleSheet += "QLabel#labelNongli {"
            "    font: "+cf+"px bold;"
            "    font-family: \"FZLanTingHei-R-GBK\";"
            "    color: #FFFFFF;"
            "}";

    cf.sprintf("%d",jjf);
    strStyleSheet += "QLabel#labelJieriJieqi {"
            "    font: "+cf+"px bold;"
            "    font-family: \"FZLanTingHei-R-GBK\";"
            "    color: #FFFFFF;"
            "}";

    QString s;
    s.sprintf("%d",g_pScaleSrc->main_maindialog_tableViewNotifier());

    strStyleSheet += "  QTableView#tableViewNotifier {"
            "      border-image: url(:/images/main/paperbox.png);"
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
            "      border-image: url(:/images/main/voip-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonVoip:focus {"
            "      border-image: url(:/images/main/voip-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "	}";
    strStyleSheet += " QPushButton#pushButtonVoip:pressed {"
            "      border-image: url(:/images/main/voip-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonTerminalHelp {"
            "      border-image: url(:/images/main/terminal_help-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonTerminalHelp:focus {"
            "      border-image: url(:/images/main/terminal_help-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "	}";
    strStyleSheet += " QPushButton#pushButtonTerminalHelp:pressed {"
            "      border-image: url(:/images/main/terminal_help-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonIntercomMan {"
            "      border-image: url(:/images/main/intercom_man-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIntercomMan:focus {"
            "      border-image: url(:/images/main/intercom_man-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "	}";
    strStyleSheet += " QPushButton#pushButtonIntercomMan:pressed {"
            "      border-image: url(:/images/main/intercom_man-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonCallCenter {"
            "      border-image: url(:/images/main/call_center-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenter:focus {"
            "      border-image: url(:/images/main/call_center-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "	}";
    strStyleSheet += " QPushButton#pushButtonCallCenter:pressed {"
            "      border-image: url(:/images/main/call_center-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonInterFreeSetting {"
            "      border-image: url(:/images/main/interfree_setting-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInterFreeSetting:focus {"
            "      border-image: url(:/images/main/interfree_setting-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInterFreeSetting:pressed {"
            "      border-image: url(:/images/main/interfree_setting-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonSystemSetting {"
            "      border-image: url(:/images/main/system_setting-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSystemSetting:focus {"
            "      border-image: url(:/images/main/system_setting-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSystemSetting:pressed {"
            "      border-image: url(:/images/main/system_setting-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonVideoMon {"
            "      border-image: url(:/images/main/video_mon-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonVideoMon:focus {"
            "      border-image: url(:/images/main/video_mon-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonVideoMon:pressed {"
            "      border-image: url(:/images/main/video_mon-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonInfoMan {"
            "      border-image: url(:/images/main/info_man-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoMan:focus {"
            "      border-image: url(:/images/main/info_man-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoMan:pressed {"
            "      border-image: url(:/images/main/info_man-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonSecurityMan {"
            "      border-image: url(:/images/main/security_man-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecurityMan:focus {"
            "      border-image: url(:/images/main/security_man-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecurityMan:pressed {"
            "      border-image: url(:/images/main/security_man-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonCallElevator {"
            "      border-image: url(:/images/main/call_elevator-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevator:focus {"
            "      border-image: url(:/images/main/call_elevator-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevator:pressed {"
            "      border-image: url(:/images/main/call_elevator-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonPhoto {"
            "      border-image: url(:/images/main/photo-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhoto:focus {"
            "      border-image: url(:/images/main/photo-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhoto:pressed {"
            "      border-image: url(:/images/main/photo-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonHomeControl {"
            "      border-image: url(:/images/main/home_control-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonHomeControl:focus {"
            "      border-image: url(:/images/main/home_control-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonHomeControl:pressed {"
            "      border-image: url(:/images/main/home_control-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonSendImg {"
            "      border-image: url(:/images/main/sendImg-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSendImg:focus {"
            "      border-image: url(:/images/main/sendImg-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSendImg:pressed {"
            "      border-image: url(:/images/main/sendImg-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonOther {"
            "      border-image: url(:/images/main/other-1.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonOther:focus {"
            "      border-image: url(:/images/main/other-2.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonOther:pressed {"
            "      border-image: url(:/images/main/other-3.png);"
            "      font: " + strmaindialogBtnFontSize + "px;"
            "      font-family: \"FZLanTingHei-R-GBK\";"
            "      color: #FFFFFF ;"
            "      font: bold;"
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
            "      border-image: url(:/" + strPath + "ok-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMyMessageBoxOK:focus {"
            "      border-image: url(:/" + strPath + "ok-1.png);"
            "	}";
    strStyleSheet += " QPushButton#pushButtonMyMessageBoxOK:pressed {"
            "      border-image: url(:/" + strPath + "ok-2.png);"
            "  }";

    //pushButtonMyMessageBoxCancel
    strStyleSheet += " QPushButton#pushButtonMyMessageBoxCancel {"
            "      border-image: url(:/" + strPath + "cancel-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMyMessageBoxCancel:focus {"
            "      border-image: url(:/" + strPath + "cancel-1.png);"
            "	}";
    strStyleSheet += " QPushButton#pushButtonMyMessageBoxCancel:pressed {"
            "      border-image: url(:/" + strPath + "cancel-2.png);"
            "  }";

    /************************************************************
    描述：info_man/Infomanmaindialog界面style
    ************************************************************/
    strPath = "images/info_man/";
    //pushButtonInfoManMainWrite
    strStyleSheet += " QPushButton#pushButtonInfoManMainWrite {"
            "      border-image: url(:/" + strPath + "write-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainWrite:focus {"
            "      border-image: url(:/" + strPath + "write-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainWrite:pressed {"
            "      border-image: url(:/" + strPath + "write-2.png);"
            "  }";

    //pushButtonInfoManMainDel
    strStyleSheet += " QPushButton#pushButtonInfoManMainDel {"
            "      border-image: url(:/" + strPath + "delete-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainDel:focus {"
            "      border-image: url(:/" + strPath + "delete-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainDel:pressed {"
            "      border-image: url(:/" + strPath + "delete-2.png);"
            "  }";

    //pushButtonInfoManMainDetail
    strStyleSheet += " QPushButton#pushButtonInfoManMainDetail {"
            "      border-image: url(:/" + strPath + "detail-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainDetail:focus {"
            "      border-image: url(:/" + strPath + "detail-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainDetail:pressed {"
            "      border-image: url(:/" + strPath + "detail-2.png);"
            "  }";

    //pushButtonInfoManMainDelAll
    strStyleSheet += " QPushButton#pushButtonInfoManMainDelAll {"
            "      border-image: url(:/" + strPath + "deleteall-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainDelAll:focus {"
            "      border-image: url(:/" + strPath + "deleteall-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainDelAll:pressed {"
            "      border-image: url(:/" + strPath + "deleteall-2.png);"
            "  }";

    //pushButtonInfoManMainPrev
    strStyleSheet += " QPushButton#pushButtonInfoManMainPrev {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainPrev:focus {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainPrev:pressed {"
            "      border-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonInfoManMainNext
    strStyleSheet += " QPushButton#pushButtonInfoManMainNext {"
            "      border-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainNext:focus {"
            "      border-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainNext:pressed {"
            "      border-image: url(:/" + strPath + "next-2.png);"
            "  }";

    //pushButtonInfoManMainReturn
    strStyleSheet += " QPushButton#pushButtonInfoManMainReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManMainReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：info_man/Infowritedialog界面style
    ************************************************************/
    strPath = "images/info_man/";
    //pushButtonInfoWriteAddress
    strStyleSheet += " QPushButton#pushButtonInfoWriteAddress {"
            "      border-image: url(:/" + strPath + "address-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoWriteAddress:focus {"
            "      border-image: url(:/" + strPath + "address-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoWriteAddress:pressed {"
            "      border-image: url(:/" + strPath + "address-2.png);"
            "  }";

    //pushButtonInfoWriteSend
    strStyleSheet += " QPushButton#pushButtonInfoWriteSend {"
            "      border-image: url(:/" + strPath + "send-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoWriteSend:focus {"
            "      border-image: url(:/" + strPath + "send-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoWriteSend:pressed {"
            "      border-image: url(:/" + strPath + "send-2.png);"
            "  }";

    //pushButtonInfoWriteReturn
    strStyleSheet += " QPushButton#pushButtonInfoWriteReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoWriteReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoWriteReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：security/securitymaindialog界面style
    ************************************************************/
    strPath = "images/security/";
    //pushButtonSecMainDel
    strStyleSheet += " QPushButton#pushButtonSecMainDel {"
            "      border-image: url(:/" + strPath + "delete-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainDel:focus {"
            "      border-image: url(:/" + strPath + "delete-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainDel:pressed {"
            "      border-image: url(:/" + strPath + "delete-2.png);"
            "  }";

    //pushButtonSecMainDetail
    strStyleSheet += " QPushButton#pushButtonSecMainDetail {"
            "      border-image: url(:/" + strPath + "detail-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainDetail:focus {"
            "      border-image: url(:/" + strPath + "detail-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainDetail:pressed {"
            "      border-image: url(:/" + strPath + "detail-2.png);"
            "  }";

    //pushButtonSecMainDelAll
    strStyleSheet += " QPushButton#pushButtonSecMainDelAll {"
            "      border-image: url(:/" + strPath + "deleteall-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainDelAll:focus {"
            "      border-image: url(:/" + strPath + "deleteall-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainDelAll:pressed {"
            "      border-image: url(:/" + strPath + "deleteall-2.png);"
            "  }";

    //pushButtonSecMainSetArm
    strStyleSheet += " QPushButton#pushButtonSecMainSetArm {"
            "      border-image: url(:/" + strPath + "setarm-1.png);"
            "      border-radius: 7px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainSetArm:focus {"
            "      border-image: url(:/" + strPath + "setarm-1.png);"
            "      border-radius: 7px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainSetArm:pressed {"
            "      border-image: url(:/" + strPath + "setarm-2.png);"
            "      border-radius: 7px;"
            "  }";

    //pushButtonSecMainReturn
    strStyleSheet += " QPushButton#pushButtonSecMainReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonSecMainPrev
    strStyleSheet += " QPushButton#pushButtonSecMainPrev {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainPrev:focus {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainPrev:pressed {"
            "      border-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonSecMainNext
    strStyleSheet += " QPushButton#pushButtonSecMainNext {"
            "      border-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainNext:focus {"
            "      border-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecMainNext:pressed {"
            "      border-image: url(:/" + strPath + "next-2.png);"
            "  }";

    /************************************************************
    描述：security/securityabnormaldialog界面style
    ************************************************************/
    strPath = "images/security/";
    //pushButtonSecAbnormalHome
    strStyleSheet += " QPushButton#pushButtonSecAbnormalHome {"
            "      border-image: url(:/" + strPath + "home-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalHome:focus {"
            "      border-image: url(:/" + strPath + "home-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalHome:pressed {"
            "      border-image: url(:/" + strPath + "home-2.png);"
            "  }";

    //pushButtonSecAbnormalOut
    strStyleSheet += " QPushButton#pushButtonSecAbnormalOut {"
            "      border-image: url(:/" + strPath + "out-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalOut:focus {"
            "      border-image: url(:/" + strPath + "out-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalOut:pressed {"
            "      border-image: url(:/" + strPath + "out-2.png);"
            "  }";

    //pushButtonSecAbnormalPrev
    strStyleSheet += " QPushButton#pushButtonSecAbnormalPrev {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalPrev:focus {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalPrev:pressed {"
            "      border-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonSecAbnormalNext
    strStyleSheet += " QPushButton#pushButtonSecAbnormalNext {"
            "      border-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalNext:focus {"
            "      border-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalNext:pressed {"
            "      border-image: url(:/" + strPath + "next-2.png);"
            "  }";

    //pushButtonSecAbnormalReturn(返回键)
    strStyleSheet += " QPushButton#pushButtonSecAbnormalReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonSecAbnormalDisArm
    strStyleSheet += " QPushButton#pushButtonSecAbnormalDisArm {"
            "      border-image: url(:/" + strPath + "disarm-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalDisArm:focus {"
            "      border-image: url(:/" + strPath + "disarm-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAbnormalDisArm:pressed {"
            "      border-image: url(:/" + strPath + "disarm-2.png);"
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
            "      border-image: url(:/" + strPath + "back-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdBack:focus {"
            "      border-image: url(:/" + strPath + "back-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPasswdBack:pressed {"
            "      border-image: url(:/" + strPath + "back-2.png);"
            "  }";

    //pushButtonSecPwdOK
    strStyleSheet += " QPushButton#pushButtonSecPwdOK {"
            "      border-image: url(:/" + strPath + "ok-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdOK:focus {"
            "      border-image: url(:/" + strPath + "ok-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdOK:pressed {"
            "      border-image: url(:/" + strPath + "ok-2.png);"
            "  }";

    //pushButtonSecPwdReturn
    strStyleSheet += " QPushButton#pushButtonSecPwdReturn {"
            "      border-image: url(:/" + strPath + "return2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdReturn:focus {"
            "      border-image: url(:/" + strPath + "return2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdReturn:pressed {"
            "      border-image: url(:/" + strPath + "return2-2.png);"
            "  }";
    //pushButtonSecPwdClear
    strStyleSheet += " QPushButton#pushButtonSecPwdClear {"
            "      border-image: url(:/" + strPath + "clear-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdClear:focus {"
            "      border-image: url(:/" + strPath + "clear-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdClear:pressed {"
            "      border-image: url(:/" + strPath + "clear-2.png);"
            "  }";

    //pushButtonSecPwdNum
    strStyleSheet += " QPushButton#pushButtonSecPwdNum0 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum0:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum0:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum1 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum1:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum1:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum2 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum2:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum2:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum3 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum3:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum3:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum4 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum4:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum4:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum5 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum5:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum5:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum6 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum6:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum6:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum7 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum7:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum7:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonSecPwdNum8 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum8:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum8:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonSecPwdNum9 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum9:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdNum9:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdDot {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdDot:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdDot:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdShop {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdShop:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPwdShop:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";

    /************************************************************
    描述：voip/Mondialog界面style
    ************************************************************/
    strPath = "images/voip/";
    //pushButtonMonList
    strStyleSheet += " QPushButton#pushButtonMonList {"
            "      border-image: url(:/" + strPath + "detail2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonList:focus {"
            "      border-image: url(:/" + strPath + "detail2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonList:pressed {"
            "      border-image: url(:/" + strPath + "detail2-2.png);"
            "  }";

    //pushButtonMonOpen
    strStyleSheet += " QPushButton#pushButtonMonOpen {"
            "      border-image: url(:/" + strPath + "open2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonOpen:focus {"
            "      border-image: url(:/" + strPath + "open2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonOpen:pressed {"
            "      border-image: url(:/" + strPath + "open2-2.png);"
            "  }";

    //pushButtonMonHangUp
    strStyleSheet += " QPushButton#pushButtonMonHangUp {"
            "      border-image: url(:/" + strPath + "hangup2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonHangUp:focus {"
            "      border-image: url(:/" + strPath + "hangup2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonHangUp:pressed {"
            "      border-image: url(:/" + strPath + "hangup2-2.png);"
            "  }";

    //pushButtonMonDial
    strStyleSheet += " QPushButton#pushButtonMonDial {"
            "      border-image: url(:/" + strPath + "connect2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonDial:focus {"
            "      border-image: url(:/" + strPath + "connect2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonDial:pressed {"
            "      border-image: url(:/" + strPath + "connect2-2.png);"
            "  }";

    //pushButtonMonReturn
    strStyleSheet += " QPushButton#pushButtonMonReturn {"
            "      border-image: url(:/" + strPath + "return2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonReturn:focus {"
            "      border-image: url(:/" + strPath + "return2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonReturn:pressed {"
            "      border-image: url(:/" + strPath + "return2-2.png);"
            "  }";

    /************************************************************
    描述：intercom_man/mediamandialog界面style
    ************************************************************/
    strPath = "images/intercom_man/";
    strStyleSheet += " QPushButton#pushButtonMediaManDelAll {"
            "      border-image: url(:/" + strPath + "deleteall-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManDelAll:focus {"
            "      border-image: url(:/" + strPath + "deleteall-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManDelAll:pressed {"
            "      border-image: url(:/" + strPath + "deleteall-2.png);"
            "  }";

    //pushButtonMediaManDetail
    strStyleSheet += " QPushButton#pushButtonMediaManDetail {"
            "      border-image: url(:/" + strPath + "detail-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManDetail:focus{"
            "      border-image: url(:/" + strPath + "detail-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManDetail:pressed {"
            "      border-image: url(:/" + strPath + "detail-2.png);"
            "  }";

    //pushButtonMediaManDel
    strStyleSheet += " QPushButton#pushButtonMediaManDel {"
            "      border-image: url(:/" + strPath + "delete-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManDel:focus {"
            "      border-image: url(:/" + strPath + "delete-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManDel:pressed {"
            "      border-image: url(:/" + strPath + "delete-2.png);"
            "  }";

    //pushButtonMediaManPrev
    strStyleSheet += " QPushButton#pushButtonMediaManPrev {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManPrev:focus {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManPrev:pressed {"
            "      border-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonMediaManNext
    strStyleSheet += " QPushButton#pushButtonMediaManNext {"
            "      border-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManNext:focus {"
            "      border-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManNext:pressed {"
            "      border-image: url(:/" + strPath + "next-2.png);"
            "  }";

    //pushButtonMediaManReturn
    strStyleSheet += " QPushButton#pushButtonMediaManReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaManReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：call_center/callcenterdialog界面style
    ************************************************************/
    strPath = "images/call_center/";
    //pushButtonCallCenterAnswer
    strStyleSheet += " QPushButton#pushButtonCallCenterAnswer {"
            "      border-image: url(:/" + strPath + "answer-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterAnswer:focus {"
            "      border-image: url(:/" + strPath + "answer-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterAnswer:pressed {"
            "      border-image: url(:/" + strPath + "answer-2.png);"
            "  }";

    //pushButtonCallCenterHangUp
    strStyleSheet += " QPushButton#pushButtonCallCenterHangUp {"
            "      border-image: url(:/" + strPath + "hangup-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterHangUp:focus {"
            "      border-image: url(:/" + strPath + "hangup-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterHangUp:pressed {"
            "      border-image: url(:/" + strPath + "hangup-2.png);"
            "  }";

    //pushButtonCallCenterVideoOpen
    strStyleSheet += " QPushButton#pushButtonCallCenterVideoOpen {"
            "      border-image: url(:/" + strPath + "videoopen-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterVideoOpen:focus {"
            "      border-image: url(:/" + strPath + "videoopen-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterVideoOpen:pressed {"
            "      border-image: url(:/" + strPath + "videoopen-2.png);"
            "  }";

    //pushButtonCallCenterVideoClose
    strStyleSheet += " QPushButton#pushButtonCallCenterVideoClose {"
            "      border-image: url(:/" + strPath + "videoclose-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterVideoClose:focus {"
            "      border-image: url(:/" + strPath + "videoclose-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterVideoClose:pressed {"
            "      border-image: url(:/" + strPath + "videoclose-2.png);"
            "  }";

    //pushButtonCallCenterDial
    strStyleSheet += " QPushButton#pushButtonCallCenterDial {"
            "      border-image: url(:/" + strPath + "dial-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterDial:focus {"
            "      border-image: url(:/" + strPath + "dial-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterDial:pressed {"
            "      border-image: url(:/" + strPath + "dial-2.png);"
            "  }";

    //pushButtonCallCenterRecord
    strStyleSheet += " QPushButton#pushButtonCallCenterRecord {"
            "      border-image: url(:/" + strPath + "record-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterRecord:focus {"
            "      border-image: url(:/" + strPath + "record-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterRecord:pressed {"
            "      border-image: url(:/" + strPath + "record-2.png);"
            "  }";

    //pushButtonCallCenterReturn
    strStyleSheet += " QPushButton#pushButtonCallCenterReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallCenterReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：photo/photomaindialog界面style
    ************************************************************/
    strPath = "images/photo/";
    //pushButtonPhotoMainNext
    strStyleSheet += " QPushButton#pushButtonPhotoMainNext {"
            "      border-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainNext:focus {"
            "      border-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainNext:pressed {"
            "      border-image: url(:/" + strPath + "next-2.png);"
            "  }";

    //pushButtonPhotoMainPrev
    strStyleSheet += " QPushButton#pushButtonPhotoMainPrev {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainPrev:focus {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainPrev:pressed {"
            "      border-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonPhotoMainImport
    strStyleSheet += " QPushButton#pushButtonPhotoMainImport {"
            "      border-image: url(:/" + strPath + "import-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainImport:focus {"
            "      border-image: url(:/" + strPath + "import-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainImport:pressed {"
            "      border-image: url(:/" + strPath + "import-2.png);"
            "  }";

    //pushButtonPhotoMainExport
    strStyleSheet += " QPushButton#pushButtonPhotoMainExport {"
            "      border-image: url(:/" + strPath + "export-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainExport:focus {"
            "      border-image: url(:/" + strPath + "export-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainExport:pressed {"
            "      border-image: url(:/" + strPath + "export-2.png);"
            "  }";

    //pushButtonPhotoMainDel
    strStyleSheet += " QPushButton#pushButtonPhotoMainDel {"
            "      border-image: url(:/" + strPath + "delete-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainDel:focus {"
            "      border-image: url(:/" + strPath + "delete-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainDel:pressed {"
            "      border-image: url(:/" + strPath + "delete-2.png);"
            "  }";

    //pushButtonPhotoMainCheck
    strStyleSheet += " QPushButton#pushButtonPhotoMainCheck {"
            "      border-image: url(:/" + strPath + "check-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainCheck:focus {"
            "      border-image: url(:/" + strPath + "check-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainCheck:pressed {"
            "      border-image: url(:/" + strPath + "check-2.png);"
            "  }";

    //pushButtonPhotoMainReturn
    strStyleSheet += " QPushButton#pushButtonPhotoMainReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoMainReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：photo/photoudiskdialog界面style
    ************************************************************/
    strPath = "images/photo/";
    //pushButtonPhotoUDiskNext
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskNext {"
            "      border-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskNext:focus {"
            "      border-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskNext:pressed {"
            "      border-image: url(:/" + strPath + "next-2.png);"
            "  }";

    //pushButtonPhotoUDiskPrev
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskPrev {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskPrev:focus {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskPrev:pressed {"
            "      border-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonPhotoUDiskImport
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskImport {"
            "      border-image: url(:/" + strPath + "import-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskImport:focus {"
            "      border-image: url(:/" + strPath + "import-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskImport:pressed {"
            "      border-image: url(:/" + strPath + "import-2.png);"
            "  }";

    //pushButtonPhotoUDiskCheck
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskCheck {"
            "      border-image: url(:/" + strPath + "check-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskCheck:focus {"
            "      border-image: url(:/" + strPath + "check-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskCheck:pressed {"
            "      border-image: url(:/" + strPath + "check-2.png);"
            "  }";

    //pushButtonPhotoUDiskReturn
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPhotoUDiskReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
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
            "      border-image: url(:/" + strPath + "back-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdBack:focus {"
            "      border-image: url(:/" + strPath + "back-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdBack:pressed {"
            "      border-image: url(:/" + strPath + "back-2.png);"
            "  }";

    //pushButtonSysPwdClear
    strStyleSheet += " QPushButton#pushButtonSysPwdClear {"
            "      border-image: url(:/" + strPath + "clear-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdClear:focus {"
            "      border-image: url(:/" + strPath + "clear-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdClear:pressed {"
            "      border-image: url(:/" + strPath + "clear-2.png);"
            "  }";

    //pushButtonSysPwdReturn
    strStyleSheet += " QPushButton#pushButtonSysPwdReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonSysPwdOK
    strStyleSheet += " QPushButton#pushButtonSysPwdOK {"
            "      border-image: url(:/" + strPath + "ok-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdOK:focus {"
            "      border-image: url(:/" + strPath + "ok-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdOK:pressed {"
            "      border-image: url(:/" + strPath + "ok-2.png);"
            "  }";

    //pushButtonSysPwdNum
    strStyleSheet += " QPushButton#pushButtonSysPwdNum0 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum0:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum0:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum1 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum1:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum1:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum2 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum2:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum2:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum3 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum3:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum3:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum4 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum4:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum4:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum5 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum5:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum5:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum6 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum6:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum6:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum7 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum7:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum7:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonSysPwdNum8 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum8:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum8:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonSysPwdNum9 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum9:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdNum9:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdDot {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdDot:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdDot:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdShop {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdShop:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysPwdShop:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
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
            "      border-image: url(:/" + strPath + "back2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetBack:focus {"
            "      border-image: url(:/" + strPath + "back2-2.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetBack:pressed {"
            "      border-image: url(:/" + strPath + "back2-2.png);"
            "  }";

    //pushButtonPwdSetSave
    strStyleSheet += " QPushButton#pushButtonPwdSetSave {"
            "      border-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetSave:focus {"
            "      border-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetSave:pressed {"
            "      border-image: url(:/" + strPath + "save-2.png);"
            "  }";

    //pushButtonPwdSetReturn
    strStyleSheet += " QPushButton#pushButtonPwdSetReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonPwdSetClear
    strStyleSheet += " QPushButton#pushButtonPwdSetClear {"
            "      border-image: url(:/" + strPath + "clear-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetClear:focus {"
            "      border-image: url(:/" + strPath + "clear-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetClear:pressed {"
            "      border-image: url(:/" + strPath + "clear-2.png);"
            "  }";

    //pushButtonPwdSetNum
    strStyleSheet += " QPushButton#pushButtonPwdSetNum0 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum0:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum0:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum1 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum1:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum1:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum2 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum2:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum2:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum3 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum3:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum3:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum4 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum4:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum4:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum5 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum5:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum5:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum6 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum6:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum6:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum7 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum7:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum7:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonPwdSetNum8 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum8:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum8:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonPwdSetNum9 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum9:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetNum9:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetDot {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetDot:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetDot:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetShop {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetShop:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonPwdSetShop:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
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
            "      border-image: url(:/" + strPath + "language-1.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnColor + ";"
            "  }";
    strStyleSheet += " QPushButton#pushButtonLagSetEn:focus {"
            "      border-image: url(:/" + strPath + "language-2.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnFocusColor + ";"
            "  }";
    strStyleSheet += " QPushButton#pushButtonLagSetEn:pressed {"
            "      border-image: url(:/" + strPath + "language-2.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnFocusColor + ";"
            "  }";

    //pushButtonLagSetSimpCh
    strStyleSheet += " QPushButton#pushButtonLagSetSimpCh {"
            "      border-image: url(:/" + strPath + "language-1.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnColor + ";"
            "  }";
    strStyleSheet += " QPushButton#pushButtonLagSetSimpCh:focus {"
            "      border-image: url(:/" + strPath + "language-2.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnFocusColor + ";"
            "  }";
    strStyleSheet += " QPushButton#pushButtonLagSetSimpCh:pressed {"
            "      border-image: url(:/" + strPath + "language-2.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnFocusColor + ";"
            "  }";

    //pushButtonLagSetTraCh
    strStyleSheet += " QPushButton#pushButtonLagSetTraCh {"
            "      border-image: url(:/" + strPath + "language-1.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnColor + ";"
            "  }";
    strStyleSheet += " QPushButton#pushButtonLagSetTraCh:focus {"
            "      border-image: url(:/" + strPath + "language-2.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnFocusColor + ";"
            "  }";
    strStyleSheet += " QPushButton#pushButtonLagSetTraCh:pressed {"
            "      border-image: url(:/" + strPath + "language-2.png);"
            "      font: " + strLagSetFont + "px;"
            "      color: " + strLagBtnFocusColor + ";"
            "  }";

    //pushButtonLagSetReturn
    strStyleSheet += " QPushButton#pushButtonLagSetReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "      font: " + strBtnFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonLagSetReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "      font: " + strBtnFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonLagSetReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
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
    strStyleSheet += "  QLabel#labelDateTImeSetYear {"
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
            "      border-image: url(:/" + strPath + "time-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetTime:focus {"
            "      border-image: url(:/" + strPath + "time-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetTime:pressed {"
            "      border-image: url(:/" + strPath + "time-2.png);"
            "  }";

    //pushButtonDateTimeSetBack
    strStyleSheet += " QPushButton#pushButtonDateTimeSetBack {"
            "      border-image: url(:/" + strPath + "back2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetBack:focus {"
            "      border-image: url(:/" + strPath + "back2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetBack:pressed {"
            "      border-image: url(:/" + strPath + "back2-2.png);"
            "  }";

    //pushButtonDateTimeSetClear
    strStyleSheet += " QPushButton#pushButtonDateTimeSetClear {"
            "      border-image: url(:/" + strPath + "clear-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetClear:focus {"
            "      border-image: url(:/" + strPath + "clear-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetClear:pressed {"
            "      border-image: url(:/" + strPath + "clear-2.png);"
            "  }";

    //pushButtonDateTimeSetSave
    strStyleSheet += " QPushButton#pushButtonDateTimeSetSave {"
            "      border-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetSave:focus {"
            "      border-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetSave:pressed {"
            "      border-image: url(:/" + strPath + "save-2.png);"
            "  }";

    //pushButtonDateTimeSetReturn
    strStyleSheet += " QPushButton#pushButtonDateTimeSetReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonDateTimeSetNum
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum0 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum0:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum0:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum1 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum1:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum1:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum2 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum2:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum2:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum3 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum3:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum3:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum4 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum4:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum4:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum5 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum5:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum5:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum6 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum6:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum6:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum7 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum7:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum7:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum8 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum8:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum8:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum9 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum9:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetNum9:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetDot {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetDot:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetDot:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetShop {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetShop:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDateTimeSetShop:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
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
            "      border-image: url(:/" + strPath + "calibrate2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefCalibrate:focus {"
            "      border-image: url(:/" + strPath + "calibrate2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefCalibrate:pressed {"
            "      border-image: url(:/" + strPath + "calibrate2-2.png);"
            "  }";

    //pushButtonAddDefReset
    strStyleSheet += " QPushButton#pushButtonAddDefReset {"
            "      border-image: url(:/" + strPath + "reset2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefReset:focus {"
            "      border-image: url(:/" + strPath + "reset2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefReset:pressed {"
            "      border-image: url(:/" + strPath + "reset2-2.png);"
            "  }";

    //pushButtonAddDefReboot
    strStyleSheet += " QPushButton#pushButtonAddDefReboot {"
            "      border-image: url(:/" + strPath + "reboot2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefReboot:focus {"
            "      border-image: url(:/" + strPath + "reboot2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefReboot:pressed {"
            "      border-image: url(:/" + strPath + "reboot2-2.png);"
            "  }";

    //pushButtonAddDefNext
    strStyleSheet += " QPushButton#pushButtonAddDefNext {"
            "      border-image: url(:/" + strPath + "next2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefNext:focus {"
            "      border-image: url(:/" + strPath + "next2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefNext:pressed {"
            "      border-image: url(:/" + strPath + "next2-2.png);"
            "  }";

    //pushButtonAddDefPrev
    strStyleSheet += " QPushButton#pushButtonAddDefPrev {"
            "      border-image: url(:/" + strPath + "prev2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefPrev:focus {"
            "      border-image: url(:/" + strPath + "prev2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefPrev:pressed {"
            "      border-image: url(:/" + strPath + "prev2-2.png);"
            "  }";

    //pushButtonAddDefDel
    strStyleSheet += " QPushButton#pushButtonAddDefDel {"
            "      border-image: url(:/" + strPath + "delete2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefDel:focus {"
            "      border-image: url(:/" + strPath + "delete2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefDel:pressed {"
            "      border-image: url(:/" + strPath + "delete2-2.png);"
            "  }";

    //pushButtonAddDefSave
    strStyleSheet += " QPushButton#pushButtonAddDefSave {"
            "      border-image: url(:/" + strPath + "save2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefSave:focus {"
            "      border-image: url(:/" + strPath + "save2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefSave:pressed {"
            "      border-image: url(:/" + strPath + "save2-2.png);"
            "  }";

    //pushButtonAddDefReturn
    strStyleSheet += " QPushButton#pushButtonAddDefReturn {"
            "      border-image: url(:/" + strPath + "return4-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefReturn:focus {"
            "      border-image: url(:/" + strPath + "return4-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefReturn:pressed {"
            "      border-image: url(:/" + strPath + "return4-2.png);"
            "  }";

    //pushButtonAddDefType
    strStyleSheet += " QPushButton#pushButtonAddDefType {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefType:focus {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefType:pressed {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";

    //pushButtonAddDefChannel
    strStyleSheet += " QPushButton#pushButtonAddDefChannel {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefChannel:focus {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefChannel:pressed {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";

    //pushButtonAddDefProperty
    strStyleSheet += " QPushButton#pushButtonAddDefProperty {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefProperty:focus {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefProperty:pressed {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";

    //pushButtonAddDefSubType
    strStyleSheet += " QPushButton#pushButtonAddDefSubType {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefSubType:focus {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefSubType:pressed {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";

    //pushButtonAddDefDelayAlarm
    strStyleSheet += " QPushButton#pushButtonAddDefDelayAlarm {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefDelayAlarm:focus {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefDelayAlarm:pressed {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonAddDefBypass {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefBypass:focus {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddDefBypass:pressed {"
            "      background-image: url();"
            "      border-image: url();"
            "      font: " + adf + "px;"
            "  }";

    /************************************************************
    描述：common/addressbookdialog界面style
    ************************************************************/
    strPath = "images/common/";
    //pushButtonAddressBookCancel
    strStyleSheet += " QPushButton#pushButtonAddressBookCancel {"
            "      border-image: url(:/" + strPath + "cancel-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddressBookCancel:focus {"
            "      border-image: url(:/" + strPath + "cancel-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonAddressBookCancel:pressed {"
            "      border-image: url(:/" + strPath + "cancel-2.png);"
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
            "      border-image: url(:/" + strPath + "dial-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctDial:focus {"
            "      border-image: url(:/" + strPath + "dial-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctDial:pressed {"
            "      border-image: url(:/" + strPath + "dial-2.png);"
            "  }";

    //pushButtonIctHangUp
    strStyleSheet += " QPushButton#pushButtonIctHangUp {"
        "      border-image: url(:/" + strPath + "hangup-1.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonIctHangUp:focus {"
        "      border-image: url(:/" + strPath + "hangup-1.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonIctHangUp:pressed {"
        "      border-image: url(:/" + strPath + "hangup-2.png);"
        "  }";

    //pushButtonIctAddressBook
    strStyleSheet += " QPushButton#pushButtonIctAddressBook {"
            "      border-image: url(:/" + strPath + "addressbook-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctAddressBook:focus {"
            "      border-image: url(:/" + strPath + "addressbook-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctAddressBook:pressed {"
            "      border-image: url(:/" + strPath + "addressbook-2.png);"
            "  }";

    //pushButtonIctVideoClose
    strStyleSheet += " QPushButton#pushButtonIctVideoClose {"
            "      border-image: url(:/" + strPath + "videoclose-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctVideoClose:focus {"
            "      border-image: url(:/" + strPath + "videoclose-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctVideoClose:pressed {"
            "      border-image: url(:/" + strPath + "videoclose-2.png);"
            "  }";

    //pushButtonIctVideoOpen
    strStyleSheet += " QPushButton#pushButtonIctVideoOpen {"
            "      border-image: url(:/" + strPath + "videoopen-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctVideoOpen:focus {"
            "      border-image: url(:/" + strPath + "videoopen-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctVideoOpen:pressed {"
            "      border-image: url(:/" + strPath + "videoopen-2.png);"
            "  }";

    //pushButtonIctRecord
    strStyleSheet += " QPushButton#pushButtonIctRecord {"
            "      border-image: url(:/" + strPath + "record-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRecord:focus {"
            "      border-image: url(:/" + strPath + "record-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRecord:pressed {"
            "      border-image: url(:/" + strPath + "record-2.png);"
            "  }";

    //pushButtonIctReturn
    strStyleSheet += " QPushButton#pushButtonIctReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonIctBack
    strStyleSheet += " QPushButton#pushButtonIctBack {"
            "      border-image: url(:/" + strPath + "back-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctBack:focus {"
            "      border-image: url(:/" + strPath + "back-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctBack:pressed {"
            "      border-image: url(:/" + strPath + "back-2.png);"
            "  }";

    //pushButtonIctNum
    strStyleSheet += " QPushButton#pushButtonIctNum0 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum0:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum0:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum1 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum1:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum1:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum2 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum2:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum2:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum3 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum3:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum3:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum4 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum4:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum4:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum5 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum5:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum5:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum6 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum6:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum6:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum7 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum7:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum7:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonIctNum8 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum8:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum8:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonIctNum9 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum9:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctNum9:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctDot {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctDot:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctDot:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctShop {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctShop:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctShop:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";

    /************************************************************
    描述：voip/ictrcvhookdialog界面style
    ************************************************************/
    strPath = "images/voip/";

    //pushButtonIctRcvHookHangUp
    strStyleSheet += " QPushButton#pushButtonIctRcvHookHangUp {"
            "      border-image: url(:/" + strPath + "hangup2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookHangUp:focus {"
            "      border-image: url(:/" + strPath + "hangup2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookHangUp:pressed {"
            "      border-image: url(:/" + strPath + "hangup2-2.png);"
            "  }";

    //pushButtonIctRcvHookVideoClose
    strStyleSheet += " QPushButton#pushButtonIctRcvHookVideoClose {"
            "      border-image: url(:/" + strPath + "videoclose2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookVideoClose:focus {"
            "      border-image: url(:/" + strPath + "videoclose2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookVideoClose:pressed {"
            "      border-image: url(:/" + strPath + "videoclose2-2.png);"
            "  }";

    //pushButtonIctRcvHookOpen
    strStyleSheet += " QPushButton#pushButtonIctRcvHookOpen {"
            "      border-image: url(:/" + strPath + "open2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookOpen:focus {"
            "      border-image: url(:/" + strPath + "open2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookOpen:pressed {"
            "      border-image: url(:/" + strPath + "open2-2.png);"
            "  }";

    //pushButtonIctRcvHookReturn
    strStyleSheet += " QPushButton#pushButtonIctRcvHookReturn {"
            "      border-image: url(:/" + strPath + "return2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookReturn:focus {"
            "      border-image: url(:/" + strPath + "return2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookReturn:pressed {"
            "      border-image: url(:/" + strPath + "return2-2.png);"
            "  }";

    //pushButtonIctRcvHookVideoOpen
    strStyleSheet += " QPushButton#pushButtonIctRcvHookVideoOpen {"
            "      border-image: url(:/" + strPath + "videoopen2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookVideoOpen:focus {"
            "      border-image: url(:/" + strPath + "videoopen2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIctRcvHookVideoOpen:pressed {"
            "      border-image: url(:/" + strPath + "videoopen2-2.png);"
            "  }";

    /************************************************************
    描述：voip/istdialog界面style
    ************************************************************/
    strPath = "images/voip/";
    //pushButtonIstAnswer
    strStyleSheet += " QPushButton#pushButtonIstAnswer {"
            "      border-image: url(:/" + strPath + "answer-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstAnswer:focus {"
            "      border-image: url(:/" + strPath + "answer-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstAnswer:pressed {"
            "      border-image: url(:/" + strPath + "answer-2.png);"
            "  }";

    //pushButtonIstVideoClose
    strStyleSheet += " QPushButton#pushButtonIstVideoClose {"
            "      border-image: url(:/" + strPath + "videoclose2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstVideoClose:focus {"
            "      border-image: url(:/" + strPath + "videoclose2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstVideoClose:pressed {"
            "      border-image: url(:/" + strPath + "videoclose2-2.png);"
            "  }";

    //pushButtonIstVideoOpen
    strStyleSheet += " QPushButton#pushButtonIstVideoOpen {"
            "      border-image: url(:/" + strPath + "videoopen2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstVideoOpen:focus {"
            "      border-image: url(:/" + strPath + "videoopen2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstVideoOpen:pressed {"
            "      border-image: url(:/" + strPath + "videoopen2-2.png);"
            "  }";

    //pushButtonIstOpen
    strStyleSheet += " QPushButton#pushButtonIstOpen {"
            "      border-image: url(:/" + strPath + "open2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstOpen:focus {"
            "      border-image: url(:/" + strPath + "open2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstOpen:pressed {"
            "      border-image: url(:/" + strPath + "open2-2.png);"
            "  }";

    //pushButtonIstHangUp
    strStyleSheet += " QPushButton#pushButtonIstHangUp {"
            "      border-image: url(:/" + strPath + "hangup2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstHangUp:focus {"
            "      border-image: url(:/" + strPath + "hangup2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonIstHangUp:pressed {"
            "      border-image: url(:/" + strPath + "hangup2-2.png);"
            "  }";

    /************************************************************
    描述：voip/monlistdialog界面style
    ************************************************************/
    strPath = "images/voip/";
    //pushButtonMonListCancel
    strStyleSheet += " QPushButton#pushButtonMonListCancel {"
            "      border-image: url(:/" + strPath + "cancel-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonListCancel:focus {"
            "      border-image: url(:/" + strPath + "cancel-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMonListCancel:pressed {"
            "      border-image: url(:/" + strPath + "cancel-2.png);"
            "  }";

    /************************************************************
    描述：info_man/infomandetaildialog界面style
    ************************************************************/
    strPath = "images/info_man/";
    //pushButtonInfoManDetailReply
    strStyleSheet += " QPushButton#pushButtonInfoManDetailReply {"
            "      border-image: url(:/" + strPath + "reply-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailReply:focus {"
            "      border-image: url(:/" + strPath + "reply-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailReply:pressed {"
            "      border-image: url(:/" + strPath + "reply-2.png);"
            "  }";

    //pushButtonInfoManDetailAttach
    strStyleSheet += " QPushButton#pushButtonInfoManDetailAttach {"
            "      border-image: url(:/" + strPath + "attach-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailAttach:focus {"
            "      border-image: url(:/" + strPath + "attach-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailAttach:pressed {"
            "      border-image: url(:/" + strPath + "attach-2.png);"
            "  }";

    //pushButtonInfoManDetailReturn
    strStyleSheet += " QPushButton#pushButtonInfoManDetailReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonInfoManDetailPrev
    strStyleSheet += " QPushButton#pushButtonInfoManDetailPrev {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailPrev:focus {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailPrev:pressed {"
            "      border-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonInfoManDetailNext
    strStyleSheet += " QPushButton#pushButtonInfoManDetailNext {"
            "      border-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailNext:focus {"
            "      border-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManDetailNext:pressed {"
            "      border-image: url(:/" + strPath + "next-2.png);"
            "  }";

    /************************************************************
    描述：info_man/infomanviewattachmentdialog界面style
    ************************************************************/
    strPath = "images/info_man/";
    //pushButtonInfoManViewZoomIn
    strStyleSheet += " QPushButton#pushButtonInfoManViewZoomIn {"
            "      border-image: url(:/" + strPath + "zoomin-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewZoomIn:focus {"
            "      border-image: url(:/" + strPath + "zoomin-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewZoomIn:pressed {"
            "      border-image: url(:/" + strPath + "zoomin-2.png);"
            "  }";

    //pushButtonInfoManViewZoomOut
    strStyleSheet += " QPushButton#pushButtonInfoManViewZoomOut {"
            "      border-image: url(:/" + strPath + "zoomout-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewZoomOut:focus {"
            "      border-image: url(:/" + strPath + "zoomout-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewZoomOut:pressed {"
            "      border-image: url(:/" + strPath + "zoomout-2.png);"
            "  }";

    //pushButtonInfoManViewNext
    strStyleSheet += " QPushButton#pushButtonInfoManViewNext {"
            "      border-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewNext:focus {"
            "      border-image: url(:/" + strPath + "next-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewNext:pressed {"
            "      border-image: url(:/" + strPath + "next-2.png);"
            "  }";

    //pushButtonInfoManViewPrev
    strStyleSheet += " QPushButton#pushButtonInfoManViewPrev {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewPrev:focus {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewPrev:pressed {"
            "      border-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonInfoManViewReturn
    strStyleSheet += " QPushButton#pushButtonInfoManViewReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonInfoManViewReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：security/securityplayerdialog界面style
    ************************************************************/
    strPath = "images/security/";
    //pushButtonSecPlayerPause
    strStyleSheet += " QPushButton#pushButtonSecPlayerPause {"
            "      border-image: url(:/" + strPath + "pause-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPlayerPause:focus {"
            "      border-image: url(:/" + strPath + "pause-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPlayerPause:pressed {"
            "      border-image: url(:/" + strPath + "pause-2.png);"
            "  }";

    //pushButtonSecPlayerPlay
    strStyleSheet += " QPushButton#pushButtonSecPlayerPlay {"
            "      border-image: url(:/" + strPath + "play-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPlayerPlay:focus {"
            "      border-image: url(:/" + strPath + "play-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPlayerPlay:pressed {"
            "      border-image: url(:/" + strPath + "play-2.png);"
            "  }";

    //pushButtonSecPlayerStop
    strStyleSheet += " QPushButton#pushButtonSecPlayerStop {"
            "      border-image: url(:/" + strPath + "stop-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPlayerStop:focus {"
            "      border-image: url(:/" + strPath + "stop-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPlayerStop:pressed {"
            "      border-image: url(:/" + strPath + "stop-2.png);"
            "  }";

    //pushButtonSecPlayerReturn
    strStyleSheet += " QPushButton#pushButtonSecPlayerReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPlayerReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecPlayerReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：security/securityalertdialog界面style
    ************************************************************/
    strPath = "images/security/";

    //pushButtonSecAlertHome
    strStyleSheet += " QPushButton#pushButtonSecAlertHome {"
            "      border-image: url(:/" + strPath + "home-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAlertHome:focus {"
            "      border-image: url(:/" + strPath + "home-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAlertHome:pressed {"
            "      border-image: url(:/" + strPath + "home-2.png);"
            "  }";

    //pushButtonSecAlertOut
    strStyleSheet += " QPushButton#pushButtonSecAlertOut {"
            "      border-image: url(:/" + strPath + "out-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAlertOut:focus {"
            "      border-image: url(:/" + strPath + "out-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAlertOut:pressed {"
            "      border-image: url(:/" + strPath + "out-2.png);"
            "  }";

    //pushButtonSecAlertDisArm
    strStyleSheet += " QPushButton#pushButtonSecAlertDisArm {"
            "      border-image: url(:/" + strPath + "disarm-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAlertDisArm:focus {"
            "      border-image: url(:/" + strPath + "disarm-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAlertDisArm:pressed {"
            "      border-image: url(:/" + strPath + "disarm-2.png);"
            "  }";

    //pushButtonSecAlertReturn
    strStyleSheet += " QPushButton#pushButtonSecAlertReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAlertReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecAlertReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：system_setting/security/securityparamsetdialog界面style
    ************************************************************/
    strPath = "images/system_setting/";

    //pushButtonSecParaSetDefense
    strStyleSheet += " QPushButton#pushButtonSecParaSetDefense {"
            "      background-image: url();"
            "      border-image: url();"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecParaSetDefense:focus {"
            "      background-image: url();"
            "      border-image: url();"
            "      color: rgb(158, 106, 26);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecParaSetDefense:pressed {"
            "      background-image: url();"
            "      border-image: url();"
            "      color: #FF0000;"
            "  }";

    //pushButtonSecParaSetSendPic
    strStyleSheet += "QPushButton#pushButtonSecParaSetSendPic {"
            "      background-image: url();"
            "       border-image:url();"
            "}";
    strStyleSheet += "QPushButton#pushButtonSecParaSetSendPic:focus {"
            "      background-image: url();"
            "       border-image:url();"
            "}";
    strStyleSheet += "QPushButton#pushButtonSecParaSetSendPic:pressed {"
            "      background-image: url();"
            "       border-image:url();"
            "}";

    //pushButtonSecParaSetSave
    strStyleSheet += " QPushButton#pushButtonSecParaSetSave {"
            "      border-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecParaSetSave:focus {"
            "      border-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecParaSetSave:pressed {"
            "      border-image: url(:/" + strPath + "save-2.png);"
            "  }";

    //pushButtonSecParaSetReturn
    strStyleSheet += " QPushButton#pushButtonSecParaSetReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecParaSetReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSecParaSetReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：intercom_man/mediaplayerdialog界面style
    ************************************************************/
    strPath = "images/intercom_man/";
    //pushButtonMediaPlayerPlay
    strStyleSheet += " QPushButton#pushButtonMediaPlayerPlay {"
            "      border-image: url(:/" + strPath + "play-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerPlay:focus {"
            "      border-image: url(:/" + strPath + "play-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerPlay:pressed {"
            "      border-image: url(:/" + strPath + "play-2.png);"
            "  }";

    //pushButtonMediaPlayerStop
    strStyleSheet += " QPushButton#pushButtonMediaPlayerStop {"
            "      border-image: url(:/" + strPath + "stop-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerStop:focus {"
            "      border-image: url(:/" + strPath + "stop-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerStop:pressed {"
            "      border-image: url(:/" + strPath + "stop-2.png);"
            "  }";

    //pushButtonMediaPlayerPause
    strStyleSheet += " QPushButton#pushButtonMediaPlayerPause {"
            "      border-image: url(:/" + strPath + "pause-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerPause:focus {"
            "      border-image: url(:/" + strPath + "pause-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerPause:pressed {"
            "      border-image: url(:/" + strPath + "pause-2.png);"
            "  }";

    //pushButtonMediaPlayerReturn
    strStyleSheet += " QPushButton#pushButtonMediaPlayerReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonMediaPlayerCall
    strStyleSheet += " QPushButton#pushButtonMediaPlayerCall {"
            "      border-image: url(:/" + strPath + "call-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerCall:focus {"
            "      border-image: url(:/" + strPath + "call-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMediaPlayerCall:pressed {"
            "      border-image: url(:/" + strPath + "call-2.png);"
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
            "      border-image: url();"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeDail:focus {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      border-image: url();"
            "      color: rgb(158, 106, 26);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeDail:pressed {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      border-image: url();"
            "      color: #FF0000;"
            "  }";

    //pushButtonRingVolumeCallIn
    strStyleSheet += " QPushButton#pushButtonRingVolumeCallIn {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      border-image: url();"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeCallIn:focus {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      border-image: url();"
            "      color: rgb(158, 106, 26);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeCallIn:pressed {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      border-image: url();"
            "      color: #FF0000;"
            "  }";

    //pushButtonRingVolumeDoor
    strStyleSheet += " QPushButton#pushButtonRingVolumeDoor {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      border-image: url();"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeDoor:focus {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      border-image: url();"
            "      color: rgb(158, 106, 26);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeDoor:pressed {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      border-image: url();"
            "      color: #FF0000;"
            "  }";

    //pushButtonRingVolumeCenter1
    strStyleSheet += " QPushButton#pushButtonRingVolumeCenter1 {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      border-image: url();"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeCenter1:focus {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      border-image: url();"
            "      color: rgb(158, 106, 26);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeCenter1:pressed {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      border-image: url();"
            "      color: #FF0000;"
            "  }";

    //pushButtonRingVolumeCenter2
    strStyleSheet += " QPushButton#pushButtonRingVolumeCenter2 {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      border-image: url();"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeCenter2:focus {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      border-image: url();"
            "      color: rgb(158, 106, 26);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeCenter2:pressed {"
            "      font: " + rf + "px;"
            "      background-image: url();"
            "      border-image: url();"
            "      color: #FF0000;"
            "  }";

    //pushButtonRingVolumeSave
    strStyleSheet += " QPushButton#pushButtonRingVolumeSave {"
            "      border-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeSave:focus {"
            "      border-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeSave:pressed {"
            "      border-image: url(:/" + strPath + "save-2.png);"
            "  }";

    //pushButtonRingVolumeReturn
    strStyleSheet += " QPushButton#pushButtonRingVolumeReturn {"
        "      border-image: url(:/" + strPath + "return-1.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeReturn:focus {"
        "      border-image: url(:/" + strPath + "return-1.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonRingVolumeReturn:pressed {"
        "      border-image: url(:/" + strPath + "return-2.png);"
        "  }";


    /************************************************************
    描述：system_setting/ring_and_volume/ringfilesetdialog界面style
    ************************************************************/
    strPath = "images/system_setting/";

    //pushButtonRingFileSetCancel
    strStyleSheet += " QPushButton#pushButtonRingFileSetCancel {"
            "      border-image: url(:/" + strPath + "cancel-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingFileSetCancel:focus {"
            "      border-image: url(:/" + strPath + "cancel-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingFileSetCancel:pressed {"
            "      border-image: url(:/" + strPath + "cancel-2.png);"
            "  }";

    //pushButtonRingFileSetSave
    strStyleSheet += " QPushButton#pushButtonRingFileSetSave {"
            "      border-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingFileSetSave:focus {"
            "      border-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonRingFileSetSave:pressed {"
            "      border-image: url(:/" + strPath + "save-2.png);"
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
            "      border-image: url(:/images/voip_dialog/answer-1.png);"
            "      color: " + strBtnColor + ";"
            "      font: " + strBtnFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonVoipMainDial:focus {"
            "      border-image: url(:/images/voip_dialog/answer-1.png);"
            "      color: " + strBtnFocusColor + ";"
            "      font: " + strBtnFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonVoipMainDial:pressed {"
            "      border-image: url(:/images/voip_dialog/answer-2.png);"
            "      color: " + strBtnPressColor + ";"
            "      font: " + strBtnFontSize + "px;"
            "  }";

    //pushButtonVoipMainReturn
    strStyleSheet += " QPushButton#pushButtonVoipMainReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonVoipMainReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonVoipMainReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    /************************************************************
    描述：voip/mstdialog界面style
    ************************************************************/
    //pushButtonMstPrev
    strStyleSheet += " QPushButton#pushButtonMstPrev {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMstPrev:focus {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMstPrev:pressed {"
            "      border-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonMstNext
    strStyleSheet += " QPushButton#pushButtonMstNext {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMstNext:focus {"
            "      border-image: url(:/" + strPath + "prev-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMstNext:pressed {"
            "      border-image: url(:/" + strPath + "prev-2.png);"
            "  }";

    //pushButtonMstHangUp
    strStyleSheet += " QPushButton#pushButtonMstHangUp {"
            "      border-image: url(:/images/voip_dialog/hangup-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMstHangUp:focus {"
            "      border-image: url(:/images/voip_dialog/hangup-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMstHangUp:pressed {"
            "      border-image: url(:/images/voip_dialog/hangup-2.png);"
            "  }";

    //pushButtonMstReturn
    strStyleSheet += " QPushButton#pushButtonMstReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMstReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonMstReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";


    /************************************************************
    描述：system_setting/device/devicesetmaindialog界面style
    ************************************************************/
    QString strDeviceSetMainFontSize;
    strDeviceSetMainFontSize.sprintf("%d",g_pScaleSrc->systemset_device_button_fonts());

#if 0
    //pushButtonCalibration
    strStyleSheet += " QPushButton#pushButtonCalibration {"
        "      border-image: url(:/images/system_setting/blank-1.png);"
        "      color: " + strBtnColor + ";"
        "      font: " + strDeviceSetMainFontSize + "px;"
        "  }";
    strStyleSheet += " QPushButton#pushButtonCalibration:focus {"
        "      border-image: url(:/images/system_setting/blank-1.png);"
        "      color: " + strBtnFocusColor + ";"
        "      font: " + strDeviceSetMainFontSize + "px;"
        "  }";
    strStyleSheet += " QPushButton#pushButtonCalibration:pressed {"
        "      border-image: url(:/images/system_setting/blank-2.png);"
        "      color: " + strBtnPressColor + ";"
        "      font: " + strDeviceSetMainFontSize + "px;"
        "  }";

    //pushButtonSystemReset
    strStyleSheet += " QPushButton#pushButtonSystemReset {"
        "      border-image: url(:/images/system_setting/systemreset-1.png);"
        "      color: " + strBtnColor + ";"
        "      font: " + strBtnFontSize + "px;"
        "  }";
    strStyleSheet += " QPushButton#pushButtonSystemReset:focus {"
        "      border-image: url(:/images/system_setting/systemreset-1.png);"
        "      color: " + strBtnFocusColor + ";"
        "      font: " + strBtnFontSize + "px;"
        "  }";
    strStyleSheet += " QPushButton#pushButtonSystemReset:pressed {"
        "      border-image: url(:/images/system_setting/systemreset-2.png);"
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
        "      border-image: url(:/" + strPath + "answer-1.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonDoorBellAnswer:focus {"
        "      border-image: url(:/" + strPath + "answer-1.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonDoorBellAnswer:pressed {"
        "      border-image: url(:/" + strPath + "answer-2.png);"
        "  }";

    //pushButtonDoorBellOpen
    strStyleSheet += " QPushButton#pushButtonDoorBellOpen {"
            "      border-image: url(:/" + strPath + "open-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDoorBellOpen:focus {"
            "      border-image: url(:/" + strPath + "open-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDoorBellOpen:pressed {"
            "      border-image: url(:/" + strPath + "open-2.png);"
            "  }";

    //pushButtonDoorBellReturn
    strStyleSheet += " QPushButton#pushButtonDoorBellReturn {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDoorBellReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDoorBellReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
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
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorReturn:focus {"
            "      border-image: url(:/" + strPath + "return-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorReturn:pressed {"
            "      border-image: url(:/" + strPath + "return-2.png);"
            "  }";

    //pushButtonCallElevatorNum
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum0 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum0:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum0:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum1 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum1:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum1:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum2 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum2:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum2:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum3 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum3:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum3:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum4 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum4:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum4:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum5 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum5:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum5:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum6 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum6:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum6:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum7 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum7:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum7:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonCallElevatorNum8 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum8:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum8:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";

    strStyleSheet += " QPushButton#pushButtonCallElevatorNum9 {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum9:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontFocusSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorNum9:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontPressSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorDot {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorDot:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorDot:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorShop {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorShop:focus {"
            "      border-image: url(:/" + strPath + "num-1.png);"
            "      font-family: \"Gurmukhi MN\";"
            "      font: " + strNumFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonCallElevatorShop:pressed {"
            "      border-image: url(:/" + strPath + "num-2.png);"
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
            "      border-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDoorMachineSave:focus {"
            "      border-image: url(:/" + strPath + "save-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonDoorMachineSave:pressed {"
            "      border-image: url(:/" + strPath + "save-2.png);"
            "  }";
#endif

    /************************************************************
    描述：system_setting/syssetmaindialog界面style
    ************************************************************/
    strPath = "images/system_setting/";

    //pushButtonSysSetMainVersion
    strStyleSheet += " QPushButton#pushButtonSysSetMainVersion {"
            "      border-image: url(:/" + strPath + "version2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainVersion:focus {"
            "      border-image: url(:/" + strPath + "version2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainVersion:pressed {"
            "      border-image: url(:/" + strPath + "version2-2.png);"
            "  }";

    //pushButtonSysSetMainLag
    strStyleSheet += " QPushButton#pushButtonSysSetMainLag {"
            "      border-image: url(:/" + strPath + "language2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainLag:focus {"
            "      border-image: url(:/" + strPath + "language2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainLag:pressed {"
            "      border-image: url(:/" + strPath + "language2-2.png);"
            "  }";

    //pushButtonSysSetMainPwd
    strStyleSheet += " QPushButton#pushButtonSysSetMainPwd {"
            "      border-image: url(:/" + strPath + "password2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainPwd:focus {"
            "      border-image: url(:/" + strPath + "password2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainPwd:pressed {"
            "      border-image: url(:/" + strPath + "password2-2.png);"
            "  }";

    //pushButtonSysSetMainTime
    strStyleSheet += " QPushButton#pushButtonSysSetMainTime {"
            "      border-image: url(:/" + strPath + "time2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainTime:focus {"
            "      border-image: url(:/" + strPath + "time2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainTime:pressed {"
            "      border-image: url(:/" + strPath + "time2-2.png);"
            "  }";

    //pushButtonSysSetMainRing
    strStyleSheet += " QPushButton#pushButtonSysSetMainRing {"
            "      border-image: url(:/" + strPath + "ring2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainRing:focus {"
            "      border-image: url(:/" + strPath + "ring2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainRing:pressed {"
            "      border-image: url(:/" + strPath + "ring2-2.png);"
            "  }";

    //pushButtonSysSetMainDevice
    strStyleSheet += " QPushButton#pushButtonSysSetMainDevice {"
            "      border-image: url(:/" + strPath + "device2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainDevice:focus {"
            "      border-image: url(:/" + strPath + "device2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainDevice:pressed {"
            "      border-image: url(:/" + strPath + "device2-2.png);"
            "  }";

    //pushButtonSysSetMainSecurity
    strStyleSheet += " QPushButton#pushButtonSysSetMainSecurity {"
            "      border-image: url(:/" + strPath + "security2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainSecurity:focus {"
            "      border-image: url(:/" + strPath + "security2-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainSecurity:pressed {"
            "      border-image: url(:/" + strPath + "security2-2.png);"
            "  }";

    //pushButtonSysSetMainReturn
    strStyleSheet += " QPushButton#pushButtonSysSetMainReturn {"
        "      border-image: url(:/" + strPath + "return2-1.png);"
        "      font: " + strBtnFontSize + "px;"
        "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainReturn:focus {"
        "      border-image: url(:/" + strPath + "return2-1.png);"
        "      font: " + strBtnFontSize + "px;"
        "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetMainReturn:pressed {"
        "      border-image: url(:/" + strPath + "return2-2.png);"
        "      font: " + strBtnFontSize + "px;"
        "  }";


    /************************************************************
    描述：system_setting/syssetengineerdialog界面style
    ************************************************************/
    strPath = "images/system_setting/";

    //pushButtonSysSetEngineerNetAddress
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerNetAddress {"
            "      border-image: url(:/" + strPath + "netaddress-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerNetAddress:focus {"
            "      border-image: url(:/" + strPath + "netaddress-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerNetAddress:pressed {"
            "      border-image: url(:/" + strPath + "netaddress-2.png);"
            "  }";

    //pushButtonSysSetEngineerDoorMachine
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerDoorMachine {"
            "      border-image: url(:/" + strPath + "doormachine-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerDoorMachine:focus {"
            "      border-image: url(:/" + strPath + "doormachine-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerDoorMachine:pressed {"
            "      border-image: url(:/" + strPath + "doormachine-2.png);"
            "  }";

    //pushButtonSysSetEngineerIpCamera
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerIpCamera {"
            "      border-image: url(:/" + strPath + "ipcamera-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerIpCamera:focus {"
            "      border-image: url(:/" + strPath + "ipcamera-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerIpCamera:pressed {"
            "      border-image: url(:/" + strPath + "ipcamera-2.png);"
            "  }";

    //pushButtonSysSetEngineerPwd
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerPwd {"
            "      border-image: url(:/" + strPath + "password-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerPwd:focus {"
            "      border-image: url(:/" + strPath + "password-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerPwd:pressed {"
            "      border-image: url(:/" + strPath + "password-2.png);"
            "  }";

    //pushButtonSysSetEngineerReturn
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerReturn {"
            "      border-image: url(:/" + strPath + "return3-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerReturn:focus {"
            "      border-image: url(:/" + strPath + "return3-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerReturn:pressed {"
            "      border-image: url(:/" + strPath + "return3-2.png);"
            "  }";

    //pushButtonSysSetEngineerSave
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerSave {"
            "      border-image: url(:/" + strPath + "save3-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerSave:focus {"
            "      border-image: url(:/" + strPath + "save3-1.png);"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerSave:pressed {"
            "      border-image: url(:/" + strPath + "save3-2.png);"
            "  }";

    //pushButtonSysSetEngineerRemoteCheck
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerRemoteCheck {"
        "      border-image: url(:/images/system_setting/page-unselected.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerRemoteCheck:focus {"
        "      border-image: url(:/images/system_setting/page-unselected.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerRemoteCheck:pressed {"
        "      border-image: url(:/images/system_setting/page-selected.png);"
        "  }";

    //pushButtonSysSetEngineerOther
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerOther {"
        "      border-image: url(:/images/system_setting/other-1.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerOther:focus {"
        "      border-image: url(:/images/system_setting/other-1.png);"
        "  }";
    strStyleSheet += " QPushButton#pushButtonSysSetEngineerOther:pressed {"
        "      border-image: url(:/images/system_setting/other-2.png);"
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
            "      border-image: url(:/images/system_setting/save-1.png);"
        "      color: " + strBtnColor + ";"
        "      font: " + strBtnFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSave:focus {"
            "      border-image: url(:/images/system_setting/save-1.png);"
        "      color: " + strBtnFocusColor + ";"
        "      font: " + strBtnFontSize + "px;"
            "  }";
    strStyleSheet += " QPushButton#pushButtonSave:pressed {"
            "      border-image: url(:/images/system_setting/save-2.png);"
        "      color: " + strBtnPressColor + ";"
        "      font: " + strBtnFontSize + "px;"
            "  }";

	strStyleSheet += " QPushButton#pushButtonCancel {"
            "      border-image: url(:/images/voip_dialog/return-1.png);"
		"      color: " + strBtnColor + ";"
		"      font: " + strBtnFontSize + "px;"
			"  }";
	strStyleSheet += " QPushButton#pushButtonCancel:focus {"
            "      border-image: url(:/images/voip_dialog/return-1.png);"
		"      color: " + strBtnFocusColor + ";"
		"      font: " + strBtnFontSize + "px;"
			"  }";
	strStyleSheet += " QPushButton#pushButtonCancel:pressed {"
            "      border-image: url(:/images/voip_dialog/return-2.png);"
		"      color: " + strBtnPressColor + ";"
		"      font: " + strBtnFontSize + "px;"
			"  }";

	//Home control
	strStyleSheet += " QPushButton#pushButtonTypeLight {"
        "      border-image: url(:/images/home_control/light.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTypeSwitch {"
        "      border-image: url(:/images/home_control/switch.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTypeCurtain {"
        "      border-image: url(:/images/home_control/curtain.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTypeAC {"
        "      border-image: url(:/images/home_control/ac.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTypeTV {"
        "      border-image: url(:/images/home_control/tv.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTypeMusic {"
        "      border-image: url(:/images/home_control/music.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTypeCustom {"
        "      border-image: url(:/images/home_control/user.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonHCReturn {"
        "      border-image: url(:/images/voip_dialog/return-1.png);"
		"      color: " + strBtnColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCReturn:focus {"
        "      border-image: url(:/images/voip_dialog/return-1.png);"
		"      color: " + strBtnFocusColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCReturn:pressed {"
        "      border-image: url(:/images/voip_dialog/return-2.png);"
		"      color: " + strBtnPressColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";

	strStyleSheet += " QPushButton#pushButtonHCEnterRoom {"
        "      border-image: url(:/images/home_control/enter_room-1.png);"
		"      color: " + strBtnColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCEnterRoom:focus {"
        "      border-image: url(:/images/home_control/enter_room-1.png);"
		"      color: " + strBtnFocusColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCEnterRoom:pressed {"
        "      border-image: url(:/images/home_control/enter_room-2.png);"
		"      color: " + strBtnPressColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";

	strStyleSheet += " QPushButton#pushButtonHCExitRoom {"
        "      border-image: url(:/images/home_control/exit_room-1.png);"
		"      color: " + strBtnColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCExitRoom:focus {"
        "      border-image: url(:/images/home_control/exit_room-1.png);"
		"      color: " + strBtnFocusColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCExitRoom:pressed {"
        "      border-image: url(:/images/home_control/exit_room-2.png);"
		"      color: " + strBtnPressColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";

	strStyleSheet += " QPushButton#pushButtonHCSceneReturn {"
        "      border-image: url(:/images/home_control/scene_return-1.png);"
		"      border-radius: 7px;"
		"      color: " + strBtnColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCSceneReturn:focus {"
        "      border-image: url(:/images/home_control/scene_return-1.png);"
		"      border-radius: 7px;"
		"      color: " + strBtnFocusColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"	}";
	strStyleSheet += " QPushButton#pushButtonHCSceneReturn:pressed {"
        "      border-image: url(:/images/home_control/scene_return-3.png);"
		"      border-radius: 7px;"
		"      color: " + strBtnPressColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";

	strStyleSheet += " QPushButton#pushButtonEnterHCScene {"
        "      border-image: url(:/images/main/home_control-1.png);"
		"      border-radius: 7px;"
		"      color: " + strBtnColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";
	strStyleSheet += " QPushButton#pushButtonEnterHCScene:focus {"
        "      border-image: url(:/images/main/home_control-1.png);"
		"      border-radius: 7px;"
		"      color: " + strBtnFocusColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"	}";
	strStyleSheet += " QPushButton#pushButtonEnterHCScene:pressed {"
        "      border-image: url(:/images/main/home_control-3.png);"
		"      border-radius: 7px;"
		"      color: " + strBtnPressColor + ";"
		"      font: " + strBtnFontSize + "px;"
		"  }";

	strStyleSheet += " QPushButton#pushButtonFloor {"
            "      border-image: url(:/images/home_control/blank-1.png);"
		"      color: " + strBtnColor + ";"
		"      font: " + strBtnFontSize + "px;"
			"  }";
	strStyleSheet += " QPushButton#pushButtonFloor:focus {"
            "      border-image: url(:/images/home_control/blank-1.png);"
		"      color: " + strBtnFocusColor + ";"
		"      font: " + strBtnFontSize + "px;"
			"  }";
	strStyleSheet += " QPushButton#pushButtonFloor:pressed {"
            "      border-image: url(:/images/home_control/blank-2.png);"
		"      color: " + strBtnPressColor + ";"
		"      font: " + strBtnFontSize + "px;"
			"  }";

	strStyleSheet += " QPushButton#pushButtonPowerOn {"
        "      border-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonPowerOn:focus {"
        "      border-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonPowerOn:pressed {"
        "      border-image: url(:/images/home_control/power-off.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonPowerOff {"
        "      border-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonPowerOff:focus {"
        "      border-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonPowerOff:pressed {"
        "      border-image: url(:/images/home_control/power-on.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBrightnessAdd {"
        "      border-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBrightnessAdd:focus {"
        "      border-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBrightnessAdd:pressed {"
        "      border-image: url(:/images/home_control/add-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBrightnessSub {"
        "      border-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBrightnessSub:focus {"
        "      border-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBrightnessSub:pressed {"
        "      border-image: url(:/images/home_control/sub-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonHCSubControlReturn {"
        "      border-image: url(:/images/home_control/return-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCSubControlReturn:focus {"
        "      border-image: url(:/images/home_control/return-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonHCSubControlReturn:pressed {"
        "      border-image: url(:/images/home_control/return-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonCurtainOpen {"
        "      border-image: url(:/images/home_control/curtain-open-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonCurtainOpen:focus {"
        "      border-image: url(:/images/home_control/curtain-open-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonCurtainOpen:pressed {"
        "      border-image: url(:/images/home_control/curtain-open-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonCurtainStop {"
        "      border-image: url(:/images/home_control/pause-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonCurtainStop:focus {"
        "      border-image: url(:/images/home_control/pause-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonCurtainStop:pressed {"
        "      border-image: url(:/images/home_control/pause-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonCurtainHalfOpen {"
        "      border-image: url(:/images/home_control/curtain-half-open-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonCurtainHalfOpen:focus {"
        "      border-image: url(:/images/home_control/curtain-half-open-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonCurtainHalfOpen:pressed {"
        "      border-image: url(:/images/home_control/curtain-half-open-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonCurtainClose {"
        "      border-image: url(:/images/home_control/curtain-close-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonCurtainClose:focus {"
        "      border-image: url(:/images/home_control/curtain-close-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonCurtainClose:pressed {"
        "      border-image: url(:/images/home_control/curtain-close-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonTVStop {"
        "      border-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVStop:focus {"
        "      border-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVStop:pressed {"
        "      border-image: url(:/images/home_control/power-on.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonTVStart {"
        "      border-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVStart:focus {"
        "      border-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVStart:pressed {"
        "      border-image: url(:/images/home_control/power-off.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonTVMode {"
        "      border-image: url(:/images/home_control/tv-mode-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVMode:focus {"
        "      border-image: url(:/images/home_control/tv-mode-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVMode:pressed {"
        "      border-image: url(:/images/home_control/tv-mode-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonTVNextChannel {"
        "      border-image: url(:/images/home_control/up-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVNextChannel:focus {"
        "      border-image: url(:/images/home_control/up-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVNextChannel:pressed {"
        "      border-image: url(:/images/home_control/up-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonTVPrevChannel {"
        "      border-image: url(:/images/home_control/down-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVPrevChannel:focus {"
        "      border-image: url(:/images/home_control/down-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVPrevChannel:pressed {"
        "      border-image: url(:/images/home_control/down-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonTVIncVolume {"
        "      border-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVIncVolume:focus {"
        "      border-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVIncVolume:pressed {"
        "      border-image: url(:/images/home_control/add-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonTVDecVolume {"
        "      border-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVDecVolume:focus {"
        "      border-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonTVDecVolume:pressed {"
        "      border-image: url(:/images/home_control/sub-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonACStart {"
        "      border-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACStart:focus {"
        "      border-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACStart:pressed {"
        "      border-image: url(:/images/home_control/power-off.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonACStop {"
        "      border-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACStop:focus {"
        "      border-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACStop:pressed {"
        "      border-image: url(:/images/home_control/power-on.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonACTemperatureAdd {"
        "      border-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACTemperatureAdd:focus {"
        "      border-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACTemperatureAdd:pressed {"
        "      border-image: url(:/images/home_control/add-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonACTemperatureSub {"
        "      border-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACTemperatureSub:focus {"
        "      border-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACTemperatureSub:pressed {"
        "      border-image: url(:/images/home_control/sub-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonACSwing {"
        "      border-image: url(:/images/home_control/ac-swing-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACSwing:focus {"
        "      border-image: url(:/images/home_control/ac-swing-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACSwing:pressed {"
        "      border-image: url(:/images/home_control/ac-swing-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonACWind {"
        "      border-image: url(:/images/home_control/ac-wind-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACWind:focus {"
        "      border-image: url(:/images/home_control/ac-wind-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACWind:pressed {"
        "      border-image: url(:/images/home_control/ac-wind-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonACMode {"
        "      border-image: url(:/images/home_control/ac-mode-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACMode:focus {"
        "      border-image: url(:/images/home_control/ac-mode-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonACMode:pressed {"
        "      border-image: url(:/images/home_control/ac-mode-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMOpen {"
        "      border-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMOpen:focus {"
        "      border-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMOpen:pressed {"
        "      border-image: url(:/images/home_control/power-off.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMClose {"
        "      border-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMClose:focus {"
        "      border-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMClose:pressed {"
        "      border-image: url(:/images/home_control/power-on.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMMute {"
        "      border-image: url(:/images/home_control/mute-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMMute:focus {"
        "      border-image: url(:/images/home_control/mute-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMMute:pressed {"
        "      border-image: url(:/images/home_control/mute-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMUnmute {"
        "      border-image: url(:/images/home_control/unmute-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMUnmute:focus {"
        "      border-image: url(:/images/home_control/unmute-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMUnmute:pressed {"
        "      border-image: url(:/images/home_control/unmute-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMSource {"
        "      border-image: url(:/images/home_control/source-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMSource:focus {"
        "      border-image: url(:/images/home_control/source-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMSource:pressed {"
        "      border-image: url(:/images/home_control/source-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMPrevMusic {"
        "      border-image: url(:/images/home_control/prev-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMPrevMusic:focus {"
        "      border-image: url(:/images/home_control/prev-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMPrevMusic:pressed {"
        "      border-image: url(:/images/home_control/prev-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMNextMusic {"
        "      border-image: url(:/images/home_control/next-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMNextMusic:focus {"
        "      border-image: url(:/images/home_control/next-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMNextMusic:pressed {"
        "      border-image: url(:/images/home_control/next-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMPlay {"
        "      border-image: url(:/images/home_control/play-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMPlay:focus {"
        "      border-image: url(:/images/home_control/play-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMPlay:pressed {"
        "      border-image: url(:/images/home_control/play-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMPause {"
        "      border-image: url(:/images/home_control/pause-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMPause:focus {"
        "      border-image: url(:/images/home_control/pause-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMPause:pressed {"
        "      border-image: url(:/images/home_control/pause-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMVolumeAdd {"
        "      border-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMVolumeAdd:focus {"
        "      border-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMVolumeAdd:pressed {"
        "      border-image: url(:/images/home_control/add-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonBMVolumeSub {"
        "      border-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMVolumeSub:focus {"
        "      border-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonBMVolumeSub:pressed {"
        "      border-image: url(:/images/home_control/sub-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCOpen {"
        "      border-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCOpen:focus {"
        "      border-image: url(:/images/home_control/power-off.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCOpen:pressed {"
        "      border-image: url(:/images/home_control/power-off.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCClose {"
        "      border-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCClose:focus {"
        "      border-image: url(:/images/home_control/power-on.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCClose:pressed {"
        "      border-image: url(:/images/home_control/power-on.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCUp {"
        "      border-image: url(:/images/home_control/up-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCUp:focus {"
        "      border-image: url(:/images/home_control/up-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCUp:pressed {"
        "      border-image: url(:/images/home_control/up-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCDown {"
        "      border-image: url(:/images/home_control/down-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCDown:focus {"
        "      border-image: url(:/images/home_control/down-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCDown:pressed {"
        "      border-image: url(:/images/home_control/down-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCLeft {"
        "      border-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCLeft:focus {"
        "      border-image: url(:/images/home_control/sub-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCLeft:pressed {"
        "      border-image: url(:/images/home_control/sub-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCRight {"
        "      border-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCRight:focus {"
        "      border-image: url(:/images/home_control/add-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCRight:pressed {"
        "      border-image: url(:/images/home_control/add-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCOK {"
        "      border-image: url(:/images/home_control/ok-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCOK:focus {"
        "      border-image: url(:/images/home_control/ok-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCOK:pressed {"
        "      border-image: url(:/images/home_control/ok-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCPlay {"
        "      border-image: url(:/images/home_control/play-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCPlay:focus {"
        "      border-image: url(:/images/home_control/play-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCPlay:pressed {"
        "      border-image: url(:/images/home_control/play-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCPause {"
        "      border-image: url(:/images/home_control/pause-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCPause:focus {"
        "      border-image: url(:/images/home_control/pause-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCPause:pressed {"
        "      border-image: url(:/images/home_control/pause-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCPrev {"
        "      border-image: url(:/images/home_control/prev-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCPrev:focus {"
        "      border-image: url(:/images/home_control/prev-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCPrev:pressed {"
        "      border-image: url(:/images/home_control/prev-2.png);"
		"  }";

	strStyleSheet += " QPushButton#pushButtonUCNext {"
        "      border-image: url(:/images/home_control/next-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCNext:focus {"
        "      border-image: url(:/images/home_control/next-1.png);"
		"  }";
	strStyleSheet += " QPushButton#pushButtonUCNext:pressed {"
        "      border-image: url(:/images/home_control/next-2.png);"
		"  }";

	/*strStyleSheet += "  QDialog#LightControlDialog {"
        "      border-image: url(:/images/home_control/background-1.png);"
		"  }";
	*/
	strStyleSheet += "  QFrame#frameHC1 {"
        "     border-image: url(:/images/home_control/panel-1.png);"
		"	  border: 0px;"
		"  }";

	strStyleSheet += "  QFrame#frameHC2 {"
        "     border-image: url(:/images/home_control/panel-2.png);"
		"	  border: 0px;"
		"  }";

	strStyleSheet += "  QFrame#frameHCCurtain {"
        "     border-image: url(:/images/home_control/panel-curtain.png);"
		"	  border: 0px;"
		"  }";

	strStyleSheet += "  QFrame#frameHCDeviceType {"
        "     border-image: url(:/images/home_control/panel-device_type.png);"
		"	  border: 0px;"
		"  }";

	//Graphics view

    strStyleSheet += "  QFrame#framePanel_video {"
        "     border-image: url(:/images/main/panel_small.png);"
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
        "      border-image:  url(:/images/system_setting/version_table.png);"
		"  }";

	strStyleSheet += "  QFrame#framePasswordSetting {"
        "      border-image:  url(:/images/system_setting/password_table.png);"
		"  }";

	strStyleSheet += "  QFrame#frameSecuritySetting {"
        "      border-image:  url(:/images/system_setting/security_table.png);"
		"  }";

	strStyleSheet += "  QFrame#frameDateTimeSetting {"
        "      border-image:  url(:/images/system_setting/datetime_table.png);"
		"  }";

	strStyleSheet += "  QFrame#frameDoorMachineSetting {"
        "      border-image:  url(:/images/system_setting/doormachine_table.png);"
		"  }";

	strStyleSheet += "  QFrame#frameIpCameraSetting {"
        "      border-image:  url(:/images/system_setting/ipcamera_table.png);"
		"  }";

	strStyleSheet += "  QFrame#frameRingAmdVolumeSetting {"
        "      border-image:  url(:/images/system_setting/address_table.png);"
		"  }";

	strStyleSheet += "  QFrame#frameOtherSetting {"
        "      background-image:  url(:/images/system_setting/address_table.png);"
		"  }";

	strStyleSheet += "  QFrame#frameAddressSetting {"
        "      border-image:  url(:/images/system_setting/address_table.png);"
		"  }";

    strStyleSheet += "  QFrame#frameDefenseSetting {"
        "      border-image:  url(:/images/system_setting/defense_table.png);"
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
        "      border-image: url(:/images/home_control/slider-handle.png);"
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
            "      border-image: url(:/images/keyboard/bg.png);"
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
