
#include "ResultRouterThread.hpp"

namespace opencctv {

ResultRouterThread::ResultRouterThread(unsigned int iAnalyticInstanceId) {
	ApplicationModel* pModel = ApplicationModel::getInstance();
	_pFlowController = NULL;
	if(pModel->containsFlowController(iAnalyticInstanceId))
	{
		_pFlowController = pModel->getFlowControllers()[iAnalyticInstanceId];
	}
	_pSerializer = util::serialization::Serializers::getInstanceOfDefaultSerializer();
	_iAnalyticInstanceId = iAnalyticInstanceId;
}

void ResultRouterThread::operator()()
{
	util::Config* pConfig = util::Config::getInstance();
	ApplicationModel* pModel = ApplicationModel::getInstance();
	if(pModel->containsResultsOutputQueueAddress(_iAnalyticInstanceId))
	{
		bool bConnected = false;
		mq::TcpMqReceiver receiver;
		try
		{
			receiver.connectToMq(pConfig->get(util::PROPERTY_ANALYTIC_SERVER_IP), pModel->getResultsOutputQueueAddresses()[_iAnalyticInstanceId]);
			bConnected = true;
		}
		catch(Exception &e)
		{
			std::string sErrMsg = "Failed to connect to Analytic Output Queue. ";
			sErrMsg.append(e.what());
			util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		}
		while(bConnected && _pFlowController)
		{
			std::string* pSSerializedResult = receiver.receive();
			analytic::AnalyticResult result = _pSerializer->deserializeAnalyticResult(*pSSerializedResult);
			std::string sMsg = "\t\tReceived Result of ";
			sMsg.append(result.getTimestamp());
			util::log::Loggers::getDefaultLogger()->debug(sMsg);

			//Saving to DB
			if(result.getWriteToDatabase())
			{
				try
				{
					opencctv::db::AnalyticResultGateway analyticResultGateway;
					analyticResultGateway.insertResults(_iAnalyticInstanceId, result);
					sMsg = "\t\t\tResult written to the database";
					util::log::Loggers::getDefaultLogger()->debug(sMsg);

				}catch(opencctv::Exception &e)
				{
					std::ostringstream sErrorMessage;
					sErrorMessage << "Failed to write results to the results database : analytic id - ";
					sErrorMessage << _iAnalyticInstanceId;
					sMsg = sErrorMessage.str();
					util::log::Loggers::getDefaultLogger()->error(sMsg);
				}
			}


			_pFlowController->received();
			if(pSSerializedResult) delete pSSerializedResult;
		}
	}
	opencctv::util::log::Loggers::getDefaultLogger()->info("Results Router Thread stopped.");
}

} /* namespace opencctv */
