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
		for (int i = 0; i < memVector_.size(); i++)
		{
			c += memVector_[i];
		}
		totalMem_ = c;
	}

	int pagesRequired(int pageSize)
	{
		if (totalMem_ % pageSize != 0)
		{
			return (totalMem_ / pageSize) + 1;
		}
		return totalMem_ / pageSize;
	}

	int findPage(int pageNumInMap)
	{
		for (int i = 0; i < pagesUsed_.size(); i++)
		{
			if (pagesUsed_[i] == pageNumInMap)
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
class Simulation {
private:
	vector<int> memMap; //virtual pages
	list<int> inputQueue; // processes index in process_list
	vector<Process> processVector;
	Map<int, list<MemEntry>> memEvents; // time , TRUE = ADD / FALSE = remove, PID
	int availablePageCount;
	int maxMem;
	int pageSize;
public:
	Simulation(int mem_size, int size) {
		maxMem = mem_size;
		if (size == 1) pageSize = 100;
		else if (size == 2) pageSize = 200;
		else pageSize = 400;
		availablePageCount = maxMem / pageSize;
		memMap = vector<int>(availablePageCount, -1);
	}

// Add_event
	
//mm_add
	
	void MM_add(int mm_add, int time) {
		cout << "\tMemory Management moves Process " << mm_add + 1 << " to memory" << endl << "\t";
		
		int size = processVector[mm_add].pagesRequired(pageSize);
		
		availablePageCount -= size;
		int k = 0;

		while (size > 0) {
			if (memMap[k] == -1) {
				memMap[k] = mm_add;
				processVector[mm_add].pagesUsed_.push_back(k);
			}
			k++;
		}
		add_event(mm_add, time + processVector[mm_add].runtime, false);
	}
  // mm_remove
	void MM_remove(int j, int l) {
		cout << "Process " << j + 1 << " completes" << endl << "\t";
		
		processVector[j].doneTime_ = l;
		int size = processVector[j].pagesUsed.size();
		for (i = 0; i < size; i++)
			memMap[processVector][j].pagesUsed[i]] = -1;
			availablePageCount += size;
			print_mem();
	}
 // enque
	void enque(int k) {
		cout << "Process " << k + 1 << " arrives" << endl << "\t";
		queue.push_back(k);
		print_queue();
	}



};
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

	while (pageSize > 3 || pageSize < 1)
	{
		cout << "Invalid page size entry: try again";
		cin >> pageSize;
	}



	//reading the file
	ifstream input("in1.txt");
	if (!input.is_open)
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


	for (int i = 0; i < numProcesses; i++)
	{
		file >> tempPid;
		file >> tempArrivalTime;
		file >> tempRunTime;
		file >> tempChunkCount;
		for (int j = 0; j < tempChunkCount; j++)
		{
			int chunk;
			file >> chunk;
			tempMemVector.push_back(n);
		}
		processList.push_back(Process(tempPid, tempArrivalTime, tempRunTime, tempMemVector));

		int pNum = processList.size() - 1;

		//add event function
		MemEntry m = new MemEntry(pnum, true); //this stuff might have to all go in the for loop. im actually positive it does
		list<MemEntry> mList = { m };
		pair<map<int, list<MemEntry>>::iterator, bool> mem = events.insert(pair<int, list<MemEntry>>(tempArrivalTime, mList));
		//insert returns a pair of (iterator to the inserted elemdent, boolean of whether it was inserted correctly or not. returns false if element already exists)
		//returns false if an equivalent key is already in the table
		//keys for this insert is arrival time
		if (!mem.second) //if the entry is already inside of events. if an event with the same arrival time already exists
		{
			mem.first.second.push_back(m); //put it in the list, since it didnt get put in last time
		}

	}
	

	int t = 0;

	while (t < 999999 && events.size() != 0)
	{
		if (t == events.begin()->first) // if any events are at time t
		{
			cout << "t = " << t << ": ";
			while (events.begin()->second.size() != 0)
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



}
