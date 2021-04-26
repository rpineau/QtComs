#include "deviceAccess.h"

//
// Set params with constructor if needed.
//
deviceAccess::deviceAccess(const char *portName,
                       const QSerialPort::BaudRate Speed,
                       const QSerialPort::DataBits dataBits,
                       const QSerialPort::StopBits stopBits,
                       const QSerialPort::FlowControl FlowControl,
                       const QSerialPort::Parity Parity,
                       const bool bDTR,
                       const int bufferSize)
{

}

deviceAccess::deviceAccess(const char *hostnameName, const unsigned int nPort, const int bufferSize)
{
}


deviceAccess::~deviceAccess(void)
{
}

//
// Open with paremters set in constructor
//
bool deviceAccess::open()
{
    return false;
}

//
// Open with all new parameters.
// Will apply default params if not specified.
// At least the port name needs to be set.
//
bool deviceAccess::open(const QString portName, const QSerialPort::BaudRate Speed, const QSerialPort::DataBits dataBits, const QSerialPort::StopBits stopBits, const QSerialPort::FlowControl FlowControl, const QSerialPort::Parity Parity, const bool bDTR )
{
    return false;
}

bool deviceAccess::open(const char *portName, const QSerialPort::BaudRate Speed, const QSerialPort::DataBits dataBits, const QSerialPort::StopBits stopBits, const QSerialPort::FlowControl FlowControl, const QSerialPort::Parity Parity, const bool bDTR )
{
    return open(QString(portName), Speed, dataBits, stopBits, FlowControl, Parity, bDTR);
}

bool deviceAccess::open(const QString hostnameName, const quint16 nPort)
{
    return false;
}

bool deviceAccess::open(const char *hostnameName, const unsigned int nPort)
{
    return false;
}


//
// clear all data buffer and close the port
//
void deviceAccess::close()
{
}

bool deviceAccess::isOpened()
{
    return false;
}

//
// Wait for data to be written, with a timeout
//
bool deviceAccess::flushDataOut(int msecTimeout)
{
    return true;
}

//
// discard all buffer content
//
bool deviceAccess::clearRxTxBuffers()
{
    return true;
}
//
// return the number of bytes available for read
//
int deviceAccess::bytesAvailable()
{
    return 0;
}

//
// Wait for a certain number of bytes to be read for read, with timeout
//
bool deviceAccess::waitForDataReady(const int &nbBytes, const int &msTimeout)
{

    return true;
}

//
// read data from serial port into destBuffer, with a timeout
// return the number of byte actually read
//
int deviceAccess::readData(char *destBuffer, const int &amountOfByteToRead, const int &msTimeout)
{
    return 0;
}

//
// Write data from srcBuffer to the  port, return the number of byte actually writen in nbByteWritten
// and return true/false if all data was written or not.
//
bool deviceAccess::writeData(const char *srcBuffer, const int &amountOfByteToWrite, int &nbByteWritten)
{
    return false;
}

//
// Get the state of the CTS pin
//
bool deviceAccess::getCTS()
{
    return false;
}

//
// Set the state of the RTS pin
//

void deviceAccess::setRTS(bool bState)
{
}

//
// return the list of available port on the machine as a std::vector<std::string>
//
void deviceAccess::getPortList(std::vector<std::string> &portList)
{

    portList.clear();
}

void deviceAccess::setReadBufferSize(const int &nBufferSize)
{
}

//
// Platform independent sleep
//
void deviceAccess::mySleep(int sleepMs)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
}
