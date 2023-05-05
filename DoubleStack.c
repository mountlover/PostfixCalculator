/*
 * DoubleStack - a stack of Doubles, great for a calculator
 * implemented in an array, written "java style"
 * Version: 2.09.2009
 * Author: Yama H
 */
#include "DoubleStack.h"
#include <stdlib.h>
/*
 * Saves contents of DoubleStack into a preallocated array and returns the
 * number of elements copied.
 */
int DoubleStack_save(double* array)
{
	int i = 0;
	while(i < DoubleStack_index)
	{
		array[i] = DoubleStack_values[i];
		i++;
	}
	return i;
}
/*
 * Loads an array of elements doubles into the DoubleStack.
 */
void DoubleStack_load(double* array, int elements)
{
	elements += DoubleStack_index;
	while(DoubleStack_index < elements)
	{
		DoubleStack_values[DoubleStack_index] = array[DoubleStack_index];
		DoubleStack_index++;
	}
}
/*
 * Initializes values of the DoubleStack
 */
void DoubleStack_init()
{
	DoubleStack_index = 0;
	DoubleStack_overflow = 0;
	DoubleStack_underflow = 0;
	DoubleStack_values = (double*)realloc(DoubleStack_values,
			DOUBLESTACK_SIZE * sizeof(double));
}
/*
 * Pushes a value onto the DoubleStack
 */
void DoubleStack_push(double val)
{
	DoubleStack_underflow = 0;
	if(DoubleStack_index < DOUBLESTACK_SIZE)
		DoubleStack_values[DoubleStack_index++] = val;
	else
		DoubleStack_overflow = 1;
}
/*
 * Pops a value from the DoubleStack
 */
double DoubleStack_pop()
{
	DoubleStack_overflow = 0;
	if(DoubleStack_index == 0)
	{
		DoubleStack_underflow = 1;
		return 0;
	}
	return DoubleStack_values[--DoubleStack_index];
}
/*
 * Reads the value on the top of the DoubleStack
 */
double DoubleStack_peek()
{
	if(DoubleStack_index == 0)
	{
		return 0;
	}
	return DoubleStack_values[DoubleStack_index-1];
}
