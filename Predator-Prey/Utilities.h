#pragma once
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/unordered_map.hpp>
#include "Enums.h"

namespace Utils
{
	inline std::string CurrentDateTime()
	{
		boost::posix_time::ptime currentTime = boost::posix_time::microsec_clock::local_time();
		std::string currentTimeStr = to_simple_string(currentTime);
		return currentTimeStr;
	}


	inline boost::unordered_map<Enums::AgentType, const char*> AgentTypeToString = boost::assign::map_list_of
		(Enums::AgentType::Grass, "Grass")
		(Enums::AgentType::Prey, "Prey")
		(Enums::AgentType::Predator, "Predator");


	inline int BoundaryCondition(int index, int size) {
		const int newIndex = index % size; return newIndex >= 0 ? newIndex : newIndex + size;
	}
}
