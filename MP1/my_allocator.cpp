/* 
    File: my_allocator.cpp

    Author: Austin Peterson
            Department of Computer Science
            Texas A&M University
    Date  : <date>

    Modified: 

    This file contains the implementation of the class MyAllocator.
	
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cstdlib>
#include<stdlib>
#include<stdio>
#include <vector>
#include "my_allocator.hpp"
#include <assert.h>
#include <iostream>

/*--------------------------------------------------------------------------*/
/* NAME SPACES */
/*--------------------------------------------------------------------------*/

using namespace std;
/* I know, it's a bad habit, but this is a tiny program anyway... */

/*--------------------------------------------------------------------------
Malloc:
1 round up to fib*basic block
2 find the freelist thats big enough and not empty
3 get that segment, return if perfect size, else split and distribute to f(i-1) and f(i-2)
4 do recursively until you return the perfect size

keep track of left and right buddy

Free (with merging):
1 get segment header
2 get size of segment to be freed (to find right fibo flist especially)
3 find buddy
4 if buddy is free && buddy size then merge(segment,buddy) then rinse and repeat w merging -- merge includes rmoving flist
	else just add to correct fibo flist, break

--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR CLASS MyAllocator */
/*--------------------------------------------------------------------------*/

MyAllocator::MyAllocator(size_t _basic_block_size, size_t _size)
{

	start_addr = (SegmentHeader *)std::malloc(_size);
	int fib_size = round_down_to_fibonacci(_size, _basic_block_size);
	cout << "size of segment header: " << sizeof(SegmentHeader) << endl;
	cout << "size: " << _size << endl
		 << "fib_size: " << fib_size << endl;
	start_addr->length = fib_size;
	available_memory = fib_size;
	memory_length = _size;
	block_size = _basic_block_size;
	int n = 1;
	while (1 == 1)
	{ //this loop is designed to calculate the size that the vector needs to be for the freelists
		int fibo = fib(n);
		if (fibo * _basic_block_size == fib_size)
		{
			break;
		}
		n++;
	}
	cout << "n: " << n << endl;
	for (int i = 0; i < n; i++)
	{
		fList_vector.push_back(FreeList());
	}
	SegmentHeader *first_head = new (start_addr) SegmentHeader(fib_size, true);
	fList_vector[fList_vector.size() - 1].Add(first_head);
}
MyAllocator::~MyAllocator()
{
	Free(start_addr);
}

Addr MyAllocator::Malloc(size_t _length)
{
	// This empty implementation just uses C standard library malloc
	cout << "<------------------------------------------------------------>" << endl;

	size_t not_rounded = _length + sizeof(SegmentHeader); //round to bbs
	size_t rounded_size = round_up_to_fibonacci(not_rounded, block_size);
	cout << endl
		 << "Memory Necessary: " << rounded_size << endl
		 << "Memory Available: " << available_memory << endl
		 << endl;
	if (rounded_size > available_memory)
	{
		cout << "Not enough available memory" << endl;
		return NULL;
	}
	else
	{
		cout << "Sufficient Memory Available" << endl;
	}
	bool loop_continue = true;
	while (loop_continue)
	{

		loop_continue = false;
		for (int i = 0; i < fList_vector.size(); i++)
		{
			cout << "Checking level " << i << endl;
			if (fList_vector[i].head != NULL && fib(i) * block_size >= rounded_size)
			{
				cout << "valid header " << fList_vector[i].head->CheckValid() << endl;
				SegmentHeader *iter = fList_vector[i].head;
				cout << "iter->length: " << iter->length << endl;
				fibRemove(iter, i);
				if (iter->length == rounded_size && iter->is_free == true && iter->CheckValid())
				{
					iter->is_free = false;
					available_memory -= iter->length;
					cout << "found proper segment" << endl;
					return ++iter;
				}
				if (iter->length > rounded_size && iter->is_free == true)
				{
					cout << "splitting at level: " << i << endl;
					fib_split(iter, i, rounded_size);
					loop_continue = true;
					break;
				}
			}
		}
	}

	return nullptr;

	//	return std::malloc(_length);
}

//fix bbs
SegmentHeader *MyAllocator::fib_split(SegmentHeader *to_split, int fib_index, int size)
{
	to_split->is_free = false;
	int size_left = fib(fib_index - 2);
	int size_right = fib(fib_index - 1);
	SegmentHeader *left = to_split;
	left->length = size_left * block_size;
	SegmentHeader *right = new ((char *)to_split + left->length) SegmentHeader(size_right * block_size, true);
	right->buddy_type = 'R';
	left->buddy_type = 'L';
	right->inherit = to_split->inherit;
	left->inherit = to_split->buddy_type;
	fibAdd(left, fib_index - 2);
	fibAdd(right, fib_index - 1);
	return nullptr;
}

void MyAllocator::fibAdd(SegmentHeader *_segment, int index)
{
	if (index >= 0)
	{
		FreeList *list = &fList_vector[index];
		_segment->is_free = true;
		if (list->head == NULL)
		{
			list->head = _segment;
			list->length += _segment->length;
		}
		else
		{

			list->length += _segment->length;
			list->head->prev = _segment;
			_segment->next = list->head;
			list->head = _segment;
		}
	}
}

void MyAllocator::fibRemove(SegmentHeader *_segment, int index)
{
	{
		FreeList *list = &fList_vector[index];

		if (list->head == _segment)
		{
			list->head = _segment->next;
			if (list->head != NULL)
			{
				list->head->prev = NULL;
			}
		}
		else
		{
			if (_segment->next != NULL)
			{
				_segment->next->prev = _segment->prev;
			}

			if (_segment->prev != NULL)
			{
				_segment->prev->next = _segment->next;
			}
		}
		list->length -= _segment->length;

		_segment->next = _segment->prev = nullptr;
	}
}
// 1 1 2 3 5 8 13 21 34 55 89 144 233 377 610
bool MyAllocator::Free(Addr _a)
{
	cout << "<------------------------------------------------------------>" << endl;
	cout << "Freeing Memory at " << _a << endl;
	SegmentHeader *to_free;
	if (_a == start_addr)
	{
		cout << "deallocating start_addr" << endl;
		to_free = ((SegmentHeader *)_a);
	}
	else
	{
		to_free = ((SegmentHeader *)_a) - 1;
	}
	int fib_counter = 2;

	while (1 == 1)
	{ //this loop is designed to calculate the size that the vector needs to be for the freelists
		int fibo = fib(fib_counter);
		if (fibo == to_free->length / block_size)
		{
			break;
		}
		else if (fib_counter >= fList_vector.size())
		{
			cout << "something is wrong" << endl;
			break;
		}
		fib_counter++;
	}

	cout << "segment length " << to_free->length << " inserted to level " << fib_counter << endl;
	fibAdd(to_free, fib_counter);
	available_memory += to_free->length;
	if (start_addr != _a)
	{
		coalesce(to_free, fib_counter);
	}
	cout << "Free done" << endl
		 << endl;

	//std::free(_a);
	return true;
}

void MyAllocator::coalesce(SegmentHeader *to_join, int start_index)
{
	//left buddy takes parent LR as inherit, right buddy takes parent inherit as inherit

	switch (to_join->buddy_type)
	{
	case ('R'):

		if (fList_vector[start_index - 1].head != NULL)
		{
			SegmentHeader *iter = fList_vector[start_index - 1].head;
			while (1 == 1)
			{
				if (iter->is_free && iter->buddy_type == 'L' && to_join == (iter + sizeof(SegmentHeader)))
				{
					cout << "Buddy Found" << endl;
					cout << "Joining segments " << iter << ", " << to_join << endl;
					SegmentHeader *joined = new (iter) SegmentHeader(to_join->length + iter->length, true);
					cout << "New segment length: " << joined->length << endl;
					joined->inherit = to_join->inherit;
					joined->buddy_type = iter->buddy_type;
					iter->is_free = to_join->is_free = false;
					joined->is_free = true;
					fibRemove(iter, start_index - 1);
					fibRemove(to_join, start_index);
					fibAdd(joined, start_index + 1);
					coalesce(joined, start_index + 1);
					break;
				}
				else if (iter->next != NULL)
				{
					iter = iter->next;
				}
				else
				{
					cout << "Buddy not found :(" << endl;
					break;
				}
			}
		}
		break;
	case ('L'):

		if (fList_vector[start_index + 1].head != NULL)
		{

			SegmentHeader *iter = fList_vector[start_index + 1].head;
			while (1 == 1)
			{
				if (iter->is_free && iter->buddy_type == 'R' && to_join == (iter - sizeof(SegmentHeader)))
				{
					cout << "Buddy Found" << endl;
					cout << "Joining segments " << iter << ", " << to_join << endl;
					SegmentHeader *joined = new ((char *)to_join) SegmentHeader(to_join->length + iter->length, true);
					cout << "New segment length: " << joined->length << endl;
					joined->inherit = iter->inherit;
					joined->buddy_type = to_join->inherit;
					iter->is_free = to_join->is_free = false;
					joined->is_free = true;
					fibRemove(iter, start_index + 1);
					fibRemove(to_join, start_index);
					fibAdd(joined, start_index + 2);

					coalesce(joined, start_index + 2);
					break;
				}
				else if (iter->next != NULL)
				{
					iter++;
				}

				else
				{
					break;
				}
			}
		}
		break;
	}
}

int MyAllocator::round_up_to_fibonacci(int unrounded, int block_size)
{ //used when rounding up to nearest fibonacci multiple in Malloc
	int n = 1;
	int result;
	while (1 == 1)
	{
		if (unrounded == fib(n) * block_size)
		{
			return unrounded;
		}
		else if (unrounded > fib(n - 1) * block_size && unrounded < fib(n) * block_size)
		{
			result = fib(n) * block_size;
			return result;
		}
		n++;
	}
}
int MyAllocator::round_down_to_fibonacci(int unrounded, size_t block_size)
{ //used for allocating the right amount of memory in my_allocator constructor
	int n = 1;
	int result;
	while (1 == 1)
	{
		if (unrounded == fib(n) * block_size)
		{
			return unrounded;
		}
		else if (unrounded > fib(n - 1) * block_size && unrounded < fib(n) * block_size)
		{
			result = fib(n - 1) * block_size;
			return result;
		}
		n++;
	}
}

int fib(int n)
{
	if (n <= 1)
		return n;
	return fib(n - 1) + fib(n - 2);
}
