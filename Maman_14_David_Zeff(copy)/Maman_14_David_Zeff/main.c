#include "main.h"

int main(int argc,char **argv){

	
	macroParser(argc, argv);/*function to parse out macros, see macroParser.c for more info*/
	firstRun(argc, argv);
	
	return 0;
}

