#ifndef _RULECONTROLLER_H
#define _RULECONTROLLER_H

// ai controllers using maps to function pointers for easy access and scalability. 

// we put this here so you can assert from all controllers without including everywhere
#include <assert.h>	
#include <string>
#include <vector>
#include "entidad.h"

using namespace std;

// rules are two arrays to member function pointers, to 
// be defined on a derived class. 
class rulecontroller;

typedef void (rulecontroller::*actionhandler)();
typedef bool (rulecontroller::*conditionhandler)();

class rulecontroller
	{

	vector<actionhandler>actions;
	vector<conditionhandler>conditions;

	public:
		bool all_rules;
		CEntidad3D *ent;
		void setEntity(CEntidad3D *);
		virtual void Init();	// resets the controller
		void Recalc();	// recompute behaviour
		void AddRule(conditionhandler,actionhandler);
	};

#endif