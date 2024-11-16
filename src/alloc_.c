#define _DEFAULT_SOURCE

#include "../include/alloc.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define HEADER_SIZE (sizeof(struct header))

void *baseptr = NULL;
int algorithm = 0;
int heapsize = 0;
struct header *head = NULL;
struct header *freeblock = NULL;
struct allocinfo infostat;

struct header *firstfit(int size) 
{
  struct header *current = freeblock ;
  struct header *temp = freeblock;

  // Find the first free node with enough size
  while (current != NULL) {
    if (current->size >= size) {
      break;
    }
    current = current->next;
  }

  // Take out the free node (case when the free node is at the start of freeblock linked list)
  if (current == freeblock)
  {
    freeblock = freeblock->next;
    return current;
  }
  // Take out free node (case when the free node is in between some nodes in linked list)
  while (temp != NULL)
  {
    if (temp->next == current)
    {
      temp->next = temp->next->next;
      break;
    }
    temp = temp->next;
  }
  return current;
}

struct header *worstfit(int size) 
{
  struct header *current = freeblock;
  struct header *worst = NULL;

  // Find the free node with largest space
  while (current != NULL) {
    if (worst == NULL)
    {
      if (current->size >= size)
        worst = current;
    }
    else if (current->size >= size && current->size > worst->size) {
      worst = current;
    }
    current = current->next;
  }

  // Take out the free node (case when the free node is at the start of freeblock linked list)
  if (worst == freeblock)
  {
    freeblock = freeblock->next;
    return worst;
  }
  current = freeblock;

  // Take out free node (case when the free node is in between some nodes in linked list)
  while (current != NULL)
  {
    if (current->next == worst)
    {
      current->next = current->next->next;
      break;
    }
    current = current->next;
  }
  return worst;
}

struct header *bestfit(int size) 
{
  struct header *current = freeblock;
  struct header *best = NULL;

  // Find the free node with closest space amount to size
  while (current != NULL) {
    if (best == NULL)
    {
      if (current->size >= size)
      {
        best = current;
      }
    }
    else if (current->size >= size && (current->size - size) >= 0 && 
        (current->size - size) > (best->size - size)) {
      best = current;
    }
    current = current->next;
  }

  // Take out the free node (case when the free node is at the start of freeblock linked list)
  if (best == freeblock)
  {
    freeblock = freeblock->next;
    return best;
  }
  
  // Take out free node (case when the free node is in between some nodes in linked list)
  current = freeblock;
  while (current != NULL)
  {
    if (current->next == best)
    {
      current->next = current->next->next;
      break;
    }
    current = current->next;
  }
  return best;
}

struct header *extend(struct header *tail, int size) 
{
  // Extends the heap size when alloc is first called or heap size is running out of space
  struct header *block = sbrk(0);
  void *extendsize = sbrk(INCREMENT);
  heapsize = heapsize + INCREMENT - HEADER_SIZE;

  if (extendsize == (void *)-1)
    return NULL;

  block->size = HEADER_SIZE + size;
  block->next = NULL;

  return block;
}

void *alloc(int size) {

  struct header *block = NULL;
  struct header *freeptr = NULL;
  
  // The alloc is called for the first time
  if (head == NULL)
  {
    // Extends the heap size 
    block = extend(NULL, size);
    head = block; 
  }

  // When heap size is 0 or below 0, extends the heap size
  if (heapsize <= 0)
  {
    write(STDOUT_FILENO, "not enough space\n", strlen("not enough space\n"));
    block = extend(NULL, size);
  }

  // When allocopt for fit option is first fit
  if (algorithm == 0 && freeblock != NULL)
  {
    freeptr = firstfit(size);
    if (freeptr != NULL)
      return freeptr;
  }
  // When allocopt for fit option is best fit
  if (algorithm == 1 && freeblock != NULL)
  {
    freeptr = bestfit(size);
    if (freeptr != NULL) 
      return freeptr;
  }
  // When allocopt for fit option is worst fit
  if (algorithm == 2 && freeblock != NULL)
  {
    freeptr = worstfit(size);
    if (freeptr != NULL)
      return freeptr;
  }

  // Updates the heap size
  int totalsize = size + HEADER_SIZE;
  heapsize -= (totalsize);

  // Case when heap size is 0 just after alloc is called 
  if (heapsize < 0)
  {
    heapsize = 0;
    return head + 1;
  }
  
  // Case when heap size is below a header size and above or equal to 0.
  // Adds the remaining heap size to newely allocated memory address
  if (heapsize <= HEADER_SIZE && heapsize >= 0) 
  {
    totalsize += HEADER_SIZE + heapsize;
  }
  
  // New address for head
  void *ptr = (void *)head + totalsize;
  
  // Get the head address and update the head.
  block = head;
  head = ptr;
  block->size = totalsize;
  block->next = NULL;
  
  // Returns the memory space excluding its header space
  return block + 1; 
  
}

void dealloc(void *pointer) 
{

  // Adds memory space of pointer back to heap size
  heapsize = heapsize + *(uint64_t *)((char *)pointer - HEADER_SIZE);
  if (heapsize > INCREMENT - HEADER_SIZE) heapsize -= HEADER_SIZE;

  ((struct header *)(pointer))->next = head;
  head = ((struct header *)(pointer));
  head->size = *(uint64_t *)((char *)pointer - HEADER_SIZE);
}

void allocopt(enum algs opt, int s) {
  
  // Reset the head node, heap size, and free node linked list and set the fit option
  head = NULL;
  freeblock = NULL;
  heapsize = 0;
}

struct allocinfo allocinfo() {

  // return the heap size
  infostat.free_size = heapsize;
  return infostat;
}
