#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;

class Process {
public:
	int pid;
	int	pArrivalTime;
	int	pRunTime;
	int	pEndTime;
	vector<int> memVector;
	vector<int> pagesUsed;

	Process(int pid_, int pArrivalTime_, int pRunTime_,
		const vector<int>& memory_needed) {
		pid = pid_;
		pArrivalTime = pArrivalTime_;
		pRunTime = pRunTime_;
		memVector = memory_needed;
	}

	int pageRequired(int pageSize) const {
		int total = 0;
		for (size_t i = 0; i < memVector.size(); i++) {
			total += memVector[i];
		}
		if (total % pageSize != 0) {
			total += pageSize;
		}
		return total / pageSize;
	} // end of pageRequired

	int findPage(int m) {
		for (size_t i = 0; i < pagesUsed.size(); i++) {
			if (pagesUsed[i] == m) {
				return i + 1;
			}
		}
		return -1;
	} // end of findPage
	~Process() {}

};

struct MemEntry
{
	int pNum;
	bool add;
	MemEntry(bool a, int b)
	{
		add = a;
		pNum = b;
	}
};

int main() {
	int mem_size, page_size;

	cout << "Input memory size: ";
	cin >> mem_size;
	cout << endl << "Input page size: (1:100, 2:200, 3:400)";
	cin >> page_size;

	while(page_size < 1 || page_size > 3)
	{
		cout << "Invalid page size entry. Try again!\nInput page size: (1:100, 2:200, 3:400)";
		cin >> page_size;
	}

	int avalablePageCount;
	int maxMem;
	int pageSize;
	vector<int> memMap;
	list<int> inputQueue;
	vector<Process> processVector;
	map<int, list<MemEntry>> memEvents;

	maxMem = mem_size;
	if (page_size == 1) pageSize = 100;
	else if (page_size == 2) pageSize = 200;
	else pageSize = 400;
	avalablePageCount = maxMem / pageSize;
	memMap = vector<int>(avalablePageCount, -1);

	ifstream file("in1.txt");
	if (!file.is_open()) {
		cout << "Can not open the file: " << "in1.txt" << endl;
		exit(0);
	}
	int pid, pArrivalTime_, pRunTime, counter, n, np;
	file >> counter;
	for (int i = 0; i < counter; i++) {
		file >> pid >> pArrivalTime_ >> pRunTime >> np;
		vector<int> memory_needed;
		int h = 0;
		while (h < np) {
			file >> n;
			memory_needed.push_back(n);
			h++;
		}
		processVector.push_back(Process(pid, pArrivalTime_, pRunTime, memory_needed));
		int j = processVector.size() - 1; // Hash Index for pid
		MemEntry temp(true, j);
		list<MemEntry> a = { temp };

		pair<map<int, list<MemEntry>>::iterator, bool> mem
				= memEvents.insert(pair<int, list<MemEntry>>(pArrivalTime_, a));
		if (!mem.second) { mem.first->second.push_back(temp); }
	}
	file.close();

	int t = 0;
	while (t <= 100000 && memEvents.size() != 0) {
		if (t == memEvents.begin()->first) {
			cout << "t = " << t << ": ";
			while (memEvents.begin()->second.size() != 0) {
				MemEntry trigger = memEvents.begin()->second.front();
				if (trigger.add){  // if true, add to queue
						cout << "Process " << trigger.pNum + 1 << " arrives" << "\n\t";
						inputQueue.push_back(trigger.pNum);
							list<int>::const_iterator frontIterator = inputQueue.cbegin();
							list<int>::const_iterator endIterator = inputQueue.cend();

							cout << "Input Queue:[ ";
							while (frontIterator != endIterator) {
								cout << *frontIterator + 1 << " ";
								frontIterator++;
							}
							cout << "]" << "\n\t";
				}//
				else {
						// removing from memory map
						cout << "Process " << trigger.pNum + 1 << " completes" << "\n\t";
						processVector[trigger.pNum].pEndTime = t;
						size_t size = processVector[trigger.pNum].pagesUsed.size();
						for (size_t i = 0; i < size; i++)
							memMap[processVector[trigger.pNum].pagesUsed[i]] = -1;
						avalablePageCount += size;

						// printing memory
							size_t mem_size = memMap.size();
							int head, tail;
							cout << "Memory Map: ";
							for (size_t i = 0; i < mem_size; i++) {
								if (memMap[i] != -1) {
									cout << i * pageSize << " - " << i * pageSize + pageSize -1 <<
										": Process " << memMap[i] + 1 << ", Page " <<
										processVector[memMap[i]].findPage(i) << "\n\t\t";
								}
								else { // if page is empty loop til we find a filled page
									head = i;
									tail = i;
									while (i < mem_size && memMap[i] == -1) {
										tail = i;
										i++;
									}
									i--;
									cout << (head) * pageSize << " - " << (tail) * pageSize + pageSize - 1 << ": Free frame(s)" << "\n\t\t";
								}
							}
							cout << endl;
					}
				memEvents.begin()->second.pop_front(); // iterate, check if correct
			}
			memEvents.erase(memEvents.begin());
		}

			list<int>::iterator frontIterator = inputQueue.begin(); // check queue
			while (frontIterator != inputQueue.end()) {
				if (processVector[*frontIterator].pageRequired(pageSize) <= avalablePageCount) { //if enough space add to mem, if not check next

						cout << "\tMM moves Process " << *frontIterator + 1 << " to memory" << "\n\t\t";
						size_t size = processVector[*frontIterator].pageRequired(pageSize);
						avalablePageCount -= size;
						int i = 0;

						while (size > 0) {
							if (memMap[i] == -1) { // check for empty page
								memMap[i] = *frontIterator;
								processVector[*frontIterator].pagesUsed.push_back(i);
								size--;
							}//
							i++;
						}

						// adding event to list
						MemEntry temp(false, *frontIterator);
						list<MemEntry> a = { temp };

						pair<map<int, list<MemEntry>>::iterator, bool> mem
								= memEvents.insert(pair<int, list<MemEntry>>(t + processVector[*frontIterator].pRunTime, a));
						if (!mem.second)
									mem.first->second.push_back(temp);
					 // end of adding event

					frontIterator = inputQueue.erase(frontIterator);

					// printing the queue
						list<int>::const_iterator frontIterator = inputQueue.cbegin();
						list<int>::const_iterator endIterator = inputQueue.cend();

						cout << "Input Queue:[ ";
						while (frontIterator != endIterator) {
							cout << *frontIterator + 1 << " ";
							frontIterator++;
						}
						cout << "]" << "\n\t";
					// end of printing queue

						size_t mem_size = memMap.size();
						int head, tail;
						cout << "Memory Map: ";
						for (size_t i = 0; i < mem_size; i++) {
							if (memMap[i] != -1) {
								cout << i * pageSize << " - " << i * pageSize + pageSize -1 <<
									": Process " << memMap[i] + 1 << ", Page " <<
									processVector[memMap[i]].findPage(i) << "\n\t\t";
							}
							else { // loops to fill page
								head = i;
								tail = i;
								while (i < mem_size && memMap[i] == -1) {
									tail = i;
									i++;
								}
								i--;
								cout << (head) * pageSize << " - " << (tail) * pageSize + pageSize - 1 << ": Free frame(s)" << "\n\t\t";
							}
						}
						cout << endl;
					//} // end of print_mem
				}
				else
					++frontIterator;
			}
		t += 100;
	}

	double total = 0;
	size_t size = processVector.size();
	for (size_t i = 0; i < size; i++) {
		total += (processVector[i].pEndTime - processVector[i].pArrivalTime);
	}
	cout << "Average Turnaround Time: " << total / size << endl;

	return 0;
}
