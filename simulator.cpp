#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <string>
using namespace std;

class Process
{
public:
	Process(int pid, int arrivalTime, int runTime, vector<int> memVector)
	:pid_(pid), arrivalTime_(arrivalTime), runTime_(runTime), memVector_(memVector)
	{
		int c = 0;
		for(int i = 0; i < memVector_.size(); i++)
		{
			c += memVector_[i];
		}
		totalMem_ = c;
	}

	int pagesRequired(int pageSize)
	{
		if(totalMem_ % pageSize != 0)
		{
		return (totalMem_ / pageSize) + 1;
		}
		return totalMem_ / pageSize;
	}

	int findPage(int pageNumInMap)
	{
		for(int i = 0; i < pagesUsed_.size(); i++)
		{
			if(pagesUsed_[i] == pageNumInMap)
			{
				return i + 1;
			}
		}
		return -1;
	}

private:
	int pid_;
	int arrivalTime_;
	int runTime_;
	int doneTime_;
	int totalMem_;
	vector<int> memVector_;
	vector<int> pagesUsed_;
}

struct MemEntry
{
	Process proc_;
	bool add_;
	MemEntry(Process p, bool b)
	{
		proc_ = p;
		add_ = b;
	}
}

/*
class Sim
{

private:
	int maxMem_;
	int pageSize_
	int availablePages_;

}
*/

int main()
{
	int maxMem_;
	int pageSize_;
	int availablePageCount_
	vector<int> pageVector_;
	queue<int> inputQueue_;
	vector<Process> processVector_;
	map<int, list<pair<bool, int>>> memEvents_;


	cout << "Enter memory size: ";
	cin >> maxMem;
	cout << "Enter page size 1 = 100, 2 = 200, 3 = 400";
	cin >> pageSize;

	while(pageSize > 3 || pageSize < 1)
	{
		cout << "Invalid page size entry: try again";
		cin >> pageSize;
	}



	//reading the file
	ifstream input("in1.txt");
	if(!input.is_open)
	{
		cout << "could not read file"
		return 0;
	}

	int numProcesses;

	int tempPid;
	int tempArrivalTime;
	int tempRunTime;
	int tempChunkCount;
	vector<int> tempMemVector;

	file >> numProcesses;


	for(int i = 0; i < numProcesses; i++)
	{
		file >> tempPid;
		file >> tempArrivalTime;
		file >> tempRunTime;
		file >> tempChunkCount;
		for(int j = 0; j < tempChunkCount; j++)
		{
			int chunk;
			file >> chunk;
			tempMemVector.push_back(n);
		}
		processList.push_back(Process(tempPid, tempArrivalTime, tempRunTime, tempMemVector));

		int pNum = processList.size() -1;

		//add event function
		MemEntry m = new MemEntry(pnum, true); //this stuff might have to all go in the for loop. im actually positive it does
		list<MemEntry> mList = {m};
		pair<map<int,list<MemEntry>>::iterator, bool> mem = events.insert(pair<int,list<MemEntry>>(tempArrivalTime,mList));
		//insert returns a pair of (iterator to the inserted elemdent, boolean of whether it was inserted correctly or not. returns false if element already exists)
		//returns false if an equivalent key is already in the table
		//keys for this insert is arrival time
		if(!mem.second) //if the entry is already inside of events. if an event with the same arrival time already exists
		{
			mem.first.second.push_back(m); //put it in the list, since it didnt get put in last time
		}

	}

	int t = 0;

	while(t < 999999 && events.size() != 0)
	{
		if( t == events.begin()->first) // if any events are at time t
		{
			cout << "t = " << t << ": ";
			while(events.begin()->second.size() != 0)
			{
				MemEntry act = events.begin()->second.front();
				if (action.first)
				{
					enqueue(action.second);
				}
				else
				{
					MM_remove(action.second, t);
				}
				events.begin()->second.pop_front();
			}
			events.erase(events.begin());
		}
		load_from_queue(t);
		t += 100;
	}

//FUNCTIONS: JUSTIN ****************************************************
//Load_from_queue				FREE_PAGES VARIABLE UNACCOUNTED FOR
list<int>::iterator iter = inputQueue_.begin(); // check queue
		while (iter != inputQueue_.end()) {
			//////////
			if (processVector_[*iter].pagesRequired(pageSize_) <= free_pages) { //if enough space add to mem, if not check next

				//MM_add(*iter, t); // REPLACED WITH MM_ADD FUNCTION
				cout << "\tMM moves Process " << k + 1 << " to memory" << endl<<"\t";
				size_t size = processVector_[k].pagesRequired(pageSize_);
				free_pages -= size;
				int i = 0;

				while (size > 0) {
					if (pageVector_[i] == -1) {// if page is free else check next page
						pageVector_[i] = k;
						processVector_[k].pages_used.push_back(i);
						size--;
					}
					i++;
				}
				add_event(k, time + processVector_[k].run_time, false); // REPLACE????
					/*	pair<bool, int> temp(action, k);
						list<pair<bool, int>> a = { temp };

						pair<map<int, list<pair<bool, int>>>::iterator, bool> mem
						= events.insert(pair<int, list<pair<bool, int>>>(t, a));
						if (!mem.second)
						mem.first->second.push_back(temp); */
				//////////////////////////

				iter = inputQueue_.erase(iter);

				//print_queue(); // REPLACED
						list<int>::const_iterator iter = inputQueue_.cbegin();
						list<int>::const_iterator iter_end = inputQueue_.cend();

						cout << "Input Queue:[ ";
						while (iter != iter_end) {
							cout << *iter + 1 << " ";
							iter++;
						}
						cout << "]" << endl<<"\t";
				//ORIGINALLY PRINT_MEM()CALL
				//Print_mem
				size_t mem_size = pageVector_.size();
				int start, tail;
				cout << "Memory Map: ";
						for (size_t i = 0; i < mem_size; i++) {
							if (pageVector_[i] != -1) {
								cout << i * pageSize_ << " - " << i * pageSize_ + pageSize_ -1 <<": Process "
								<< pageVector_[i] + 1 << ", Page " << processVector_[pageVector_[i]].findPage(i) << endl<<"\t\t";
							}
							else { // if page is empty loop til we find a filled page
								tail = i;
								start = i;
									while (i < mem_size && pageVector_[i] == -1) {
										tail = i;
										i++;
									}
								i--;
								cout << (start) * pageSize_ << " - " << (tail) * pageSize_ + pageSize_ - 1 << ": Free frame(s)"
								<< endl<<"\t\t";
							}
						}
						cout<<endl;
				//END PRINT MEM
			}
			else
				++iter;
		}

//Virtual_clock




//****************************************************************************



}
