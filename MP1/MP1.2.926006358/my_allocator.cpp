/* 
    File: my_allocator.cpp

    Author: Austin Peterson
            Department of Computer Science
            Texas A&M University
    Date  : <date>

    Modified: 

    This file contains the implementation of the class MyAllocator.
	<---------NOTE---------> Still a work in progress<-------------->
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cstdlib>
#include "my_allocator.hpp"
#include <assert.h>
#include <iostream>

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

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR CLASS MyAllocator */
/*--------------------------------------------------------------------------*/

MyAllocator::MyAllocator(size_t _basic_block_size, size_t _size) {
  // We don't do anything yet...
	start_addr = (char*)std::malloc(_size);
	available_memory = _size;
	memory_length = _size;
	block_size = _basic_block_size;
	SegmentHeader* fh = new (start_addr)SegmentHeader(_size, true); //check second argument --- new start_addr
	
	flist.Add(fh);
}
MyAllocator::~MyAllocator() {
  // Same here...
	Free(start_addr);
}

Addr MyAllocator::Malloc(size_t _length) {
  // This empty implementation just uses C standard library malloc 
  cout<<"<------------------------------------------------------------>"<<endl;
    SegmentHeader* iter = flist.head;
	size_t reqL = _length + sizeof(SegmentHeader); 
	cout<<endl<<"Memory Necessary: "<<reqL<<endl<<"Memory Available: "<<available_memory<<endl<<endl;
	if (reqL > available_memory) {
		cout << "Not enough available memory" << endl;
		return NULL;
	}
	else{
		cout<<"Sufficient Memory Available"<<endl;
	}
	iter = flist.head;
	cout<<"Current iter: "<<iter<<endl;
	while(1==1){
		cout<<"Current iter length: "<<iter->length<<endl;
		if(iter->length>=reqL && iter->is_free==true){
			cout<<"Suitable segment found @ "<<iter<<endl<<"Segment Length: "<<iter->length<<endl<<endl;
			break;
		}
		else{
			cout << "Checking next SH"<<endl;
			iter = iter->next;
		}
	}
	available_memory-=reqL;
	SegmentHeader* to_return=flist.split(iter, reqL);	
	
	return (char*)to_return+sizeof(SegmentHeader);
	 //	return std::malloc(_length);
}

bool MyAllocator::Free(Addr _a) {
  // This empty implementation just uses C standard library free
    cout<<"<------------------------------------------------------------>"<<endl;

        cout<<"Freeing Memory at "<<_a<<endl;
		SegmentHeader* to_free = new ((SegmentHeader*)_a-1)(SegmentHeader)(memory_length-available_memory, true);
		flist.Add(to_free);
		cout<<"Free done"<<endl;
		return true;
	
 // std::free(_a);

}

