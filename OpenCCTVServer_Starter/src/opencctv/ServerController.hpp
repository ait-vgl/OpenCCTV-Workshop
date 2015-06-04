/*
 * ServerController.hpp
 *
 *  Created on: Jun 1, 2015
 *      Author: anjana
 */

#ifndef SERVERCONTROLLER_HPP_
#define SERVERCONTROLLER_HPP_

#include <iostream>
#include <string>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <boost/lexical_cast.hpp>
#include "mq/MqUtil.hpp"
#include "util/log/Loggers.hpp"
#include "util/Config.hpp"
#include "Exception.hpp"
#include "util/xml/OpenCCTVServerMessage.hpp"
#include "util/TypeDefinitions.hpp"

namespace opencctv {

using namespace std;

class ServerController {
private:

	static ServerController *_pServerController;
	static zmq::socket_t* _tcpMq;
	static string _sServerStatus;
	static string _sServerPath;
	static pid_t _iServerProcessId;
	static unsigned int _iOpenCCTVServerPort;

	ServerController();
	ServerController(ServerController const&);
	void operator=(ServerController const&);
	void stopServer();

public:
	virtual ~ServerController();

	static ServerController* getInstance();
	void execServerStart(const string& sRequestMsg);
	void execServerStop(const string& sRequestMsg);
	void execServerRestart(const string& sRequestMsg);
	void sendErrorReply(const string& sMessageContent);
	void sendStatusReply();
	string readMessage();
};

} /* namespace opencctv */

#endif /* SERVERCONTROLLER_HPP_ */
