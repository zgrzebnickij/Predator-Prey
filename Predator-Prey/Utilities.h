#pragma once
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/unordered_map.hpp>
#include "Enums.h"
#include <stack>

namespace Utils
{
	inline std::string CurrentDateTime()
	{
		const boost::posix_time::ptime currentTime = boost::posix_time::microsec_clock::local_time();
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

	static int max = 0;
	static std::stack<int> freeIds;

	inline int getfreeId() {
		
		if (freeIds.empty()) {
			max++;
			return max;
		}
		int id = freeIds.top();
		freeIds.pop();
		return id;
	}
	inline void addIdToStack(int id) {
		freeIds.push(id);
	}
}

