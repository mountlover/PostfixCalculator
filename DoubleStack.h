/**
 * Interface for a DoubleStack
 */
#define DOUBLESTACK_SIZE 64	// Max size of stack

// Array to contain values
double* DoubleStack_values;
// index of stack array
int DoubleStack_index;
// overflow boolean
int DoubleStack_overflow;
// underflow boolean
int DoubleStack_underflow;

/*
 * Saves contents of DoubleStack into a preallocated array and returns the
 * number of elements copied.
 */
int DoubleStack_save(double* array);
/*
 * Loads an array of elements doubles into the DoubleStack.
 */
void DoubleStack_load(double* array, int elements);
/*
 * Initializes values of the DoubleStack.
 */
void DoubleStack_init();
/*
 * Pushes a value onto the stack.
 */
void DoubleStack_push(double value);
/*
 * Pops a value off of the stack.
 */
double DoubleStack_pop();
/*
 * Returns the value on top of the stack.
 */
double DoubleStack_peek();
