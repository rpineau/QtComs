#include <QCoreApplication>
#include <QStringList>
#include <QTextStream>


#include "pegasus.h"
#include "RTI-Dome.h"

int main(int argc, char *argv[])
{
    int nErr = DMFC_OK;
    char szFirmware[256];
    int nPos;
    unsigned int i = 0;
    bool bGotoComplete;
    std::vector<std::string> portList;
    QCoreApplication coreApplication(argc, argv);
    const int argumentCount = QCoreApplication::arguments().size();
    const QStringList argumentList = QCoreApplication::arguments();
    double dHomeAz;
    double dParkAz;
    double dAz;
    bool bComplete;
    bool bShutterPresent;

    serialPort::getPortList(portList);

    for(i=0;i<portList.size(); i++)
        printf("Port %s detected\n", portList.at(i).c_str());

/*
    CRTIDome mDome;

    mDome.setConnectionType(TYPE_TCP);
    
    nErr = mDome.Connect("", "192.168.252.8", 2323);
    if(nErr) {
        printf("Error connecting to dome : %d\n", nErr);
        return nErr;
    }
    printf("Connected to dome\n");

    nErr = mDome.getFirmwareVersion(szFirmware, 256);
    if(nErr) {
        printf("Error getting firmware : %d\n", nErr);
        return nErr;
    }

    // while(true) {
        printf("Firmware : %s\n", szFirmware);
        dHomeAz = mDome.getHomeAz();
        printf("dHomeAz : %3.2f\n", dHomeAz);
        dParkAz = mDome.getParkAz();
        printf("dParkAz : %3.2f\n", dParkAz);
        dAz = mDome.getCurrentAz();
        printf("dAz : %3.2f\n", dAz);
        mDome.getShutterPresent(bShutterPresent);
        printf("getShutterPresent : %s\n", bShutterPresent?"Yes":"No");
        i = mDome.getCurrentShutterState();
        printf("Shutter State : %d\n", i);
        mDome.isOpenComplete(bComplete);
        printf("isOpenComplete : %s\n", bComplete?"Yes":"No");
        mDome.isCloseComplete(bComplete);
        printf("isCloseComplete : %s\n", bComplete?"Yes":"No");
    // }
    mDome.Disconnect();
    */
    /*
    serialPort::getPortList(portList);

    for(i=0;i<portList.size(); i++)
        printf("Port %s detected\n", portList.at(i).c_str());

    CPegasusController mDMFC;
    CRTIDome mDome;

    mDMFC.setConnectionType(TYPE_SERIAL);

    printf("Connecting to DMFC\n");
    nErr = mDMFC.Connect("cu.usbserial-PA19Z6OX");
    if(nErr) {
        printf("Error connecting to DMFC : %d\n", nErr);
        return 1;
    }

    nErr = mDMFC.getFirmwareVersion(szFirmware, 256);
    if(nErr) {
        printf("Error getting firmware : %d\n", nErr);
        return 1;
    }

    printf("Firmware version %s\n", szFirmware);

    nErr = mDMFC.getPosition(nPos);
    if(nErr) {
        printf("Error getting position : %d\n", nErr);
        return 1;
    }

    printf("Current position %d\n", nPos);
    nErr = mDMFC.syncMotorPosition(0);
    if(nErr) {
        printf("Error setting position position to 0 : %d\n", nErr);
        return 1;
    }
    nErr = mDMFC.gotoPosition(1234);
    if(nErr) {
        printf("Error goto position 1234 : %d\n", nErr);
        return 1;
    }

    printf("Waiting for end of goto\n");
    bGotoComplete = false;
    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        nErr = mDMFC.isGoToComplete(bGotoComplete);
        mDMFC.getPosition(nPos);
        printf("Position : %d\n", nPos);
    } while(!bGotoComplete);

    mDMFC.Disconnect();
     */
    
    return 0;
}
