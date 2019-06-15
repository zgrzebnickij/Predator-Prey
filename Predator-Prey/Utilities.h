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
		(Enums::AgentType::Field, "Field")
		(Enums::AgentType::Prey, "Prey")
		(Enums::AgentType::Predator, "Predator");


	inline int BoundaryCondition(int index, int size) {
		const int newIndex = index % size; return newIndex >= 0 ? newIndex : newIndex + size;
	}

	static int max = 0;
	static std::stack<int> freeIDs;

	inline int getFreeID() {
		if (freeIDs.empty()) {
			max++;
			return max;
		}

		const int ID = freeIDs.top();
		freeIDs.pop();

		return ID;
	}

	inline void addIDToStack(const int ID) {
		freeIDs.push(ID);
	}

	struct envData //deklaracja struktury
	{ //opis struktury
		double preyMatingProb;
		double predatorMatingProb;
		int preyMaxHealth;
		int preyHelthToMate;
		int predatorMaxHealth;
		bool customModel;
	};
}

