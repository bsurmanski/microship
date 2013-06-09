/**
 * vl_array.h
 * microship
 * August 17 2011
 * Brandon Surmanski
 */

#ifndef _VL_ARRAY_H
#define _VL_ARRAY_H

#include "string.h"

/*structs*/
typedef struct vl_array vl_array;

/*functions*/
void add_element(vl_array *array, void* element);

void remove_element(vl_array *array, void* element);

void* get_element(vl_array *array, int index);

size_t vl_array_length(vl_array *array);

vl_array *new_array(size_t element_size);

void delete_array(vl_array *array);

void copy_array(vl_array *array, void * buf);

#endif
