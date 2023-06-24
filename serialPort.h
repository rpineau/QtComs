#ifndef __SERIAL_PORT__
#define __SERIAL_PORT__
#include <chrono>
#include <thread>
#include <string>
#include <vector>

#include "deviceAccess.h"

#define QSERIAL_BUFFER_SIZE 16384

#define SERIAL_OK           0
#define NO_PORT_SELECTED    1
#define NOT_CONNECTED       2
#define NORESPONSE          3
#define WRITE_ERROR         4

class serialPort: public deviceAccess
{
    friend class deviceAccess;

public:
    serialPort(const char *portName ="",
               const QSerialPort::BaudRate Speed = QSerialPort::Baud9600,
               const QSerialPort::DataBits dataBits = QSerialPort::Data8,
               const QSerialPort::StopBits stopBits = QSerialPort::OneStop ,
               const QSerialPort::FlowControl FlowControl= QSerialPort::NoFlowControl,
               const QSerialPort::Parity Parity = QSerialPort::NoParity,
               const bool bDTR = false,
               const int bufferSize = QSERIAL_BUFFER_SIZE);

    virtual ~serialPort();

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

    virtual void    close();    // clear all data buffer and close the port
    virtual bool    isOpened(); // return the state of the port (open = true)

    // data transfer
    virtual bool    flushDataOut(int msecTimeout = 30000);  // Wait for data to be written, with a timeout
    virtual bool    clearRxTxBuffers(void);                 // discard all buffer content
    virtual int     bytesAvailable();                       // return the number of byte available for read
    virtual bool    waitForDataReady(const int &nbBytes, const int &msTimeout = 30000);                     // Wait for a certain number of bytes to be read for read, with timeout
    virtual int     readData(char *destBuffer, const int &amountOfByteToRead, const int &msTimeout);        // read data from serial port into destBuffer, with a timeout
    virtual bool    writeData(const char *srcBuffer, const int &amountOfByteToWrite, int &nbByteWritten);   // Write data from srcBuffer to the serial port, return the number of byte actually writen in nbByteWritten
    // flow ccontrol pins
    virtual bool    getCTS();            // Get the state of the CTS pin
    virtual void    setRTS(bool bState); // Set the state of the RTS pin

    // port list
    static void    getPortList(std::vector<std::string> &portList);

    // buffer management
    virtual void    setReadBufferSize(const int &nBufferSize);

private:

    void            msSleep(int sleepMs); // Platform independent sleep

    QSerialPort                 m_SerialDeviceConnection;
    QString                     m_PortName;
    QSerialPort::BaudRate       m_Speed;
    QSerialPort::DataBits       m_DataBits;
    QSerialPort::StopBits       m_StopBits;
    QSerialPort::FlowControl    m_FlowControl;
    QSerialPort::Parity         m_Parity;
    bool                        m_DTR;
    qint64                      m_ReadBufferSize;

    bool                        m_bPortOpenned;


};
#endif // __SERIAL_PORT__

