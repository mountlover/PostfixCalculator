Programmable Postfix Calculator v3.5
by Yama Habib

Usage: To use, run the executable, and at the prompt, either enter a postfix calculation using any of the available operations (listed below), or type "func" to enter function programming mode. The prompt will appear as follows:

Input function letter (i.e. F for F(x)): 

Input any letter (A-Z) in uppercase to label the function (note that P and E are by default programmed to the functions P(x) = pi*x and E(x) = e^x, respectively. After entering a label, the following prompt will appear:

Input number of variables (1-3): 

Input the number of dependent variables (i.e. the equation for a two dimensional parabola has one dependent variable, F(x) = x^2). The variables used will be x, y, and z. After choosing the number of variables, the equation should be entered in postfix notation (i.e. Positive Quadratic Formula: Q(x,y,z) = -y y 2 p 4 x * z * - 2 r + 2 x * /).

To call the function, simply reference it in the middle of a calculation as shown:

>> -1 Q(1,5,7) *

One can also graph a defined (single-variable) function by typing "graph" to enter graphing mode. A prompt will appear as follows:

Input function letter (i.e. F for F(x)): 

Input the function letter of the function you want to graph. After doing this, another two prompts will appear as follows:

min x value: 
max x value: 

These are the minimum and maximum labels along the x axis of the graph. The y axis will be auto-scaled to the bounds of the graph. For instance, if you want to graph a function between -100 and 100, x min would be -100 and x max would be 100.

Supported Arithmetic Operations:
Addition [+]: 
	Traditional: 1 + 2
	Postfix: 1 2 +
	Ans: 3
Subtraction [-]:
	Traditional: 3 - 2
	Postfix: 3 2 -
	Ans: 1
Multiplication [*]: 
	Traditional: 2 * 3
	Postfix: 2 3 *
	Ans: 6
Division [/]: 
	Traditional: 6 / 2
	Postfix: 6 2 /
	Ans: 3
Exponents [p]: 
	Traditional: 2^3
	Postfix: 2 3 p
	Ans: 8
Roots [r]:
	Traditional: sqrt(9)
	Postfix: 9 2 r
	Ans: 3
Scientific Notation [e]:
	Traditional: 2.1 * 10^3
	Postfix: 2.1 3 e
	Ans: 2100
Modulus (remainder) [%]:
	Traditional: 5 % 2
	Postfix: 5 2 %
	Ans: 1
Logarithms [l]:
	Traditional: log3(9), ln(1)
	Postfix: 9 3 l, 1 E(1) l
	Ans: 2, 0
Factorials [!]:
	Traditional: 5!
	Postfix: 5 !
	Ans: 120
Absolute value [|]:
	Traditional: |-1|
	Postfix: -1 |
	Ans: 1
Ceiling Function [c]:
	Traditional: ceil(1.2)
	Postfix: 1.2 c
	Ans: 2
Floor Function [f]:
	Traditional: floor(1.7)
	Postfix: 1.7 f
	Ans: 1
Previous Result [n]:
	Traditional: 1 + 2 --> n + 3
	Postfix: 1 2 + --> n 3 +
	Ans: 6
Pi [P(x) = pi*x]:
	Ex: P(1) 1 +
	Ans: 4.14159...
e [E(x) = e^x]:
	Ex: E(1) 1 +
	Ans: 3.71828...
Trigonometric Functions:
	Sine (radians): sin
		Ex: P(1) sin
		Ans: 0
	Cosine (radians): cos
		Ex: P(1) cos
		Ans: -1
	Tangent (radians): tan
		Ex: P(0.25) tan
		Ans: 1
	Arcsin (radians): asin
		Ex: 1 asin
		Ans: 1.57... (pi/2)
	Arccos (radians): acos
		Ex: -1 acos
		Ans: 3.14... (pi)
	Arctan (radians): atan
		Ex: 1 atan
		Ans: 0.785... (pi/4)
	Sine (degrees): sind
		Ex: 180 sind
		Ans: 0
	Cosine (degrees): cosd
		Ex: 180 cosd
		Ans: -1
	Tangent (degrees): tand
		Ex: 45 tand
		Ans: 1
	Arcsin (degrees): asind
		Ex: 1 asind
		Ans: 90
	Arccos (degrees): acosd
		Ex: -1 acosd
		Ans: 180
	Arctan (degrees): atand
		Ex: 1 atand
		Ans: 45

To convert a value to a fraction: type "frac" at the prompt to convert the previous value to a fraction.

Warning: Using the arrow keys will irreversably interfere with the current operation and may lead to an incorrect result.
	




