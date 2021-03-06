//
//  RTI-Dome.h
//  RTI-Dome
//
//  Created by Rodolphe Pineau on 2020/10/4.
//  RTI-Dome X2 plugin

#ifndef __RTI_Dome__
#define __RTI_Dome__

#define SB_MAC_BUILD

// standard C includes
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <memory.h>
#include <math.h>
#include <string.h>
#include <time.h>
#ifdef SB_MAC_BUILD
#include <unistd.h>
#endif
// C++ includes
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>

#include "deviceAccess.h"
#include "serialPort.h"
#include "TCPPort.h"
#include "UDPPort.h"

#include "StopWatch.h"

#define MAKE_ERR_CODE(P_ID, DTYPE, ERR_CODE)  (((P_ID<<24) & 0xff000000) | ((DTYPE<<16) & 0x00ff0000)  | (ERR_CODE & 0x0000ffff))

#define SERIAL_BUFFER_SIZE 256
#define R_MAX_TIMEOUT 500
#define MAX_READ_WAIT_TIMEOUT 250
#define NB_RX_WAIT 3
#define ND_LOG_BUFFER_SIZE 256
#define PANID_TIMEOUT 15    // in seconds
#define RAIN_CHECK_INTERVAL 10

// to be unified latter
#define FIRMWARE_NOT_SUPPORTED 1
#define PLUGIN_RXTIMEOUT 2
#define PLUGIN_DEVICEPARKED 3
#define PLUGIN_CONNECTION_FAILED 4

#define PLUGIN_VERSION      2.65
#define PLUGIN_ID   1

#define PLUGIN_DEBUG 3


// error codes
// Error code
enum RTIDomeErrors {PLUGIN_OK=0, PLUGIN_NOT_CONNECTED, PLUGIN_CANT_CONNECT, BAD_CMD_RESPONSE, PLUGIN_COMMAND_FAILED, RTI_COMMAND_TIMEOUT};
enum RTIDomeShutterState {OPEN = 0, CLOSED, OPENING, CLOSING, BOTTOM_OPEN, BOTTOM_CLOSED, BOTTOM_OPENING, BOTTOM_CLOSING, SHUTTER_ERROR };
enum HomeStatuses {NOT_AT_HOME = 0, HOMED, ATHOME};
enum RainActions {DO_NOTHING=0, HOME, PARK};

// RG-11
enum RainSensorStates {RAINING= 0, NOT_RAINING, RAIN_UNKNOWN};

class CRTIDome
{
public:
    CRTIDome();
    ~CRTIDome();

    int         Connect(const char *pszPort, const char *IpAddr, int nIpPort);
    void        Disconnect(void);
    bool        IsConnected(void) { return m_bIsConnected; }

    // Dome commands
    int syncDome(double dAz, double dEl);
    int parkDome(void);
    int unparkDome(void);
    int gotoAzimuth(double dNewAz);
    int openShutter();
    int closeShutter();
    int getFirmwareVersion(char *szVersion, int nStrMaxLen);
    int getFirmwareVersion(float &fVersion);
    int getShutterFirmwareVersion(char *szVersion, int nStrMaxLen);
    int goHome();
    int calibrate();

    // command complete functions
    int isGoToComplete(bool &bComplete);
    int isOpenComplete(bool &bComplete);
    int isCloseComplete(bool &bComplete);
    int isParkComplete(bool &bComplete);
    int isUnparkComplete(bool &bComplete);
    int isFindHomeComplete(bool &bComplete);
    int isCalibratingComplete(bool &bComplete);

    int abortCurrentCommand();
    int sendShutterHello();
    int getShutterPresent(bool &bShutterPresent);
    // getter/setter
    int getNbTicksPerRev();
    int setNbTicksPerRev(int nSteps);

    int getBatteryLevel();

    double getHomeAz();
    int setHomeAz(double dAz);

    double getParkAz();
    int setParkAz(double dAz);

    double getCurrentAz();
    double getCurrentEl();

    int getCurrentShutterState();
    int getBatteryLevels(double &domeVolts, double &dDomeCutOff, double &dShutterVolts, double &dShutterCutOff);
    int setBatteryCutOff(double dDomeCutOff, double dShutterCutOff);

    int getDefaultDir(bool &bNormal);
    int setDefaultDir(bool bNormal);

    int getRainSensorStatus(int &nStatus);

    int getRotationSpeed(int &nSpeed);
    int setRotationSpeed(int nSpeed);

    int getRotationAcceleration(int &nAcceleration);
    int setRotationAcceleration(int nAcceleration);

    int getShutterSpeed(int &nSpeed);
    int setShutterSpeed(int nSpeed);

    int getShutterAcceleration(int &nAcceleration);
    int setShutterAcceleration(int nAcceleration);

    void setHomeOnPark(const bool bEnabled);
    void setHomeOnUnpark(const bool bEnabled);

	int	getSutterWatchdogTimerValue(int &nValue);
	int	setSutterWatchdogTimerValue(const int &nValue);

    int getRainAction(int &nAction);
    int setRainAction(const int &nAction);

    int getPanId(int &nPanId);
    int setPanId(const int nPanId);
    int getShutterPanId(int &nPanId);
    int isPanIdSet(const int nPanId, bool &bSet);
    
    int restoreDomeMotorSettings();
    int restoreShutterMotorSettings();
    
    void enableRainStatusFile(bool bEnable);
    void getRainStatusFileName(std::string &fName);
    void writeRainStatus();

    // network config
    int getMACAddress(std::string &MACAddress);
    int reconfigureNetwork();

    int getUseDHCP(bool &bUseDHCP);
    int setUseDHCP(bool bUseDHCP);

    int getIpAddress(std::string &IpAddress);
    int setIpAddress(std::string IpAddress);

    int getSubnetMask(std::string &subnetMask);
    int setSubnetMask(std::string subnetMask);

    int getIPGateway(std::string &IpAddress);
    int setIPGateway(std::string IpAddress);

    void        setConnectionType(const int nConnectionType);
    void        getConnectionType(int &nConnectionType);

    
protected:

    int             domeCommand(const char *cmd, char *result, char respCmdCode, int resultMaxLen, int nTimeout = R_MAX_TIMEOUT);
    int             readResponse(char *respBuffer, int nBufferLen, int nTimeout = R_MAX_TIMEOUT);
    int             getDomeAz(double &dDomeAz);
    int             getDomeEl(double &dDomeEl);
    int             getDomeHomeAz(double &dAz);
    int             getDomeParkAz(double &dAz);
    int             getShutterState(int &nState);
    int             getDomeStepPerRev(int &nStepPerRev);
    int             setDomeStepPerRev(int nStepPerRev);

    bool            isDomeMoving();
    bool            isDomeAtHome();
    int             parseFields(const char *pszResp, std::vector<std::string> &svFields, char cSeparator);

    deviceAccess    *m_devicePort;
    
    int             m_DeviceConnectionType;
    
    std::string     m_DeviceName;
    int             m_nDevicePort;
    
    bool            m_bNetworkConnected;

    bool            m_bIsConnected;
    bool            m_bParked;
    bool            m_bShutterOpened;
    bool            m_bCalibrating;

    int             m_nNbStepPerRev;
    double          m_dShutterBatteryVolts;
    double          m_dHomeAz;

    double          m_dParkAz;

    double          m_dCurrentAzPosition;
    double          m_dCurrentElPosition;

    double          m_dGotoAz;

    float           m_fVersion;

    char            m_szFirmwareVersion[SERIAL_BUFFER_SIZE];
    int             m_nShutterState;
    bool            m_bShutterOnly; // roll off roof so the arduino is running the shutter firmware only.
    char            m_szLogBuffer[ND_LOG_BUFFER_SIZE];
    int             m_nHomingTries;
    int             m_nGotoTries;
    bool            m_bParking;
    bool            m_bUnParking;
    int             m_nIsRaining;
    bool            m_bHomeOnPark;
    bool            m_bHomeOnUnpark;
    bool            m_bShutterPresent;

    std::string     m_sRainStatusfilePath;
    FILE            *RainStatusfile;
    bool            m_bSaveRainStatus;
    int             m_nRainStatus;
    CStopWatch      m_cRainCheckTimer;
    
    std::string     m_IpAddress;
    std::string     m_SubnetMask;
    std::string     m_GatewayIP;
    bool            m_bUseDHCP;
    
    void            mySleep(int sleepMs); // Platform independent sleep
    
#ifdef PLUGIN_DEBUG
    std::string m_sLogfilePath;
    // timestamp for logs
    char *timestamp;
    time_t ltime;
    FILE *Logfile;	  // LogFile
#endif

};

#endif
