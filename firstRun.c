#include "firstRun.h"

void printInBinary(unsigned int,FILE*);/* converts binary commands into base 64 aand print to output file*/
int extractIntFromString( char *);

void convertToBinary(int ,int ,int ,char* ,char*,int,FILE*,FILE*);/*converts commands to bin and then send to'printtobin' which will convert to 64 and print */
int ERROR = 0;
int sizeOfStr(char*);
int checkIfOperation(char*);
void firstRun(int argc, char** argv) {/*divide each line into a command line struct to be dealt with more easily*/

	FILE* ft;
	FILE* fp;
	FILE* fe;
	FILE* fc;
	FILE* fk;	/*to read from the initial input and write into a new filewith the macros parsed out*/
	char str[maxLineLength] = { 0 };/*this will hold every string from the txt file with (',' )(' ')('\n') used as separators*/
	char c;/*hold every char from the txt file*/
	int i;/*used for 'for' loops*/
	unsigned int number;
	instructionNode *headInstruction = NULL;
	int numOfInstructions = 0;/*flags and counters*/
	int amountOfOps = 0;
	int numOfData = 0;
	int foundRegister = 0;
	int foundData = 0;
	int foundString = 0;
	int foundExtern = 0;
	int foundEntry = 0;
	varnode *head = NULL;
	int strLen = 0;
	int countLine = 100;
	int existsOperation = 0;
	instruction *currentInstruction = (instruction*)malloc(sizeof(instruction));/*will hold instruction*/
	if (!currentInstruction) {

       		 printf("Error allocating memory on line 31 :firstRun.c\n");
        	 
   	}
	currentInstruction->operation[0]='\0';
	ft = fopen("Prog.as", "r");/*will read from object file*/
	fp = fopen("Prog.ob", "w+");/*will write to .ob file*/
	fe = fopen("errorFile.txt", "w+");
	if ((!fp)||(!ft)) {

       		 printf("Error opening the file on line 33 or 34 :firstRun.c\n");
        	 
   	 }
	

/*first run through the .as file we will figure out all variables entries and externs and the lines theyre on*/
	while (!feof(ft)) {
		
		c = fgetc(ft);
		
		if (c != ' ' && c != ',' && c != '\n') {	/* check that char is not a seperator,if not separator add char*/
			
			str[strLen++] = c;
		
		}
		else if (strLen > 0) {	/*if seperator and we have a word in str than check it*/
			
			if(str[strLen-1]==':'){
				
				/*printf(" %s(%d) ",str,countLine);*/
				str[strLen-1] = '\0';
				head = addVarNode(head,str, countLine,0,0);
				RESET_STRING
				continue;
			
			}
			if(foundRegister && (str[0] == '@')){
				
				/*printf(" reg%s(%d) ",str,countLine);*/
				
			}else if(foundData){
				/*printf(" data%s(%d) ",str,countLine);*/
				countLine++;
				numOfInstructions++;
				
			}else if(foundString){
				/*printf(" str%s,strsize(%d)(%d) ",str,sizeOfStr(str),countLine);*/
				numOfInstructions+=sizeOfStr(str)+1;
				countLine+=sizeOfStr(str);
			}else if(strcmp(".extern",str)&&strcmp(".entry",str)&&!foundEntry&&!foundExtern){
				/*printf(" other%s(%d) ",str,countLine);*/
				countLine++;
				numOfData++;
			}
			if(str[0] == '@'){
				
				foundRegister = 1;

			}else if(!strcmp(".data",str)){
				foundData = 1;
				countLine--;
				numOfData--;
			}else if(!strcmp(".string",str)){
				foundString = 1;
				numOfData--;
			}else if(!strcmp(".extern",str)){
				
				fc = fopen("Prog.ext", "w+");/*if extern mark it and open folder*/
				foundExtern = 1;
				RESET_STRING
				continue;
			
			}else if(!strcmp(".entry",str)){
				fk = fopen("Prog.ent", "w+");
				foundEntry = 1;
				RESET_STRING
				continue;
			}
			if(foundExtern){
				/*printf(" %s ",str);*/
				head = addVarNode(head,str, countLine,0,1);
				
			}else if(foundEntry){
				/*printf(" %s ",str);*/
				head = addVarNode(head,str,countLine,1,0);
			}
			if (c == '\n') {/*reset everything because end of line*/
				
				foundRegister = 0;
				foundExtern = 0;
				foundData = 0;
				foundString = 0;
				foundEntry = 0;
				RESET_STRING
				
				continue;
				
			
			}
			
			RESET_STRING
		
		}
		
	}
	/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
	/*now that we know which variables are which we will apply their addresses and print to the .ext and the .ent files*/
	
	/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
	rewind(ft);
	RESET_STRING
	countLine = 100;



	while (!feof(ft)) {
		
		c = fgetc(ft);
		
		if (c != ' ' && c != ',' && c != '\n') {	/* check that char is not a seperator,if not separator add char*/
			
			str[strLen++] = c;
		
		}
		else if (strLen > 0) {	/*if seperator and we have a word in str than check it*/
			
			if(str[strLen-1]==':'){
				
				
				str[strLen-1] = '\0';
				if(findVarNode(head,str)){
					findVarNode(head,str)->memoryAddress = countLine;
					if(findVarNode(head,str)->entry){/*found a variable, check our variable list and apply the current address*/
						fprintf(fk,"%s %d\n",str,countLine);	
					}
					
				}
				RESET_STRING
				continue;
			
			}
			if(foundRegister && (str[0] == '@')){
				
				
				
			}else if(foundData){
				
				
				countLine++;
				
				
			}else if(foundString){
				
				
				countLine+=sizeOfStr(str);

			}else if(strcmp(".extern",str)&&strcmp(".entry",str)&&!foundEntry&&!foundExtern){
				
				
				if(findVarNode(head,str)){/*found a variable, check our variable list and apply the current address*/
					if(findVarNode(head,str)->exter){
						fprintf(fc,"%s %d\n",str,countLine);	
					}
					
				}
				countLine++;
				
			}
			if(str[0] == '@'){
				
				foundRegister = 1;

			}else if(!strcmp(".data",str)){
				foundData = 1;
				countLine--;
				
			}else if(!strcmp(".string",str)){
				foundString = 1;
				
			}else if(!strcmp(".extern",str)){
				foundExtern = 1;
				RESET_STRING
				continue;
			
			}else if(!strcmp(".entry",str)){
				
				foundEntry = 1;
				RESET_STRING
				continue;
			}
			if(foundExtern){
				
				
				
			}else if(foundEntry){
				
				
			}
			if (c == '\n') {/*reset everything because end of line*/
				
				foundRegister = 0;
				foundExtern = 0;
				foundData = 0;
				foundString = 0;
				foundEntry = 0;
				RESET_STRING
				
				continue;
				
			
			}
			
			RESET_STRING
		
		}
		
	}/*we counted the amount of instructions, apply them and print them*/
	fprintf(fp,"%d,%d\n",numOfData,numOfInstructions);/*counter for data and instructions*/
/*  --------------------------------------------------------------------------------------------------------------------------------*/
	rewind(ft);
	RESET_STRING
	countLine = 100;

	
/*
i need 6 pieces of information in order to convert each instruction to binary:


void convertToBinary(int SourceNum,int destinationNum,int operationNum,int address,char* SourceType,char* destinationType,FILE* fp)

1)value of source operative

2)value of destination operative

3)value of operation

4)amount of operations

5)type of source operative 

6)type pf destination operative


depending on the type of information on line we will collect the relevant data and then send it to a function to be converted to base 64 

*/



	while (!feof(ft)) {
		
		c = fgetc(ft);
		
		if (c != ' ' && c != ',' && c != '\n') {	/* check that char is not a seperator,if not separator add char*/
			
			str[strLen++] = c;
		
		}
		else if (strLen > 0) {	/*if seperator and we have a word in str than check it*/
			
			if(str[strLen-1]==':'){
				
				str[strLen-1] = '\0';
				RESET_STRING
				continue;
			
			}
			if(!strcmp(".data",str)){
	
				/*printf(" TypeDATA%s(%d) ",str,countLine);*/
				strcpy(currentInstruction->type,"data");/*note that its type data line*/
				
			}else if(!strcmp(".string",str)){
	
				/*printf(" TypeSTRING%s(%d) ",str,countLine);*/
				strcpy(currentInstruction->type,"string");/*note that its type string line*/
	
			}else if(checkIfOperation(str)!=-1){
				existsOperation = 1;
				/*printf(" TypeOPERATION%s ",str);*//*note that its the operation type take down the name and number*/
				strcpy(currentInstruction->type,"operation");
				strcpy(currentInstruction->operation,str);
				currentInstruction->operationNum = checkIfOperation(str);
		
			}else if(str[0] == '@'){

				amountOfOps++;/*keep track of amount of epratives*/
				if(amountOfOps == 1){
					strcpy(currentInstruction->opSource,"register");
					currentInstruction->opSourceNum = str[2]-'0';/*found register as first operative(source). take its number and type*/
				}else{
					strcpy(currentInstruction->opDestination,"register");
					currentInstruction->opDestinationNum = str[2]-'0';/*found register as second operative(destination). take its number and type*/
				}
				/*printf(" reg%s ",str);*/
				

			}else if(foundData){

				/*printf(" data%s ",str);*/
				number = atoi(str);/*found data so convert the number to binary*/
				printInBinary(number,fp);
				countLine++;
				
				
			}else if(foundString){
				/*printf(" str%s ",str);*/
				i = 3;
				while((str[i]>='a'&&str[i]<='z')||(str[i]>='A'&&str[i]<='Z')){
					/*printf("%c,%d ",str[i],(int)str[i]);*/
					number = (int)str[i];/*found string so convert each letter asci to binary*/
					printInBinary(number,fp);
					i++;
				}
				
				countLine+=sizeOfStr(str);
			}else if(strcmp(".extern",str)&&strcmp(".entry",str)&&!foundEntry&&!foundExtern&&atoi(str)){
				
				/*printf(" num%s ",str);*/
				amountOfOps++;
	
				if(amountOfOps == 1){/*found number so convert it to binary*/
					strcpy(currentInstruction->opSource,"number");
					currentInstruction->opSourceNum = (str[0]=='-')?-(str[1]-'0'):str[0]-'0';
				}else{
					strcpy(currentInstruction->opDestination,"number");
					currentInstruction->opDestinationNum = (str[0]=='-')?-(str[1]-'0'):str[0]-'0';
				}
				countLine++;
				
			}else if(strcmp(".extern",str)&&strcmp(".entry",str)&&!foundEntry&&!foundExtern){
				
				/*printf(" variable%s ",str);*/
				amountOfOps++;
				if(amountOfOps == 1){/*found variavle so figure out if its extern otherwise consider it as entry and keep track of its address and type*/				if(findVarNode(head,str)){
						strcpy(currentInstruction->opSource,(((findVarNode(head,str)->exter)==1)?"externVar":"entryVar"));
						currentInstruction->opSourceNum = findVarNode(head,str)->memoryAddress;
					}else{
						fprintf(fe,"error11, undefined variable( %s)\n",str);/*couldnt find variable so must be undefined*/
						ERROR = 1;
						RESET_STRING
						continue;
					}
				}else{
					if(findVarNode(head,str)){
						strcpy(currentInstruction->opDestination,(((findVarNode(head,str)->exter)==1)?"externVar":"entryVar"));
						currentInstruction->opDestinationNum = findVarNode(head,str)->memoryAddress;
					}else{
						fprintf(fe,"error12, undefined variable( %s)\n",str);/*couldnt find variable so must be undefined*/
						ERROR = 1;
						RESET_STRING
						continue;
					}
				}
				
				countLine++;
				
			}
			if(!strcmp(".data",str)){
				foundData = 1;
				countLine--;
				
			}else if(!strcmp(".string",str)){
				foundString = 1;
				
			}else if(!strcmp(".extern",str)){
				
				
				foundExtern = 1;
				RESET_STRING
				continue;
			
			}else if(!strcmp(".entry",str)){
				
				foundEntry = 1;
				RESET_STRING
				continue;
			}
			
			if (c == '\n') {/*reset everything because end of line*/
				if(existsOperation==1){
					if(!strcmp(str,"stop")){
						fprintf(fp,"Hg\n");	
					}else if(!strcmp(str,"rts")){
						fprintf(fp,"HA\n");
					}
					/*printf("\n*----*\n");
					printf(" SourceNum:%d,destinationNum:%d,operationNum:%d, SourceType:%s,destinationType:%s,numOfOPerands:%d\n\n",currentInstruction->opSourceNum,currentInstruction->opDestinationNum,currentInstruction->operationNum,currentInstruction->opSource,currentInstruction->opDestination,amountOfOps);
					printf("\n*----*\n");*/
					convertToBinary(currentInstruction->opSourceNum,currentInstruction->opDestinationNum,currentInstruction->operationNum,currentInstruction->opSource,currentInstruction->opDestination, amountOfOps, fp,fe);
				}
				currentInstruction = NULL;
				currentInstruction = (instruction*)malloc(sizeof(instruction));
				if(foundString){/*last string will print 0 in base 64*/
					number = 0;
					printInBinary(number,fp);

				}/*end of line will reset everything*/
				amountOfOps = 0;
				foundExtern = 0;
				foundData = 0;
				foundString = 0;
				foundEntry = 0;
				existsOperation = 0;
				RESET_STRING
				/*printf("\n");*/
				continue;
				
			
			}
			
			RESET_STRING
		
		}
		
	}
/*------------------------------------------------------------------------------------------------------------------------------------*/
	
	
	
	
	/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*/	
	
	
	
	
	
	
	/*convertToMachineCode(head,headInstruction,fp);*//*now that we have our instructions and nodes we will move on to converting */
	
	if(ERROR){
		
		printf("errors exists in code check error file(errorFile.txt), no output.\n");

		remove("Prog.ob");
		remove("Prog.ent");
		remove("Prog.ext");
	
	
	}else{
		printf("successful run\n");
		remove("errorFile.txt");
	}/*if there was an error there will not be an output file*/
	
	fclose(fp);
	freeInsList(headInstruction);
	freeVarList(head);
	
}


/*------------------------------------------------------------------------------------------------------------------------------------------------*/
void convertToBinary(int SourceNum,int destinationNum,int operationNum,char* SourceType,char* destinationType,int numOfOperands,FILE* fp,FILE* fe){


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
	
	
	/*printf( "SourceNum:%d, destinationNum:%d, operationNum:%d, SourceType:%s, destinationType:%s\n",SourceNum, destinationNum,operationNum, SourceType, destinationType);*/
	  
	if((operationNum==14||operationNum==15)&&numOfOperands>0){/*check if its stop or rts than must be an error, we will do something similiar for all cases*/
		fprintf(fe,"error0, on line with stop or rts\n");
		ERROR = 1;
	}
	
	if(SourceType&&destinationType){
		if(!strcmp(SourceType,"register")&&!strcmp(destinationType,"register")){
		
		/*in this option i dont use the macros just to showcase what it would look like without macros, each number is put into a part of athe binary number and then they are combined into one number and sent to be converted and printed, all the rest of the logic tabke will be with macros depending on if they are number registers or variables*/
   			
			if(!(operationNum==0)&&!(operationNum==1)&&!(operationNum==2)&&!(operationNum==3)){
				fprintf(fe,"error1, on line with 2 registers, the operation cant be done \n");
				ERROR = 1;
			}
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
		
			if(!(operationNum==1)){
				fprintf(fe,"error2, on line with 2 numbers, the operation cant be done \n");
				ERROR = 1;
			}
		
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
			if(!(operationNum==1)){
				fprintf(fe,"error3, on line with register and number, the operation cant be done \n");
				ERROR = 1;
			}
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
			if(!(operationNum==1)){
				fprintf(fe,"error4, on line with register and number, the operation cant be done \n");
				ERROR = 1;
			}
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
			
			if(!(operationNum==1)){
				fprintf(fe,"error5, on line with 2 variables, the operation cant be done using two vars\n");
				ERROR = 1;
			}
			command->dest = 3;
			command->source = 3;
			COMMAND_PRINT
			SOURCE_ENTRY_VAR_PRINT
			DESTINATION_ENTRY_VAR_PRINT
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
	
	
	
		}else if(!strcmp(SourceType,"entryVar")&&!strcmp(destinationType,"number")){
			
			if(!(operationNum==0)&&!(operationNum==1)&&!(operationNum==2)&&!(operationNum==3)){
				fprintf(fe,"error6, on line with variable and number, the operation cant be done\n");
				ERROR = 1;
			}
	
			command->dest = 1;
			command->source = 3;
			COMMAND_PRINT
			SOURCE_ENTRY_VAR_PRINT
			NUM_PRINT
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
	
	
		}else if(!strcmp(SourceType,"number")&&!strcmp(destinationType,"entryVar")){
	
			if(!(operationNum==0)&&!(operationNum==1)&&!(operationNum==2)&&!(operationNum==3)){
				fprintf(fe,"error7, on line with number and variable, the operation cant be done\n");
				ERROR = 1;
			}
			command->dest = 3;
			command->source = 1;
			COMMAND_PRINT
			NUM_PRINT
			DESTINATION_ENTRY_VAR_PRINT
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
	
		}else if(!strcmp(SourceType,"entryVar")&&!strcmp(destinationType,"register")){
			if(!(operationNum==0)&&!(operationNum==1)&&!(operationNum==2)&&!(operationNum==3)){
				fprintf(fe,"error8, on line with number and variable, the operation cant be done\n");
				ERROR = 1;
			}
			
			command->dest = 5;
			command->source = 3;
			COMMAND_PRINT
			SOURCE_ENTRY_VAR_PRINT
			DEST_REG_PRINT
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
	
		}else if(!strcmp(SourceType,"register")&&!strcmp(destinationType,"entryVar")){

			if(!(operationNum==0)&&!(operationNum==1)&&!(operationNum==2)&&!(operationNum==3)){
				fprintf(fe,"error9, on line with number and variable, the operation cant be done\n");
				ERROR = 1;
			}
			command->dest = 3;
			command->source = 5;
			COMMAND_PRINT
			SOURCE_REG_PRINT
			DESTINATION_ENTRY_VAR_PRINT
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
	
		}else if(!strcmp(SourceType,"entryVar")&&!strcmp(destinationType,"externVar")){
	
			
			
			command->dest = 1;
			command->source = 1;
			COMMAND_PRINT
			SOURCE_ENTRY_VAR_PRINT
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
			DESTINATION_ENTRY_VAR_PRINT
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
	
		}

	}
	if(SourceType){
			
		 if(!strcmp(SourceType,"register")){

			if(!(operationNum<=13&&operationNum<=4)){
				fprintf(fe,"error10, on line with number and variable, the operation cant be done\n");
				ERROR = 1;
			}
			command->dest = 5;
			command->source = 0;
			COMMAND_PRINT
			SOURCE_REG_PRINT
			free(command);
        		free(Register);
			free(Direct);
			
			return;
	
	
		}else if(!strcmp(SourceType,"number")){
			
			if(!(operationNum==12)){
				fprintf(fe,"error10, on line with number and variable, the operation cant be done\n");
				ERROR = 1;
			}
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
			SOURCE_ENTRY_VAR_PRINT
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
	"100010","100011","100100","100101","100110","100111","101000",
"101001","101010","101011","101100","101101",
	"101110","101111","110000","110001","110010",
"110011","110100","110101","110110","110111","111000",
"111001","111010","111011","111100","111101","111110","111111"};
    char* str = (char*)malloc(sizeof(char)*6);
    unsigned int mask = 1 << 11;
    int i = 0;
   
    
	

    

    for (; mask; mask >>= 1) {
	
	str[i] = ((mask & num) ? '1' : '0');
	/*printf("%d",str[i]-48);*/
       	i++;
	if(i==6){
		str[i] = '\0';
		
		for(i = 0; i <64;i++){
			if(!strcmp(str,base64Table2[i])){
				
				fprintf(fp, "%c", base64Table[i]);
				/*printf("%c", base64Table[i]);*/
				break;
			}
		}
		str[0] = '\0';
		i = 0;
	}

	
	
	
    }

   /* printf("\n");*/
    fprintf(fp,"\n");
		
    
    
}
int sizeOfStr(char* str){
	int size  = 0,i=0;
	while(str[i]!='\0'){
		str++;
		size++;
		i++;
	}
	return size;
}

int checkIfOperation(char* str){
	int i;
	for(i=0;i<16;i++){
					
		if(!strcmp(str,operations[i])){
						
			return i;
				
		}
					
				
	}
        return -1;


}

int extractIntFromString( char *input) {
    char numberString[50]; 
    int number;
    int i = 0;
    char *start = input;

   
    while (*start && !isdigit(*start) && *start != '-') {
        start++;
    }

   
    

    while (*start && (isdigit(*start) || *start == '-')) {
        numberString[i++] = *start++;
    }

    numberString[i] = '\0';

    
    number = atoi(numberString);
    return number;
}
		
	







	


