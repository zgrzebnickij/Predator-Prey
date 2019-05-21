#pragma once
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace Util
{
	inline std::string CurrentDateTime()
	{
		boost::posix_time::ptime currentTime = boost::posix_time::microsec_clock::local_time();
		std::string currentTimeStr = to_simple_string(currentTime);
		return currentTimeStr;
	}
}
