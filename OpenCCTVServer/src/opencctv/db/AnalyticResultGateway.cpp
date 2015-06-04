/*
 * AnalyticResultGateway.cpp
 *
 *  Created on: Jun 4, 2015
 *      Author: anjana
 */

#include "AnalyticResultGateway.hpp"

namespace opencctv {
namespace db {

const std::string AnalyticResultGateway::_INSERT_ANALYTIC_RESULT_SQL = "INSERT INTO results (analytic_id, timestamp, result_text) VALUES (?, ?, ?)";

AnalyticResultGateway::AnalyticResultGateway() {
	_pDbConnPtr = DbConnector::getConnection_ResultsDB();

}

AnalyticResultGateway::~AnalyticResultGateway() {
	delete _pDbConnPtr;
	_pDbConnPtr = NULL;
}

int AnalyticResultGateway::insertResults(unsigned int iAnalyticInstanceId,analytic::AnalyticResult analyticResult)
{
	int result = 0;

	try{
		sql::PreparedStatement* statementPtr = (*_pDbConnPtr).prepareStatement(_INSERT_ANALYTIC_RESULT_SQL);
		(*statementPtr).setInt(1, iAnalyticInstanceId);
		(*statementPtr).setString(2, analyticResult.getTimestamp());
		(*statementPtr).setString(3, analyticResult.getCustomText());
		result = (*statementPtr).executeUpdate();

		(*statementPtr).close();
		delete statementPtr;

	}catch(sql::SQLException &e)
	{
		result = -1;
		std::string sErrorMsg = "Error while Inserting analytics results to the database.";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}

	return result;

}



} /* namespace db */
} /* namespace opencctv */
