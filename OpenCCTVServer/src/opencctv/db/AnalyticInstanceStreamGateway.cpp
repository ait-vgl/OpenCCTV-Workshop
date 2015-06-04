/*
 * AnalyticInstanceStreamGateway.cpp
 *
 *  Created on: Jun 3, 2015
 *      Author: anjana
 */

#include "AnalyticInstanceStreamGateway.hpp"

namespace opencctv {
namespace db {

const std::string AnalyticInstanceStreamGateway::_SELECT_ANALYTIC_INSTANCE_STREAM_SQL = "SELECT ainsts.id, ainsts.analytic_instance_id, ainps.name, a.filename FROM analytic_instance_streams AS ainsts, analytic_input_streams AS ainps, analytics AS a WHERE (ainsts.stream_id = ?) AND (ainsts.analytic_input_stream_id = ainps.id ) AND (ainps.analytic_id = a.id)";

AnalyticInstanceStreamGateway::AnalyticInstanceStreamGateway() {
	_pDbConnPtr = DbConnector::getConnection();

}

AnalyticInstanceStreamGateway::~AnalyticInstanceStreamGateway() {
	delete _pDbConnPtr;
	_pDbConnPtr = NULL;
}

void AnalyticInstanceStreamGateway::findAllForStream(unsigned int iStreamId, std::vector<opencctv::dto::AnalyticInstanceStream>& vToStoreAIS)
{
	try
	{
		sql::PreparedStatement* statementPtr = (*_pDbConnPtr).prepareStatement(_SELECT_ANALYTIC_INSTANCE_STREAM_SQL);
		(*statementPtr).setInt(1, iStreamId);
		sql::ResultSet* pResultsPtr = (*statementPtr).executeQuery();

		opencctv::dto::AnalyticInstanceStream ais;
		while((*pResultsPtr).next())
		{
			ais.setId((*pResultsPtr).getInt("id"));
			ais.setAnalyticInstanceId((*pResultsPtr).getInt("analytic_instance_id"));
			ais.setInputName((*pResultsPtr).getString("name"));

			//opencctv::util::Config* pConfig = opencctv::util::Config::getInstance();
			//std::string sAnalyticsBasePath = pConfig->get(opencctv::util::PROPERTY_ANALYTIC_PLUGIN_DIR);
			//std::string sAnalyticsDir = (*pResultsPtr).getString("filename");
			//std::string sAnalyticsDirLocation;
			//sAnalyticsDirLocation.append(sAnalyticsBasePath);
			//sAnalyticsDirLocation.append("/");
			//sAnalyticsDirLocation.append(sAnalyticsDir);
			//ais.setAnalyticDirLocation(sAnalyticsDirLocation);

			ais.setAnalyticFilename((*pResultsPtr).getString("filename"));
			ais.setAnalyticDirLocation((*pResultsPtr).getString("filename"));

			vToStoreAIS.push_back(ais);
			//ainsts.id, ainsts.analytic_instance_id, ainsts.analytic_input_stream_id, ainps.name, a.filename
		}
		(*pResultsPtr).close();
		(*statementPtr).close();
		delete pResultsPtr;
		delete statementPtr;
	}
	catch(sql::SQLException &e)
	{
		//std::cerr << "AnalyticGateway:findAnalytics: Error while finding analytics from the database. " << e.what() << std::endl;
		std::string sErrorMsg = "AnalyticInstanceStreamGateway::findAllForStream: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		//TODO :: Add to log
	}
}

} /* namespace db */
} /* namespace opencctv */


