//
// Created by jg.wang on 2020/2/13.
//

#pragma once

#include <functional>
#include <string>
namespace omf {
	class OmfMsgSiteUser {
	public:///send
		/**
		 * send msg.
		 * @param name [in]:msg name
		 * @param data [in]: msg body
		 * @param size [in]: msg size
		 * @return true/false
		 */
		static bool Send(const char* name, const void *data, int size);
		static bool Send(const char* name, const std::string&);
		static bool Send(const char* name, const char*);
		static bool Send(const std::string&);
		static bool Send(const char*);
	public:
		/**
		 * send msg.
		 * @param name [in]:msg name
		 * @param data [in]: msg body
		 * @param size [in]: msg size
		 * @param onDropFunc [in]:callback function is called when dropping this message.
		 * @param timeoutInMs [int]:the longest time to wait for receiving this message.
		 * @return true/false
		 */
		using OnDropFunc = void(*)(const void* data, int size);
		static bool Send(const char* name, const void *data, int size,OnDropFunc onDropFunc,int timeoutInMs);
		static bool Send(const char* name, const std::string&,OnDropFunc onDropFunc,int timeoutInMs);
		static bool Send(const char* name, const char*,OnDropFunc onDropFunc,int timeoutInMs);
		static bool Send(const std::string&,OnDropFunc onDropFunc,int timeoutInMs);
		static bool Send(const char*,OnDropFunc onDropFunc,int timeoutInMs);
	public:
		/**
		 * the receive callback
		 * @param data [in]:msg body
		 * @param size [in]:msg size
		 */
		using FuncProc = std::function<bool(const void *data, int size)>;
	public:///receive
		/**
		 * receive message
		 * @param name [in]: msg name.
		 * @param cb [in]: receive callback
		 * @return true/false
		 */
		static bool Receive(const char* name, const FuncProc &cb);
	public:///register
		/**
		 * register the receive callback to msg site.
		 * @param name [in]: msg name.
		 * @param cb [in]: receive callback
		 * @return true/false
		 */
		static bool Register(const char* name, const FuncProc &cb);
		/**
		 * unregister the msg receiver.
		 * @param name [in]: msg name.
		 * @return true/fasle
		 */
		static bool UnRegister(const char* name);
	public:
		/**
		 * check the msg receiver is register.
		 * @param name [in]: msg name.
		 * @return true/fasle
		 */
		static bool ReveiverCheck(const char* name);
	};
}

