#include "AVLTree.h"
bool AVLTree::Insert(VaccinationData* pVac) //Insert node into AVLTree
{
	if (root == NULL)
	{
		root = new AVLNode();
		root->setVacData(pVac);
		root->setBF(0);
		root->setLeft(NULL);
		root->setRight(NULL);
		return true;
	}
	//Locate insertion point
	AVLNode* a = root; //most recent node with bf = +-1
	AVLNode* pa = NULL; //parent of a
	AVLNode* p = root; //p moves through the tree
	AVLNode* pp = NULL; //parent of p
	AVLNode* rootSub = NULL;

	while (p)
	{
		if (p->getBF() != 0) { a = p; pa = pp; }
		if(toLower(pVac->GetUserName()) < toLower(p->getVacData()->GetUserName())) { pp = p; p = p->getLeft(); }
		else if (toLower(pVac->GetUserName()) > toLower(p->getVacData()->GetUserName())) { pp = p; p = p->getRight(); }
		else { p->getVacData()->SetUserName(pVac->GetUserName()); return true; } //update UserName
	}

	//Insert and Rebalance
	AVLNode* y = new AVLNode();
	y->setVacData(pVac);
	y->setLeft(NULL);
	y->setRight(NULL);
	if (toLower(pVac->GetUserName()) < toLower(pp->getVacData()->GetUserName())) { pp->setLeft(y); } //insert as left child
	else { pp->setRight(y); } //insert as right child

	//Adjust Balance factor
	int d;
	AVLNode* b; //child of a
	AVLNode* c; //child of b

	if (toLower(pVac->GetUserName()) > toLower(a->getVacData()->GetUserName())) { b = p = a->getRight(); d = -1; }
	else { b = p = a->getLeft(); d = 1; }

	while (p != y)
	{
		if (toLower(pVac->GetUserName()) > toLower(p->getVacData()->GetUserName())) { p->setBF(-1); p = p->getRight(); } //height of right increase
		else { p->setBF(1); p = p->getLeft(); } //height of left increase
	}
	if (a->getBF() == 0 || (a->getBF() + d == 0)) { a->setBF(a->getBF() + d); return true; } //check if tree is balanced

	if (d == 1) //left imbalance
	{
		if (b->getBF() == 1) //LL rotation
		{
			a->setLeft(b->getRight());
			b->setRight(a);
			rootSub = b;
		}
		else //LR rotation
		{
			c = b->getRight();
			b->setRight(c->getLeft());
			a->setLeft(c->getRight());
			c->setLeft(b);
			c->setRight(a);
			switch (c->getBF())
			{
			case 0:
				b->setBF(0); a->setBF(0);
				break;
			case 1:
				a->setBF(-1); b->setBF(0);
				break;
			case -1:
				b->setBF(1); a->setBF(0);
				break;
			}
			c->setBF(0); rootSub = c;
		}
	}
	else //right imbalance
	{
		if (b->getBF() == -1) //RR rotation
		{
			a->setRight(b->getLeft());
			b->setLeft(a);
			a->setBF(0); b->setBF(0);
			rootSub = b;
		}
		else //RL rotation
		{
			c = b->getLeft();
			b->setLeft(c->getRight());
			a->setRight(c->getLeft());
			c->setRight(b); c->setLeft(a);
			switch (c->getBF())
			{
			case 0:
				b->setBF(0); a->setBF(0);
				break;
			case 1:
				b->setBF(-1); a->setBF(0);
				break;
			case -1:
				a->setBF(1); b->setBF(0);
				break;
			}
			c->setBF(0); rootSub = c;
		}
	}
	if (pa == NULL) { root = rootSub; }
	else if (a == pa->getLeft()) { pa->setLeft(rootSub); }
	else { pa->setRight(rootSub); }
	return true;
}

void AVLTree::GetVector(vector<VaccinationData*>& v) //Insert every data into the Print_vector from AVLTree
{
	//Insert by level order using queue
	queue <AVLNode*> q; 
	AVLNode* cur = root;
	while (cur)
	{
		v.push_back(cur->getVacData());
		if (cur->getLeft()) 
		{
			q.push(cur->getLeft());
		}
		if (cur->getRight())
		{
			q.push(cur->getRight());
		}
		if (q.empty()) return;
		cur = q.front();
		q.pop();
	}
	return;
}

VaccinationData* AVLTree::Search(string name) //Search the data with 'name' from AVLTree
{
	if (root == NULL)
		return NULL;
	AVLNode* cur = root;
	while (cur)
	{
		if (toLower(name) < toLower(cur->getVacData()->GetUserName())) { cur = cur->getLeft(); }
		else if (toLower(name) > toLower(cur->getVacData()->GetUserName())) { cur = cur->getRight(); }
		else { return cur->getVacData(); }
	}
	return NULL;
}

void AVLTree::Deallocate() //Deallocate node in AVLTree
{
	if (root == NULL)
		return;
	queue <AVLNode*> q;
	AVLNode* cur = root;
	while (cur)
	{
		AVLNode* deletenode = cur;
		if (cur->getLeft())
		{
			q.push(cur->getLeft());
		}
		if (cur->getRight())
		{
			q.push(cur->getRight());
		}
		delete deletenode;
		if (q.empty()) return;
		cur = q.front();
		q.pop();
	}
	return;	
}
