#include "stdafx.h"
#include "Module.h"




bool k = false;
void connect_handler(const boost::system::error_code& error)
{
	if (error)
	{
		printf("error\n");

	}
	else{
		k = true;
		printf("connect\n");
	}
}


void handle_wait(const boost::system::error_code& error,
	boost::asio::deadline_timer& t, int& count)
{
	if (!error)
	{

		if (count++ < 2)
		{
			if (k == true){
				return;
			}
			t.expires_from_now(boost::posix_time::seconds(1));
			t.async_wait(boost::bind(handle_wait, boost::asio::placeholders::error,
				boost::ref(t), boost::ref(count)));
		}
		else{
			boost::system::error_code err(boost::asio::error::host_not_found_try_again);
			connect_handler(err);
			t.get_io_service().stop();
			t.cancel();
		}
		std::cout << "正在重新尝试连接到internet" << count << endl;
	}

}
bool RasConnection(string username, string password){
	bool breturn=false;
	HRASCONN m_hRasconn;
	RASDIALPARAMS RasDialParams;
	// 总是设置dwSize 为RASDIALPARAMS结构的大小
	RasDialParams.dwSize = sizeof(RASDIALPARAMS);
	m_hRasconn = NULL;
	// 设置szEntryName为空字符串将允许RasDial使用缺省拨号属性
	_tcscpy_s(RasDialParams.szEntryName, _T("宽带连接"));
	RasDialParams.szPhoneNumber[0] = _T('\0');
	RasDialParams.szCallbackNumber[0] = _T('\0');
	_tcscpy_s(RasDialParams.szUserName, _T(username.c_str()));
	_tcscpy_s(RasDialParams.szPassword, _T(password.c_str()));
	RasDialParams.szDomain[0] = _T('\0');
	DWORD Ret = RasDial(NULL, NULL, &RasDialParams, 0, NULL, &m_hRasconn);

	if (Ret == 0){
		breturn = true;
		printf("连接成功!\n");
		WSADATA wsadata;
		WSAStartup(MAKEWORD(2, 2), &wsadata);
		char sIP[512];
		HOSTENT *pHostent;
		if ((pHostent = gethostbyname("www.baidu.com")) != NULL){
			strcpy(sIP, inet_ntoa(*((struct in_addr *)pHostent->h_addr)));
		}
		WSACleanup();
		Sleep(1000);
		k = false;
		boost::asio::io_service io_service;
		tcp::socket socket(io_service);
		boost::asio::deadline_timer t(io_service);
		socket.async_connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(sIP), 80), connect_handler);
		size_t a = t.expires_from_now(boost::posix_time::seconds(1));
		int count = 0;
		t.async_wait(boost::bind(handle_wait, boost::asio::placeholders::error,
			boost::ref(t), boost::ref(count)));
		io_service.run();
		if (k != true){
			printf("该账户已欠费\n");
			breturn = false;
		}
	}
	



	char szError[512];
	RasGetErrorString(Ret, szError, 512);
	printf("%s\n", szError);

	


	if (m_hRasconn != NULL)
	{
		RasHangUp(m_hRasconn);
		m_hRasconn = NULL;
		RASCONNSTATUS rStatus;
		while (RasGetConnectStatus(m_hRasconn, &rStatus) != ERROR_INVALID_HANDLE)
		{
			::Sleep(0);
		}
	}
	printf("%s\n", "链接已断开!\n");
	return breturn;
}


string EnCode(string _string)
{
	char cas_str[] = "9012345678abcdeABCDEFGHIJKLMNfghijklmnUVWXYZxyzuvwopqrstOPQRST";
	int cas_str_buffer[16];
	int cas_esi = 37;
	char src_str[128] = "", dec_str[128] = "";
	int k = 0;
	int cas_eax, cas_edx;
	div_t x;
	unsigned int  i, j;
	cas_str_buffer[15] = 25;
	cas_str_buffer[14] = 35;
	cas_str_buffer[13] = 182;
	cas_str_buffer[12] = 236;
	cas_str_buffer[11] = 43;
	cas_str_buffer[10] = 41;
	cas_str_buffer[9] = 53;
	cas_str_buffer[8] = 18;
	cas_str_buffer[7] = 226;
	cas_str_buffer[6] = 215;
	cas_str_buffer[5] = 24;
	cas_str_buffer[4] = 117;
	cas_str_buffer[3] = 35;
	cas_str_buffer[2] = 201;
	cas_str_buffer[1] = 52;
	cas_str_buffer[0] = 17;
	strcpy_s(src_str, _string.c_str());
	for (i = 0; i < strlen(src_str); i++)
	{
		for (j = 0; j < strlen(cas_str); j++)
		{

			if (src_str[i] == cas_str[j]){
				if (i < 16)
					cas_eax = cas_str_buffer[i];
				else {
					x = div(i, 16);
					cas_eax = cas_str_buffer[x.rem];
				}
				cas_edx = cas_esi + cas_esi * 2;
				cas_eax = cas_eax^cas_edx;
				cas_eax = cas_eax^k;
				cas_eax = cas_eax + j;
				x = div(cas_eax, 62);
				dec_str[i] = cas_str[x.rem];
				cas_edx = x.rem;
				cas_esi = cas_esi ^ (cas_edx + 9433);
				break;
			}
		}
		if (dec_str[i] == 0)
			dec_str[i] = src_str[i];
		k = k + 5;
	}

	_string = "2:";
	_string += dec_str;
	return _string;
}



