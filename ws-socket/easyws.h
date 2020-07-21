#pragma once

#include <string>
#include <vector>


//接收数据回调
typedef void(__stdcall*cbRecv)(std::vector<uint8_t> rbuf);
extern cbRecv g_pcbRecv;


namespace easywsclient 
{


	class WebSocket 
	{
	  public:
		typedef WebSocket * pointer;
		typedef enum readyStateValues 
		{
			CLOSING,
			CLOSED, 
			CONNECTING,
			OPEN 
		} readyStateValues;

	
		static pointer from_url(const std::string& url, const std::string& origin = std::string());
		static pointer from_url_no_mask(const std::string& url, const std::string& origin = std::string());

		// Interfaces:
		virtual ~WebSocket() { }
		virtual void poll(int timeout = 0) = 0; // timeout in milliseconds
		virtual void send(const std::string& message) = 0;
		virtual void sendBinary(const std::string& message) = 0;
		virtual void sendBinary(const std::vector<uint8_t>& message) = 0;
		virtual void sendPing() = 0;
		virtual void close() = 0;
		virtual readyStateValues getReadyState() const = 0;

		virtual void closesk() = 0; 
		void SetCallbk(PVOID pFun0)
		{
			if (!g_pcbRecv)
				g_pcbRecv = (cbRecv)pFun0;

		}

		
	};



}