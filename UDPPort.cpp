#include "UDPPort.h"

//
// Set params with constructor if needed.
//
UDPDevice::UDPDevice(const char *hostnameName, const unsigned int nPort,const int bufferSize):deviceAccess(hostnameName,nPort, bufferSize)
{
    m_bPortOpenned = false;

    m_HostName = hostnameName;
    m_nNetworkPort = nPort;
    m_ReadBufferSize = qint64(bufferSize);
    m_UDPDeviceConnection.setReadBufferSize(m_ReadBufferSize);
}

UDPDevice::~UDPDevice(void)
{
    if(m_bPortOpenned)
        close();
}

//
// Open with paremters set in constructor
//
bool UDPDevice::open()
{
    if(m_HostName.isEmpty()) {
        m_bPortOpenned = false;
    }
    else {
        if(!m_bPortOpenned) {
            m_UDPDeviceConnection.connectToHost(m_HostName, m_nNetworkPort, QIODevice::ReadWrite, QAbstractSocket::AnyIPProtocol);
            m_bPortOpenned = m_UDPDeviceConnection.isOpen();
        }
    }

    return m_bPortOpenned;
}

//
// Open with all new parameters.
// Will apply default params if not specified.
// At least the port name needs to be set.
//
bool UDPDevice::open(const QString hostnameName, const quint16 nPort)
{
    m_bPortOpenned = false;

    if(hostnameName.isEmpty() && m_HostName.isEmpty())
        return false;

    m_HostName = hostnameName;
    m_nNetworkPort = nPort;
    m_UDPDeviceConnection.setReadBufferSize(m_ReadBufferSize);

    m_UDPDeviceConnection.connectToHost(m_HostName, m_nNetworkPort, QIODevice::ReadWrite, QAbstractSocket::AnyIPProtocol);
    m_UDPDeviceConnection.waitForConnected(1000);
    m_bPortOpenned = (m_UDPDeviceConnection.state() == QAbstractSocket::ConnectedState);

    return m_bPortOpenned;
}

bool UDPDevice::open(const char *hostnameName, const unsigned int nPort)
{
    return open(QString(hostnameName), nPort);
}


//
// clear all data buffer and close the port
//
void UDPDevice::close()
{
    if(m_bPortOpenned) {
        m_UDPDeviceConnection.close();
        m_bPortOpenned = false;
    }
}

bool UDPDevice::isOpened()
{
    m_bPortOpenned = (m_UDPDeviceConnection.state()  == QTcpSocket::ConnectedState);
    return m_bPortOpenned;
}

//
// Wait for data to be written, with a timeout
//
bool UDPDevice::flushDataOut(int msecTimeout)
{
    if(!m_bPortOpenned) {
        return false;
    }
    return m_UDPDeviceConnection.waitForBytesWritten(msecTimeout);
}

//
// discard all buffer content
//
bool UDPDevice::clearRxTxBuffers()
{
    if(!m_bPortOpenned) {
        return false;
    }
    m_UDPDeviceConnection.readAll();
    return m_UDPDeviceConnection.flush();
}

//
// return the number of bytes available for read
//
int UDPDevice::bytesAvailable()
{

    if(!isOpened()) {
        return 0;
    }

    if(!m_UDPDeviceConnection.waitForReadyRead(1000))
        return 0; // we had a timeout

    return int(m_UDPDeviceConnection.bytesAvailable());
}


//
// Wait for a certain number of bytes to be read for read, with timeout
//
bool UDPDevice::waitForDataReady(const int &nbBytes, const int &msTimeout)
{
    qint64 nbByteAvail = 0;
    int nTimeout = 0;

    if(nbBytes > m_ReadBufferSize)
        return false;

    do {
        nbByteAvail = m_UDPDeviceConnection.bytesAvailable();
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
int UDPDevice::readData(char *destBuffer, const int &amountOfByteToRead, const int &msTimeout)
{
    qint64 nbRead;
    qint64 nbByteAvailable;
    bool bByteReady;

    memset(destBuffer, 0, amountOfByteToRead);
    bByteReady = waitForDataReady(amountOfByteToRead, msTimeout);
    if(!bByteReady) {
        // read what we can
        nbByteAvailable = m_UDPDeviceConnection.bytesAvailable();
        nbRead = m_UDPDeviceConnection.read(destBuffer, nbByteAvailable);
    }
    else {
        nbRead = m_UDPDeviceConnection.read(destBuffer, qint64(amountOfByteToRead));
    }

    return int(nbRead);
}

//
// Write data from srcBuffer to the serial port, return the number of byte actually writen in nbByteWritten
// and return true/false if all data was written or not.
//
bool UDPDevice::writeData(const char *srcBuffer, const int &amountOfByteToWrite, int &nbByteWritten)
{
    nbByteWritten = int(m_UDPDeviceConnection.write(srcBuffer, quint64(amountOfByteToWrite)));
    return (nbByteWritten == amountOfByteToWrite);
}


void UDPDevice::setReadBufferSize(const int &nBufferSize)
{
    m_ReadBufferSize = qint64(nBufferSize);
    m_UDPDeviceConnection.setReadBufferSize(m_ReadBufferSize);
}

//
// Platform independent sleep
//
void UDPDevice::msSleep(int sleepMs)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
}
