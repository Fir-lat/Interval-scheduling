#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <algorithm>
#include <map>
#include <iterator>
#include <list>
#include <string>
#include <stack>

#define MAXLEN 5
#define TASKNUM 7
#define RANGE 10
using namespace std;

class Task {
public:
	pair<int, int> Pair;
	int Index;

	Task(pair<int, int> pair, int index) {
		this->Pair = pair;
		this->Index = index;
	}
	~Task() {}

	static bool cmpWithPair(const Task& task1, const Task& task2) {
		if (task1.Pair.second < task2.Pair.second) {
			return true;
		}
		else if (task1.Pair.second == task2.Pair.second) {
			return task1.Pair.first < task2.Pair.first;
		}
		else return false;
	}

	static bool cmpWithTi(const Task& task1, const Task& task2) {
		return (task1.Pair.second - task1.Pair.first) < (task2.Pair.second - task2.Pair.first);
	}
};

int myAlgorithm(vector<pair<int, int>> tasks, vector<Task> Tasks);
int best(vector<pair<int, int>>& tasks);
void allsequence(vector<vector<int>>& allseq, int I, vector<int> seq, vector<int> nums);
int isOk(vector<int>& seq, vector<pair<int, int>>& tasks);

int main() {
	vector<pair<int, int>> tasks;
	vector<Task> Tasks;
	unsigned seed = time(0);
	srand(seed);
	for (int i = 0; i < TASKNUM; i++) {
		int x_i = rand() % RANGE;
		int d_i = x_i + rand() % MAXLEN + 1;
		d_i = d_i > RANGE ? RANGE : d_i;
		pair<int, int> Pair(x_i, d_i);
		tasks.push_back(Pair);
		Task current(Pair, i);
		Tasks.push_back(current);
		cout << x_i << " " << d_i << endl;
	}
	
	int Loptimal = best(tasks);
	cout << "best = " << Loptimal << endl;
	int L = myAlgorithm(tasks, Tasks);
	return 0;
}

int myAlgorithm(vector<pair<int, int>> tasks, vector<Task> Tasks) {
	vector<int> giveup;
	vector<int> sequence;
	int s = 0;
	sort(Tasks.begin(), Tasks.end(), Task::cmpWithPair);
	for (vector<Task>::iterator iter = Tasks.begin(); iter != Tasks.end(); ) {
		if ((iter->Pair).first < s) {
			giveup.push_back(iter->Index);
			iter = Tasks.erase(iter);
			continue;
		}
		vector<Task> set;
		set.push_back(*iter);
		vector<Task>::iterator subiter = iter + 1;
		while (subiter != Tasks.end()) {
			if ((subiter->Pair).first < (iter->Pair).second) {
				set.push_back(*subiter);
				subiter = Tasks.erase(subiter);
			}
			else {
				subiter++;
			}
		}
		iter = Tasks.erase(iter);
		sort(set.begin(), set.end(), Task::cmpWithTi);
		for (vector<Task>::iterator iter = set.begin(); iter != set.end(); iter++) {
			s += (iter->Pair).second - (iter->Pair).first;
			if ((iter->Pair).second < s) {
				giveup.push_back(iter->Index);
				s -= (iter->Pair).second - (iter->Pair).first;
			}
			else {
				sequence.push_back(iter->Index);
			}
		}
	}
	sequence.insert(sequence.end(), giveup.begin(), giveup.end());
	cout << "Output of my algorithm : ";
	for (int x : sequence) {
		cout << x << " ";
	}
	cout << endl;
	cout << "My Algorithm = ";
	return isOk(sequence, tasks);
}

int best(vector<pair<int, int>>& tasks) {
	int best = TASKNUM;
	vector<vector<int>> allSeq;
	vector<int> seq, nums;
	for (int i = 0; i < TASKNUM; i++) {
		nums.push_back(i);
	}
	allsequence(allSeq, 0, seq, nums);
	for (vector<int> sequence : allSeq) {
		cout << "sequence: ";
		for (int x : sequence) {
			cout << x << " ";
		}
		cout << " undue = ";
		best = std::min(best, isOk(sequence, tasks));
	}
	return best;
}

void allsequence(vector<vector<int>>& allseq, int I, vector<int> seq, vector<int> nums) {
	if (I == TASKNUM) {
		allseq.push_back(seq);
		return;
	}
	else {
		for (int i = 0; i < nums.size(); i++) {
			int num = nums[i];
			seq.push_back(num);
			nums.erase(nums.begin() + i);
			allsequence(allseq, I + 1, seq, nums);
			seq.pop_back();
			nums.insert(nums.begin() + i, num);
		}
		return;
	}
}

int isOk(vector<int>& seq, vector<pair<int, int>>& tasks) {
	int ddl = 0;
	int undue = 0;
	for (int i = 0; i < seq.size(); i++) {
		pair<int, int> Pair= tasks[seq[i]];
		ddl += Pair.second - Pair.first;
		if (Pair.second < ddl) {
			undue++;
		}
	}
	cout << undue << endl;
	return undue;
}