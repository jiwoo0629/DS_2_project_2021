#include "Manager.h"
Manager::Manager(int bpOrder) //Constructor of manager
{
	avl = new AVLTree();
	bp = NULL;
}

Manager::~Manager() //Destructor of manager
{
	delete bp;
	delete avl;
	vector<VaccinationData*>().swap(Print_vector);
}

void Manager::run(const char* command_txt)
{
	fin.open(command_txt);
	flog.open("log.txt", ios::out);
	if (!fin)
	{
		flog << "File Open Error" << endl;
		return;
	}

	while (!fin.eof())
	{
		string cmd;
		getline(fin, cmd); //read from the file line by line
		char* cmd_buf = new char[cmd.length() + 1];
		strcpy(cmd_buf, cmd.c_str());

		char* tmp = strtok(cmd_buf, " ");
		string str;		
		if(tmp != NULL)
			str = string(tmp);
		if (str == "LOAD") //if the command is "LOAD"
		{
			char* cmd = strtok(NULL, " ");
			if (cmd == NULL)
				LOAD();
			else //if the command has more parameter
				printErrorCode(100);
		}
		else if (str == "VLOAD") //if the command is "VLOAD"
		{
			char* cmd = strtok(NULL, " ");
			if (cmd == NULL)
				VLOAD();
			else //if the command has more parameter
				printErrorCode(200);
		}
		else if (str == "ADD") //if the command is "ADD"
		{
			int count = 1;
			string name, vaccine, location;
			int age = 0;
			//if the command doesn't fit its correct form (ex)command doesn't have four parameters)
			if (count == 1)
			{
				tmp = strtok(NULL, " ");
				name = string(tmp);
				if (name == "")
				{
					printErrorCode(300);
					count = 0;
				}
			}
			if (count == 1)
			{
				tmp = strtok(NULL, " ");
				vaccine = string(tmp);
				if (vaccine == "")
				{
					printErrorCode(300);
					count = 0;
				}
			}
			if (count == 1)
			{
				tmp = strtok(NULL, " ");
				age = atoi(tmp);
				if (age == 0)
				{
					printErrorCode(300);
					count = 0;
				}
			}
			if (count == 1)
			{
				tmp = strtok(NULL, " ");
				location = string(tmp);
				if (location == "")
				{
					printErrorCode(300);
					count = 0;
				}
			}
			if (count == 1)
			{
				char* cmd = strtok(NULL, " ");
				if (cmd == NULL)
				{
					ADD(name, vaccine, age, location);
				}
			}
		}
		else if (str == "SEARCH_BP") //if the command is "SEARCH_BP"
		{
			tmp = strtok(NULL, " ");
			if (tmp == NULL) //if there is no parameter
			{
				printErrorCode(400);
			}
			else
			{
				string first = string(tmp);
				tmp = strtok(NULL, " ");
				if (tmp == NULL) //if there is only one parameter
				{
					SEARCH_BP(first);
				}
				else
				{
					string end = string(tmp);
					char* cmd = strtok(NULL, " ");
					if (cmd == NULL)
					{
						if ((first.length() == 1) && (end.length() == 1)) //if there are two parameters and there length is 1
						{
							SEARCH_BP(first, end);
						}
						else
							printErrorCode(400);
					}
					else
						printErrorCode(400);
				}
			}
		}
		else if (str == "SEARCH_AVL") //if the command is "SEARCH_AVL"
		{
			tmp = strtok(NULL, " ");
			if (tmp == NULL) //if there is no parameter
				printErrorCode(500);
			else
			{
				string name = string(tmp);
				char* cmd = strtok(NULL, " ");
				if (cmd == NULL)
				{
					SEARCH_AVL(name);
				}
				else //if there are more parameters
					printErrorCode(500);
			}
		}
		else if (str == "VPRINT") //if the command is "VPRINT"
		{
			tmp = strtok(NULL, " ");
			if (tmp == NULL) //if there is no parameter
				printErrorCode(600);
			else
			{
				string type = string(tmp);
				char* cmd = strtok(NULL, " ");
				if (cmd == NULL)
					VPRINT(type);
				else //if there are more parameters
					printErrorCode(600);
			}
		}
		else if (str == "PRINT_BP") //if the command is "PRINT_BP"
		{
			char* cmd = strtok(NULL, " ");
			if (cmd == NULL)
			{
				if (bp == NULL) //if BPTree doesn't exist
					printErrorCode(700);
				else
					bp->Print();
			}
			else //if there are more parameters
				printErrorCode(700);
		}
		else if (str == "EXIT") //if the command is "EXIT"
		{
			printSuccess("EXIT");
			fin.close();
			flog.close();
			return; //finish the program
		}
		else
			printErrorCode(800);
	}
	fin.close();
	flog.close();
}

bool Manager::LOAD() //LOAD the data from "input_data_txt" file and insert the data into BPTree and AVLTree
{
	ifstream fin2;
	fin2.open("input_data.txt");
	if (!fin2) //if there is no file to read
	{
		printErrorCode(100);
		fin2.close();
		return false;
	}
	else if (bp != NULL) //if BPTree already exists
	{
		printErrorCode(100);
		fin2.close();
		return false;
	}
	else
	{
		bp = new BpTree(3);
		while (!fin2.eof()) //while the file ends
		{
			string name;
			int age;
			string vaccine;
			string location;
			int count;
			fin2 >> name >> vaccine >> count >> age >> location;
			VaccinationData* vdata = new VaccinationData();
			vdata->SetUserName(name);
			vdata->SetAge(age);
			vdata->SetVaccineName(vaccine);
			vdata->SetLocationName(location);
			vdata->SetTimes(count);
			bp->Insert(vdata); //Insert data into BPTree using VaccinationData
		}
		printSuccess("LOAD");
		fin2.close();
		return true;
	}
}

bool Manager::VLOAD() //Insert every data into the Print_vector from AVLTree
{
	if (avl->Getroot() == NULL) //if AVLTree is empty
	{
		printErrorCode(200);
		return false;
	}
	else
	{
		if (!Print_vector.empty()) //if Print_vector is not empty, clear Print_vector and insert the data
			Print_vector.clear();
		avl->GetVector(Print_vector);
		printSuccess("VLOAD");
		return true;
	}
}

bool Manager::ADD(string name, string vaccine, int age, string location) //Insert data into BPTree and AVLTree directly
{
	if ((name == "") || (vaccine == "") || (age == 0) || (location == ""))
	{
		printErrorCode(300);
		return false;
	}
	else
	{
		VaccinationData* vdata = new VaccinationData();
		vdata->SetUserName(name);
		vdata->SetAge(age);
		vdata->SetTimesInc();
		vdata->SetVaccineName(vaccine);
		vdata->SetLocationName(location);

		BpTreeNode* newnode = bp->searchDataNode(name); //Check if the data with 'name' already exists in BPTree
		if (newnode != NULL) //if the data exists
		{
			map<string, VaccinationData*>* ma = newnode->getDataMap();
			VaccinationData* newVnode = new VaccinationData(); //VaccinationData that has a data whose name is 'name'
			for (auto iter = ma->begin(); iter != ma->end(); iter++)
			{
				if (iter->second->GetUserName() == name)
					newVnode = iter->second;
			}
			if ((newVnode->GetVaccineName() == "Pfizer") || (newVnode->GetVaccineName() == "Moderna") || (newVnode->GetVaccineName() == "AstraZeneca")) //Vaccines that we should get 2 times
			{
				if (newVnode->GetTimes() == 2) //if user already got the vaccine 2 times, insert the data only into the AVLTree
				{
					printErrorCode(300);
					VaccinationData* searchVnode = avl->Search(name); //if the data whose name is 'name' already exists, do not insert.
					if (searchVnode == NULL)
						avl->Insert(newVnode);
					return false;
				}
				else //if user got the vaccine less than 2 times, insert the data into BPTree and AVLTree
				{
					bp->Insert(vdata);
					if (newVnode->GetTimes() == 2)
					{
						VaccinationData* searchVnode = avl->Search(name);
						if (searchVnode == NULL)
							avl->Insert(newVnode);
					}
					flog << "======ADD======" << endl;
					flog << vdata->GetUserName() << " " << vdata->GetVaccineName() << " " << vdata->GetAge() << " " << vdata->GetLocationName() << endl;
					flog << "===============" << endl << endl;
					return true;
				}
			}
			else if (newVnode->GetVaccineName() == "Janssen") //Vaccine that we should get only 1 time
			{
				if (newVnode->GetTimes() == 1)
				{
					printErrorCode(300);
					VaccinationData* searchVnode = avl->Search(name);
					if (searchVnode == NULL)
						avl->Insert(newVnode);
					return false;
				}
				else if (newVnode->GetTimes() == 0)
				{
					bp->Insert(vdata);
					if (newVnode->GetTimes() == 1)
					{
						VaccinationData* searchVnode = avl->Search(name);
						if (searchVnode == NULL)
							avl->Insert(newVnode);
					}
					flog << "======ADD======" << endl;
					flog << vdata->GetUserName() << " " << vdata->GetVaccineName() << " " << vdata->GetAge() << " " << vdata->GetLocationName() << endl;
					flog << "===============" << endl << endl;
					return true;
				}
			}
			else
			{
				printErrorCode(300);				
				return false;
			}
		}
		else //if the data doesnt' exist
		{
			bp->Insert(vdata);
			if (vdata->GetVaccineName() == "Janssen") //if the vaccine is Janssen, add to AVLTree directly because we should get it only 1 time.
			{
				VaccinationData* searchVnode = avl->Search(name);
				if (!searchVnode)
					avl->Insert(vdata);
			}
			flog << "======ADD======" << endl;
			flog << vdata->GetUserName() << " " << vdata->GetVaccineName() << " " << vdata->GetAge() << " " << vdata->GetLocationName() << endl;
			flog << "===============" << endl << endl;
			return true;
		}
	}
}

bool Manager::SEARCH_BP(string name) //Search the data in the BPTree by name
{
	if (name == "") //if 'name' is not entered
	{
		printErrorCode(400);
		return false;
	}
	else if (bp == NULL) //if BPTree does not exist
	{
		printErrorCode(400);
		return false;
	}
	else
	{
		BpTreeNode* newnode = bp->searchDataNode(name); //Check if the data with 'name' exists in BPTree
		if (newnode == NULL) //if the data doesn't exist
		{
			printErrorCode(400);
			return false;
		}
		else
		{
			map <string, VaccinationData*>* m = newnode->getDataMap();
			flog << "======SEARCH_BP======" << endl;
			for (auto iter = m->begin(); iter != m->end(); iter++)
			{
				if (iter->second->GetUserName() == name)
					flog << iter->second->GetUserName() << " " << iter->second->GetVaccineName() << " " << iter->second->GetTimes() << " " << iter->second->GetAge() << " " << iter->second->GetLocationName() << endl;
			}
			flog << "=====================" << endl << endl;
			return true;
		}
	}
}

bool Manager::SEARCH_BP(string start, string end) //Search every data from whose name starts with 'start' to whose name starts with 'end' in BPTree
{
	if ((start == "") && (end == "")) //if 'start' and 'end' is not entered
	{
		printErrorCode(400);
		return false;
	}
	else if (bp == NULL) //if BPTree does not exist
	{
		printErrorCode(400);
		return false;
	}
	else
	{
		bp->SearchRange(start, end);
		return true;
	}
}

bool Manager::SEARCH_AVL(string name) //Search the data with 'name' from AVLTree
{
	if (name == "") //if 'name' is not entered
	{
		printErrorCode(500);
		return false;
	}
	else if (avl->Getroot() == NULL) //if AVLTree is empty
	{
		printErrorCode(500);
		return false;
	}
	else
	{
		VaccinationData* node = avl->Search(name); //Check if there is a data with 'name' in AVLTree
		if (node == NULL) //if the data does not exist
		{
			printErrorCode(500);
			return false;
		}
		else
		{
			flog << "======SEARCH_AVL======" << endl;
			flog << node->GetUserName() << " " << node->GetVaccineName() << " " << node->GetTimes() << " " << node->GetAge() << " " << node->GetLocationName() << endl;
			flog << "=====================" << endl << endl;
			return true;
		}
	}
}

bool CompareA(VaccinationData* vac1, VaccinationData* vac2)
{
	if (vac1->GetTimes() == vac2->GetTimes())
	{
		if (vac1->GetAge() == vac2->GetAge()) { return vac1->GetUserName() < vac2->GetUserName(); } //Compare Name in ascending order (last priority)
		else { return vac1->GetAge() < vac2->GetAge(); } //Compare Age in ascending order (second priority)
	}
	else { return vac1->GetTimes() < vac2->GetTimes(); } //Compare GetTimes in ascending order (first priority)
}

bool CompareB(VaccinationData* vac1, VaccinationData* vac2)
{
	if (vac1->GetLocationName() == vac2->GetLocationName())
	{
		if (vac1->GetAge() == vac2->GetAge()) { return vac1->GetUserName() < vac2->GetUserName(); } //Compare Name in ascending order (last priority)
		else { return vac1->GetAge() > vac2->GetAge(); } //Compare Age in descending order (second priority)
	}
	else { return vac1->GetLocationName() < vac2->GetLocationName(); } //Compare LocationName in ascending order (first priority)
}

bool Manager::VPRINT(string type_) //Print every data in Print_vector with compare condition A or B
{
	if (avl->Getroot() == NULL) //if AVLTree is empty
	{
		printErrorCode(600);
		return false;
	}
	else if (Print_vector.empty()) //if Print_vector is empty
	{
		printErrorCode(600);
		return false;
	}
	else if (type_ == "") //if type is not entered
	{
		printErrorCode(600);
		return false;
	}
	else if (type_ == "A") //if compare condition is A
	{
		sort(Print_vector.begin(), Print_vector.end(), CompareA); //sort by CompareA function
		flog << "======VPRINT A======" << endl;
		for (auto it = Print_vector.begin(); it != Print_vector.end(); it++)
		{
			flog << (*it)->GetUserName() << " " << (*it)->GetVaccineName() << " " << (*it)->GetTimes() << " " << (*it)->GetAge() << " " << (*it)->GetLocationName() << endl;
		}
		flog << "====================" << endl << endl;
		return true;
	}
	else if (type_ == "B") //if compare condition is B
	{
		sort(Print_vector.begin(), Print_vector.end(), CompareB); //sort by CompareB function
		flog << "======VPRINT B======" << endl;
		for (auto it = Print_vector.begin(); it != Print_vector.end(); it++)
		{
			flog << (*it)->GetUserName() << " " << (*it)->GetVaccineName() << " " << (*it)->GetTimes() << " " << (*it)->GetAge() << " " << (*it)->GetLocationName() << endl;
		}
		flog << "====================" << endl << endl;
		return true;
	}
	else
	{
		printErrorCode(600);
		return false;
	}
}

void Manager::printErrorCode(int n)
{
	flog << "========== ERROR ==========" << endl;
	flog << n << endl;
	flog << "===========================" << endl << endl;
}

void Manager::printSuccess(const char* command)
{
	flog << "==========" << command << "==========" << endl;
	flog << "Success" << endl;
	flog << "===========================" << endl << endl;
}
