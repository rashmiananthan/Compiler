Following are the file required for compiling:
1. makefile
2. driver.c
3. parser.y
4. scanner.l
5. tree.c
6. tree.h
7. test file for testing like test.mC

Steps for compilation :
1. In the command line type 'make'. This creates the executable file called mcc.
2. type ./mcc < test.mC 
3. If the parsing is successful, the parse tree for the given program is generated and displayed, also the symbol table is displayed for the parsed program.

Things Not implemented:
1. There is a limitation in the number of children for the tree nodes and it is 30.

Test:
I have included the test program posted in the site, also have modified to include string and character constants.

/* IMPORTANT */
DANGLING IF-ELSE:
I have solved the dangling if-else problem by introducing the brackets. Even a single if statement should have {}(brackets). Following are the acceptable forms:

if(){
}

if(){}else{}

if(){
if(){
}
}else {
}

OUTPUT:
1. I am displaying all the nodes , identifiers, operators, if, else,while, return, string constants, character constants, integer values.
2. Depending on the input test file, the tree may grow to bigger sizes.
3. The symbol table is displayed at the last after the tree generation.
