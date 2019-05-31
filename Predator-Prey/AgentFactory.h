#include "Interfaces/IAgent.h"
#include "Enums.h"
#include <map>
#include <memory>

class AgentFactory
{
public:
	using creatorFunctionType = std::unique_ptr<Agent>(*)();

	AgentFactory();
	std::unique_ptr<Agent> createAgent(Enums::AgentType agentType);

private:
	void registerAgentType(Enums::AgentType agentType, creatorFunctionType creatorFunction);

	std::map<Enums::AgentType, creatorFunctionType> factoryRegister;
};
