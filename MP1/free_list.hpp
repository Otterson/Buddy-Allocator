/* 
    File: free_list.hpp

    Author: Austin Peterson
            Department of Computer Science and Engineering
            Texas A&M University
    Date  : <date>

    Modified:
  	<---------NOTE---------> Still a work in progress<-------------->

*/

#ifndef _free_list_hpp_ // include file only once
#define _free_list_hpp_
#include <cassert>

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cstdlib>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

class SegmentHeader
{
public:
  SegmentHeader *next;
  SegmentHeader *prev;
  size_t length;
  bool is_free;
  char buddy_type;
  char inherit;

private:
  static const unsigned COOKIE_VALUE = 0xBAAB00;
  unsigned int cookie; /* To check whether this is a genuine header! */

  // You will need additional data here!

public:
  SegmentHeader(size_t _length, bool _is_free = true);

  ~SegmentHeader();
  /* We probably won't need the destructor. */

  bool CheckValid();
  /* Check if the cookie is valid. */
};

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS  FreeList */
/*--------------------------------------------------------------------------*/

class FreeList
{

private:
  /* Here you add whatever private members you need...*/

public:
  SegmentHeader *head;
  SegmentHeader *tail;
  size_t length;
  FreeList();
  /* This function initializes a new free-list. */

  ~FreeList();
  /* We probably don't need a destructor. */

  bool Remove(SegmentHeader *_segment);
  /* Remove the given segment from the given free list. 
     Returns true if the function succeeds.
  */

  bool Add(SegmentHeader *_segment);
  /* Add the segment to the given free list. */
  SegmentHeader *split(SegmentHeader *segment, size_t l);
};

#endif
