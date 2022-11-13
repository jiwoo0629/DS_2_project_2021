#include "BpTree.h"

bool BpTree::Insert(VaccinationData* newData) //Insert node into BPTree
{
	if (root == NULL) //if BPTree is empty
	{
		BpTreeNode* newDnode = new BpTreeDataNode();
		newDnode->insertDataMap(newData->GetUserName(), newData);
		root = newDnode; //new node is root
		return true;
	}
	BpTreeNode* newnode = this->searchDataNode(newData->GetUserName()); //Check if there is a node that has the same name with 'newData'
	if (newnode != NULL) //if the data already exists
	{
		map <string, VaccinationData*>* m = newnode->getDataMap();
		for (auto iter = m->begin(); iter != m->end(); iter++)
		{
			if (iter->second->GetUserName() == newData->GetUserName())
				iter->second->SetTimesInc(); //Increase GetTimes
		}
		return true;
	}
	else
	{
		BpTreeNode* newDnode = new BpTreeDataNode();
		newDnode->insertDataMap(newData->GetUserName(), newData);
		if (root->getMostLeftChild() == NULL) //if inserting BpTreeNode is root
		{
			root->insertDataMap(newDnode->getDataMap()->begin()->first, newDnode->getDataMap()->begin()->second); //insert data into the map of root
			if (this->exceedDataNode(root) == true) //Check if order of root is 3
			{
				this->splitDataNode(root);
			}
		}
		else
		{
			BpTreeNode* cur = root;
			while (cur->getMostLeftChild())
			{
				cur = cur->getMostLeftChild(); //Move to BpTreeDataNode
			}
			while (cur->getNext()) //find the BpTreeDataNode to insert the data
			{
				if (newData->GetUserName() < cur->getNext()->getDataMap()->begin()->first)
					break;
				cur = cur->getNext();
			}
			cur->insertDataMap(newDnode->getDataMap()->begin()->first, newDnode->getDataMap()->begin()->second);
			if (this->exceedDataNode(cur) == true) //Check if order of the node is 3
				this->splitDataNode(cur);
			if(cur->getParent())
				cur = cur->getParent();
			while (cur->getParent())
			{
				if (this->exceedIndexNode(cur) == true) //Check if cur causes other BpTreeIndexNodes to split
					this->splitIndexNode(cur);
				if(cur->getParent())
					cur = cur->getParent();
			}
		}
		while (root->getParent()) //update the root
		{
			root = root->getParent();
		}
		return true;
	}
}

BpTreeNode* BpTree::searchDataNode(string n) ////Search the BpTreeNode which mapdata has the name 'n'
{
	if (!root)
		return NULL;
	BpTreeNode* cur = root;
	while (cur->getMostLeftChild())
	{
		cur = cur->getMostLeftChild();
	}
	while (cur)
	{
		map <string, VaccinationData*> *ma = cur->getDataMap();
		if (ma->find(n) != ma->end())
			return cur;
		cur = cur->getNext();
	}
	return NULL;
}
void BpTree::splitDataNode(BpTreeNode* pDataNode) //Split BpTreeDataNode
{
	map <string, VaccinationData*>* ma = pDataNode->getDataMap();
	BpTreeNode* newDatanode = new BpTreeDataNode(); //Create new BpTreeDataNode and insert the mapdata of begin
	newDatanode->insertDataMap(ma->begin()->second->GetUserName(), ma->begin()->second);
	pDataNode->deleteMap(ma->begin()->first); //erase the mapdata of begin from pDataNode
	if (!pDataNode->getPrev()) //if pDataNode doesn't have prev node, consider as single linked list
	{
		newDatanode->setNext(pDataNode);
		pDataNode->setPrev(newDatanode);
	}
	else //consider as double linked list
	{
		newDatanode->setNext(pDataNode);
		pDataNode->getPrev()->setNext(newDatanode);
		newDatanode->setPrev(pDataNode->getPrev());
		pDataNode->setPrev(newDatanode);
	}
	if (!pDataNode->getParent()) //if pDataNode doesn't have parent (pDataNode is root)
	{
		BpTreeNode* newIndexnode = new BpTreeIndexNode(); //Create new BpTreeIndexNode
		newIndexnode->insertIndexMap(ma->begin()->second->GetUserName(), pDataNode);
		//Connect new BpTreeIndexNode and BpTreeDataNodes as a parent and child
		newDatanode->setParent(newIndexnode);
		pDataNode->setParent(newIndexnode);
		newIndexnode->setMostLeftChild(newDatanode);
	}
	else //if pDataNode already have parent
	{
		//Connect new BpTreeDataNode with its parent
		pDataNode->getParent()->insertIndexMap(ma->begin()->second->GetUserName(), pDataNode);
		newDatanode->setParent(pDataNode->getParent());
	}
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode)
{
	map <string, BpTreeNode*>* ma = pIndexNode->getIndexMap();
	BpTreeNode* newIndexnode1 = new BpTreeIndexNode(); //most left index node
	newIndexnode1->insertIndexMap(ma->begin()->first, ma->begin()->second);
	newIndexnode1->setMostLeftChild(pIndexNode->getMostLeftChild());
	pIndexNode->deleteMap(ma->begin()->first);
	if (!pIndexNode->getParent()) //if pIndexNode doesn't have a parent node
	{
		BpTreeNode* newIndexnode2 = new BpTreeIndexNode(); //middle index node that will become parent node
		newIndexnode2->insertIndexMap(ma->begin()->first, pIndexNode);
		pIndexNode->setMostLeftChild(ma->begin()->second); //pIndexNode : most right index node
		ma->begin()->second->deleteMap(ma->begin()->first);
		//Connect new BpTreeIndexNodes and pIndexNode as a parent and child
		newIndexnode2->setMostLeftChild(newIndexnode1);
		newIndexnode1->setParent(newIndexnode2);
		pIndexNode->setParent(newIndexnode2);
	}
	else
	{
		//Connect new BpTreeIndexNode as a parent and child
		pIndexNode->getParent()->insertIndexMap(ma->begin()->first, pIndexNode);
		pIndexNode->setMostLeftChild(ma->begin()->second);
		ma->begin()->second->deleteMap(ma->begin()->first);
		pIndexNode->setMostLeftChild(newIndexnode1);
		newIndexnode1->setParent(pIndexNode->getParent());
	}
}

bool BpTree::exceedDataNode(BpTreeNode* pDataNode) //check if the size of map is 3
{
	map <string, VaccinationData*>* ma = pDataNode->getDataMap();
	int count = 0;
	for (auto iter = ma->begin(); iter != ma->end(); iter++)
	{
		count++;
	}
	if (count == 3)
		return true;
	else
		return false;
}

bool BpTree::exceedIndexNode(BpTreeNode* pIndexNode) //check if the size of map is 3
{
	map <string, BpTreeNode*>* ma = pIndexNode->getIndexMap();
	int count = 0;
	for (auto iter = ma->begin(); iter != ma->end(); iter++)
	{
		count++;
	}
	if (count == 3)
		return true;
	else
		return false;
}

void BpTree::SearchRange(string start, string end) //Search the node by a range of name's first letter
{
	ofstream fout;
	fout.open("log.txt", ios::app);
	BpTreeNode* cur = root;
	while (cur->getMostLeftChild())
	{
		cur = cur->getMostLeftChild();
	}
	int count = 0; //Count value that becomes 1 if the node whose name is in the range exists
	while (cur)
	{
		map <string, VaccinationData*>* ma = cur->getDataMap();
		for (auto iter = ma->begin(); iter != ma->end(); iter++)
		{
			char start_c, end_c; //char that contains name's first letter
			start_c = start.c_str()[0];
			end_c = end.c_str()[0];
			if ((iter->second->GetUserName()[0] >= start_c) && (iter->second->GetUserName()[0] <= end_c))
			{
				count = 1;
			}
		}
		cur = cur->getNext();
	}
	if (count == 1) //if the node whose name is in the range exists
	{
		BpTreeNode* cur2 = root;
		while (cur2->getMostLeftChild())
		{
			cur2 = cur2->getMostLeftChild();
		}
		fout << "======SEARCH_BP======" << endl;
		while (cur2)
		{
			map <string, VaccinationData*>* ma = cur2->getDataMap();
			for (auto iter = ma->begin(); iter != ma->end(); iter++)
			{
				char start_c, end_c;
				start_c = start.c_str()[0];
				end_c = end.c_str()[0];
				if ((iter->second->GetUserName()[0] >= start_c) && (iter->second->GetUserName()[0] <= end_c))
				{
					fout << iter->second->GetUserName() << " " << iter->second->GetVaccineName() << " " << iter->second->GetTimes() << " " << iter->second->GetAge() << " " << iter->second->GetLocationName() << endl;
				}
			}
			cur2 = cur2->getNext();
		}
		fout << "=====================" << endl << endl;
		fout.close();
	}
	else //if there is no node that exists in the range, print Error code.
	{
		fout << "=======ERROR=========" << endl;
		fout << "400" << endl;
		fout << "=====================" << endl;
		fout.close();
	}
	return;
}

void BpTree::Print() //Print every data in the BPTree
{
	ofstream fout;
	fout.open("log.txt", ios::app);
	BpTreeNode* cur = root;
	while (cur->getMostLeftChild())
	{
		cur = cur->getMostLeftChild();
	}
	fout << "======PRINT_BP======" << endl;
	while (cur)
	{
		map <string, VaccinationData*>* ma = cur->getDataMap();
		for (auto iter = ma->begin(); iter != ma->end(); iter++)
		{
			fout << iter->second->GetUserName() << " " << iter->second->GetVaccineName() << " " << iter->second->GetTimes() << " " << iter->second->GetAge() << " " << iter->second->GetLocationName() << endl;
		}
		cur = cur->getNext();
	}
	fout << "====================" << endl << endl;
	fout.close();
	return;
}

void BpTree::Deallocate() //Deallocate BpTreeDataNode
{
	if (root == NULL)
		return;
	queue <BpTreeNode*> q;
	BpTreeNode* cur = root;
	BpTreeNode* cur2 = root;
	while (cur2->getMostLeftChild())
	{
		cur2 = cur2->getMostLeftChild();
	}
	while (cur->getMostLeftChild())
	{
		BpTreeNode* deletenode = cur;
		map<string, BpTreeNode*>* ma = cur->getIndexMap();
		if(cur->getMostLeftChild())
			q.push(cur->getMostLeftChild());
		for (auto iter = ma->begin(); iter != ma->end(); iter++)
		{
			if (iter->second)
				q.push(iter->second);
			iter->second->deleteMap(iter->first);
		}
		ma->clear();
		delete deletenode;
		if (q.empty()) return;
		cur = q.front();
		q.pop();
	}
	while (cur2)
	{
		BpTreeNode* deletenode = cur2;
		map<string, VaccinationData*>* ma = cur2->getDataMap();
		ma->clear();
		cur2 = cur2->getNext();
		delete deletenode;
	}
	while (!q.empty())
	{
		q.pop();
	}
}

