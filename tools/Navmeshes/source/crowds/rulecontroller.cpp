#include "rulecontroller.h"

void rulecontroller::Init()
{
}


void rulecontroller::setEntity(CEntidad3D *pent) 
{ 
ent=pent; 
}

void rulecontroller::Recalc()
{
bool found=false;
int ruleid=0;

while (!found && ruleid<conditions.size())
	{
	bool res=(this->*conditions[ruleid])();
	if (res)
		{
		// rule triggered
		if (!all_rules) found=true;
		(this->*actions[ruleid])();
		}
	ruleid++;
	}
}


void rulecontroller::AddRule(conditionhandler ch,actionhandler ah)
{
conditions.push_back(ch);
actions.push_back(ah);
}
