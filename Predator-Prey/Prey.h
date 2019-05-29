#include "Interfaces/IAgent.h"


class Prey : public Agent
{
public:
	Prey(int ID);
	void updateHealth() override;	
};
