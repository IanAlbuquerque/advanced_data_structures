#ifndef D_HEAP_H
#define D_HEAP_H

#include <iostream>
#include <vector>

using namespace std;

template <class ValueType>
class DHeap
{
private:
	int d;
	int size;
	vector<ValueType> values;

	void swap(int index_1, int index_2);
	int parent(int vertex_index);
	int son(int vertex_index, int son_number);
	void adjustUp(int vertex_index);
	void adjustDown(int vertex_index);
public:
	DHeap(int d);
	~DHeap();
	void insert(ValueType value);
	ValueType min();
	void deleteMin();
	void print();
};

template <class ValueType>
DHeap<ValueType>::DHeap(int d)
{
	this->d = d;
	this->size = 0;
}

template <class ValueType>
DHeap<ValueType>::~DHeap()
{
	(this->values).clear();
}

template <class ValueType>
int DHeap<ValueType>::parent(int vertex_index)
{
	if(vertex_index==0) return -1;
	else return (vertex_index-1)/(this->d);
}

template <class ValueType>
int DHeap<ValueType>::son(int vertex_index, int son_number)
{
	return vertex_index*(this->d)+son_number;
}

template <class ValueType>
void DHeap<ValueType>::swap(int index_1, int index_2)
{
	ValueType temp = (this->values)[index_1];
	(this->values)[index_1] = (this->values)[index_2];
	(this->values)[index_2] = temp;
}

template <class ValueType>
void DHeap<ValueType>::adjustUp(int vertex_index)
{
	int parent_index = parent(vertex_index);
	if(parent_index<0)
	{
		return;
	}
	else if((this->values)[vertex_index] >= (this->values)[parent_index])
	{
		return;
	}
	else
	{
		this->swap(vertex_index,parent_index);
		this->adjustUp(parent_index);
	}
}

template <class ValueType>
void DHeap<ValueType>::adjustDown(int vertex_index)
{
	int first_son = son(vertex_index,1);
	if(first_son >= this->size)
	{
		return;
	}

	int theorical_last_son = son(vertex_index,this->d);

	ValueType lowest_value = (this->values)[first_son];
	int lowest_son = first_son;

	for(int son=first_son;son<=theorical_last_son;son++)
	{
		if(son >= this->size)
		{
			break;
		}
		if((this->values)[son] < lowest_value)
		{
			lowest_value = (this->values)[son];
			lowest_son =  son;
		}
	}

	if(lowest_value >= (this->values)[vertex_index])
	{
		return;
	}

	this->swap(vertex_index,lowest_son);
	this->adjustDown(lowest_son);
}

template <class ValueType>
void DHeap<ValueType>::insert(ValueType value)
{
	(this->values).push_back(value);
	(this->size)++;
	this->adjustUp(this->size - 1);
}

template <class ValueType>
ValueType DHeap<ValueType>::min()
{
	return (this->values)[0];
}

template <class ValueType>
void DHeap<ValueType>::deleteMin()
{
	this->swap(0,this->size - 1);
	(this->size)--;
	this->adjustDown(0);
}

template <class ValueType>
void DHeap<ValueType>::print()
{
	if(this->size <= 0) return;
	cout << (this->values)[0];
	if(this->size > 1) cout << endl;
	for(int i=1;i<this->size;i++)
	{
		cout << (this->values)[i] << " " ;
		if(i % this->d == 0 && i != this->size - 1) cout << endl;
	}
	cout << endl;
}

#endif