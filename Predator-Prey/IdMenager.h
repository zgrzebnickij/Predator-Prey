#pragma once
#include <stack>

class IdMenager{
private:	
	int max = 0;
	std::stack<int> freeIds;
public:
	int getfreeId() {
		if(freeIds.empty()){
			max++;
			return max;
		}
		int id = freeIds.top();
		freeIds.pop();
		return id;
	}
	void addIdToStack(int id) {
		freeIds.push(id);
	}
};