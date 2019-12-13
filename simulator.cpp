#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;

class Process {
private:
	int pid;
	int	pArrivalTime;
	int	pRunTime;
	int	pEndTime;
	vector<int> memVector;
	vector<int> pagesUsed;
	friend class simulator;

public:
	Process(int pid_, int a_time, int pRunTime_,
		const vector<int>& memory_needed) {
		pid = pid_;
		pArrivalTime = a_time;
		pRunTime = pRunTime_;
		memVector = memory_needed;
	} // done

	int pages_needed(int pageSize) const {
		int total = 0;
		for (size_t i = 0; i < memVector.size(); i++) {
			total += memVector[i];
		}
		if (total % pageSize != 0) total += pageSize;
		return total / pageSize;
	}

	int find_page(int m) {
		for (size_t i = 0; i < pagesUsed.size(); i++)
			if (pagesUsed[i] == m)
				return i + 1;
		return -1;
	}

	int turnaroundTime() const {
		return pEndTime - pArrivalTime;
	} // end of turnaroundtime

	~Process() {}

};

class simulator {
private:
	int freePages;
	int capacity;
	int pageSize;
	vector<int> memMap; // virtual pages
	list<int> queue; // processes index in processList
	vector<Process> processList; // vector of processes
	map<int, list<pair<bool, int>>> events; // time , TRUE = ADD / FALSE = remove, PID


public:
	simulator(int mem_size, int size) {
		capacity = mem_size;
		if (size == 1) pageSize = 100;
		else if (size == 2) pageSize = 200;
		else pageSize = 400;
		freePages = capacity / pageSize;
		memMap = vector<int>(freePages, -1);
	}

	bool read_file(string& path) {
		ifstream file("in1.txt");
		if (!file.is_open()) {
			cout << "Can not open the file: " << "in1.txt" << endl;
			exit(0);
		}

		int k, pid, a_time, pRunTime, count, n, n_piece, c;

		file >> count;
		for (int i = 0; i < count; i++) {
			file >> pid;
			file >> a_time;
			file >> pRunTime;
			file >> n_piece;
			vector<int> memory_needed;
			c = 0;
			while (c < n_piece) {
				file >> n;
				memory_needed.push_back(n);
				c++;
			}

			processList.push_back(Process(pid, a_time, pRunTime, memory_needed));
			k = processList.size() - 1; // Hash Index for pid
			pair<bool, int> temp(true, k);
			list<pair<bool, int>> a = { temp };

			pair<map<int, list<pair<bool, int>>>::iterator, bool> mem
					= events.insert(pair<int, list<pair<bool, int>>>(a_time, a));
			if (!mem.second)
						mem.first->second.push_back(temp);
			 // end of add event 1

		} // end of for loop
		file.close();
		file.clear();
		return true;
	}

	void virtual_clock() {
		int t = 0;
		while (t <= 100000 && events.size() != 0) {
			if (t == events.begin()->first) {//check if any events at time t // bug at 1700
				cout << "t = " << t << ": ";
				while (events.begin()->second.size() != 0) { // iterate through event list
					pair<bool, int> action = events.begin()->second.front(); // pointer to first elem in list
					if (action.first){  // if true, enqueue
						//enqueue(action.second);
						//void enqueue(int k) {
							cout << "Process " << action.second + 1 << " arrives" << endl<< "\t";
							queue.push_back(action.second);
								list<int>::const_iterator iter = queue.cbegin();
								list<int>::const_iterator iter_end = queue.cend();

								cout << "Input Queue:[ ";
								while (iter != iter_end) {
									cout << *iter + 1 << " ";
									iter++;
								}
								cout << "]" << endl<< "\t";
					}//
					else {//if false terminate process
							// removing frp, memory map
							cout << "Process " << action.second + 1 << " completes" << endl<<"\t";
							processList[action.second].pEndTime = t;
							size_t size = processList[action.second].pagesUsed.size();
							for (size_t i = 0; i < size; i++)
								memMap[processList[action.second].pagesUsed[i]] = -1;
							freePages += size;

							// printing memory
								size_t mem_size = memMap.size();
								int head, tail;
								cout << "Memory Map: ";
								for (size_t i = 0; i < mem_size; i++) {
									if (memMap[i] != -1) {
										cout << i * pageSize << " - " << i * pageSize + pageSize -1 <<
											": Process " << memMap[i] + 1 << ", Page " <<
											processList[memMap[i]].find_page(i) << endl<<" \t\t";
									}
									else { // if page is empty loop til we find a filled page
										head = i;
										tail = i;
										while (i < mem_size && memMap[i] == -1) {
											tail = i;
											i++;
										}
										i--;
										cout << (head) * pageSize << " - " << (tail) * pageSize + pageSize - 1 << ": Free frame(s)" << endl<<"\t\t";
									}
								}
								cout << endl;
							// end of printing memory

						} // end of else
					events.begin()->second.pop_front(); // iterate, check if correct
				} // end of while
				events.erase(events.begin());
			} // end of if (t<=100000)

				list<int>::iterator iter = queue.begin(); // check queue
				while (iter != queue.end()) {
					if (processList[*iter].pages_needed(pageSize) <= freePages) { //if enough space add to mem, if not check next
						//MM_add(*iter, t);
						//void MM_add(int k, int time) {
							cout << "\tMM moves Process " << *iter + 1 << " to memory" << endl<<"\t";
							size_t size = processList[*iter].pages_needed(pageSize);
							freePages -= size;
							int i = 0;

							while (size > 0) {
								if (memMap[i] == -1) { // check for empty page
									memMap[i] = *iter;
									processList[*iter].pagesUsed.push_back(i);
									size--;
								}//
								i++;
							}

							// adding event to list
							pair<bool, int> temp(false, *iter);
							list<pair<bool, int>> a = { temp };

							pair<map<int, list<pair<bool, int>>>::iterator, bool> mem
									= events.insert(pair<int, list<pair<bool, int>>>(t + processList[*iter].pRunTime, a));
							if (!mem.second)
										mem.first->second.push_back(temp);
						 // end of adding event


						iter = queue.erase(iter);

						// printing the queue
							list<int>::const_iterator iter = queue.cbegin();
							list<int>::const_iterator iter_end = queue.cend();

							cout << "Input Queue:[ ";
							while (iter != iter_end) {
								cout << *iter + 1 << " ";
								iter++;
							}
							cout << "]" << endl<<"\t";
						// end of printing queue


						//print_mem();
						//void print_mem() {
							size_t mem_size = memMap.size();
							int head, tail;
							cout << "Memory Map: ";
							for (size_t i = 0; i < mem_size; i++) {
								if (memMap[i] != -1) {
									cout << i * pageSize << " - " << i * pageSize + pageSize -1 <<
										": Process " << memMap[i] + 1 << ", Page " <<
										processList[memMap[i]].find_page(i) << endl<<"\t\t";
								}
								else { // if page is empty loop til we find a filled page
									head = i;
									tail = i;
									while (i < mem_size && memMap[i] == -1) {
										tail = i;
										i++;
									}
									i--;
									cout << (head) * pageSize << " - " << (tail) * pageSize + pageSize - 1 << ": Free frame(s)" << endl<<"\t\t";
								}
							}
							cout<<endl;
						//} // end of print_mem
					}
					else
						++iter;
				}
			t += 100;
		} // end of first while
	} // end of virtual

	void turn_around_time() {
		double total = 0;
		size_t size = processList.size();
		for (size_t i = 0; i < size; i++) {
			total += processList[i].turnaroundTime();
		}
		cout << "Average Turnaround Time: " << total / size << endl;
	} // end of turn_around_time

	~simulator() {}

};

int main() {
	int mem_size;
	int pageSize;          // Memory Size and Page Size for user input

	cout << "Input memory size: ";
	cin >> mem_size;
	cout << endl << "Input page size: (1:100, 2:200, 3:400)";
	cin >> pageSize;

	while(pageSize < 1 || pageSize > 3)
	{
		cout << "Invalid page size entry. Try again!\nInput page size: (1:100, 2:200, 3:400)";
		cin >> pageSize;
	}

	simulator sim(mem_size, pageSize);
	if (!sim.read_file("in1.txt"))
		cout << "Cant read file";

	sim.virtual_clock();
	sim.turn_around_time();

	return 0;
}
