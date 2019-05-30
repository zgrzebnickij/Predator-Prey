#pragma once
#include "Enums.h"

class Agent
{
public:
	Agent(const int ID_, const Enums::AgentType agentType_): ID(ID_), agentType(agentType_) {}
	virtual ~Agent() = default;

	int getID() const { return ID; }
	Enums::AgentType getAgentType() const { return agentType; }

protected:
	int					ID;
	Enums::AgentType	agentType;
};

