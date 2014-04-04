Following are the file required for compiling:
1. makefile
2. driver.c
3. tokendef.h
4. scanner.l

Steps for compilation :
1. In the command line type 'make'. This creates the executable file called scanner.
2. After successful completion of make, type ./scanner < Input.mC (Here Input.mC is your test file which is given as an input file.)

The output display the <token, value> pairs for the recognised input and generates error message for various error cases.

ASSUMPTIONS:
1. I have assumed that a string can have a maximum number of characters to be 1024

I have tested in various possible cases and have given below the set of inputs and the corresponding output generated:

1. IDENTIFIERS
	a.  
		input - ififif
		output - <ififif , IDENTIFIER>
	b. 
		input - 9name
		output - <9 , INTCONST>  <name , IDENTIFIER>
           (Here 9 is considered as an integer while name is considered as valid identifier)
	c. 
		input - name@
		output - <name , IDENTIFIER>
			<@ , ILLEGAL TOKEN at Line:1 Column:4   >
	   (@ is considered an illegal token)

2. Reserved words
	a. 
		input - else
		output - <ELSE , KEYWORD>
	b. 
		input - ELse
		output - <ELse , IDENTIFIER>
	   (Though ELse could be a keyword, mC is case sensitive, hence it categorizes it as an identifier)
	
3. Integer constants
	a. 
		input - 4567889
		output - <4567889 , INTCONST>
	b. 
		input - -8765
		output - <SUB , OPERATOR>        <8765 , INTCONST>   [Here - is considered as a valid operator, and so 8765 is a valid integer constant]
	c. 
		input - 00004567
		output -  <0 , INTCONST>  <0 , INTCONST>  <0 , INTCONST>  <0 , INTCONST>  <0 , INTCONST>  <4567 , INTCONST> [Here 0 is a valid integer constant] 

4. Character Constant
	a.
		input - 'a'
		output - <a , CHARCONST>
	b.
		input - 'f
		output - Improper character constant termination at Line:2 Column:3 [reports the line and column where the \' is missing]
	c. 
		input - ''
		output - Improper character constant termination at Line:2 Column:3
	d.
		input - ' '
		output - <  , CHARCONST>

5. String constants
	a.
		input - "america"
		output - <america , STRCONST>
	b.
		input - "russia\toylmpics"
		output -  <russia olympics , STRCONST>  [captures \t]
	c.
		input - "sochi\nrussia"
		output -  <sochi 
russia , STRCONST>  [captures \n and prints russia in next line]
	d. 
		input - "america\\gold"
		output - <america\gold , STRCONST>   [captures \\]
	e.
		input - "russia\"gold\"will win"
		output - <russia"gold"will win , STRCONST> [captures \"]
	f. 
		input - "gold in   [this input is illegal as it crosses line boundaries]
olympics"  
		output - Illegal String termination at Line:6 Columnn:10

			<olympics , IDENTIFIER>  [recognizes olympics as a valid identifier]

			Illegal String termination at Line:7 Columnn:10 [the \" is considred a start for new string and as no terminating \" is found, error is reported]

6. Comments
	a.
		input - /* this
is * a valid ** comments /*****/
		output - NONE [because this is a valid comment] 

	b .
		input - /* this
is * a valid ** comments /
		output - Unterminated comment at Line:9 [reports the starting line number]


