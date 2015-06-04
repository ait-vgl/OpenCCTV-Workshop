/*
 * ServerController.cpp
 *
 *  Created on: Jun 1, 2015
 *      Author: anjana
 */

#include "ServerController.hpp"

namespace opencctv {

ServerController* ServerController::_pServerController = NULL;
string ServerController::_sServerStatus = opencctv::util::SVR_STATUS_STOPPED;
pid_t ServerController::_iServerProcessId = 0;
unsigned int ServerController::_iOpenCCTVServerPort = 0;
zmq::socket_t* ServerController::_tcpMq = NULL;
string ServerController::_sServerPath = "";

ServerController::ServerController()
{
}

ServerController* ServerController::getInstance()
{
   if (!_pServerController) // Only allow one instance of class to be generated.
   {
	   _pServerController = new ServerController();
	   //Try to read and set the configuration details
	   opencctv::util::Config* pConfig = opencctv::util::Config::getInstance();

		string sStartingPort = pConfig->get(opencctv::util::PROPERTY_OPENCCTV_SERVER_PORT);
		if(sStartingPort.empty())
		{
			throw opencctv::Exception("Failed to retrieve OpenCCTV Server Starter port from the configuration file.");
		}
		_iOpenCCTVServerPort = boost::lexical_cast<unsigned int>(sStartingPort);

		_sServerPath = pConfig->get(opencctv::util::PROPERTY_OPENCCTV_SERVER_DIR);
		if(_sServerPath.empty())
	   	{
	   		throw opencctv::Exception("Failed to retrieve OpenCCTV Server installation directory from the configuration file.");
	   	}

	    //Initialize the REQ-REP instance of ZeroMQ
	   	try
	   	{
	   		_tcpMq = opencctv::mq::MqUtil::createNewMq(sStartingPort, ZMQ_REP);
	   	}
	   	catch(std::runtime_error &e)
	   	{
	   		throw opencctv::Exception("Failed to create OpenCCTV Server's request-reply MQ.");
	   	}
   }

	return _pServerController;
}

string ServerController::readMessage()
{
	string message = "";
	try
	{
		opencctv::mq::MqUtil::readFromSocket(_tcpMq, message);
		cout << "Received : " << message << endl << endl;
	}
	catch(std::runtime_error &e)
	{
		std::string sErrMsg = "Failed to read request from the request-reply MQ. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}

	return message;
}

void ServerController::execServerStart(const string& sRequestMsg)
{
	//=========Testing========
	/*
	string sReplyMessage;
	string sContent = "OpenCCTV Server Started";
	_iServerProcessId = 1111;
	_sServerStatus = opencctv::util::SVR_STATUS_RUNNING;

	try
	{
		opencctv::util::xml::OpenCCTVServerMessage::createStartMessageReply(sContent, _sServerStatus, _iServerProcessId, sReplyMessage);
		cout << sReplyMessage << endl << endl;
		opencctv::mq::MqUtil::writeToSocket(_tcpMq, sReplyMessage);
	}
	catch(opencctv::Exception &e)
	{
		throw e;
	}
	catch(std::runtime_error &e)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
	}
	*/
	//========================

	pid_t pid;

	if(_sServerStatus.compare(opencctv::util::SVR_STATUS_STOPPED) == 0 || _sServerStatus.compare(opencctv::util::SVR_STATUS_UNKNOWN)== 0 )
	{
		pid = fork();

		if(pid == 0)
		{
			//cout << "ServerController::execServerStart: Starting OpenCCTV Process .............." << endl;

			//TODO:: Check with the parameters of the current implementation
			char* exec_args[2];

			string sPathStr = _sServerPath;
			exec_args[0] = &sPathStr[0];
			exec_args[1] = NULL;

			execv(exec_args[0], exec_args);

			//If exec returns, process must have failed.
			throw opencctv::Exception("Starting the OpenCCTVServer Process Failed");
		}
		else if(pid > 0)
		{
			_iServerProcessId = pid;
			_sServerStatus = opencctv::util::SVR_STATUS_RUNNING;

			string sReplyMessage;
			string sContent = "Starting the OpenCCTV Server....";

			try
			{
				opencctv::util::xml::OpenCCTVServerMessage::createStartMessageReply(sContent, _sServerStatus, _iServerProcessId, sReplyMessage);
				opencctv::mq::MqUtil::writeToSocket(_tcpMq, sReplyMessage);
			}
			catch(opencctv::Exception &e)
			{
				throw e;
			}
			catch(std::runtime_error &e)
			{
				opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
			}
		}
		else
		{
			throw opencctv::Exception("Starting the OpenCCTVServer Process Failed");
		}
	}
	else
	{
		throw opencctv::Exception("The OpenCCTV Server is already running");
	}
}

void ServerController::execServerStop(const string& sRequestMsg)
{
	//=========Testing========
	/*
	string sReplyMessage;
	string sContent = "OpenCCTV Server Stopped";
	_iServerProcessId = 0;
	_sServerStatus = opencctv::util::SVR_STATUS_STOPPED;
	try
	{
		opencctv::util::xml::OpenCCTVServerMessage::createStopMessageReply(sContent, _sServerStatus, _iServerProcessId, sReplyMessage);
		cout << sReplyMessage << endl << endl;
		opencctv::mq::MqUtil::writeToSocket(_tcpMq, sReplyMessage);
	}
	catch(opencctv::Exception &e)
	{
		throw e;
	}
	catch(std::runtime_error &e)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
	}
	*/
	//========================

	stopServer();

	string sReplyMessage;
	string sContent = "OpenCCTV Server Stopped";
	try
	{
		opencctv::util::xml::OpenCCTVServerMessage::createStopMessageReply(sContent, _sServerStatus, _iServerProcessId, sReplyMessage);
		opencctv::mq::MqUtil::writeToSocket(_tcpMq, sReplyMessage);
	}
	catch(opencctv::Exception &e)
	{
		throw e;
	}
	catch(std::runtime_error &e)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());;
	}
}

void ServerController::execServerRestart(const string& sRequestMsg)
{
	//=========Testing========
	/*
	 string sReplyMessage;
	string sContent = "OpenCCTV Server Started";
	_iServerProcessId = 1111;
	_sServerStatus = opencctv::util::SVR_STATUS_RUNNING;

	try
	{
		opencctv::util::xml::OpenCCTVServerMessage::createStartMessageReply(sContent, _sServerStatus, _iServerProcessId, sReplyMessage);
		cout << sReplyMessage << endl << endl;
		opencctv::mq::MqUtil::writeToSocket(_tcpMq, sReplyMessage);
	}
	catch(opencctv::Exception &e)
	{
		throw e;
	}
	catch(std::runtime_error &e)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
	}
	*/
	//========================

	stopServer();
	execServerStart(sRequestMsg);
}

void ServerController::sendStatusReply()
{
	string sReplyMessage;

	string sContent = "Replying for Status Request";

	try
	{
		opencctv::util::xml::OpenCCTVServerMessage::createStatusReply(sContent, _sServerStatus, _iServerProcessId, sReplyMessage);
		opencctv::mq::MqUtil::writeToSocket(_tcpMq, sReplyMessage);
	}
	catch(opencctv::Exception &e)
	{
		throw e;
	}
	catch(std::runtime_error &e)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
	}
}

void ServerController::sendErrorReply(const string& sMessageContent)
{
	string sReplyMessage;

	try
	{
		opencctv::util::xml::OpenCCTVServerMessage::createInvalidMessageReply(sMessageContent, _sServerStatus, _iServerProcessId, sReplyMessage);
	}
	catch(opencctv::Exception &e)
	{
		sReplyMessage = "<?xml version=\"1.0\" encoding=\"utf-8\"?> <opencctvmsg><type>Error</type><content>";
		sReplyMessage.append(e.what());
		sReplyMessage.append("</content><serverstatus>Unknown</serverstatus><serverpid>0</serverpid></opencctvmsg>");
	}

	try
	{
		opencctv::mq::MqUtil::writeToSocket(_tcpMq, sReplyMessage);
	}
	catch(std::runtime_error &e)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
	}
}

void ServerController::stopServer()
{
	if(_iServerProcessId > 0){
		kill(_iServerProcessId, SIGTERM);

		bool died = false;
		for (int loop = 1; !died && loop < 10 ; ++loop)
		{
			int status;
			//pid_t id;
			sleep(1);
			if (waitpid(_iServerProcessId, &status, WNOHANG) == _iServerProcessId) died = true;
		}

		if (!died) kill(_iServerProcessId, SIGKILL);
	}

	_iServerProcessId = 0;
	_sServerStatus = opencctv::util::SVR_STATUS_STOPPED;
}

ServerController::~ServerController()
{
	delete _tcpMq; _tcpMq = NULL;
}

} /* namespace opencctv */
