#ifndef _AGENTINSTANCE_INC
#define _AGENTINSTANCE_INC

#include "rulecontroller.h"

class agentInstance: public CEntidad3D
	{
	rulecontroller *ctrl;

	public:
		agentInstance(){}
		agentInstance(rulecontroller *pc) { ctrl=pc; }
		void Create(rulecontroller *pc) { ctrl=pc; }
		void Recalc() { assert(ctrl!=NULL); ctrl->Recalc(); }
		void Init() { assert(ctrl!=NULL); ctrl->Init(); }
		void setEntity( CEntidad3D *pent) { assert(ctrl!=NULL); ctrl->setEntity(pent); }
		rulecontroller *getRuleController() { return ctrl; }
	};


#endif