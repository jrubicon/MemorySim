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


void printInputQueue(queue<int> q) //queue keeps its contents because it calls by value and not by reference
{
	cout << "[";
    while(!q.empty())
    {
        cout << q.front();
        cout << " ";
        q.pop();
    }
    cout << "]";
}

void printMemoryMap(vector<int> memMap, int pageSize)
//memory map differs based on page size
{
	cout << "Memory Map: ";
	for(int i = 0; i < memMap.size(); i++)
	{
		if(memMap[i] == -1) // if page is empty
		{

		}
		else
		{
			cout << i * pageSize << " - " << i * pageSize + pageSize-1;
			cout << ": Process " << memMap[i]+1 << ", Page " << processVector_[memMap[i]].findPage(i) << endl;
		}
	}
}

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
	ifstream file("in1.txt");
	if(!file.is_open)
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
		Process tempProc = Process(tempPid, tempArrivalTime, tempRunTime, tempMemVector);
		processVector_.push_back(tempProc);

		int pNum = processVector_.size() -1;

		//add event function
		MemEntry m = new MemEntry(tempProc, true); //true means its going into the memory map
		list<MemEntry> mList = {m};
		pair<map<int,list<MemEntry>>::iterator, bool> mem = memEvents_.insert(pair<int,list<MemEntry>>(tempArrivalTime,mList)); 
		//inserts a pair of when it arrived, and a list of processes that get added at that time
		//insert returns a pair of (iterator to the inserted elemdent, boolean of whether it was inserted correctly or not. returns false if element already exists)
		//returns false if an equivalent key is already in the table
		//keys for this insert is arrival time
		if(!mem.second) //if the entry is already inside of events. if an event with the same arrival time already exists
		{
			mem.first.second.push_back(m); //put it in the list, since it didnt get put in last time
		}

	}

	file.close()

	int t = 0;

	while(t < 999999 && memEvents_.size() != 0)
	{
		if( t == memEvents_.begin()->first) // if any events are at time t
		{
			cout << "t = " << t << ": ";
			while(memEvents_.begin()->second.size() != 0) //iterates through the events of that t value
			{
				MemEntry act = memEvents_.begin()->second.front(); //first MemEntry in List
				if (act.bool_)//data type of act is MemEntry and second the bool is whether to add it or not
				{
					cout << "Process " << processVector_.size() << "to memory" << endl;
					inputQueue_.push(processVector_.size()-1);
					printInputQueue(inputQueue_);
					//enqueue(action.second); act.first would be tempArrivTime
				}
				else
				{
					//MM_remove(action.second, t);
					cout << "Process " << processVector_.size() << "completes" << endl;
					processVector_[processVector_.size()-1].doneTime_ = t;
					for(int i = 0; i < processVector_[processVector_.size()-1].pagesUsed_.size(); i++)
					{
						pageVector_[processVector_[processVector_.size()-1].pagesUsed_[i]] = -1;
					}
					availablePageCount_ += processVector_[processVector_.size()-1].pagesUsed_.size();
					//printMem()

					//pop off front to remove it
					memEvents_.begin()->second.pop_front();
				}
				 //after handling act it removes it by popping it off then repeats
			}
			memEvents_.erase(memEvents_.begin());//erases first element of the map
		}
		load_from_queue(t);
		t += 100;
	}
	


}