#ifndef __UDP_PORT__
#define __UDP_PORT__
#include <chrono>
#include <thread>
#include <string>
#include <vector>

#include "deviceAccess.h"

#define QTCP_BUFFER_SIZE 16384

#define SERIAL_OK           0
#define NO_PORT_SELECTED    1
#define NOT_CONNECTED       2
#define NORESPONSE          3
#define WRITE_ERROR         4

class UDPDevice: public deviceAccess
{
    friend class deviceAccess;

public:
    UDPDevice(const char *hostnameName ="", const unsigned int nPort = 0,const int bufferSize = QTCP_BUFFER_SIZE);

    virtual ~UDPDevice();

    // open with param set in constructor
    virtual bool    open();
    // open with new params.
    virtual bool    open(const char *hostnameName ="", const unsigned int nPort = 0);
    virtual bool    open(const QString hostnameName ="", const quint16 nPort = 0);

    virtual void    close();    // clear all data buffer and close the port
    virtual bool    isOpened(); // return the state of the port (open = true)

    // data transfer
    virtual bool    flushDataOut(int msecTimeout = 30000);  // Wait for data to be written, with a timeout
    virtual bool    clearRxTxBuffers(void);                 // discard all buffer content
    virtual int     bytesAvailable();                       // return the number of byte available for read
    virtual bool    waitForDataReady(const int &nbBytes, const int &msTimeout = 30000);                     // Wait for a certain number of bytes to be read for read, with timeout
    virtual int     readData(char *destBuffer, const int &amountOfByteToRead, const int &msTimeout);        // read data from serial port into destBuffer, with a timeout
    virtual bool    writeData(const char *srcBuffer, const int &amountOfByteToWrite, int &nbByteWritten);   // Write data from srcBuffer to the serial port, return the number of byte actually writen in nbByteWritten

    // buffer management
    virtual void    setReadBufferSize(const int &nBufferSize);

private:

    void            msSleep(int sleepMs); // Platform independent sleep

    qint64          m_ReadBufferSize;
    bool            m_bPortOpenned;

    QUdpSocket      m_UDPDeviceConnection;
    QString         m_HostName;
    quint16         m_nNetworkPort;

};
#endif // __UDP_PORT__

