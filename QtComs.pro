QT = core
QT += serialport
QT += network

CONFIG += console
CONFIG -= app_bundle

TARGET = QtComs
TEMPLATE = app

SOURCES += main.cpp \
	serialPort.cpp \
	TCPPort.cpp \
	UDPPort.cpp \
	deviceAccess.cpp \
	pegasus.cpp \
	RTI-Dome.cpp

target.path = ./
INSTALLS += target
