#include "dheap.h"

using namespace std;

int main(int argn, char* argv[])
{
	DHeap<int> my_d_heap(3);
	int v[] = {6,7,4,5,2,3,1,8,0,-1};

	for(int i=0;v[i]!=-1;i++)
	{
		cout << "Inserting " << v[i] << endl;
		my_d_heap.insert(v[i]);
		my_d_heap.print();
		cout << "MIN = " << my_d_heap.min() << endl;
		cout << "\n------------------\n";
	}

	for(int i=0;v[i]!=-1;i++)
	{
		cout << "Deleting Min = " << my_d_heap.min() << endl;
		my_d_heap.deleteMin();
		my_d_heap.print();
		cout << "\n------------------\n";
	}

	return 0;
}
