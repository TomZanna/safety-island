TARGET = uEdge_FSoE_master
CONFIG += console
CONFIG   -= app_bundle

TEMPLATE = app

FSOEDIR = $$_PRO_FILE_PWD_/fsoe

INCLUDEPATH += $$FSOEDIR/src $$FSOEDIR/FSoEappl $$FSOEDIR/include $$FSOEDIR/src/port/linux

QMAKE_LFLAGS += -Wall -dl -pthread -lrt -lgpiod -lm -Wl,--gc-sections

HEADERS += \
	applInterface.h \
	coeappl.h \
	eoeappl.h \
	foeappl.h \
	diag.h \
	ecat_def.h \
	ecatappl.h \
	ecatcoe.h \
	ecateoe.h \
	ecatfoe.h \
	bootmode.h \
	ecatslv.h \
	eeprom.h \
	emcy.h \
	esc.h \
	ini_file.h \
	mailbox.h \
	mcihw.h \
	objdef.h \
	sdoserv.h \
	SSC-Device.h \
	SSC-DeviceObjects.h \
	system_header.h \
	fsoe//src/port/linux/fsoeport_macros.h \
	fsoe/FSoEappl/utypes.h \
	fsoe/src/fsoelog.h \
	fsoe/src/fsoechannel.h \
	fsoe/src/fsoeframe.h \
	fsoe/src/fsoeport.h \
	fsoe/src/fsoesracrc.h \
	fsoe/src/fsoeutil.h \
	fsoe/src/fsoewatchdog.h

SOURCES += \
#	coeappl.c \
#	eoeappl.c \
#	foeappl.c \
#	diag.c \
#	ecatappl.c \
#	ecatcoe.c \
#	ecateoe.c \
#	ecatfoe.c \
#	bootmode.c \
#	ecatslv.c \
#	emcy.c \
#	ini_file.c \
#	mailbox.c \
#	mcihw.c \
#	objdef.c	\
#	plc_2000.c \
#	plc_3000.c \
#	plc_4000.c	\
#	sdoserv.c \
	SSC-Device.c \
	safety_main.c \
	fsoe/src/fsoemaster.c \
	fsoe//src/port/linux/fsoeport.c \
#	fsoe/FSoEappl/safety_main.c \
#	fsoe/src/fsoeslave.c \
	fsoe/src/fsoeapp.c \
	fsoe/src/fsoechannel.c \
	fsoe/src/fsoeframe.c \
#	fsoe/src/fsoeoptions.c \
#	fsoe/src/fsoesracrc.c \
	fsoe/src/fsoeutil.c \
	fsoe/src/fsoewatchdog.c

QMAKE_CFLAGS += -g -rdynamic -funwind-tables -Wall -Wextra -Wno-unused-parameter
QMAKE_CXXFLAGS += $$QMAKE_CFLAGS


