cmake_minimum_required(VERSION 3.22)
project(hardware_section)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(lib/ESPAsyncWebServer-master/examples)
include_directories(lib/ESPAsyncWebServer-master/examples/CaptivePortal)
include_directories(lib/ESPAsyncWebServer-master/examples/ESP_AsyncFSBrowser)
include_directories(lib/ESPAsyncWebServer-master/examples/regex_patterns)
include_directories(lib/ESPAsyncWebServer-master/examples/simple_server)
include_directories(lib/ESPAsyncWebServer-master/src)
include_directories(smart_socket)

add_executable(hardware_section
        include/Functions.h
        include/Libraries.h
        include/Prototypes.h
        include/Variables.h
        lib/ESPAsyncWebServer-master/examples/CaptivePortal/CaptivePortal.ino
        lib/ESPAsyncWebServer-master/examples/ESP_AsyncFSBrowser/ESP_AsyncFSBrowser.ino
        lib/ESPAsyncWebServer-master/examples/regex_patterns/regex_patterns.ino
        lib/ESPAsyncWebServer-master/examples/simple_server/simple_server.ino
        lib/ESPAsyncWebServer-master/src/AsyncEventSource.cpp
        lib/ESPAsyncWebServer-master/src/AsyncEventSource.h
        lib/ESPAsyncWebServer-master/src/AsyncJson.h
        lib/ESPAsyncWebServer-master/src/AsyncWebSocket.cpp
        lib/ESPAsyncWebServer-master/src/AsyncWebSocket.h
        lib/ESPAsyncWebServer-master/src/AsyncWebSynchronization.h
        lib/ESPAsyncWebServer-master/src/ESPAsyncWebServer.h
        lib/ESPAsyncWebServer-master/src/SPIFFSEditor.cpp
        lib/ESPAsyncWebServer-master/src/SPIFFSEditor.h
        lib/ESPAsyncWebServer-master/src/StringArray.h
        lib/ESPAsyncWebServer-master/src/WebAuthentication.cpp
        lib/ESPAsyncWebServer-master/src/WebAuthentication.h
        lib/ESPAsyncWebServer-master/src/WebHandlerImpl.h
        lib/ESPAsyncWebServer-master/src/WebHandlers.cpp
        lib/ESPAsyncWebServer-master/src/WebRequest.cpp
        lib/ESPAsyncWebServer-master/src/WebResponseImpl.h
        lib/ESPAsyncWebServer-master/src/WebResponses.cpp
        lib/ESPAsyncWebServer-master/src/WebServer.cpp
        smart_socket/smart_socket.ino
        src/main.cpp)
