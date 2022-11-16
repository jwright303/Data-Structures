/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Jackson Wright
 * Email: wrighjac@oregonstate.edu
 */

#include <stdlib.h>
#include <stdio.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq {
  struct dynarray* array;
};

struct pq_node {
  int priority;
  void* val;
};


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
  struct pq* pr_q = malloc(sizeof(struct pq));
  pr_q->array = dynarray_create();
  return pr_q;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
  dynarray_free(pq->array);
  free(pq);
  return;
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
  int size = dynarray_size(pq->array);
  if(size == 0) {
    return 1;
  } else {
    return 0;
  }
  return 1;
}

void percolate_up(struct pq* pq) {
  int kid_in = dynarray_size(pq->array) - 1; //index of the last element
  int parent_in = (kid_in-1) / 2; //index of the aboves parent
  struct pq_node* kid = dynarray_get(pq->array, kid_in); //gets the node at the kid index
  struct pq_node* parent = dynarray_get(pq->array, parent_in); //gets the node at the parent index 

  while(kid->priority < parent->priority) {
    //switch the two values
    dynarray_set(pq->array, parent_in, kid);
    dynarray_set(pq->array, kid_in, parent);
    kid_in = parent_in;
    parent_in = (kid_in-1) / 2;
    kid = dynarray_get(pq->array, kid_in);
    parent = dynarray_get(pq->array, parent_in);
    if(kid_in == 0) {
      break;
    }

  }
  return;
}

/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
  struct pq_node* new_val = malloc(sizeof(struct pq_node));
  new_val->priority = priority;
  new_val->val = value;
  dynarray_insert(pq->array, new_val);
  
  if(dynarray_size(pq->array) > 1) {
    percolate_up(pq);
  }

  return;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
  struct pq_node* node = dynarray_get(pq->array, 0);
  void* val = node->val;
  return val;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
  //struct pq_node* node = pq_first(pq);
  struct pq_node* node = dynarray_get(pq->array, 0);
  int first_p = node->priority;
  return first_p;
}

void percolate_down(struct pq* pq) {
  int parent_in = 0;
  int left_in = 1; 
  int right_in = 2;
  int lesser_in = 0;
  int in_cap = dynarray_size(pq->array)-1;
  struct pq_node* parent = dynarray_get(pq->array, parent_in);
  struct pq_node* l_kid;
  struct pq_node* r_kid;
  struct pq_node* lesser;
  
  if(left_in > in_cap) {
    return;
  } else if(right_in > in_cap) {
    lesser = dynarray_get(pq->array, left_in);
    lesser_in = left_in;
  } else {
    l_kid = dynarray_get(pq->array, left_in);
    r_kid = dynarray_get(pq->array, right_in);
    if(l_kid->priority < r_kid->priority) {
      lesser = l_kid;
      lesser_in = left_in;
    } else {
      lesser = r_kid;
      lesser_in = right_in;
    }
  }

  while(lesser->priority < parent->priority) {
    dynarray_set(pq->array, parent_in, lesser);
    dynarray_set(pq->array, lesser_in, parent);
    parent_in = lesser_in;
    left_in = (2 * parent_in) + 1;
    right_in = (2 * parent_in) + 2;
    parent = dynarray_get(pq->array, parent_in);
    if(left_in > in_cap) {
      return;
    } else if(right_in > in_cap) {
      lesser = dynarray_get(pq->array, left_in);
      lesser_in = left_in;
    } else {
      l_kid = dynarray_get(pq->array, left_in);
      r_kid = dynarray_get(pq->array, right_in);
      if(l_kid->priority < r_kid->priority) {
        lesser = l_kid;
        lesser_in = left_in;
      } else {
        lesser = r_kid;
        lesser_in = right_in;
      }
    }
  }
  return;
}

/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
  struct pq_node* first = dynarray_get(pq->array, 0);
  void* val = first->val;
  int last_in = dynarray_size(pq->array) - 1;
  struct pq_node* last = dynarray_get(pq->array, last_in);
  dynarray_set(pq->array, 0, last);
  dynarray_remove(pq->array, last_in);
  if(last_in > 0) {
    percolate_down(pq);
  }
  free(first);
  return val;
}
