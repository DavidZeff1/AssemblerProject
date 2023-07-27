#include "macroParser.h"

void macroParser(int argc,char **argv){

	FILE *fp,*fd;	/*to read from the initial input and write into a new filewith the macros parsed out*/
	char str[maxLineLength] = {0};/*this will hold every string from the txt file with (',' )(' ') ('\n') used as separators*/
	char c;/*hold every char from the txt file*/
	int i;/*used for 'for' loops*/
	macronode *head = NULL;
	macronode *temp= NULL;
	int foundMacro = 0;/*flag*/
	int strLen = 0;
	int countLine = 0;
	fp = fopen("unParsedProg","r+");
	fd = fopen("Prog.as","w+");
	
	
	while(!feof(fp)){
		c = fgetc(fp);
		countLine++;
		SKIP_COMMENT/*see macroParser.h for info*/
		CLOSE_EMPTYLINE
		if (c != ' ' && c != ',' && c != '\n') {	/* check that char is not a seperator*/
   			 str[strLen++] = c;	
		}else if (strLen > 0) {	/*if seperator and we have a word in str than check it to see if its a macro*/
				
				if(!strcmp("mcro",str)){/*if macro turn on flag*/
					foundMacro++;
					RESET_STRING 
					continue;
				}
				
				if(foundMacro == 1){/* second word after macro is its name so set macro array index name to macro name*/
					head = addNode(head,str,"name");
					RESET_STRING 
					foundMacro++;
					continue;
				}
				if(!strcmp("endmcro",str)){	/*reached end of macro, reset string and continue parse*/
					foundMacro=0;
					RESET_STRING 
					continue;
				}
				if(foundMacro>1){		/*when saving the string of macro we must add the seperator char to insure proper fprinting*/
					str[strLen++] = c;
					str[strLen] = '\0';
					head = addNode(head,str,"contents");
					RESET_STRING 
					continue;
				}
				
				temp = findNode(head,str);/*check every word if its a macro name, if it is then parse its contents to file*/
				if(temp){
						
						fprintf(fd,"%s",temp->contents);
						RESET_STRING
						temp = NULL;
						continue;
				}
				
						/*if not a macro fprint*/
				
				fprintf(fd,"%s%c",str,c);
				
				RESET_STRING 
    			}else{
				
				fprintf(fd,"%c",c);
			}
			
			

		
		
		
		}
		
/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
        freeList(head);
	fclose(fp);
	fclose(fd);



	
		
}












	
