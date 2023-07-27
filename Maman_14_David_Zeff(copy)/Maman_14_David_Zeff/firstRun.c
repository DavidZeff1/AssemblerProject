#include "firstRun.h"

void printInBinary(unsigned int,FILE*);/* converts binary commands into base 64 aand print to output file*/
void convertToMachineCode(varnode*,instructionNode*,FILE*);/*check for errors in commands and sends commands to  'convertToBinary' to be converted to bin*/
void convertToBinary(int ,int ,int ,int ,char* ,char*,FILE*);/*converts commands to bin and then send to'printtobin' which will convert to 64 and print */
int ERROR = 0;
void firstRun(int argc, char** argv) {/*divide each line into a command line struct to be dealt with more easily*/

	FILE* ft;
	FILE* fp;
	FILE* fc;
	FILE* fk;	/*to read from the initial input and write into a new filewith the macros parsed out*/
	char str[maxLineLength] = { 0 };/*this will hold every string from the txt file with (',' )(' ')('\n') used as separators*/
	char c;/*hold every char from the txt file*/
	int i;/*used for 'for' loops*/
	instructionNode *headInstruction = NULL;
	int numOfInstructions = 0;/*flags and counters*/
	int numOfData = 0;
	int foundInstruction = 0;
	int foundInstruction2 = 0;
	int foundData = 0;
	int foundString = 0;
	int foundExtern = 0;
	int foundEntry = 0;
	int foundInitial = 0;
	varnode *head = NULL;
	int dataCount = 0;
	int wordCounterByLine = 0;
	int strLen = 0;
	int countLine = 0;
	instruction *currentInstruction = (instruction*)malloc(sizeof(instruction));/*will hold instruction*/
	if (!currentInstruction) {

       		 printf("Error allocating memory on line 31 :firstRun.c\n");
        	 
   	 }
	currentInstruction->operation[0]='\0';
	ft = fopen("Prog.as", "r");/*will read from object file*/
	fp = fopen("Prog.ob", "w+");/*will write to .ob file*/
	if ((!fp)||(!ft)) {

       		 printf("Error opening the file on line 33 or 34 :firstRun.c\n");
        	 
   	 }
	


	while (!feof(ft)) {
		
		c = fgetc(ft);
		
		if (c != ' ' && c != ',' && c != '\n') {	/* check that char is not a seperator,if not separator add char*/
			
			str[strLen++] = c;
		
		}
		else if (strLen > 0) {	/*if seperator and we have a word in str than check it*/
			
			if(str[strLen-1]==':'){
				
				numOfInstructions++;
				str[strLen-1] = '\0';
				head = addVarNode(head,str, countLine,0);
				RESET_STRING
				continue;
			
			}
			if(!strcmp(".extern",str)){
				
				fc = fopen("Prog.ext", "w+");/*if extern mark it and open folder*/
				foundExtern++;
				RESET_STRING
				continue;
			
			}
			if(foundExtern){
				
				head = addVarNode(head,str, -1,0);
				
			}
			if (c == '\n') {/*reset everything because end of line*/
				
				foundExtern = 0;
				countLine++;
				RESET_STRING
				continue;
			
			}
			
			RESET_STRING
		
		}
		
	}
	/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
	/*we finished our first pass through Prog.ob and will now start a second pass through this time we have the addresses of variables*/
	
	/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
	rewind(ft);
	RESET_STRING
	countLine = 0;

	while (!feof(ft)) {
		
		c = fgetc(ft);
		
		if (c != ' ' && c != ',' && c != '\n') {	
			
			str[strLen++] = c;
		
		}
		else if (strLen > 0) {	
			
			wordCounterByLine++;
			
			if(str[strLen-1]==':'){
				
				str[strLen-1] = '\0';/*if a variable instruction turn on flag and check if entry*/
				
				if(findVarNode(head,str)){
					
					if(findVarNode(head,str)->entry==1){
						
						fprintf(fk,"%s %d\n",str,countLine);
					
					}
				}
				
				foundInstruction++;
				RESET_STRING
				continue;
			
			}
			if(!strcmp(".entry",str)){
				
				fk = fopen("Prog.ent", "w+");
				foundEntry++;
				RESET_STRING
				continue;
			
			}
			if(foundEntry){
				
				findVarNode(head,str)->entry=1;
				
			}
			if(!foundInstruction){/*found an instruction put it into our current instruction node*/
				
				for(i=0;i<16;i++){
					
					if(!strcmp(str,operations[i])){
						
						numOfInstructions++;
						strcpy(currentInstruction->operation,operations[i]);
						currentInstruction-> operationNum = i;
						foundInstruction2++;
						RESET_STRING
						continue;
					
					}
					
				
				}
				if(foundInstruction2){
			        	
					if(currentInstruction->numOfOperands == 0){/*add operands */
						
						currentInstruction->memoryAddress = countLine;
						(currentInstruction->numOfOperands)++;
			      	 	 
					}
					else if(currentInstruction->numOfOperands == 1){
							
							(currentInstruction->numOfOperands)++;
							strcpy(currentInstruction->opSource,str);
							if(findVarNode(head,str)){
								if(findVarNode(head,str)->memoryAddress==-1){/*check if entry or extern if so print it to its file*/
									fprintf(fc,"%s %d\n",str,countLine);
								
								}else if(findVarNode(head,str)->entry==1){
									
									fprintf(fk,"%s %d\n",str,countLine);
								
								}
							}
					}else if(currentInstruction->numOfOperands == 2){
							
							(currentInstruction->numOfOperands)++;
							strcpy(currentInstruction->opDestination,str);
					
					}

				
				}
				
			
				
			}
			if(findVarNode(head,str)){
				if(findVarNode(head,str)->entry==1){/*if entry print to its file */
					
					fprintf(fk,"%s %d\n",str,countLine);
					
				}
			}
			if(foundInitial){	
					
				        if(foundInitial == 1){/*found instruction start adding operands by placement*/
							
							(currentInstruction->numOfOperands)++;
							strcpy(currentInstruction->opSource,str);
					
					}else if(foundInitial == 2){
							
							(currentInstruction->numOfOperands)++;
							strcpy(currentInstruction->opDestination,str);
					
					}
					
					foundInitial++;
			}
			if(foundInstruction){
				
				if(!strcmp(".data",str)){
						/*check if data typ*/
					foundData++;
					RESET_STRING
					continue;
					
				}
				if(!strcmp(".string",str)){

					/*check if string type*/
					foundString++;
					RESET_STRING
					continue;
					
				}else if(wordCounterByLine==2){
					
					currentInstruction->memoryAddress = countLine;/*if operation check op table and add*/
					
					for(i=0;i<16;i++){
						if(!strcmp(str,operations[i])){
						
							strcpy(currentInstruction->operation,operations[i]);
							currentInstruction-> operationNum = i;
							i = 15;
						
						}
					
				
				   	}
					foundInitial++;
					
				}
				
			}
			if(foundData){/*if data type add to counter and add to data*/
				
				(currentInstruction->memoryAddress) = countLine;
				numOfData++;
				(currentInstruction->data)[dataCount++] = atoi(str);
				
			}
			if(foundString){
				
				numOfData+=1+strlen(str)-(strlen(str)/2);
				(currentInstruction->memoryAddress) = countLine;
				strcpy((currentInstruction->string),str);
				
			}
			
			
			if (c == '\n') {
				if(foundInstruction){/*end of line, add instruction to list and refresh everything*/
					if(foundData){
						
						headInstruction = addInstructionNode(headInstruction, currentInstruction);
						
					}
					if(foundString){
						
						headInstruction = addInstructionNode(headInstruction, currentInstruction);
					
					}
					if(foundInitial){
						
						headInstruction = addInstructionNode(headInstruction, currentInstruction);
					
					}
			        }
				
				if(foundInstruction2){
				        
					 currentInstruction->numOfOperands--;
					 headInstruction = addInstructionNode(headInstruction, currentInstruction);
					
				}
				
			        currentInstruction = (instruction*)malloc(sizeof(instruction));
				currentInstruction->operation[0]='\0';
				countLine++;
				foundEntry = 0;
				dataCount = 0;
				foundData = 0;
				foundInitial = 0;
				foundString = 0;
				foundInstruction = 0;
				foundInstruction2 = 0;
				wordCounterByLine = 0;
				RESET_STRING
				continue;
			}
			RESET_STRING
		}
	}
	fprintf(fp,"%d,%d\n",numOfData,numOfInstructions);/*counter for data and instructions*/
	
	
	/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*/	
	
	
	
	fclose(ft);
	fclose(fk);
	fclose(fc);

	
	convertToMachineCode(head,headInstruction,fp);/*now that we have our instructions and nodes we will move on to converting */
	
	if(ERROR){
		
		printf("errors exists in code check error file(errorFile.txt), no output.\n");

		remove("Prog.ob");
		remove("Prog.ent");
		remove("Prog.ext");
	
	
	}else{
		printf("successful run\n");
		remove("errorfile.txt");
	}/*if there was an error there will not be an output file*/
	
	fclose(fp);
	freeInsList(headInstruction);
	freeVarList(head);
	
}

void convertToMachineCode(varnode* varNodeHead,instructionNode* instructionNodeHead,FILE* fp){
	FILE* fe;
	int i;
	int source;
	int dest;
	int sourceOpNum;
	instructionNode* temp = instructionNodeHead;
	varnode* tempVar = varNodeHead;
	varnode* tempVar2 = varNodeHead;
	fe = fopen("errorfile.txt","w+");/*when we detect errors we will write to this file*/
	 if (!fe) {
       		 printf("Error opening the file on line 346 :firstRun.c\n");
        	 
   	 }
	
	
	
   	 while (temp) {
       		 if(temp->instruct->operation[0]=='\0'){
			if(temp->instruct->data[0] == '\0'){

				for(i=3;isalpha(temp->instruct->string[i]);i++){/*if type is string send it to the function that will wirte it to the file*/
					
					printInBinary((unsigned int)(temp->instruct->string[i]),fp); 
				
				}
				temp = temp->next;
				continue;
				
			}


				/*if type data write it to file via function*/
            		/*for (i = 0; temp->instruct->data[i] != '\0'; i++) {
				if(i ==0){
	       			 	
				}
               			
				if(feof(fp))
				printInBinary((unsigned int)(temp->instruct->data[i]),fp); 
				if(temp->instruct->data[i+1] == '\0'){
					 	
				}
           		}*/
	     }else{
			/*send to function that writes to file .ob depending on amount of operands*/
			
			if(temp->instruct->numOfOperands==0){/*if 0 operands that means that it has to be stop or rts otherwise error*/
				if(strcmp(temp->instruct->operation,"stop")&&strcmp(temp->instruct->operation,"rts")){
					
					fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
				
				}
				if(!strcmp("stop",temp->instruct->operation)){
					
					fprintf(fp,"PA\n");
				
				}else if(!strcmp("rts",temp->instruct->operation)){
					
					fprintf(fp,"OA\n");
	
				}else{

					fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
					ERROR = 1;

					}
				
			}else if(temp->instruct->numOfOperands==1){
				 /*if not a command of the proper type for 1 operand than print error*/
				if(strcmp(temp->instruct->operation,"not")&&strcmp(temp->instruct->operation,"clr")&&
					strcmp(temp->instruct->operation,"inc")&&strcmp(temp->instruct->operation,"dec")&&
					strcmp(temp->instruct->operation,"jmp")&&strcmp(temp->instruct->operation,"bne")&&
					strcmp(temp->instruct->operation,"red")&&strcmp(temp->instruct->operation,"prn")&&
					strcmp(temp->instruct->operation,"jsr")){
						
						fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
						ERROR = 1;
				
				}
				
				if(temp->instruct->opSource[0]=='@'){/*1 register operand*/
					
					sourceOpNum = atoi(temp->instruct->opSource);
					convertToBinary(sourceOpNum,0,temp->instruct->operationNum,
					temp->instruct->memoryAddress+100,"register",'\0',fp);

				}else if(atoi(temp->instruct->opSource)){/*1 number operand*/
					
					sourceOpNum = atoi(temp->instruct->opSource);
					convertToBinary(sourceOpNum,0,temp->instruct->operationNum,
					temp->instruct->memoryAddress+100,"number",'\0',fp);	
				
				}else{	/*1 variable operand*/

					tempVar= findVarNode(varNodeHead, temp->instruct->opSource);
					
					if(tempVar->memoryAddress==-1){	/*1 extern variable operand*/
						
							convertToBinary(tempVar->memoryAddress,
							0,temp->instruct->operationNum,
							temp->instruct->memoryAddress+100,
							"externVar",'\0',fp);	

					}else{/*1 entry variable operand*/

							convertToBinary(tempVar->memoryAddress,
							0,temp->instruct->operationNum,temp->instruct->memoryAddress+100,
							"entryVar",'\0',fp);	
						
					}
					
				}

				
			
			}else if(temp->instruct->numOfOperands==2){/*if not proper com for 2 operands than error*/
				
					if(strcmp(temp->instruct->operation,"mov")&&strcmp(temp->instruct->operation,"cmp")
					&&strcmp(temp->instruct->operation,"add")&&strcmp(temp->instruct->operation,"sub")
					&&strcmp(temp->instruct->operation,"lea")){
						
						fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
						ERROR = 1;
				
					}
					
					tempVar= findVarNode(varNodeHead, temp->instruct->opSource);
					
					if(tempVar){
						
						source = tempVar->memoryAddress;
						
					}
					
					tempVar2= findVarNode(varNodeHead, temp->instruct->opDestination);
					
					if(tempVar2){
						
						dest = tempVar2->memoryAddress;
	
					}

/*--------------------------------------------------------------------------------------------------------------------------------------------*/
/*up until the end of the function will check all the different possibilites and if true will send it to be printed,all while cheching that the combination is legal if not than will print error*/




					if(temp->instruct->opSource[0]=='@'&&temp->instruct->opDestination[0]=='@'){

						if(strcmp(temp->instruct->operation,"mov")&&
						strcmp(temp->instruct->operation,"cmp")&&strcmp(temp->instruct->operation,"add")&&
						strcmp(temp->instruct->operation,"sub")&&strcmp(temp->instruct->operation,"lea")){
					
							fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
							ERROR = 1;
				
						}
						convertToBinary(temp->instruct->opSource[2]-'0',
						temp->instruct->opDestination[2]-'0',temp->instruct->operationNum,
						temp->instruct->memoryAddress+100,"register","register",fp);

					
					}else if(temp->instruct->opSource[0]=='@'&&atoi(temp->instruct->opDestination)){

							if(strcmp(temp->instruct->operation,"mov")&&strcmp(temp->instruct->operation,"cmp")&&
							strcmp(temp->instruct->operation,"add")&&strcmp(temp->instruct->operation,"sub")&&
							strcmp(temp->instruct->operation,"lea")){
								
								fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
				
							}
					
							convertToBinary(temp->instruct->opSource[2]-'0',atoi(temp->instruct->opDestination),
							temp->instruct->operationNum,temp->instruct->memoryAddress+100,"register","number",fp);
					

					}else if(atoi(temp->instruct->opSource)&&temp->instruct->opDestination[0]=='@'){
							
							if(strcmp(temp->instruct->operation,"mov")&&strcmp(temp->instruct->operation,"cmp")&&
							strcmp(temp->instruct->operation,"add")&&strcmp(temp->instruct->operation,"sub")&&
							strcmp(temp->instruct->operation,"lea")){
					
								fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
								ERROR = 1;
					
							}
					
							convertToBinary(temp->instruct->opSource[2]-'0',temp->instruct->opDestination[2]-'0',
							temp->instruct->operationNum,temp->instruct->memoryAddress+100,"number","register",fp);

					}else if(atoi(temp->instruct->opSource)&&atoi(temp->instruct->opDestination)){
					
							if(strcmp(temp->instruct->operation,"mov")&&strcmp(temp->instruct->operation,"cmp")&&
							strcmp(temp->instruct->operation,"add")&&strcmp(temp->instruct->operation,"sub")&&
							strcmp(temp->instruct->operation,"lea")){					
								
								fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
								ERROR = 1;
							
							}
					
							convertToBinary(atoi(temp->instruct->opSource),atoi(temp->instruct->opDestination),
							temp->instruct->operationNum,temp->instruct->memoryAddress+100,"number","number",fp);
					

					}else if(temp->instruct->opSource[0]=='@'&&dest==-1){


							if(strcmp(temp->instruct->operation,"mov")&&strcmp(temp->instruct->operation,"cmp")&&
							strcmp(temp->instruct->operation,"add")&&strcmp(temp->instruct->operation,"sub")&&
							strcmp(temp->instruct->operation,"lea")){
					
								fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
								ERROR = 1;
				
							}
							convertToBinary(temp->instruct->opSource[2]-'0',-1,temp->instruct->operationNum,
							temp->instruct->memoryAddress+100,"register","externVar",fp);
					
					}else if(source==-1&&temp->instruct->opDestination[0]=='@'){


							if(strcmp(temp->instruct->operation,"mov")&&strcmp(temp->instruct->operation,"cmp")&&
							strcmp(temp->instruct->operation,"add")&&strcmp(temp->instruct->operation,"sub")&&
							strcmp(temp->instruct->operation,"lea")){
					
								fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
								ERROR = 1;
				
							}
							convertToBinary(-1,temp->instruct->opDestination[2]-'0',temp->instruct->operationNum,
							temp->instruct->memoryAddress+100,"externVar","register",fp);
						
					}else if(atoi(temp->instruct->opSource)&&dest==-1){
							
							if(strcmp(temp->instruct->operation,"mov")&&strcmp(temp->instruct->operation,"cmp")&&
							strcmp(temp->instruct->operation,"add")&&strcmp(temp->instruct->operation,"sub")&&
							strcmp(temp->instruct->operation,"lea")){
					
								fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
								ERROR = 1;
				
							}
						
							convertToBinary(atoi(temp->instruct->opSource),-1,temp->instruct->operationNum,
							temp->instruct->memoryAddress+100,"number","externVar\n",fp);
					
					}else if(source==-1&&atoi(temp->instruct->opDestination)){


							if(strcmp(temp->instruct->operation,"mov")&&strcmp(temp->instruct->operation,"cmp")&&
							strcmp(temp->instruct->operation,"add")&&strcmp(temp->instruct->operation,"sub")&&
							strcmp(temp->instruct->operation,"lea")){
					
								fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
								ERROR = 1;
				
							}
							convertToBinary(-1,atoi(temp->instruct->opDestination),temp->instruct->operationNum,
							temp->instruct->memoryAddress+100,"exterVar","number",fp);
						
					}else if(atoi(temp->instruct->opDestination)){

							if(strcmp(temp->instruct->operation,"mov")&&strcmp(temp->instruct->operation,"cmp")&&
							strcmp(temp->instruct->operation,"add")&&strcmp(temp->instruct->operation,"sub")&&
							strcmp(temp->instruct->operation,"lea")){
					
								fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
								ERROR = 1;
				
							}
							if(!tempVar){
								if(strcmp(temp->instruct->operation,"mov")&&strcmp(temp->instruct->operation,"cmp")&&
								strcmp(temp->instruct->operation,"add")&&strcmp(temp->instruct->operation,"sub")&&
								strcmp(temp->instruct->operation,"lea")){
			
									fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
									ERROR = 1;
				
								}
							
								convertToBinary(tempVar->memoryAddress,atoi(temp->instruct->opDestination),
								temp->instruct->operationNum,temp->instruct->memoryAddress+100,"entryVar","number",fp);
						
							}
					}else if(atoi(temp->instruct->opSource)){
							if(!tempVar2){
								if(strcmp(temp->instruct->operation,"mov")&&strcmp(temp->instruct->operation,"cmp")&&
								strcmp(temp->instruct->operation,"add")&&strcmp(temp->instruct->operation,"sub")&&
								strcmp(temp->instruct->operation,"lea")){
					
									fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
									ERROR = 1;
				
								}
								convertToBinary(atoi(temp->instruct->opSource),tempVar2->memoryAddress,
								temp->instruct->operationNum,temp->instruct->memoryAddress+100,"number","entryVar",fp);
							}
					}else if(temp->instruct->opSource[0]=='@'){
							if(tempVar2){
								if(strcmp(temp->instruct->operation,"mov")&&strcmp(temp->instruct->operation,"cmp")&&
								strcmp(temp->instruct->operation,"add")&&strcmp(temp->instruct->operation,"sub")&&
								strcmp(temp->instruct->operation,"lea")){
					
									fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
									ERROR = 1;
				
								}
								convertToBinary(temp->instruct->opSource[2]-'0',tempVar2->memoryAddress,
								temp->instruct->operationNum,temp->instruct->memoryAddress+100,"register","entryVar",fp);
							}
					}else if(temp->instruct->opDestination[0]=='@'){
							if(tempVar){


								if(strcmp(temp->instruct->operation,"mov")&&strcmp(temp->instruct->operation,"cmp")&&
								strcmp(temp->instruct->operation,"add")&&strcmp(temp->instruct->operation,"sub")&&
								strcmp(temp->instruct->operation,"lea")){
					
									fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
									ERROR = 1;

								}
								convertToBinary(tempVar->memoryAddress,temp->instruct->opDestination[2]-'0',
								temp->instruct->operationNum,temp->instruct->memoryAddress+100,"entryVar","register",fp);
						
							}
					}else if(tempVar2&&tempVar2){
							if(source==-1&&dest==-1){
								if(strcmp(temp->instruct->operation,"mov")&&strcmp(temp->instruct->operation,"cmp")&&
								strcmp(temp->instruct->operation,"add")&&strcmp(temp->instruct->operation,"sub")&&
								strcmp(temp->instruct->operation,"lea")){
					
									fprintf(fe,"error on line %d: wrong command\n",temp->instruct->memoryAddress);
									ERROR = 1;

								}
								convertToBinary(-1,-1,temp->instruct->operationNum,
								temp->instruct->memoryAddress+100,"externVar","externVar",fp);
							}
					}else{
						convertToBinary(tempVar->memoryAddress,tempVar2->memoryAddress,temp->instruct->operationNum,
						temp->instruct->memoryAddress+100,"entrynVar","entryVar",fp);
						
					}
					
					
			
			}
	     

	}
	   
        
        
             temp = temp->next;
    }

 	fclose(fe);
	freeInsList(instructionNodeHead);
	freeVarList(tempVar);
	freeVarList(tempVar2);
    }
/*------------------------------------------------------------------------------------------------------------------------------------------------*/
void convertToBinary(int SourceNum,int destinationNum,int operationNum,int address,char* SourceType,char* destinationType,FILE* fp){


/*like the end of the last function this will check all the possibilites and convert them into a "word" which will be a 12 digit binary numbery which will than be sent to another funtion where it will be converted to base 64 and printed to file .ob*/
       
   	
	
	typedef struct BitField {
    		unsigned int are : 2;
    		unsigned int dest : 3;
   	        unsigned int op : 4;
                unsigned int source : 3;
         }BitField;
	typedef struct BitField2 {
    		unsigned int are : 2;
    		unsigned int all : 10;
   	      
         }BitField2;
	typedef struct BitField3 {
    		unsigned int are : 2;
    		unsigned int dest : 5;
   	        unsigned int source :5;
              
         }BitField3;
		

	BitField *command = malloc(sizeof(BitField));
	BitField2 *Direct= malloc(sizeof(BitField2));
	BitField3 *Register= malloc(sizeof(BitField3));
	unsigned int combinedValue = 0;
	
	
	/*printf( "%d %d %d %d %s %s\n",SourceNum, destinationNum,operationNum,address, SourceType, destinationType);
	*/
	if(SourceType&&destinationType){
		if(!strcmp(SourceType,"register")&&!strcmp(destinationType,"register")){
		
		/*in this option i dont use the macros just to showcase what it would look like without macros, each number is put into a part of athe binary number and then they are combined into one number and sent to be converted and printed, all the rest of the logic tabke will be with macros depending on if they are number registers or variables*/
   		
		
			command->dest = 5;
			command->source = 5;
			COMMAND_PRINT
			Register->are = 0;
			Register->dest =destinationNum;
			Register->source = SourceNum;
			combinedValue = 0;
			combinedValue |= Register->are;
   			combinedValue |= Register->dest << 2;
   			combinedValue |= Register->source << 7;
			printInBinary(combinedValue,fp);
			combinedValue = 0;
			free(command);
        		free(Register);
			free(Direct);
		
			return;
		
		}else if(!strcmp(SourceType,"number")&&!strcmp(destinationType,"number")){
		
		
		
			command->dest = 1;
			command->source = 1;
			COMMAND_PRINT
			NUM_PRINT
			NUM_PRINT
			free(command);
        		free(Register);
			free(Direct);
		
			return;
		
		
		
		}else if(!strcmp(SourceType,"externVar")&&!strcmp(destinationType,"externVar")){

			command->dest = 3;
			command->source = 3;
			COMMAND_PRINT
			fprintf(fp,"AB\n");
			fprintf(fp,"AB\n");
			combinedValue = 0;
			free(command);
        		free(Register);
			free(Direct);
		
			return;




		}else if(!strcmp(SourceType,"register")&&!strcmp(destinationType,"number")){

			command->dest = 1;
			command->source = 5;
			COMMAND_PRINT
			SOURCE_REG_PRINT
			NUM_PRINT
			free(command);
        		free(Register);
			free(Direct);
		
			return;




		}else if(!strcmp(SourceType,"number")&&!strcmp(destinationType,"register")){

			command->dest = 5;
			command->source = 1;
			COMMAND_PRINT
			NUM_PRINT
			DEST_REG_PRINT
			free(command);
        		free(Register);
			free(Direct);
		
			return;





		}else if(!strcmp(SourceType,"entryVar")&&!strcmp(destinationType,"entryVar")){
			command->dest = 3;
			command->source = 3;
			COMMAND_PRINT
			ENTRY_VAR_PRINT
	
	
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
	
	
	
		}else if(!strcmp(SourceType,"entryVar")&&!strcmp(destinationType,"number")){
	
			command->dest = 1;
			command->source = 3;
			COMMAND_PRINT
			ENTRY_VAR_PRINT
			NUM_PRINT
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
	
	
		}else if(!strcmp(SourceType,"number")&&!strcmp(destinationType,"entryVar")){
	
			command->dest = 3;
			command->source = 1;
			COMMAND_PRINT
			NUM_PRINT
			ENTRY_VAR_PRINT
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
	
		}else if(!strcmp(SourceType,"entryVar")&&!strcmp(destinationType,"register")){
	
			
			command->dest = 5;
			command->source = 3;
			COMMAND_PRINT
			ENTRY_VAR_PRINT
			DEST_REG_PRINT
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
	
		}else if(!strcmp(SourceType,"register")&&!strcmp(destinationType,"entryVar")){
			command->dest = 3;
			command->source = 5;
			COMMAND_PRINT
			SOURCE_REG_PRINT
			ENTRY_VAR_PRINT
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
	
		}else if(!strcmp(SourceType,"entryVar")&&!strcmp(destinationType,"externVar")){
	
			command->dest = 1;
			command->source = 1;
			COMMAND_PRINT
			ENTRY_VAR_PRINT
			fprintf(fp,"AB\n");
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
		}else if(!strcmp(SourceType,"externVar")&&!strcmp(destinationType,"entryVar")){
			command->dest = 1;
			command->source = 1;
			COMMAND_PRINT
			fprintf(fp,"AB\n");
			ENTRY_VAR_PRINT
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
	
		}

	}
	if(SourceType){
			
		 if(!strcmp(SourceType,"register")){
			command->dest = 5;
			command->source = 0;
			COMMAND_PRINT
			SOURCE_REG_PRINT
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
		}else if(!strcmp(SourceType,"number")){
			
			command->dest = 1;
			command->source = 0;
			COMMAND_PRINT
			NUM_PRINT
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
		}else if(!strcmp(SourceType,"entryVar")){
			
			command->dest = 3;
			command->source = 0;
			COMMAND_PRINT
			ENTRY_VAR_PRINT
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
		}else if(!strcmp(SourceType,"externVar")){
			command->dest = 3;
			command->source = 0;
			COMMAND_PRINT
			fprintf(fp,"AB\n");
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
	
		}
	
    	}	
		
	
		
	
}

void printInBinary(unsigned int num,FILE* fp) {/*this function converts a number into base 64 and than gets printed to the ob file*/
    char base64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char *base64Table2[]  = {"000000",
"000001","000010","000011","000100","000101","000110","000111","001000",
"001001","001010","001011","001100","001101","001110","001111",
"010000","010001","010010","010011","010100","010101",
	"010110","010111","011000","011001","011010","011011",
"011100","011101","011110","011111","100000","100001",
	"100010","100011","100100","100101","100110","100111","",
"101000","101001","101010","101011","101100","101101",
	"101110","101111","110000","110001","110010",
"110011","110100","110101","110110","110111","111000",
"111001","111010","111011","111100","111101","111110","111111"};
    char* str = (char*)malloc(sizeof(char)*6);
    unsigned int mask = 1 << 11;
    int i = 0;
    
	

    

    for (; mask; mask >>= 1) {
	str[i] = ((mask & num) ? '1' : '0');
	
       	i++;
	if(i==6){
		str[i] = '\0';
		
		for(i = 0; i <64;i++){
			if(!strcmp(str,base64Table2[i])){
				
				fprintf(fp, "%c", base64Table[i]);
				break;
			}
		}
		str[0] = '\0';
		i = 0;
	}

	
	
	
    }

    
    fprintf(fp,"\n");
		
    
    
}
		
	







	


