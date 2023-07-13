#include "serialPort.h"

//
// Set params with constructor if needed.
//
serialPort::serialPort(const char *portName,
                       const QSerialPort::BaudRate Speed,
                       const QSerialPort::DataBits dataBits,
                       const QSerialPort::StopBits stopBits,
                       const QSerialPort::FlowControl FlowControl,
                       const QSerialPort::Parity Parity,
                       const bool bDTR,
                       const int bufferSize) : deviceAccess(portName,Speed, dataBits, stopBits, FlowControl, Parity, bDTR, bufferSize)
{
    m_bPortOpenned = false;

    m_PortName = portName;
    m_Speed = Speed;
    m_DataBits = dataBits;
    m_StopBits = stopBits;
    m_FlowControl = FlowControl;
    m_Parity = Parity;
    m_DTR = bDTR;
    m_ReadBufferSize = qint64(bufferSize);

    m_SerialDeviceConnection.setPortName(m_PortName);
    m_SerialDeviceConnection.setBaudRate(m_Speed, QSerialPort::AllDirections);
    m_SerialDeviceConnection.setDataBits(m_DataBits);
    m_SerialDeviceConnection.setStopBits(m_StopBits);
    m_SerialDeviceConnection.setFlowControl(m_FlowControl);
    m_SerialDeviceConnection.setParity(m_Parity);
    m_SerialDeviceConnection.setReadBufferSize(m_ReadBufferSize);
}

serialPort::~serialPort(void)
{
    if(m_bPortOpenned)
        close();
}

//
// Open with paremters set in constructor
//
bool serialPort::open()
{
    if(m_PortName.isEmpty()) {
        m_bPortOpenned = false;
    }
    else {
        if(!m_bPortOpenned) {
            m_bPortOpenned = m_SerialDeviceConnection.open(QIODevice::ReadWrite);
            if(m_bPortOpenned)
                m_SerialDeviceConnection.setDataTerminalReady(m_DTR);
        }
    }

    return m_bPortOpenned;
}

//
// Open with all new parameters.
// Will apply default params if not specified.
// At least the port name needs to be set.
//
bool serialPort::open(const QString portName, const QSerialPort::BaudRate Speed, const QSerialPort::DataBits dataBits, const QSerialPort::StopBits stopBits, const QSerialPort::FlowControl FlowControl, const QSerialPort::Parity Parity, const bool bDTR )
{
    m_bPortOpenned = false;

    if(portName.isEmpty() && m_PortName.isEmpty())
        return false;

    m_PortName = portName;
    m_Speed = Speed;
    m_DataBits = dataBits;
    m_StopBits = stopBits;
    m_FlowControl = FlowControl;
    m_Parity = Parity;
    m_DTR = bDTR;

    m_SerialDeviceConnection.setPortName(m_PortName);
    m_SerialDeviceConnection.setBaudRate(m_Speed, QSerialPort::AllDirections);
    m_SerialDeviceConnection.setDataBits(m_DataBits);
    m_SerialDeviceConnection.setStopBits(m_StopBits);
    m_SerialDeviceConnection.setFlowControl(m_FlowControl);
    m_SerialDeviceConnection.setParity(m_Parity);
    m_SerialDeviceConnection.setReadBufferSize(m_ReadBufferSize);

    m_bPortOpenned = m_SerialDeviceConnection.open(QIODevice::ReadWrite);
    if(m_bPortOpenned)
        m_SerialDeviceConnection.setDataTerminalReady(m_DTR);

    return m_bPortOpenned;
}

bool serialPort::open(const char *portName, const QSerialPort::BaudRate Speed, const QSerialPort::DataBits dataBits, const QSerialPort::StopBits stopBits, const QSerialPort::FlowControl FlowControl, const QSerialPort::Parity Parity, const bool bDTR )
{
    return open(QString(portName), Speed, dataBits, stopBits, FlowControl, Parity, bDTR);
}


//
// clear all data buffer and close the port
//
void serialPort::close()
{
    if(m_bPortOpenned) {
        m_SerialDeviceConnection.clear(QSerialPort::AllDirections);
        m_SerialDeviceConnection.close();
        m_bPortOpenned = false;
    }
}

bool serialPort::isOpened()
{
    return m_SerialDeviceConnection.isOpen();
}

//
// Wait for data to be written, with a timeout
//
bool serialPort::flushDataOut(int msecTimeout)
{
    if(!m_bPortOpenned) {
        return false;
    }
    return m_SerialDeviceConnection.waitForBytesWritten(msecTimeout);
}

//
// discard all buffer content
//
bool serialPort::clearRxTxBuffers()
{
    if(!m_bPortOpenned) {
        return false;
    }
    return m_SerialDeviceConnection.clear(QSerialPort::AllDirections);
}

//
// return the number of bytes available for read
//
int serialPort::bytesAvailable()
{

    if(!isOpened()) {
        return 0;
    }

    if(!m_SerialDeviceConnection.waitForReadyRead(1000))
        return 0; // we had a timeout

    return int(m_SerialDeviceConnection.bytesAvailable());
}

//
// Wait for a certain number of bytes to be read for read, with timeout
//
bool serialPort::waitForDataReady(const int &nbBytes, const int &msTimeout)
{
    qint64 nbByteAvail = 0;
    int nTimeout = 0;

    if(nbBytes > m_ReadBufferSize)
        return false;

    do {
        nbByteAvail = m_SerialDeviceConnection.bytesAvailable();
        if(nbByteAvail < nbBytes) {
            if(nbByteAvail == 0)
                nTimeout += 1000; //  bytesAvailable timeout after 1000ms so add this to the current total timeout counter
            else {
                msSleep(10);
                nTimeout += 10; //  we waited 10ms so add this to the current total timeout counter
            }
        }
        if(nTimeout >= msTimeout)
            return false; // we reached the timeout without enought bytes in the receive buffer
    } while(nbByteAvail<nbBytes);

    return true;
}

//
// read data from serial port into destBuffer, with a timeout
// return the number of byte actually read
//
int serialPort::readData(char *destBuffer, const int &amountOfByteToRead, const int &msTimeout)
{
    qint64 nbRead;
    qint64 nbByteAvailable;
    bool bByteReady;

    memset(destBuffer, 0, amountOfByteToRead);
    bByteReady = waitForDataReady(amountOfByteToRead, msTimeout);
    if(!bByteReady) {
        // read what we can
        nbByteAvailable = m_SerialDeviceConnection.bytesAvailable();
        nbRead = m_SerialDeviceConnection.read(destBuffer, nbByteAvailable);
    }
    else {
        nbRead = m_SerialDeviceConnection.read(destBuffer, qint64(amountOfByteToRead));
    }

    return int(nbRead);
}

//
// Write data from srcBuffer to the serial port, return the number of byte actually writen in nbByteWritten
// and return true/false if all data was written or not.
//
bool serialPort::writeData(const char *srcBuffer, const int &amountOfByteToWrite, int &nbByteWritten)
{
    nbByteWritten = int(m_SerialDeviceConnection.write(srcBuffer, quint64(amountOfByteToWrite)));
    return (nbByteWritten == amountOfByteToWrite);
}

//
// Get the state of the CTS pin
//
bool serialPort::getCTS()
{
    int nSignals;
    bool bCTS = false;

    if(!m_bPortOpenned) {
        return bCTS;
    }

    nSignals = m_SerialDeviceConnection.pinoutSignals();
    bCTS = (nSignals & QSerialPort::ClearToSendSignal) == QSerialPort::ClearToSendSignal;

    return bCTS;
}

//
// Set the state of the RTS pin
//

void serialPort::setRTS(bool bState)
{
    if(!m_bPortOpenned) {
        return;
    }
    m_SerialDeviceConnection.setRequestToSend(bState);
}

//
// return the list of available port on the machine as a std::vector<std::string>
//
void serialPort::getPortList(std::vector<std::string> &portList)
{
    QList<QSerialPortInfo> myList;
    portList.clear();
    // get serial devices
    myList = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &portInfo : myList) {
        if(!portInfo.portName().startsWith("tty.", Qt::CaseInsensitive) ) {// for macOS
            // printf("%s : %s\n", portInfo.portName().toUtf8().constData(), portInfo.description().toUtf8().constData());
            portList.push_back(portInfo.portName().toUtf8().constData());
        }
    }
#ifndef _WIN32
    // get symlinks
    QDir deviceDir("/dev");
    std::string sTmp;
    std::vector<std::string>::iterator it;
    QFileInfo fileInfo;
    deviceDir.setFilter(QDir::Files);
    QFileInfoList fileList = deviceDir.entryInfoList();
    for (int i = 0; i < fileList.size(); ++i) {
        fileInfo = fileList.at(i);
        if(fileInfo.isSymLink()) {
            sTmp = QFileInfo(fileInfo.symLinkTarget()).baseName().toStdString();
            it = std::find(portList.begin(), portList.end(), sTmp);
            if (it != portList.end()) {
                // printf("Symlink to %s : %s added \n", it->c_str(), fileInfo.fileName().toStdString().c_str());
                portList.push_back(fileInfo.fileName().toStdString());
            }
        }
    }
#endif
}

void serialPort::getPortListWithInfo(std::vector<std::vector<std::string>> &portList)
{
    QList<QSerialPortInfo> myList;
    std::vector<std::string> portEntryInfo;
    std::vector<std::string> tmpPortList;

    portList.clear();
    // get serial devices
    myList = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &portInfo : myList) {
        portEntryInfo.clear();
        if(!portInfo.portName().startsWith("tty.", Qt::CaseInsensitive) ) {// for macOS
            printf("%s : %s\n", portInfo.portName().toUtf8().constData(), portInfo.description().toUtf8().constData());
            tmpPortList.push_back(portInfo.portName().toUtf8().constData());
            portEntryInfo.push_back(portInfo.portName().toUtf8().constData());
            portEntryInfo.push_back(portInfo.description().toUtf8().constData());
            portList.push_back(portEntryInfo);
        }
    }
#ifndef _WIN32
    // get symlinks
    QDir deviceDir("/dev");
    std::string sTmp;
    std::vector<std::string>::iterator it;
    QFileInfo fileInfo;
    deviceDir.setFilter(QDir::Files);
    QFileInfoList fileList = deviceDir.entryInfoList();
    for (int i = 0; i < fileList.size(); ++i) {
        fileInfo = fileList.at(i);
        if(fileInfo.isSymLink()) {
            sTmp = QFileInfo(fileInfo.symLinkTarget()).baseName().toStdString();
            it = std::find(tmpPortList.begin(), tmpPortList.end(), sTmp);
            if (it != tmpPortList.end()) {
                portEntryInfo.clear();
                printf("Symlink to %s : %s added \n", it->c_str(), fileInfo.fileName().toStdString().c_str());
                portEntryInfo.push_back(fileInfo.fileName().toStdString());
                portEntryInfo.push_back(std::string("Symlink to ").append(*it));
                portList.push_back(portEntryInfo);
            }
        }
    }
#endif

}

void serialPort::setReadBufferSize(const int &nBufferSize)
{
    m_ReadBufferSize = qint64(nBufferSize);
    m_SerialDeviceConnection.setReadBufferSize(m_ReadBufferSize);
}

//
// Platform independent sleep
//
void serialPort::msSleep(int sleepMs)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
}
