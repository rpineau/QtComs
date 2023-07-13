#include <QCoreApplication>
#include <QStringList>
#include <QTextStream>


#include "pegasus.h"
#include "RTI-Dome.h"

int main(int argc, char *argv[])
{
    int nErr = DMFC_OK;
    std::string sFirmware;
    float fFirmware;
    int nPos;
    unsigned int i = 0;
    bool bGotoComplete;
    std::vector<std::string> portList;
    std::vector<std::vector<std::string>> portListWithInfo;
    QCoreApplication coreApplication(argc, argv);
    const int argumentCount = QCoreApplication::arguments().size();
    const QStringList argumentList = QCoreApplication::arguments();
    double dHomeAz;
    double dParkAz;
    double dAz;
    bool bComplete;
    bool bShutterPresent;
    std::string sShuterState;
    serialPort::getPortList(portList);
    serialPort::getPortListWithInfo(portListWithInfo);

    //for(i=0;i<portList.size(); i++) {
    //    std::cout << "Port "<< portList.at(i) << " detected." << std::endl;
    //}

    for(i=0;i<portListWithInfo.size(); i++) {
        std::cout << "Port "<< portListWithInfo.at(i).at(0) << " [ " << portListWithInfo.at(i).at(1) << " ]" << std::endl;

/*
        printf("Opening %s\n", portList.at(i).c_str());
        bool b_open = false;
        serialPort *m_devicePort;
        m_devicePort = new serialPort();
        b_open = m_devicePort->open(portList.at(i).c_str(), QSerialPort::Baud115200, QSerialPort::Data8, QSerialPort::OneStop, QSerialPort::NoFlowControl, QSerialPort::NoParity, true);
        if(b_open) {
            printf("Open ok, closing\n");
            m_devicePort->clearRxTxBuffers();
            m_devicePort->close();
        }
        else {
            printf("Error opening %s\n", portList.at(i).c_str());
        }
*/
    }
/*
    CRTIDome mDome;

    mDome.setConnectionType(TYPE_TCP);

    nErr = mDome.Connect("", "192.168.252.8", 2323);
    if(nErr) {
        printf("Error connecting to dome : %d\n", nErr);
        return nErr;
    }
    printf("Connected to dome\n");

    nErr = mDome.getFirmwareVersion(sFirmware, fFirmware);
    if(nErr) {
        printf("Error getting firmware : %d\n", nErr);
        return nErr;
    }

    // while(true) {
        printf("Firmware : %s\n", sFirmware.c_str());
        dHomeAz = mDome.getHomeAz();
        printf("dHomeAz : %3.2f\n", dHomeAz);
        dParkAz = mDome.getParkAz();
        printf("dParkAz : %3.2f\n", dParkAz);
        dAz = mDome.getCurrentAz();
        printf("dAz : %3.2f\n", dAz);
        mDome.getShutterPresent(bShutterPresent);
        printf("getShutterPresent : %s\n", bShutterPresent?"Yes":"No");
        mDome.isOpenComplete(bComplete);
        if(bComplete)
            sShuterState="Open";
        else {
            mDome.isCloseComplete(bComplete);
            if(bComplete)
                sShuterState="Closed";
            else
                sShuterState="Uknown";
        }
        printf("Shutter State : %s\n", sShuterState.c_str());
        mDome.isOpenComplete(bComplete);
        printf("isOpenComplete : %s\n", bComplete?"Yes":"No");
        mDome.isCloseComplete(bComplete);
        printf("isCloseComplete : %s\n", bComplete?"Yes":"No");
    // }
    mDome.Disconnect();
*/

/*
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
