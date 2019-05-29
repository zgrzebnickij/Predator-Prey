#include "Interfaces/Agent.h"


class Prey : public Agent
{
public:
	Prey();
	Prey(int ID);
	void updateHealth() override;	
};
