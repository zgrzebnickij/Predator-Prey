#include "Agent.h"


class Prey : public Agent
{
public:
	Prey(const int ID);
	void updateHealth() override;	
};
