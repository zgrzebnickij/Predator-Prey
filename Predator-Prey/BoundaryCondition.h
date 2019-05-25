#pragma once
class BoundaryCondition {
public:
	BoundaryCondition(int size_):size(size_){ };
	~BoundaryCondition() {};
	int torus(int index){
		int newIndex = index % size;
		if (newIndex < 0) {
			newIndex += size;
		}
		return newIndex;
	}
private:
	int size;
};