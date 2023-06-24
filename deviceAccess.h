#ifndef __DEVICE_PORT__
#define __DEVICE_PORT__
#include <chrono>
#include <thread>
#include <string>
#include <vector>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QDir>

#define QDEVICE_BUFFER_SIZE 16384

#define SERIAL_OK           0
#define NO_PORT_SELECTED    1
#define NOT_CONNECTED       2
#define NORESPONSE          3
#define WRITE_ERROR         4

enum DeviceConnectionType {TYPE_SERIAL = 0, TYPE_TCP, TYPE_UDP, TYPE_UNKNOWN};

class deviceAccess
{

public:
    deviceAccess(const char *portName ="",
               const QSerialPort::BaudRate Speed = QSerialPort::Baud9600,
               const QSerialPort::DataBits dataBits = QSerialPort::Data8,
               const QSerialPort::StopBits stopBits = QSerialPort::OneStop ,
               const QSerialPort::FlowControl FlowControl= QSerialPort::NoFlowControl,
               const QSerialPort::Parity Parity = QSerialPort::NoParity,
               const bool bDTR = false,
               const int bufferSize = QDEVICE_BUFFER_SIZE);

    deviceAccess(const char *hostnameName ="", const unsigned int nPort = 0,const int bufferSize = QDEVICE_BUFFER_SIZE);


    virtual ~deviceAccess();

    // open with param set in constructor
    virtual bool    open();
    // open with new params.
    virtual bool    open(const QString portName,
                         const QSerialPort::BaudRate Speed = QSerialPort::Baud9600,
                         const QSerialPort::DataBits dataBits = QSerialPort::Data8,
                         const QSerialPort::StopBits stopBits = QSerialPort::OneStop ,
                         const QSerialPort::FlowControl FlowControl= QSerialPort::NoFlowControl,
                         const QSerialPort::Parity Parity = QSerialPort::NoParity,
                         const bool bDTR = false);

    virtual bool    open(const char *portName,
                         const QSerialPort::BaudRate Speed = QSerialPort::Baud9600,
                         const QSerialPort::DataBits dataBits = QSerialPort::Data8,
                         const QSerialPort::StopBits stopBits = QSerialPort::OneStop ,
                         const QSerialPort::FlowControl FlowControl= QSerialPort::NoFlowControl,
                         const QSerialPort::Parity Parity = QSerialPort::NoParity,
                         const bool bDTR = false);

    virtual bool    open(const char *hostnameName ="", const unsigned int nPort = 0);
    virtual bool    open(const QString hostnameName ="", const quint16 nPort = 0);

    virtual void    close();    // clear all data buffer and close the port
    virtual bool    isOpened(); // return the state of the port (open = true)

    // data transfer
    virtual bool    flushDataOut(int msecTimeout = 30000);  // Wait for data to be written, with a timeout
    virtual bool    clearRxTxBuffers(void);                 // discard all buffer content
    virtual int     bytesAvailable();                       // return the number of byte available for read
    virtual bool    waitForDataReady(const int &nbBytes, const int &msTimeout = 30000);                     // Wait for a certain number of bytes to be read for read, with timeout
    virtual int     readData(char *destBuffer, const int &amountOfByteToRead, const int &msTimeout);        // read data from  port into destBuffer, with a timeout
    virtual bool    writeData(const char *srcBuffer, const int &amountOfByteToWrite, int &nbByteWritten);   // Write data from srcBuffer to the port, return the number of byte actually writen in nbByteWritten
    // flow ccontrol pins
    virtual bool    getCTS();            // Get the state of the CTS pin
    virtual void    setRTS(bool bState); // Set the state of the RTS pin

    // port list
    static void     getPortList(std::vector<std::string> &portList);

    // buffer management
    virtual void    setReadBufferSize(const int &nBufferSize);

private:

    void            msSleep(int sleepMs); // Platform independent sleep

};
#endif // __DEVICE_PORT__

