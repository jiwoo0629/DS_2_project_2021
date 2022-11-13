#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include "VaccinationData.h"
#include "AVLNode.h"

class AVLTree {
private:
	AVLNode* root;

public:
	AVLTree() { root = NULL; }
	AVLNode* Getroot() { return root; }
	bool Insert(VaccinationData* pVac);
	VaccinationData* Search(string name);
	void GetVector(vector<VaccinationData*>& v);
	string toLower(string s) //compare the word regardless of capital and lower case
	{
		transform(s.begin(), s.end(), s.begin(), ::tolower);
		return s;
	}
	void Deallocate();
	~AVLTree() { Deallocate(); }
};

#endif
