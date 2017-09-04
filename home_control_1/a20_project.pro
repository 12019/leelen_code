#-------------------------------------------------
#
# Project created by QtCreator 2011-05-12T00:20:19
#
#-------------------------------------------------

#11.13 add resolution negotiate when monitor
#11.15 flash space is not released when stop record at the alert and record
#12.31 sometimes tvout has not video out

#2014.4.21 modity the voip delay and break down when record to multi machine

#2014.5.8  security record and info record have some problem about ...
#2014.5.8 add home contro and romote control

QT             += core gui network xml declarative sql
TEMPLATE  	= app
OBJECTS_DIR 	= build
MOC_DIR 	= build
UI_DIR 		= build

#define the different library arm11 or cortex-a7
CONFIG         += cortex-a7

#define the different resolution platform
CONFIG         += resolution_negotiation

resolution_800x480 {
        QMAKE_CXXFLAGS = -O2 -DRESOLUTION_800x480
        FORMSROOTDIR = ./ui/ui_800x480
        DESTDIR   	= ~/workspace/Projects/e60-test-env/mnt/disk/
        TARGET    	= main_projectv1.0-20150204
}

#resolution_1024x600 {
#	QMAKE_CXXFLAGS = -O2 -DRESOLUTION_1024x600
#	FORMSROOTDIR = ./ui/ui_1024x600
#	DESTDIR   	= ~/workspace/Projects/e70-test-env/
#	TARGET    	= main_project-v4.2C-1024x600
#}

#resolution_1280x800 {
#	QMAKE_CXXFLAGS = -O2 -DRESOLUTION_1280x800
#	FORMSROOTDIR = ./ui/ui_1280x800
#	DESTDIR   	= ~/share/
#	TARGET    	= main_project-v4.2C-1280x800
#}

#define STYLE_YELLOW or STYLE_BLUE
resolution_negotiation {
        QMAKE_CXXFLAGS = -O2 -DRESOLUTION_NEGOTIATION -DSTYLE_BLUE
        FORMSROOTDIR = ./ui/ui_800x480
        #DESTDIR   	= ~/share/
        TARGET    	= main_6602B_V2.0_20160301
}

RESOURCELIBPATH = $$PWD
LIVE555LIBPATH = $$PWD/lib-live555

arm11 {
        QMAKE_CXXFLAGS += -DARM11
        LILINLIBPATH = $$PWD/libMMv0.2

        #DESTDIR   	= ~/share/
        TARGET    	= main_6602B_V2.0_20150526
}


arm11 {
	QMAKE_CXXFLAGS += -DARM11
		LILINLIBPATH = $$PWD/libMMv0.2
	INCLUDEPATH += -I $$LILINLIBPATH/osip/include \
		       -I $$LILINLIBPATH/mediastreamer2/include \
		       -I $$LILINLIBPATH/oRTP/include
}

cortex-a7 {
	QMAKE_CXXFLAGS += -DCORTEXA7
                LILINLIBPATH = $$PWD/lib-a20

	INCLUDEPATH += -I $$LILINLIBPATH/osip/include \
		       -I $$LILINLIBPATH/mediastreamer2/include \
                       -I $$LILINLIBPATH/oRTP/include       \
                        -I $$LILINLIBPATH/codec    \
                        -I $$LILINLIBPATH/mediastreamer2/src \
                        -I $$LILINLIBPATH/codec/include/include_system \
                        -I $$LILINLIBPATH/codec/include \
                        -I $$LILINLIBPATH/codec/include/include_vencoder \
                        -I $$LILINLIBPATH/codec/include/include_vdecoder \
                        -I $$LILINLIBPATH/codec/include/include_system \
                        -I $$LILINLIBPATH/codec/include/include_platform/CHIP_A20/disp \
			-I $$LIVE555LIBPATH/include/BasicUsageEnvironment \
			-I $$LIVE555LIBPATH/include/liveMedia \
			-I $$LIVE555LIBPATH/include/groupsock \
			-I $$LIVE555LIBPATH/include/UsageEnvironment \
                        -I $$PWD/protocol
}

INCLUDEPATH += -I main -I info_man -I intercom_man -I terminal_help -I voip -I voip/ip_camera -I system_setting -I call_elevator -I door_bell \
		   -I system_setting/address -I system_setting/door_machine -I system_setting/ip_camera -I system_setting/language -I system_setting/password -I system_setting/datetime \
	       -I system_setting/ring_and_volume -I system_setting/device -I system_setting/version -I system_setting/security \
	       -I system_setting/calibration -I system_setting/other -I security -I call_center -I photo -I common -I home_control -I serialport \
	       -I input_method -I input_method/phrase -I input_method/scim \
               -I other

LIBS += -L$$RESOURCELIBPATH -lresource

arm11 {
LIBS += -L$$LILINLIBPATH/osip \
        -L$$LILINLIBPATH/mediastreamer2 \
        -L$$LILINLIBPATH/oRTP \
        -losip -lmediastreamer -lortp -lts -lrt
}

cortex-a7 {
LIBS += -L$$LILINLIBPATH/osip \
        -L$$LILINLIBPATH/mediastreamer2 \
        -L$$LILINLIBPATH/oRTP \
        -L$$LILINLIBPATH/codec/lib/A20 \
	-L$$LIVE555LIBPATH/lib \
        -L$$LILINLIBPATH/ \
        -L$$PWD/protocol \
	-lvdecoder -lvecore -lcdxalloc -lvencoder \
        -losip -lmediastreamer -lortp -lts -lrt \
	-lliveMedia -lgroupsock -lUsageEnvironment -lBasicUsageEnvironment \
        -ljpeg \
        -lleelenudp
}

SOURCES  += main.cpp \
        info_man/infomanviewattachmentdialog.cpp \
        info_man/infomanmaindialog.cpp \
        info_man/infomandetaildialog.cpp \
        intercom_man/mediaplayerdialog.cpp \
        intercom_man/mediamandialog.cpp \
        main/netrecvsocket.cpp \
        main/maindialog.cpp \
        voip/voipmaindialog.cpp \
        voip/mstdialog.cpp \
        voip/istdialog.cpp \
        voip/ictrcvhookdialog.cpp \
        voip/ictdialog.cpp \
            main/clock.cpp \
            main/digitalclock.cpp \
            main/calendar.cpp \
        system_setting/syssetmaindialog.cpp \
        system_setting/datetime/datetimesetdialog.cpp \
        system_setting/datetime/datetimesetthread.cpp\
        system_setting/datetime/timemode2dialog.cpp \
        system_setting/datetime/timemode1dialog.cpp \
        system_setting/datetime/timemode3dialog.cpp \
        main/helpdialog.cpp \
        call_center/callcenterdialog.cpp \
        system_setting/syssetpassworddialog.cpp \
        system_setting/language/languagesetdialog.cpp \
        stylesheet.cpp \
        stylesheet-1280x800.cpp \
        system_setting/address/addresssetdialog.cpp \
        system_setting/password/passwordsetdialog.cpp \
        system_setting/ring_and_volume/ringandvolumedialog.cpp \
        system_setting/ring_and_volume/ringfilesetdialog.cpp \
        system_setting/device/devicesetmaindialog.cpp \
        system_setting/device/addlampdialog.cpp \
        system_setting/device/adddefensedialog.cpp \
        system_setting/security/securityparamsetdialog.cpp \
        system_setting/security/securityphonesetdialog.cpp \
        security/serial1thread.cpp \
        security/securitymaindialog.cpp \
    security/securitypassworddialog.cpp \
    security/securityabnormaldialog.cpp \
    security/securityalertdialog.cpp \
    security/securityplayerdialog.cpp \
    security/jpegsendthread.cpp \
    security/sendalarminfothread.cpp\
    security/uploaddeviceinfothread.cpp\
    terminal_help/terminalhelpthread.cpp \
    voip/getpeeripthread.cpp \
    main/application.cpp \
    system_setting/calibration/calibrationdialog.cpp \
    voip/mondialog.cpp \
    input_method/public.cpp \
    input_method/PinyinFrame.cpp \
    input_method/PinyinEngine.cpp \
    input_method/mytextedit.cpp \
    input_method/mylineedit.cpp \
    input_method/inputmethod.cpp \
    input_method/digitpanel.cpp \
    input_method/Config.cpp \
    input_method/Big5pinyinpanel.cpp \
    input_method/Big5frame.cpp \
    input_method/Big5engine.cpp \
    input_method/phrase/Big5phrase.cpp \
    input_method/scim/Big5pinyin_char.cpp \
    main/keypadreadthread.cpp \
    main/updownloadsocket.cpp \
	voip/jpgsavetestthread.cpp \
    call_elevator/callelevatordialog.cpp \
    system_setting/version/versiondialog.cpp \
    door_bell/doorbelldialog.cpp \
    system_setting/syssetengineerdialog.cpp \
    system_setting/address/checkip.cpp \
	common/scrolllineedit.cpp \
    main/tscalibratethread.cpp \
    info_man/infowritedialog.cpp \
    main/mypushbutton.cpp \
    main/mylabel.cpp \
    common/commonpushbutton.cpp \
    common/mymessagebox.cpp \
    voip/monlistdialog.cpp \
    system_setting/door_machine/doormachinedialog.cpp \
    common/addressbookdialog.cpp \
    photo/photomaindialog.cpp \
    photo/photoudiskdialog.cpp \
    main/sendimgthread.cpp \
    voip/callinfouploadthread.cpp \
    security/securityrecvdoorwaysocket.cpp \
    main/keepwatchlivethread.cpp \
    security/defensegpioscanthread.cpp \
    voip/addresslist.cpp \
    system_setting/remotecheck/remotecheckdialog.cpp \
    input_method/inputinfodialog.cpp \
    global.cpp \
    other/fmylineedit.cpp \
    main/detectnetworkthread.cpp \
    voip/ip_camera/packetHandler.cpp \
    voip/ip_camera/testRTSPClient.cpp \
    system_setting/ip_camera/ipcameradialog.cpp \
    system_setting/other/otherdialog.cpp \
    main/doorbellringthread.cpp \
    info_man/sendinfothread.cpp \
    voip/authorizeelevatorthread.cpp \
    main/callelevatorthread.cpp \
    security/uploadsetsecuritystatethread.cpp \
    home_control/tvcontroldialog.cpp \
    home_control/rcvdatathread.cpp \
    home_control/musiccontroldialog.cpp \
    home_control/linrcvsever.cpp \
    home_control/lightcontroldialog.cpp \
    home_control/irpasswd.cpp \
    home_control/irlearndialog.cpp \
    home_control/housemapxmlthread.cpp \
    home_control/homecontrolsceneedit.cpp \
    home_control/homecontrolscenedialog.cpp \
    home_control/homecontrolmaindlg.cpp \
    home_control/hcstatusbar.cpp \
    home_control/hcsetting.cpp \
    home_control/hcsceneapplythread.cpp \
    home_control/hcroomselect.cpp \
    home_control/hcqextserialport.cpp \
    home_control/curtaincontroldialog.cpp \
    home_control/accontroldialog.cpp \
    home_control/serialport/qextserialport.cpp \
    home_control/serialport/qextserialbase.cpp \
    home_control/serialport/posix_qextserialport.cpp \
    home_control/MyQByteArray/myqbytearray.cpp \
    home_control/LIN_CLOUD_SDK/linclouddevsdkthread.cpp \
    home_control/ParaStyle/parastyle.cpp \
    home_control/Developer/developer.cpp \
    home_control/Security/riscoalarmsys.cpp \
    home_control/content/luxdomolist.cpp \
    home_control/content/luxdomopulldownlist.cpp \
    home_control/content/luxdomostylelist.cpp \
    home_control/content/luxdomotoolbar.cpp \
    home_control/content/luxdomoverticalprogressbar.cpp \
    home_control/LIN_CLOUD_SDK/linversionupdatethread.cpp \
    home_control/Security/luxdomorisco.cpp \
    home_control/Security/luxdomoriscocontrol.cpp \
    home_control/Security/luxdomoriscofinder.cpp \
    home_control/Security/luxdomoriscohistorymeaasge.cpp \
    home_control/Security/luxdomoriscosecurity.cpp \
    home_control/Security/luxdomoriscosecuritydata.cpp \
    home_control/device_control/luxdomoaccontrol.cpp \
    home_control/device_control/luxdomocurtaincontrol.cpp \
    home_control/device_control/luxdomomusiccontrol.cpp \
    home_control/device_control/luxdomotvcontrol.cpp \
    home_control/device_control/luxdomovar_lightcontrol.cpp \
    home_control/home_control/luxdomodevicefolder.cpp \
    home_control/home_control/luxdomodevicefolderpage.cpp \
    home_control/home_control/luxdomoroomcontrol.cpp \
    home_control/home_control/luxdomoroomcontrolpage.cpp \
    home_control/home_control/luxdomoroomdevice.cpp \
    home_control/home_manage/luxdomoacedit.cpp \
    home_control/home_manage/luxdomoaclearn.cpp \
    home_control/home_manage/luxdomodevice.cpp \
    home_control/home_manage/luxdomodevicecontainer.cpp \
    home_control/home_manage/luxdomodevicefunctionoption.cpp \
    home_control/home_manage/luxdomodeviceoption.cpp \
    home_control/home_manage/luxdomohomemanage.cpp \
    home_control/home_manage/luxdomomusicedit.cpp \
    home_control/home_manage/luxdomomusiclearn.cpp \
    home_control/home_manage/luxdomoroom.cpp \
    home_control/home_manage/luxdomoroomcontainer.cpp \
    home_control/home_manage/luxdomoroomfunctionoption.cpp \
    home_control/home_manage/luxdomoroommanage.cpp \
    home_control/home_manage/luxdomotvedit.cpp \
    home_control/home_manage/luxdomotvlearn.cpp \
    home_control/home_manage/luxdomovarlight.cpp \
    home_control/main_interface/luxdomoloadinglabel.cpp \
    home_control/main_interface/luxdomomaininterface.cpp \
    home_control/main_interface/luxdomomenu.cpp \
    home_control/main_interface/luxdomoscene.cpp \
    home_control/main_interface/luxdomothemebar.cpp \
    home_control/setting/luxdomoaboutsetting.cpp \
    home_control/setting/luxdomocloudsetting.cpp \
    home_control/setting/luxdomofeedbacksetting.cpp \
    home_control/setting/luxdomomajorsetting.cpp \
    home_control/setting/luxdomoscenefunctionoption.cpp \
    home_control/setting/luxdomoscenesetting.cpp \
    home_control/setting/luxdomoscenetoolbar.cpp \
    home_control/setting/luxdomosetting.cpp \
    home_control/setting/luxdomosynchronization.cpp \
    home_control/setting/luxdomoterminaisetting.cpp \
    home_control/setting/luxdomoversionssettting.cpp \
    home_control/thread/luxdomoapplyscene.cpp \
    home_control/thread/luxdomochangdevicedata.cpp \
    home_control/thread/luxdomohomescene.cpp \
    home_control/thread/luxdomoloadingdatatoui.cpp \
    home_control/thread/luxdomomanagehomedata.cpp \
    home_control/thread/luxdomocheckwlan.cpp \
    home_control/thread/luxdomodealwlandata.cpp \
    home_control/setting/luxdomohomepagesetting.cpp \
    home_control/setting/luxdomowlansetting.cpp \
    home_control/thread/luxdomomail.cpp \
    home_control/device_control/luxdomosensorinformation.cpp \
    home_control/home_manage/luxdomoroomscenecontrol.cpp \
    home_control/thread/luxdomosendcommand.cpp \
    home_control/server_client/luxdomoclient.cpp \
    home_control/server_client/luxdomoserver.cpp


HEADERS  += \
    data_types.h \
    info_man/infomanviewattachmentdialog.h \
    info_man/infomanmaindialog.h \
    info_man/infomandetaildialog.h \
    intercom_man/mediaplayerdialog.h \
    intercom_man/mediamandialog.h \
    main/netrecvsocket.h \
    main/maindialog.h \
    voip/voipmaindialog.h \
    voip/mstdialog.h \
    voip/istdialog.h \
    voip/ictrcvhookdialog.h \
    voip/ictdialog.h \
    main/clock.h \
    main/digitalclock.h \
    main/calendar.h \
	system_setting/syssetmaindialog.h \
	system_setting/datetime/datetimesetdialog.h \
        system_setting/datetime/datetimesetthread.h\
	system_setting/datetime/timemode2dialog.h \
	system_setting/datetime/timemode1dialog.h \
	system_setting/datetime/timemode3dialog.h \
    main/helpdialog.h \
    call_center/callcenterdialog.h \
	system_setting/syssetpassworddialog.h \
	system_setting/language/languagesetdialog.h \
	system_setting/address/addresssetdialog.h \
	system_setting/password/passwordsetdialog.h \
	system_setting/ring_and_volume/ringandvolumedialog.h \
	system_setting/ring_and_volume/ringfilesetdialog.h \
	system_setting/device/devicesetmaindialog.h \
    system_setting/device/addlampdialog.h \
    system_setting/device/adddefensedialog.h \
    system_setting/security/securityparamsetdialog.h \
    system_setting/security/securityphonesetdialog.h \
	security/serial1thread.h \
	security/securitymaindialog.h \
    security/securitypassworddialog.h \
    security/securityabnormaldialog.h \
    security/securityalertdialog.h \
    security/securityplayerdialog.h \
    security/jpegsendthread.h \
    security/sendalarminfothread.h\
    security/uploaddeviceinfothread.h\
	terminal_help/terminalhelpthread.h \
    voip/getpeeripthread.h \
    main/application.h \
    system_setting/calibration/calibrationdialog.h \
    voip/mondialog.h \
    input_method/public.h \
    input_method/PinyinFrame.h \
    input_method/PinyinEngine.h \
    input_method/mytextedit.h \
    input_method/mylineedit.h \
    input_method/inputmethod.h \
    input_method/digitpanel.h \
    input_method/Config.h \
    input_method/Big5pinyinpanel.h \
    input_method/Big5frame.h \
    input_method/Big5engine.h \
    input_method/phrase/Big5phrase.h \
    input_method/scim/Big5pinyin_char.h \
    main/keypadreadthread.h \
    main/updownloadsocket.h \
	voip/jpgsavetestthread.h \
    call_elevator/callelevatordialog.h \
    system_setting/version/versiondialog.h \
    door_bell/doorbelldialog.h \
    system_setting/syssetengineerdialog.h \
    system_setting/address/checkip.h \
	common/scrolllineedit.h \
    main/tscalibratethread.h \
    info_man/infowritedialog.h \
    main/mypushbutton.h \
    main/mylabel.h \
    common/commonpushbutton.h \
    common/mymessagebox.h \
    voip/monlistdialog.h \
    system_setting/door_machine/doormachinedialog.h \
    common/addressbookdialog.h \
    photo/photomaindialog.h \
    photo/photoudiskdialog.h \
    main/sendimgthread.h \
    voip/callinfouploadthread.h \
    security/securityrecvdoorwaysocket.h \
    main/keepwatchlivethread.h \
    security/defensegpioscanthread.h \
    voip/addresslist.h \
    system_setting/remotecheck/remotecheckdialog.h \
    input_method/inputinfodialog.h \
    global.h \
    other/fmylineedit.h \
    main/detectnetworkthread.h \
    voip/ip_camera/packetHandler.h \
    voip/ip_camera/testRTSPClient.h \
    system_setting/ip_camera/ipcameradialog.h \
    system_setting/other/otherdialog.h \
    main/doorbellringthread.h \
    info_man/sendinfothread.h \
    voip/authorizeelevatorthread.h \
    main/callelevatorthread.h \
    security/uploadsetsecuritystatethread.h \
    home_control/tvcontroldialog.h \
    home_control/rcvdatathread.h \
    home_control/MyBuffer.h \
    home_control/musiccontroldialog.h \
    home_control/linrcvsever.h \
    home_control/lightcontroldialog.h \
    home_control/irpasswd.h \
    home_control/irlearndialog.h \
    home_control/housemapxmlthread.h \
    home_control/homecontrolsceneedit.h \
    home_control/homecontrolscenedialog.h \
    home_control/homecontrolmaindlg.h \
    home_control/hcstatusbar.h \
    home_control/hcsetting.h \
    home_control/hcsceneapplythread.h \
    home_control/hcroomselect.h \
    home_control/hcqextserialport.h \
    home_control/curtaincontroldialog.h \
    home_control/accontroldialog.h \
    home_control/serialport/qextserialport.h \
    home_control/serialport/qextserialbase.h \
    home_control/serialport/posix_qextserialport.h \
    home_control/MyQByteArray/myqbytearray.h \
    home_control/LIN_CLOUD_SDK/linclouddevsdkthread.h \
    home_control/ParaStyle/parastyle.h \
    home_control/Developer/developer.h \
    home_control/Security/riscoalarmsys.h \
    home_control/content/luxdomolist.h \
    home_control/content/luxdomopulldownlist.h \
    home_control/content/luxdomostylelist.h \
    home_control/content/luxdomotoolbar.h \
    home_control/content/luxdomoverticalprogressbar.h \
    home_control/LIN_CLOUD_SDK/linversionupdatethread.h \
    home_control/Security/luxdomorisco.h \
    home_control/Security/luxdomoriscocontrol.h \
    home_control/Security/luxdomoriscofinder.h \
    home_control/Security/luxdomoriscohistorymeaasge.h \
    home_control/Security/luxdomoriscosecurity.h \
    home_control/Security/luxdomoriscosecuritydata.h \
    home_control/device_control/luxdomoaccontrol.h \
    home_control/device_control/luxdomocurtaincontrol.h \
    home_control/device_control/luxdomomusiccontrol.h \
    home_control/device_control/luxdomotvcontrol.h \
    home_control/device_control/luxdomovar_lightcontrol.h \
    home_control/home_control/luxdomodevicefolder.h \
    home_control/home_control/luxdomodevicefolderpage.h \
    home_control/home_control/luxdomoroomcontrol.h \
    home_control/home_control/luxdomoroomcontrolpage.h \
    home_control/home_control/luxdomoroomdevice.h \
    home_control/home_manage/luxdomoacedit.h \
    home_control/home_manage/luxdomoaclearn.h \
    home_control/home_manage/luxdomodevice.h \
    home_control/home_manage/luxdomodevicecontainer.h \
    home_control/home_manage/luxdomodevicefunctionoption.h \
    home_control/home_manage/luxdomodeviceoption.h \
    home_control/home_manage/luxdomohomemanage.h \
    home_control/home_manage/luxdomomusicedit.h \
    home_control/home_manage/luxdomomusiclearn.h \
    home_control/home_manage/luxdomoroom.h \
    home_control/home_manage/luxdomoroomcontainer.h \
    home_control/home_manage/luxdomoroomfunctionoption.h \
    home_control/home_manage/luxdomoroommanage.h \
    home_control/home_manage/luxdomotvedit.h \
    home_control/home_manage/luxdomotvlearn.h \
    home_control/home_manage/luxdomovarlight.h \
    home_control/main_interface/luxdomoloadinglabel.h \
    home_control/main_interface/luxdomomaininterface.h \
    home_control/main_interface/luxdomomenu.h \
    home_control/main_interface/luxdomoscene.h \
    home_control/main_interface/luxdomothemebar.h \
    home_control/setting/luxdomoaboutsetting.h \
    home_control/setting/luxdomocloudsetting.h \
    home_control/setting/luxdomofeedbacksetting.h \
    home_control/setting/luxdomomajorsetting.h \
    home_control/setting/luxdomoscenefunctionoption.h \
    home_control/setting/luxdomoscenesetting.h \
    home_control/setting/luxdomoscenetoolbar.h \
    home_control/setting/luxdomosetting.h \
    home_control/setting/luxdomosynchronization.h \
    home_control/setting/luxdomoterminaisetting.h \
    home_control/setting/luxdomoversionssettting.h \
    home_control/thread/luxdomoapplyscene.h \
    home_control/thread/luxdomochangdevicedata.h \
    home_control/thread/luxdomohomescene.h \
    home_control/thread/luxdomoloadingdatatoui.h \
    home_control/thread/luxdomomanagehomedata.h \
    home_control/thread/luxdomocheckwlan.h \
    home_control/thread/luxdomodealwlandata.h \
    home_control/setting/luxdomohomepagesetting.h \
    home_control/setting/luxdomowlansetting.h \
    home_control/thread/luxdomomail.h \
    home_control/device_control/luxdomosensorinformation.h \
    home_control/home_manage/luxdomoroomscenecontrol.h \
    home_control/thread/luxdomosendcommand.h \
    home_control/server_client/luxdomoclient.h \
    home_control/server_client/luxdomoserver.h


    FORMS    += \
    $$FORMSROOTDIR/info_man/infomanviewattachmentdialog.ui \
    $$FORMSROOTDIR/info_man/infomanmaindialog.ui \
    $$FORMSROOTDIR/info_man/infomandetaildialog.ui \
    intercom_man/mediaplayerdialog.ui \
    intercom_man/mediamandialog.ui \
    main/maindialog.ui \
    voip/voipmaindialog.ui \
    voip/mstdialog.ui \
    $$FORMSROOTDIR/voip/istdialog.ui \
    voip/ictrcvhookdialog.ui \
    $$FORMSROOTDIR/voip/ictdialog.ui \
    main/clock.ui \
    main/calendar.ui \
    $$FORMSROOTDIR/system_setting/syssetmaindialog.ui \
    system_setting/datetime/timemode2dialog.ui \
    system_setting/datetime/timemode1dialog.ui \
    system_setting/datetime/timemode3dialog.ui \
    $$FORMSROOTDIR/system_setting/datetime/datetimesetdialog.ui \
    $$FORMSROOTDIR/system_setting/password/passwordsetdialog.ui \
    $$FORMSROOTDIR/system_setting/syssetpassworddialog.ui \
    main/helpdialog.ui \
    call_center/callcenterdialog.ui \
    $$FORMSROOTDIR/system_setting/language/languagesetdialog.ui \
    $$FORMSROOTDIR/system_setting/address/addresssetdialog.ui \
    $$FORMSROOTDIR/system_setting/ring_and_volume/ringandvolumedialog.ui \
    $$FORMSROOTDIR/system_setting/ring_and_volume/ringfilesetdialog.ui \
    system_setting/device/devicesetmaindialog.ui \
    system_setting/device/addlampdialog.ui \
    $$FORMSROOTDIR/system_setting/device/adddefensedialog.ui \
    $$FORMSROOTDIR/system_setting/security/securityparamsetdialog.ui \
    system_setting/security/securityphonesetdialog.ui \
    $$FORMSROOTDIR/security/securitymaindialog.ui \
    $$FORMSROOTDIR/security/securitypassworddialog.ui \
    security/securityabnormaldialog.ui \
    security/securityalertdialog.ui \
    security/securityplayerdialog.ui \
    $$FORMSROOTDIR/voip/mondialog.ui \
    call_elevator/callelevatordialog.ui \
    $$FORMSROOTDIR/system_setting/version/versiondialog.ui \
    door_bell/doorbelldialog.ui \
    $$FORMSROOTDIR/system_setting/syssetengineerdialog.ui \
    $$FORMSROOTDIR/info_man/infowritedialog.ui \
    $$FORMSROOTDIR/voip/monlistdialog.ui \
    $$FORMSROOTDIR/system_setting/door_machine/doormachinedialog.ui \
    $$FORMSROOTDIR/common/addressbookdialog.ui \
    photo/photomaindialog.ui \
    photo/photoudiskdialog.ui \
    $$FORMSROOTDIR/system_setting/remotecheck/remotecheckdialog.ui \
    $$FORMSROOTDIR/input_method/digitpanel.ui \
    input_method/big5pinyinpanel.ui \
    $$FORMSROOTDIR/input_method/inputinfodialog.ui \
    $$FORMSROOTDIR/system_setting/ip_camera/ipcameradialog.ui \
    $$FORMSROOTDIR/system_setting/other/otherdialog.ui
unix:!macx: LIBS += -L$$PWD/home_control/lib/ -lHCImage


unix:!macx: LIBS += -L$$PWD/home_control/LINCLOUDLIB/ -lClientSDK_SO_H_CrossLinux
INCLUDEPATH += $$PWD/home_control/LINCLOUDLIB
DEPENDPATH += $$PWD/home_control/LINCLOUDLIB

INCLUDEPATH += $$PWD/home_control
DEPENDPATH += $$PWD/home_control

RESOURCES    += languages.qrc \
    home_control/ir.qrc \
    home_control/homecontrol_qml.qrc \
    home_control/home_control_images.qrc \
    home_control/profile.qrc
OTHER_FILES  +=
TRANSLATIONS += en.ts zh_CN.ts zh_TW.ts
QTPLUGIN     +=
