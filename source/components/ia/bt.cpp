#include "mcv_platform.h"
#include "bt.h"


void bt::create(string s)
{
	name=s;	
}


btnode *bt::createNode(string s)
{
	if (findNode(s)!=NULL) 
		{
		printf("Error: node %s already exists\n",s.c_str());
		return NULL;	// error: node already exists
		}
	btnode *btn=new btnode(s);
	tree[s]=btn;
	return btn;
}


btnode *bt::findNode(string s)
{
	if (tree.find(s)==tree.end()) return NULL;
	else return tree[s];
}


btnode *bt::createRoot(string s,int type,btcondition btc,btaction bta)
{
	btnode *r=createNode(s);
	r->setParent(NULL);
	root=r;
	r->setType(type);
	if (btc!=NULL) addCondition(s,btc);
	if (bta!=NULL) addAction(s,bta);

	current=NULL;
	return r;
}


btnode *bt::addChild(string parent,string son,int type,btcondition btc,btaction bta)
{
	btnode *p=findNode(parent);
	btnode *s=createNode(son);
	p->addChild(s);
	s->setParent(p);
	s->setType(type);
	if (btc!=NULL) addCondition(son,btc);
	if (bta!=NULL) addAction(son,bta);
	return s;
}


void bt::recalc(float dt)
{
	DT = dt;
	if (current==NULL) root->recalc(this);	// I'm not in a sequence, start from the root
	else current->recalc(this);				// I'm in a sequence. Continue where I left
}

void bt::setCurrent(btnode *nc)
{
current=nc;
}


void bt::addAction(string s,btaction act)
{
if (actions.find(s)!=actions.end())
	{
	printf("Error: node %s already has an action\n",s.c_str());
	return;	// if action already exists don't insert again...
	}
actions[s]=act;
}


int bt::execAction(string s)
{
if (actions.find(s)==actions.end()) 
	{
	printf("ERROR: Missing node action for node %s\n",s.c_str());
	return LEAVE; // error: action does not exist
	}
return (this->*actions[s])();
}


void bt::addCondition(string s,btcondition cond)
{
if (conditions.find(s)!=conditions.end())
	{
	printf("Error: node %s already has a condition\n",s.c_str());
	return;	// if condition already exists don't insert again...
	}
conditions[s]=cond;
}


bool bt::testCondition(string s)
{
if (conditions.find(s)==conditions.end())
	{
	return true;	// error: no condition defined, we assume TRUE
	}
return (this->*conditions[s])();;
}


TCompTransform* bt::getMyTransform() {
	TCompTransform* c = h_transform;
	if (!c) {
		CEntity* e = h_entity;
		h_transform = e->get< TCompTransform >();
		c = h_transform;
	}
	return c;
}

TCompRender* bt::getMyRender() {
	TCompRender* r = h_render;
	if (!r) {
		CEntity* e = h_entity;
		h_render = e->get< TCompRender >();
		r = h_render;
	}
	return r;
}

TCompCollider* bt::getMyCollider() {
	TCompCollider* c = h_collider;
	if (!c) {
		CEntity* e = h_entity;
		h_collider = e->get< TCompCollider >();
		c = h_collider;
	}
	return c;
}

void bt::setEntity(CHandle new_entity) {
	h_entity = new_entity;
}

void bt::debugInMenu() {
}