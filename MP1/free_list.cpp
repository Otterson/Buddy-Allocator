/* 
    File: free_list.cpp

    Author: <Austin Peterson>
            Department of Computer Science
            Texas A&M University
    Date  : <date>

    Modified: 

    This file contains the implementation of the class FreeList.


*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <iostream>
#include <cassert>
#include <vector>
#include "free_list.hpp"

/*--------------------------------------------------------------------------*/
/* NAME SPACES */
/*--------------------------------------------------------------------------*/

using namespace std;
/* I know, it's a bad habit, but this is a tiny program anyway... */

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

static const std::vector<int> fibo{1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144};

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR CLASS SegmentHeader */
/*--------------------------------------------------------------------------*/

SegmentHeader::SegmentHeader(size_t _length, bool _is_free)
{
	length = _length;
	is_free = _is_free;
	cookie = COOKIE_VALUE;
	next = NULL;
	prev = NULL;

	// You may need to initialize more members here!
}

SegmentHeader::~SegmentHeader()
{
	next = NULL;
	prev = NULL;
	// You may need to add code here.
}

SegmentHeader *FreeList::split(SegmentHeader *segment, size_t l)
{
	SegmentHeader *cseg = new ((char *)segment + l)(SegmentHeader)(segment->length - l, true);
	Add(cseg);
	segment->length = l;
	Remove(segment);
	return segment;
}

bool SegmentHeader::CheckValid()
{
	if (cookie != COOKIE_VALUE)
	{
		cout << "INVALID SEGMENT HEADER!!" << endl;
		return false;
		// You will need to check with the debugger to see how we got into this
		// predicament.
	}
	else
	{
		return true;
	}
}

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR CLASS FreeList */
/*--------------------------------------------------------------------------*/

FreeList::FreeList()
{ //normal doubly linked list.
	// You will need to add code here!
	head = NULL;
	length = 0;
}

FreeList::~FreeList()
{
	// You may need to add code here.
}

bool FreeList::Add(SegmentHeader *_segment)
{
	_segment->is_free = true;
	if (head == NULL)
	{
		head = _segment;
		length += _segment->length;
	}
	else
	{
		length += _segment->length;
		head->prev = _segment;
		_segment->next = head;
		head = _segment;
	}
	return false;
	// This implementation does nothing, other than abort.
}

bool FreeList::Remove(SegmentHeader *_segment)
{
	_segment->is_free = false;
	if (_segment->next == NULL && _segment->prev == NULL)
	{ //only segment in list
		cout << "Segment selected is the only item in the FreeList" << endl;
	}
	else if (_segment->prev == NULL)
	{ //_segment==head
		cout << "f1" << endl;
		length -= _segment->length;
		_segment->next->prev = NULL;
		head = _segment->next;
	}
	else if (_segment->next == NULL)
	{ //segment==tail
		cout << "f2" << endl;
		length -= _segment->length;
		_segment->prev->next = NULL;
		_segment->prev = NULL;
	}
	else
	{ //removing from middle of list
		cout << "f3" << endl;
		if (_segment->next != NULL)
			_segment->prev->next = _segment->next;
		cout << "a1" << endl;
		if (_segment->prev != NULL)
			_segment->next->prev = _segment->prev;
		cout << "a2" << endl;
		length -= _segment->length;
		cout << "a3" << endl;
		_segment->next = NULL;
		cout << "a4" << endl;
		_segment->prev = NULL;
		cout << "a5" << endl;
		cout << "end f3" << endl;
	}
	return false; // This implementation does nothing, other than abort.
}
