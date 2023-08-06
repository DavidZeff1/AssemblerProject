#ifndef FIRSTRUN_H
#define FIRSTRUN_H
#include "global.h"
#define NUM_PRINT Direct->are = 0;Direct->all = SourceNum;combinedValue = 0;combinedValue |= Direct->are;combinedValue |= Direct->all << 2;printInBinary(combinedValue,fp);combinedValue = 0;

#define SOURCE_ENTRY_VAR_PRINT Direct->are = 2;Direct->all = (SourceNum);combinedValue = 0;combinedValue |= Direct->are;combinedValue |= Direct->all << 2;printInBinary(combinedValue,fp);combinedValue = 0;

#define DESTINATION_ENTRY_VAR_PRINT Direct->are = 2;Direct->all = (destinationNum);combinedValue = 0;combinedValue |= Direct->are;combinedValue |= Direct->all << 2;printInBinary(combinedValue,fp);combinedValue = 0;

#define DEST_REG_PRINT Register->are = 0;Register->dest = destinationNum;Register->source = 0;combinedValue = 0;combinedValue |= Register->are;combinedValue |= Register->dest << 2;combinedValue |= Register->source << 7;printInBinary(combinedValue,fp);combinedValue = 0;

#define SOURCE_REG_PRINT Register->are = 0;Register->dest = 0;Register->source = SourceNum;combinedValue = 0;combinedValue |= Register->are;combinedValue |= Register->dest << 2;combinedValue |= Register->source << 7;printInBinary(combinedValue,fp);combinedValue = 0;

#define COMMAND_PRINT command->op = operationNum;command->are = 0;combinedValue |= command->are;combinedValue |= command->dest << 2;combinedValue |= command->op << 5;combinedValue |= command->source << 9;printInBinary(combinedValue,fp);

typedef struct varnode{	/*nodes which represent a variable*/
	char name[100];
	int memoryAddress;
	int entry;
	int exter;
	struct varnode *next;
	

}varnode;

typedef struct instruction{/*nodes which represent a instructions*/
	char type[100];
	char opSource[100];
	char opDestination[100];
	char operation[100];
	char string[100];
	char data[100];
	varnode *variable;
	int memoryAddress;
	int operationNum;
	int opSourceNum;
	int opDestinationNum;
	int numOfOperands;
	
}instruction;

typedef struct instructionNode{	/*linked list of instructions*/
	instruction *instruct;
	struct instructionNode *next;
	

}instructionNode;
			/*map of operations and registers in order to assign values to operands*/
char *operations[] = {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};
char *registers[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};






varnode* findVarNode(varnode* head, char* str) {/*will find node by name and return it*/
    varnode* temp = head;

    while (temp) {

        if (!strcmp(temp->name, str)) {

            return temp;
        }
        temp = temp->next;
    }

    return NULL;

}
	



/*print list of var nodes*/
void printVarList(varnode* head) {
    varnode* temp = head;
    varnode* next;
    while (temp) {
	printf("%s,%d, entry:%d, extern:%d\n",temp->name,temp->memoryAddress,temp->entry,temp->exter);
        next = temp->next;
        temp = next;
    }
}



varnode* addVarNode(varnode* head, char* str,int num, int ent,int exter) { /*will add node and assign name or concatnate contents*/
    	varnode* temp = head;
    	
        if (!head) {
            head = (varnode*)malloc(sizeof(varnode));/*name assignment if first macro*/
		
	    if (!head) {
       		 printf("Error allocating memory on line 89 :firstRun.h\n");
        	 
   	    }
            head->next = NULL;
            strcpy(head->name, str);
	    head->memoryAddress = num;
	    head->entry = ent;
	    head->exter = exter;
	
            return head;
        }

        while (temp->next) {
	    
	    if(!strcmp(temp->name,str)){
		 
		
		if(ent){
			temp->entry = ent;
		}
		if(exter){
			temp->exter = exter;
		}
		return head;
	    }
            temp = temp->next;
        }

        temp->next = (varnode*)malloc(sizeof(varnode));
	if (!temp->next) {
       		 printf("Error allocating memory on line 107 :firstRun.h\n");
        	 
   	}
        temp->next->next = NULL;
        strcpy(temp->next->name, str);
	temp->next->memoryAddress = num;
	temp->next->entry = ent;
	temp->next->exter = exter;
	
        return head;


}
/*add instruction to list*/
instructionNode* addInstructionNode(instructionNode* head, instruction* instr) {
    instructionNode* temp = head;
    instructionNode* new_node;

    if (!head) {
        head = (instructionNode*)malloc(sizeof(instructionNode));
	if (!head) {
       		 printf("Error allocating memory on line 126 :firstRun.h\n");
        	 
   	}
        head->next = NULL;
        head->instruct = instr;
        return head;
    }

    while (temp->next) {
        temp = temp->next;
    }

    new_node = (instructionNode*)malloc(sizeof(instructionNode));
    if (!new_node) {
       		 printf("Error allocating memory on line 140 :firstRun.h\n");
        	 
    }
    new_node->next = NULL;
    new_node->instruct = instr;
    temp->next = new_node;

    return head;
}

/*print instruction list*/
void printInsList(instructionNode* head) {
    	int i;
	instructionNode* temp = head;
    

    while (temp) {
       
            
        
            if(temp->instruct->operation[0]=='\0'){
			if(temp->instruct->data[0] == '\0'){
				printf("{%d}, %s\n", temp->instruct->memoryAddress , temp->instruct->string);
				temp = temp->next;
				continue;
				
			}



            		for (i = 0; temp->instruct->data[i] != '\0'; i++) {
				if(i ==0){
	       			 	printf("{%d}, ",temp->instruct->memoryAddress);
				}
               			printf("%d, ",temp->instruct->data[i]);
				if(temp->instruct->data[i+1] == '\0'){
					 printf("\n");	
				}
           		}
	     }else{
			printf("{%d},%s, %s, %s\n", temp->instruct->memoryAddress, temp->instruct->operation, temp->instruct->opSource,
                        temp->instruct->opDestination);
	     }
	   
        
        
             temp = temp->next;
    }
}
/*will free a list of var nodes*/
void freeVarList(varnode* head) {
    
    varnode* temp = head;
    varnode* next;
    if(!head){ return;}

    while (!temp) {
        next = temp->next;
	
        free(temp);
        temp = next;
    }
}

void freeInsList(instructionNode* head) {
    
    instructionNode* temp = head;
    instructionNode* next;
    if(!head){ return;}

    while (!temp) {
        next = temp->next;
	
        free(temp);
        temp = next;
    }
}













#endif
