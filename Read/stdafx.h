// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

// TODO:  在此处引用程序需要的其他头文件
#include <windows.h>
#include <ras.h>
#include <string>
#include <iostream>
#include <fstream>
#pragma comment(lib,"Rasapi32.lib")

using boost::asio::ip::tcp;
using namespace std;
