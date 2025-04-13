#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

#include <Windows.h>
#include <iostream>
#include <string>
#include <stdint.h>
#include <stdexcept>
#include <exception>
#include <typeinfo>
#include <winerror.h>
#include <system_error>
#include <type_traits>
#include <memory>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")
