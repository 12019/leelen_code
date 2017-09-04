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
QT       += core gui network xml

TARGET = main_project-v4.2C-201400715Voice

TEMPLATE = app


SOURCES += main.cpp \
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
	system_setting/datetime/timemode2dialog.cpp \
	system_setting/datetime/timemode1dialog.cpp \
	system_setting/datetime/timemode3dialog.cpp \
    main/helpdialog.cpp \
    call_center/callcenterdialog.cpp \
	system_setting/syssetpassworddialog.cpp \
	system_setting/language/languagesetdialog.cpp \
    stylesheet.cpp \
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
    home_control/homecontroldialog.cpp \
    home_control/lightcontroldialog.cpp \
    home_control/curtaincontroldialog.cpp \
    home_control/accontroldialog.cpp \
    home_control/tvcontroldialog.cpp \
    home_control/hcgraphicsscene.cpp \
    home_control/hcgraphicspixmapitem.cpp \
    home_control/homecontrolscenedialog.cpp \
    main/mypushbutton.cpp \
    main/mylabel.cpp \
    common/commonpushbutton.cpp \
    common/mymessagebox.cpp \
    voip/monlistdialog.cpp \
    system_setting/door_machine/doormachinedialog.cpp \
    home_control/switchcontroldialog.cpp \
    home_control/musiccontroldialog.cpp \
    home_control/usercontroldialog.cpp \
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
    home_control/serial2thread.cpp

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
	terminal_help/terminalhelpthread.h \
    voip/getpeeripthread.h \
    main/application.h \
    system_setting/calibration/calibration.h \
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
    home_control/homecontroldialog.h \
    home_control/lightcontroldialog.h \
    home_control/curtaincontroldialog.h \
    home_control/accontroldialog.h \
    home_control/tvcontroldialog.h \
    home_control/hcgraphicsscene.h \
    home_control/hcgraphicspixmapitem.h \
    home_control/homecontrolscenedialog.h \
    main/mypushbutton.h \
    main/mylabel.h \
    common/commonpushbutton.h \
    common/mymessagebox.h \
    voip/monlistdialog.h \
    system_setting/door_machine/doormachinedialog.h \
    home_control/switchcontroldialog.h \
	home_control/musiccontroldialog.h \
    home_control/usercontroldialog.h \
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
    home_control/serial2thread.h

FORMS    += \
    info_man/infomanviewattachmentdialog.ui \
    info_man/infomanmaindialog.ui \
    info_man/infomandetaildialog.ui \
    intercom_man/mediaplayerdialog.ui \
    intercom_man/mediamandialog.ui \
    main/maindialog.ui \
    voip/voipmaindialog.ui \
    voip/mstdialog.ui \
    voip/istdialog.ui \
    voip/ictrcvhookdialog.ui \
    voip/ictdialog.ui \
    main/clock.ui \
    main/calendar.ui \
	system_setting/syssetmaindialog.ui \
	system_setting/datetime/datetimesetdialog.ui \
	system_setting/datetime/timemode2dialog.ui \
	system_setting/datetime/timemode1dialog.ui \
	system_setting/datetime/timemode3dialog.ui \
    main/helpdialog.ui \
    call_center/callcenterdialog.ui \
	system_setting/syssetpassworddialog.ui \
	system_setting/language/languagesetdialog.ui \
	system_setting/address/addresssetdialog.ui \
	system_setting/password/passwordsetdialog.ui \
	system_setting/ring_and_volume/ringandvolumedialog.ui \
	system_setting/ring_and_volume/ringfilesetdialog.ui \
	system_setting/device/devicesetmaindialog.ui \
    system_setting/device/addlampdialog.ui \
    system_setting/device/adddefensedialog.ui \
    system_setting/security/securityparamsetdialog.ui \
    system_setting/security/securityphonesetdialog.ui \
    security/securitymaindialog.ui \
    security/securitypassworddialog.ui \
    security/securityabnormaldialog.ui \
    security/securityalertdialog.ui \
    security/securityplayerdialog.ui \
	voip/mondialog.ui \
    input_method/digitpanel.ui \
    input_method/big5pinyinpanel.ui \
    call_elevator/callelevatordialog.ui \
    system_setting/version/versiondialog.ui \
    door_bell/doorbelldialog.ui \
    system_setting/syssetengineerdialog.ui \
    info_man/infowritedialog.ui \
    home_control/homecontroldialog.ui \
    home_control/lightcontroldialog.ui \
    home_control/curtaincontroldialog.ui \
    home_control/accontroldialog.ui \
    home_control/tvcontroldialog.ui \
    home_control/homecontrolscenedialog.ui \
	voip/monlistdialog.ui \
    system_setting/door_machine/doormachinedialog.ui \
    home_control/switchcontroldialog.ui \
	home_control/musiccontroldialog.ui \
    home_control/usercontroldialog.ui \
    common/addressbookdialog.ui \
    photo/photomaindialog.ui \
    photo/photoudiskdialog.ui \
    system_setting/remotecheck/remotecheckdialog.ui

INCLUDEPATH += -I /home/david/work/Ehome/Lphone/lphoneMM/lphonev0.2Voice/lphone/osip/include \
    -I /home/david/work/Ehome/Lphone/lphoneMM/lphonev0.2Voice/lphone/mediastreamer2/include \
    -I /home/david/work/Ehome/Lphone/lphoneMM/lphonev0.2Voice/lphone/oRTP/include \
	-I main -I info_man -I intercom_man -I terminal_help -I voip -I system_setting -I call_elevator -I door_bell \
	-I system_setting/address -I system_setting/door_machine -I system_setting/language -I system_setting/password -I system_setting/datetime \
	-I system_setting/ring_and_volume -I system_setting/device -I system_setting/version -I system_setting/security \
	-I system_setting/calibration -I security -I call_center -I photo -I common -I home_control \
	-I input_method -I input_method/phrase -I input_method/scim \
	-I /usr/local/Trolltech/QtEmbedded-4.7.2-arm/include \
	-I /usr/local/Trolltech/QtEmbedded-4.7.2-arm/include/QtCore \
	-I /usr/local/Trolltech/QtEmbedded-4.7.2-arm/include/QtGui \
	-I /usr/local/Trolltech/QtEmbedded-4.7.2-arm/include/QtNetwork \
	-I /usr/local/Trolltech/QtEmbedded-4.7.2-arm/include/QtXml \
	-I /usr/local/arm/arm-none-linux-gnueabi/arm-none-linux-gnueabi/libc/usr/include \
	-I /usr/local/arm/arm-none-linux-gnueabi/arm-none-linux-gnueabi/include/c++/4.3.2

LIBS += -L/home/david/work/Ehome/Lphone/lphoneMM/lphonev0.2Voice/lphone/osip \
        -L/home/david/work/Ehome/Lphone/lphoneMM/lphonev0.2Voice/lphone/mediastreamer2 \
        -L/home/david/work/Ehome/Lphone/lphoneMM/lphonev0.2Voice/lphone/oRTP \
        -losip -lmediastreamer -lortp -lts

RESOURCES += \
    languages.qrc

OTHER_FILES +=

TRANSLATIONS += en.ts zh_CN.ts zh_TW.ts
