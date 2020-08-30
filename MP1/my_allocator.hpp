/* 
    File: my_allocator.hpp

    Author: Austin Peterson
            Department of Computer Science and Engineering
            Texas A&M University
    Date  : <date>

    Modified:
	

*/

#ifndef _my_allocator_hpp_                   // include file only once
#define _my_allocator_hpp_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cstdlib>
#include<vector>
#include "free_list.hpp"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

typedef void * Addr; 

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS   MyAllocator */
/*--------------------------------------------------------------------------*/
int fib(int n);

class MyAllocator {

 private:
	 SegmentHeader* start_addr;
    std::vector<SegmentHeader*> used_memory;
	 int available_memory;
	 int block_size;
	 int memory_length;
    
	 FreeList flist;

  // YOU MAY NEED TO ADD YOUR OWN DATA STRUCTURES HERE.
  
 public:
  MyAllocator(size_t _basic_block_size, size_t _size); 
  void coalesce(SegmentHeader* to_join, int start_index);
  std::vector<FreeList> fList_vector;
  int round_down_to_fibonacci(int unrounded, size_t block_size);
int round_up_to_fibonacci(int unrounded, int block_size);
  /* This function initializes the memory allocator and makes a portion of 
     ’_size’ bytes available. The allocator uses a ’_basic_block_size’ as 
     its minimal unit of allocation. 
     NOTE: In the old days one used 'unsigned int' for the size. The C
     Standards Committee has added the 'size_t' data type to ensure to 
     represent memory sizes. This makes the code more portable. Depending
     on the architecture, 'size_t' may or may not be the same 
     as 'unsigned int'. 
  */ 

  ~MyAllocator(); 
  /* This function returns any allocated memory to the operating system. 
  */ 

  Addr Malloc(size_t _length); 
  /* Allocate _length number of bytes of free memory and returns the 
     address of the allocated portion. Returns NULL when out of memory. 
  */ 
 SegmentHeader* fib_split(SegmentHeader* to_split, int fib_index, int size);
 void fibAdd(SegmentHeader* _segment, int index);
 void fibRemove(SegmentHeader* _segment, int index);

  bool Free(Addr _a); 
  /* Frees the section of physical memory previously allocated 
     using ’Malloc’. Returns true if everything ok. */ 
};

#endif 
