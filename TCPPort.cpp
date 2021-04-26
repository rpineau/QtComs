#include "TCPPort.h"

//
// Set params with constructor if needed.
//
TCPDevice::TCPDevice(const char *hostnameName, const unsigned int nPort, const int bufferSize):deviceAccess(hostnameName,nPort, bufferSize)
{
    m_bPortOpenned = false;

    m_HostName = hostnameName;
    m_nNetworkPort = nPort;
    m_ReadBufferSize = qint64(bufferSize);
    m_TCPDeviceConnection.setReadBufferSize(m_ReadBufferSize);
}

TCPDevice::~TCPDevice(void)
{
    if(m_bPortOpenned)
        close();
}

//
// Open with paremters set in constructor
//
bool TCPDevice::open()
{
    if(m_HostName.isEmpty()) {
        m_bPortOpenned = false;
    }
    else {
        if(!m_bPortOpenned) {
            m_TCPDeviceConnection.connectToHost(m_HostName, m_nNetworkPort, QIODevice::ReadWrite, QAbstractSocket::AnyIPProtocol);
            m_bPortOpenned = m_TCPDeviceConnection.isOpen();
        }
    }

    return m_bPortOpenned;
}

//
// Open with all new parameters.
// Will apply default params if not specified.
// At least the port name needs to be set.
//
bool TCPDevice::open(const QString hostnameName, const quint16 nPort)
{
    m_bPortOpenned = false;
    
    if(hostnameName.isEmpty() && m_HostName.isEmpty())
        return false;

    m_HostName = hostnameName;
    m_nNetworkPort = nPort;
    m_TCPDeviceConnection.setReadBufferSize(m_ReadBufferSize);
    m_TCPDeviceConnection.connectToHost(m_HostName, m_nNetworkPort, QIODevice::ReadWrite, QAbstractSocket::AnyIPProtocol);

    m_TCPDeviceConnection.waitForConnected(1000);    
    m_bPortOpenned = (m_TCPDeviceConnection.state()  == QTcpSocket::ConnectedState);
    
    return m_bPortOpenned;
}

bool TCPDevice::open(const char *hostnameName, const unsigned int nPort)
{
    return open(QString(hostnameName), nPort);
}


//
// clear all data buffer and close the port
//
void TCPDevice::close()
{
    if(m_bPortOpenned) {
        m_TCPDeviceConnection.close();
        m_bPortOpenned = false;
    }
}

bool TCPDevice::isOpened()
{
    m_bPortOpenned = (m_TCPDeviceConnection.state()  == QTcpSocket::ConnectedState);
    return m_bPortOpenned;
}

//
// Wait for data to be written, with a timeout
//
bool TCPDevice::flushDataOut(int msecTimeout)
{
    if(!m_bPortOpenned) {
        return false;
    }
    
    return m_TCPDeviceConnection.waitForBytesWritten(msecTimeout);
}

//
// discard all buffer content
//
bool TCPDevice::clearRxTxBuffers()
{
    if(!m_bPortOpenned) {
        return false;
    }
    m_TCPDeviceConnection.readAll();
    return m_TCPDeviceConnection.flush();
}

//
// return the number of bytes available for read
//
int TCPDevice::bytesAvailable()
{
    if(!isOpened()) {
        return 0;
    }

    if(!m_TCPDeviceConnection.waitForReadyRead(1000))
        return 0; // we had a timeout

    return int(m_TCPDeviceConnection.bytesAvailable());
}

//
// Wait for a certain number of bytes to be read for read, with timeout
//
bool TCPDevice::waitForDataReady(const int &nbBytes, const int &msTimeout)
{
    qint64 nbByteAvail = 0;
    int nTimeout = 0;

    if(nbBytes > m_ReadBufferSize)
        return false;

    do {
        nbByteAvail = m_TCPDeviceConnection.bytesAvailable();
        if(nbByteAvail < nbBytes) {
            if(nbByteAvail == 0)
                nTimeout += 1000; //  bytesAvailable timeout after 1000ms so add this to the current total timeout counter
            else {
                mySleep(10);
                nTimeout += 10; //  we waited 10ms so add this to the current total timeout counter
            }
        }
        if(nTimeout >= msTimeout)
            return false; // we reached the timeout without enought bytes in the receive buffer
    } while(nbByteAvail<nbBytes);

    return true;
}

//
// read data from port into destBuffer, with a timeout
// return the number of byte actually read
//
int TCPDevice::readData(char *destBuffer, const int &amountOfByteToRead, const int &msTimeout)
{
    qint64 nbRead;
    qint64 nbByteAvailable;
    bool bByteReady;

    memset(destBuffer, 0, amountOfByteToRead);
    bByteReady = waitForDataReady(amountOfByteToRead, msTimeout);
    if(!bByteReady) {
        // read what we can
        nbByteAvailable = m_TCPDeviceConnection.bytesAvailable();
        nbRead = m_TCPDeviceConnection.read(destBuffer, nbByteAvailable);
    }
    else {
        nbRead = m_TCPDeviceConnection.read(destBuffer, qint64(amountOfByteToRead));
    }

    return int(nbRead);
}

//
// Write data from srcBuffer to the port, return the number of byte actually writen in nbByteWritten
// and return true/false if all data was written or not.
//
bool TCPDevice::writeData(const char *srcBuffer, const int &amountOfByteToWrite, int &nbByteWritten)
{
    nbByteWritten = int(m_TCPDeviceConnection.write(srcBuffer, quint64(amountOfByteToWrite)));
    
    return (nbByteWritten == amountOfByteToWrite);
}


void TCPDevice::setReadBufferSize(const int &nBufferSize)
{
    m_ReadBufferSize = qint64(nBufferSize);
    m_TCPDeviceConnection.setReadBufferSize(m_ReadBufferSize);
}

//
// Platform independent sleep
//
void TCPDevice::mySleep(int sleepMs)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
}
