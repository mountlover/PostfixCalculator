/*
 * Calculator - A postfix calculator that uses a Stack of Doubles for it's operations
 * Version: 3.5
 * Revision: 4.02.2011
 * Author: Yama H
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "DoubleStack.h"
#define INPUT_MAX_LEN 618	// Maximum length of input string
#define MAX_NUM_ACC 308		// Maximum precision of calculations (in digits)
#define GRAPH_X_UNITS 70	// Number of horizontal ascii units to use for graphing
#define GRAPH_Y_UNITS 20	// Number of vertical ascii units to use for graphing
/*
 * Non-faulty gets method.
 * string - string to recieve as input
 */
char* getstr(char* string)
{
	char* returnchar;
	do
	{
		returnchar = gets(string);
	}
	while(string[0] == 10 || string[0] == 0);
	return returnchar;
}
/*
 * Non-faulty scan method for chars.
 * c - character to scan as input
 */
int scanchar(char* c)
{
	int returnint;
	do
	{
		returnint = scanf("%c", c);
	}
	while(*c == 10 || *c == 0);
	return returnint;
}
/*
 * Function data type - stores all information needed for saving a function
 */
typedef struct
{
	char funcLet;
	char* expression;
	int vars;
}Function;
double evaluate(char input[INPUT_MAX_LEN], double prevResult, Function* functions);
/*
 * Checks function parameters for embedded function calls.
 * variable - current function parameter, in string format
 * functions - array of all functions
 * index - position of variable to begin checking at
 */
void checkRecursion(char* variable, Function* functions, int index)
{
	int param = 1;
	if(isalpha(variable[index]) && isupper(variable[index]))
	{
		while(param < functions[variable[0]-65].vars)
		{
			index += 2;
			checkRecursion(variable, functions, index);
			while(variable[index] != '\0') index++;
			strcpy(&variable[index++], ",");
			strcpy(&variable[index], strtok(NULL, ","));
			param++;
		}
	}
}
/*
 * Formats a function call into a string to be passed to the evaluate() function.
 * Returns the result.
 * label - function label
 * args - arguments of function
 * prevResult - previous result
 * functions - array of functions
 */
double solveFunc(char label, char* args, double prevResult, Function* functions)
{
	int index = label - 65;
	char* tmpstr  = (char*)calloc(MAX_NUM_ACC, sizeof(char));
	char* expression = (char*)calloc(MAX_NUM_ACC, sizeof(char));
	strcpy(expression, functions[index].expression);
	char* x = (char*)calloc(INPUT_MAX_LEN, sizeof(char));
	char* y = (char*)calloc(INPUT_MAX_LEN, sizeof(char));
	char* z = (char*)calloc(INPUT_MAX_LEN, sizeof(char));
	int i = 0;	// counter variable
	if(functions[index].vars >= 1)
	{
		strcpy(x, strtok(args, ","));
		checkRecursion(x, functions, 0);
	}
	if(functions[index].vars >= 2)
	{
		strcpy(y, strtok(NULL, ","));
		checkRecursion(y, functions, 0);
	}
	if(functions[index].vars == 3)
	{
		strcpy(z, strtok(NULL, ","));
		checkRecursion(z, functions, 0);
	}
	while(expression[i] != '\0')
	{
		if(expression[i] == 'x')
		{
			strcpy(tmpstr, &expression[i+1]);
			strcpy(&expression[i], x);
			while(expression[i] != '\0') i++;
			strcpy(&expression[i], tmpstr);
		}
		else if(expression[i] == 'y')
		{
			strcpy(tmpstr, &expression[i+1]);
			strcpy(&expression[i], y);
			while(expression[i] != '\0') i++;
			strcpy(&expression[i], tmpstr);
		}
		else if(expression[i] == 'z')
		{
			strcpy(tmpstr, &expression[i+1]);
			strcpy(&expression[i], z);
			while(expression[i] != '\0') i++;
			strcpy(&expression[i], tmpstr);
		}
		i++;
	}
	double returnVal = evaluate(expression, prevResult, functions);
	free(tmpstr);
	free(expression);
	free(x);
	free(y);
	free(z);
	return returnVal;
}
/*
 * Parses a string with a function call at index and sends it to solveFunc(), and returns
 * the result.
 * string - input string
 * index - position of function call
 * prevResult - previous result
 * functions - array of functions
 */
double parseFuncCall(char* string, int index, double prevResult, Function* functions)
{
	char label = string[index++];
	int recursiveFuncCt = 0;
	if(string[index] != '(')
		return 0;
	index++;
	char* tmpstr = (char*)calloc(MAX_NUM_ACC, sizeof(char));
	int i = 0;
	do
	{
		if(string[index] == '(') // recursive function call
			recursiveFuncCt++;
		else if(string[index] == ')')
			recursiveFuncCt--;
		tmpstr[i++] = string[index++];
	}
	while((string[index] != ')' || recursiveFuncCt > 0) && string[index] != '\0');
	if(string[index] == '\0')
		return 0;
	tmpstr[i++] = '\0';
	double tmp = solveFunc(label, tmpstr, prevResult, functions);
	free(tmpstr);
	return tmp;
}
/*
 * Simplistic recursive factorial, decimal and negative factorials must be handled
 * outside of the function call.
 * prev - value to take factorial of
 */
double factorial(double prev)
{
	if(prev <= 0) return 1;
	return prev*factorial(prev-1);
}
/*
 * Takes in a string that represents a delimited postfix operation and returns a result.
 * note: for the 'n' operator to work, we need a previous result to be passed in as well
 * input - input string
 * prevResult - previous result
 * functions - array of functions
 */
double evaluate(char input[INPUT_MAX_LEN], double prevResult, Function* functions)
{
	double val_1;
	double val_2;
	int recursiveFuncCt = 0;
	int index = 0;
	DoubleStack_init();
	while(input[index] != '\0')	// constantly check for null character (termination of string)
	{
		while(	input[index] != 'a' && input[index] != 'c' &&
				input[index] != 's' && input[index] != 't' &&
				input[index] != '+' && input[index] != '%' &&
				input[index] != '*' && input[index] != '/' &&
				input[index] != 'p' && input[index] != 'r' &&
				input[index] != 'e' && input[index] != '-' &&
				input[index] != '.' && input[index] != 'n' &&
				input[index] != 'l' && input[index] != '\0' &&
				input[index] != '!' && input[index] != '|' &&
				input[index] != 'f' &&
				!(input[index] <= '9' && input[index] >= '0') &&
				!(isalpha(input[index]) && isupper(input[index])))
		{
			index++;	// skip all delimiters
		}
		if(input[index] == '\0') break;
		if(	input[index] != 'a' && input[index] != 'c' &&	// weed out operators (except
			input[index] != 's' && input[index] != 't' &&	// minus sign)
			input[index] != '+' && input[index] != '%' &&
			input[index] != '*' && input[index] != '/' &&
			input[index] != 'p' && input[index] != 'r' &&
			input[index] != 'e' && input[index] != 'n' &&
			input[index] != 'l' && input[index] != '!' &&
			input[index] != '|' && input[index] != 'f' &&
			!(isalpha(input[index]) && isupper(input[index])))
		{
			char* tmpstr = calloc(MAX_NUM_ACC, sizeof(char));	// String to contain numbers in char format
			int tmpstr_index = 0;		// index of aforementioned String
			int negative = 0;			// 1 if number will be negative
			// If you reach a minus, and the next character is a number (not a delimiter)
			if(	input[index] == '-' && ((input[index+1] <= '9' && input[index+1] >= '0') ||
				input[index+1] == '.'))
			{
				negative = 1;	// then the number is negative...
				index++;		// remember that and keep reading the proceeding number as usual
			}
			// While we've got a number, keep reading it
			while((input[index] <= '9' && input[index] >= '0') || input[index] == '.')
			{
				if(tmpstr_index < MAX_NUM_ACC)	// make sure we can hold this number
				{
					tmpstr[tmpstr_index++] = input[index++];
				}
				else	// we can't--print an error.
				{
					printf("Error: please use numbers with less than %d digits of precision\n",
							MAX_NUM_ACC+1);
					return 0;
				}
			}
			if(tmpstr_index != 0)			// if it isn't zero, then we know we hit a number
			{
				double number = strtod(tmpstr, NULL);	// convert to double
				if(negative)	// check for negativity
				{
					DoubleStack_push(-1*number);	// if negative, push a negative
					negative = 0;					// and reinitialize negative var
				}
				else
					DoubleStack_push(number);		// else just push the number
				if(DoubleStack_overflow)
				{
					printf("Error: stack overflow, please use less than %d numbers", DOUBLESTACK_SIZE);
					return 0;
				}
			}
			else if(input[index] != '-')	// else it IS, and we shouldn't skip subtraction.
				index++;					// increment index if we're NOT at a minus
			free(tmpstr);
		}
		// we know now that we're at some operator
		if(input[index] == 's' && input[index+1] == 'i' && input[index+2] == 'n') // sin(d)
		{
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			if(input[index+3] == 'd')
			{
				index += 3;
				val_1 /= 180;
				val_1 *= acos(-1);
			}
			else
			{
				index += 2;
			}
			if(((int)(val_1*180/acos(-1)) == (float)(val_1*180/acos(-1)) &&
					(int)(val_1*180/acos(-1)) % 180 == 0) || val_1 == 0)
				DoubleStack_push(0);
			else
				DoubleStack_push(sin(val_1));
		}
		// cos(d)
		else if(input[index] == 'c' && input[index+1] == 'o' && input[index+2] == 's')
		{
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			if(input[index+3] == 'd')
			{
				index += 3;
				val_1 /= 180;
				val_1 *= acos(-1);
			}
			else
			{
				index += 2;
			}
			if((int)(val_1*180/acos(-1)) == (float)(val_1*180/acos(-1)) &&
					(int)(val_1*180/acos(-1)+90) % 180 == 0)
				DoubleStack_push(0);
			else
				DoubleStack_push(cos(val_1));
		}
		// tan(d)
		else if(input[index] == 't' && input[index+1] == 'a' && input[index+2] == 'n')
		{
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			if(input[index+3] == 'd')
			{
				index += 3;
				val_1 /= 180;
				val_1 *= acos(-1);
			}
			else
			{
				index += 2;
			}
			DoubleStack_push(tan(val_1));
		}
		// arcsin(d)
		else if(input[index] == 'a' && input[index+1] == 's' && input[index+2] == 'i' &&
				input[index+3] == 'n')
		{
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			val_1 = asin(val_1);
			if(input[index+4] == 'd')
			{
				index += 4;
				val_1 /= acos(-1);
				val_1 *= 180;
			}
			else
			{
				index += 3;
			}
			DoubleStack_push(val_1);
		}
		// arccos(d)
		else if(input[index] == 'a' && input[index+1] == 'c' && input[index+2] == 'o' &&
				input[index+3] == 's')
		{
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			val_1 = acos(val_1);
			if(input[index+4] == 'd')
			{
				index += 4;
				val_1 /= acos(-1);
				val_1 *= 180;
			}
			else
			{
				index += 3;
			}
			DoubleStack_push(val_1);
		}
		// arctan(d)
		else if(input[index] == 'a' && input[index+1] == 't' && input[index+2] == 'a' &&
				input[index+3] == 'n')
		{
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			val_1 = atan(val_1);
			if(input[index+4] == 'd')
			{
				index += 4;
				val_1 /= acos(-1);
				val_1 *= 180;
			}
			else
			{
				index += 3;
			}
			DoubleStack_push(val_1);
		}
		else if(input[index] == '+')	// if its a plus, do addition
		{
			val_2 = DoubleStack_pop();
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			DoubleStack_push(val_1+val_2);
		}
		else if(input[index] == '-') // if it's a minus, do subtraction
		{
			val_2 = DoubleStack_pop();
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			DoubleStack_push(val_1-val_2);
		}
		else if(input[index] == '*') // if it's an asterisk, do multiplication
		{
			val_2 = DoubleStack_pop();
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			DoubleStack_push(val_1*val_2);
		}
		else if(input[index] == '/') // if it's a backslash, do division
		{
			val_2 = DoubleStack_pop();
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			DoubleStack_push(val_1/val_2);
		}
		else if(input[index] == 'p') // if it's a 'p', we've got a power
		{
			val_2 = DoubleStack_pop();
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			DoubleStack_push(pow(val_1, val_2));
		}
		else if(input[index] == 'e') // if it's an 'e', we have scientific notation
		{
			val_2 = DoubleStack_pop();
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			DoubleStack_push(val_1 * pow(10, val_2));
		}
		else if(input[index] == 'r') // if it's an 'r', we've got a root
		{
			val_2 = DoubleStack_pop();
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			DoubleStack_push(pow(val_1, 1/val_2));
		}
		else if(input[index] == '%') // if it's a percent sign, do modulus
		{
			val_2 = DoubleStack_pop();
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			DoubleStack_push(fmod(val_1, val_2));
		}
		else if(input[index] == 'n') // if it's an 'n', push the previous result
		{
			DoubleStack_push(prevResult);
		}
		else if(input[index] == 'l') // if it's an 'l', we have log base x
		{
			val_2 = DoubleStack_pop();
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			DoubleStack_push(log(val_1) / log(val_2));
		}
		else if(input[index] == '|') // if it's a '|', take the absolute value
		{
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			if(val_1 < 0) val_1 *= -1;
			DoubleStack_push(val_1);
		}
		else if(input[index] == '!') // if it's a '!', take the factorial of the result
		{
			val_1 = DoubleStack_pop();
			val_2 = 0;
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			if(val_1 < 0)
			{
				val_1 *= -1;
				val_2 = 1;
			}
			if(val_1 != round(val_1))
			{
				val_1 = factorial(val_1)*pow(acos(-1),0.5); // Gamma function
			}
			else
			{
				val_1 = factorial(val_1);
			}
			if(val_2)
			{
				val_1 *= -1;
			}
			DoubleStack_push(val_1);
		}
		else if(input[index] == 'c') // 'c' for "ceiling"
		{
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			DoubleStack_push(ceil(val_1));
		}
		else if(input[index] == 'f') // 'f' for "floor"
		{
			val_1 = DoubleStack_pop();
			if(DoubleStack_underflow)
			{
				printf("Error: stack underflow (more operations than operands)\n");
				return 0;
			}
			DoubleStack_push(floor(val_1));
		}
		else if(isalpha(input[index]) && isupper(input[index])) // check for function call
		{
			double* tmparray = (double*)calloc(DOUBLESTACK_SIZE, sizeof(double));
			val_1 = DoubleStack_save(tmparray);
			val_2 = parseFuncCall(input, index, prevResult, functions);
			DoubleStack_init();
			DoubleStack_load(tmparray, (int)val_1);
			DoubleStack_push(val_2);
			free(tmparray);
			do
			{
				index++;
				if(input[index] == '(')
					recursiveFuncCt++;
				else if(input[index] == ')')
					recursiveFuncCt--;
			}
			while(recursiveFuncCt > 0 && input[index] != '\0');
			if(input[index] == '\0' || recursiveFuncCt < 0)
				return 0;
		}
		index++;
	}
	if(DoubleStack_index != 1) // if it isn't 1, then there are still numbers on the stack
		printf("Warning: more operands than operations, calculation may not be correct.\n");
	return DoubleStack_pop();
}
/*
 * Calling this method shifts the calculator into function mode,
 * where functions can be assigned.
 * Returns error code or 0 upon successful termination.
 * functions - array of functions
 */
int funcMode(Function* functions)
{
	char tmp;
	int index;
	printf("Input function letter (i.e. F for F(x)): ");
	fflush(stdin);
	scanchar(&tmp);
	while(!isalpha(tmp))
	{
		printf("\nInvalid character, please input a letter: ");
		fflush(stdin);
		scanchar(&tmp);
	}
	if(islower(tmp))
		tmp = toupper(tmp);
	index = tmp - 65;	// Convert uppercase letter to a numeric index value
	functions[index].funcLet = tmp;
	functions[index].expression = (char*)realloc(functions[index].expression,
			INPUT_MAX_LEN * sizeof(char));
	printf("Input number of variables (1-3): ");
	fflush(stdin);
	scanf("%d", &functions[index].vars);
	while(functions[index].vars <= 0 || functions[index].vars > 3)
	{
		printf("\nInvalid option, please enter a number between 1 and 3: ");
		fflush(stdin);
		scanf("%d", &functions[index].vars);
	}
	if(functions[index].vars == 1)
		printf("Input function expression (in postfix) in terms of x: \n\t%c(x) = ",
				functions[index].funcLet);
	else if(functions[index].vars == 2)
		printf("Input function expression (in postfix) in terms of x and y: \n\t%c(x,y) = ",
				functions[index].funcLet);
	else if(functions[index].vars == 3)
		printf("Input function expression (in postfix) in terms of x, y, and z: "
				"\n\t%c(x,y,z) = ", functions[index].funcLet);
	else
		return 1;
	fflush(stdin);
	getstr(functions[index].expression);
	printf("Define another function? (y/n): ");
	fflush(stdin);
	scanchar(&tmp);
	if(tolower(tmp) == 'y')
		funcMode(functions);
	else
		printf("\n");
	return 0;
}
/*
 * Converts a decimal into a fraction, returns the higher number of digits in the fraction
 * 		if successful, 0 if unsuccessful.
 * numerator - a pointer to an integer that will contain the numerator, if the calculation
 * 		is doable
 * denominator - a pointer to an integer that will contain the denominator, if the
 * 		calculation is doable
 * X - this should be the value to attempt to convert to a fraction
 * Zprev - recursion variable, this should be the same as X initially
 * Nprev - recursion variable, this should be initialized to 0
 * Dprev - recursion variable, this should be initialized to 1
 * Dprevprev - recursion variable, this should be initialized to 0
 */
int dec2frac(int* numerator, int* denominator, double X, double Zprev, int Nprev, int Dprev,
		int Dprevprev)
{
	//printf("%g\n", Zprev - (double)((int)Zprev));
	if((float)Zprev == ((int)Zprev))
	{
		if(Dprev < 0) // numerator should take negative priority
		{
			Dprev = Dprev * -1;
			Nprev = Nprev * -1;
		}
		*numerator = Nprev;
		*denominator = Dprev;
		if(Dprev == 1) // This happens when X is an integer
		{
			*numerator = (int)X;
		}
		int digits = 0;
		while(abs(*numerator) >= pow(10,digits) || abs(*denominator) >= pow(10,digits))
		{
			digits++;
		}
		if(X < 0 && abs(*numerator) >= pow(10,digits-1)) digits++; // extra digit for '-'
		return digits;
	}
	else
	{
		double Z = 1 / (Zprev - (double)((int)Zprev));
		int D = Dprev * ((int)Z) + Dprevprev;
		int N = round(X*D);
		if((float)((double)Nprev/(double)Dprev) == (float)((double)N/(double)D))
			return 0;
		else
			return dec2frac(numerator, denominator, X, Z, N, D, Dprev);
	}
}
/*
 * Easier way to call dec2frac. Would be an overloaded method if C supported them.
 * numerator - a pointer to an integer that will contain the numerator, if the calculation
 * 		is doable
 * denominator - a pointer to an integer that will contain the denominator, if the
 * 		calculation is doable
 * value - this should be the value to attempt to convert to a fraction
 */
int convertToFraction(int* numerator, int* denominator, double value)
{
	return dec2frac(numerator, denominator, value, value, 0, 1, 0);
}
double adjustPrecision(double val, int width)
{
	int leftwidth;
	if(log10(abs(val)) < 0)
		leftwidth = 1;
	else
		leftwidth = 1+(int)log10(abs(val));
	int rightwidth = width-1-leftwidth;
	if(rightwidth <= 0) return (double)round(val);
	if(val < 0) rightwidth--;
	double rightVal = val - (double)((int)val);
	rightVal *= pow(10,rightwidth);
	rightVal = (double)round(rightVal);
	rightVal /= pow(10,rightwidth);
	val = (double)((int)val) + rightVal;
	return val;
}
void graph(Function* functions, char funcLet, double xmin, double xmax)
{
	if(functions == NULL) return;
	if(functions[funcLet-'A'].vars != 1)
	{
		printf("Error: cannot graph multivariable functions\n");
		return;
	}
	double xlabels[GRAPH_X_UNITS];
	double ylabels[GRAPH_Y_UNITS+1];
	double vals[GRAPH_X_UNITS];
	double ymin, ymax;
	char tmp[INPUT_MAX_LEN];
	double prev = 0;
	double xInc = (xmax-xmin)/(GRAPH_X_UNITS);
	int i;
	for(i=0; i<GRAPH_X_UNITS; i++)
	{
		xlabels[i] = xmin+i*xInc;
		sprintf(tmp, "%lf", xlabels[i]);
		prev = solveFunc(funcLet, tmp, prev, functions);
		vals[i] = prev;
		if(i==0)
		{
			ymin = prev;
			ymax = prev;
		}
		else
		{
			if(prev < ymin) ymin = prev;
			if(prev > ymax) ymax = prev;
		}
	}
	double yInc = (ymax-ymin)/(GRAPH_Y_UNITS);
	int j;
	printf("\n");
	for(i=GRAPH_Y_UNITS; i>0; i--)
	{
		ylabels[i] = ymin+i*yInc;
		printf("%-9g|", adjustPrecision(ylabels[i], 9));
		for(j=0; j<GRAPH_X_UNITS-1; j++)
		{
			if(vals[j] < ylabels[i] &&
			   vals[j] >= ymin+(i-1)*yInc)
			{
				if(vals[j+1] < ylabels[i] &&
				   vals[j+1] >= ymin+(i-1)*yInc)
				{
					printf("-");
				}
				else if(vals[j] < vals[j+1])
					printf("/");
				else
					printf("\\");
			}
			else printf(" ");
		}
		printf("\n");
	}
	printf  ("          ");
	for(i=0; i<GRAPH_X_UNITS-1; i++)
	{
		printf("-");
	}
	printf("\n         ");
	for(i=0; i<GRAPH_X_UNITS; i += j)
	{
		j = printf("%-4g|", adjustPrecision(xlabels[i], 4));
	}
	printf("\n");
}
void graphMode(Function* functions)
{
	char tmp;
	double xmin, xmax;
	printf("Input function letter (i.e. F for F(x)): ");
	fflush(stdin);
	scanchar(&tmp);
	while(!isalpha(tmp))
	{
		printf("\nInvalid character, please input a letter: ");
		fflush(stdin);
		scanchar(&tmp);
	}
	if(islower(tmp))
		tmp = toupper(tmp);
	printf("min x value: ");
	fflush(stdin);
	scanf("%lf", &xmin);
	printf("max x value: ");
	fflush(stdin);
	scanf("%lf", &xmax);
	graph(functions, tmp, xmin, xmax);
}
/*
 * Main Method - prompts user, provides UI
 */
int main()
{
	char* input = (char*)calloc(INPUT_MAX_LEN, sizeof(char));
	Function* functions = (Function*)calloc(26, sizeof(Function));
	int frac = 0;
	int numerator = 0;
	int denominator = 0;
	int i = 0;
	while(i <= 'Z'-65)
	{
		if(i != 'P'-65 && i != 'E'-65)
		{
			functions[i].expression = (char*)malloc(2*sizeof(char));
			strcpy(functions[i].expression, "0");
			functions[i].vars = 0;
		}
		i++;
	}
	functions['P'-65].expression = (char*)malloc(23*sizeof(char));
	strcpy(functions['P'-65].expression, "x 3.1415926535897932 *");
	functions['P'-65].vars = 1;
	functions['E'-65].expression = (char*)malloc(23*sizeof(char));
	strcpy(functions['E'-65].expression, "2.7182818284590452 x p");
	functions['E'-65].vars = 1;
	int break_op = 0;
	double result = 0;
	printf("Postfix Calculator\nInput a postfix operation below, 'exit' to end, or 'help' ");
	printf("for more information\n");
	while(!break_op)
	{
		printf(">> ");
		fflush(stdin);
		getstr(input);
		printf("\n");
		if(!strcmp("exit", input) || !strcmp("EXIT", input)|| !strcmp("Exit", input))
			break_op = 1;
		else if(!strcmp("help", input) || !strcmp("HELP", input) || !strcmp("Help", input))
		{
			puts("Postfix Calculator v3.5");
			puts("Made by: Yama Habib\n");
			puts("Supported arithmetic operations: ");
			puts("+ : add previous two numbers e.g. 1 2 + = 3");
			puts("- : subtract previous two numbers e.g. 2 1 - = 1");
			puts("* : multiply previous two numbers e.g. 1 2 * = 2");
			puts("/ : divide previous two numbers e.g. 4 2 / = 2");
			puts("p : first number ^ second number e.g. 2 3 p = 8");
			puts("r : take the [second number] root of the first number e.g. 4 2 r = 2");
			puts("e : first number x 10 ^ second number e.g. 2 1 e = 20");
			puts("% : gives the modulus (remainder of division) e.g. 5 2 % = 1");
			puts("l : log base second number of first number e.g. 9 3 l = 2");
			puts("! : gives the factorial of the previous number e.g. 5 ! = 120");
			puts("| : gives the absolute value of the previous number e.g. -1 | = 1");
			printf("c/f : gives the ceiling/floor rounding of the previous number ");
			puts("e.g. 1.7 f = 1");
			puts("n : retrieves result of previous calculation");
			puts("[FUNCTIONLETTER](variable(s)) : call a programmed function (see below)");
			puts("P(x) : retreives pi * x (P(1) returns pi)");
			puts("E(x) : retrieves e^x (E(1) returns e)");
			printf("(a)(sin/cos/tan)(d) : takes trig function, 'a' corresponds to inverse ");
			puts("trig, 'd' corresponds to decimal notation");
			puts("func : enters function programming mode");
			puts("frac : converts the previous result to a fraction");
			puts("graph : enters graphing mode");
			puts("exit : closes the calculator\n");
		}
		else if(!strcmp("Function", input) || !strcmp("function", input) ||
				!strcmp("FUNCTION", input) || !strcmp("func", input) ||
				!strcmp("FUNC", input) || !strcmp("Func", input))
		{
			funcMode(functions);
		}
		else if(!strcmp("Fraction", input) || !strcmp("fraction", input) ||
				!strcmp("FRACTION", input) || !strcmp("frac", input) ||
				!strcmp("FRAC", input) || !strcmp("Frac", input))
		{
			frac = convertToFraction(&numerator, &denominator, result);
			if(frac)
			{
				printf("Ans: \n\t%d\n\t", numerator);
				i = 0;
				while (i < frac)
				{
					printf("-");
					i++;
				}
				printf("\n\t%d\n\n", denominator);
			}
			else
				printf("Ans: %11.11lg\n\n", result);
		}
		else if(!strcmp("Graph", input) || !strcmp("graph", input) ||
				!strcmp("GRAPH", input))
		{
			graphMode(functions);
		}
		else if(strcmp("", input))
		{
			result = evaluate(input, result, functions);
			printf("Ans: %11.11lg\n\n", result);
		}
	}
	free(input);
	return 0;
}
