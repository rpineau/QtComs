cmake_minimum_required(VERSION 3.16)

project(helloworld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core
                        SerialPort
                         Network)

qt_standard_project_setup()

qt_add_executable(QtComs
    main.cpp 
	serialPort.cpp 
	TCPPort.cpp 
	UDPPort.cpp 
	deviceAccess.cpp 
	pegasus.cpp 
	RTI-Dome.cpp
)

target_link_libraries(QtComs PRIVATE Qt6::Core 
                    Qt6::SerialPort
                    Qt6::Network)

set_target_properties(QtComs PROPERTIES
    MACOSX_BUNDLE OFF
)
