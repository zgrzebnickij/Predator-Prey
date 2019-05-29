#include "AgentFactory.h"
#include "Predator.h"
#include "Prey.h"

AgentFactory::AgentFactory()
{
	//TODO: Make constructors with ID from Utils -> waiting for Kuba
	registerAgentType(Enums::AgentType::Predator, []() -> std::unique_ptr<Agent> { return std::make_unique<Predator>(); });
	registerAgentType(Enums::AgentType::Prey, []() -> std::unique_ptr<Agent> { return std::make_unique<Prey>(); });
}

std::unique_ptr<Agent> AgentFactory::createAgent(Enums::AgentType agentType)
{
	if (auto it = factoryRegister.find(agentType); it != factoryRegister.end()) {
		return it->second();
	}

	return nullptr;
}

void AgentFactory::registerAgentType(Enums::AgentType agentType, creatorFunctionType creatorFunction)
{
	factoryRegister.insert(std::pair<Enums::AgentType, creatorFunctionType>(agentType, creatorFunction));
}
