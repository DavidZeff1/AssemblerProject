#ifndef MACROPARSER_H
#define MACROPARSER_H
#include "global.h"
#define SKIP_COMMENT if(c==';'){while(c!='\n'){ c = fgetc(fp);}countLine =0; continue;}	/* skip comment when parsing macros*/
#define CLOSE_EMPTYLINE if(c=='\n'){if(countLine == 1){countLine =0;continue;}countLine = 0;}/*  skip empty line when parsing macros*/
typedef struct macronode{	/*nodes which represent a macros name + its contents*/
	char name[100];
	char contents[100];
	struct macronode *next;
	

}macronode;

macronode* addNode(macronode* head, char* str, char* type) { /*will add node and assign name or concatnate contents*/
    macronode* temp = head;
    if (!strcmp("name", type)) {
        if (!head) {
            head = (macronode*)malloc(sizeof(macronode));/*name assignment if first macro*/
	    if (!head) {
       		 printf("Error allocating memory in :macroParser.h/addNode function\n");
        	 
   	    }
            head->next = NULL;
            strcpy(head->name, str);
            return head;
        }

        while (temp->next) {
            temp = temp->next;
        }

        temp->next = (macronode*)malloc(sizeof(macronode));
	
	 if (!temp->next) {
       		 printf("Error allocating memory in :macroParser.h/addNode function\n");
	}
        temp->next->next = NULL;
        strcpy(temp->next->name, str);
        return head;

    }

    while (temp->next) {
        temp = temp->next;

    }
    strcat(temp->contents, str);
    return head;


}


macronode* findNode(macronode* head, char* str) {/*will find node by name and return it*/
    macronode* temp = head;

    while (temp) {

        if (!strcmp(temp->name, str)) {

            return temp;
        }
        temp = temp->next;
    }

    return NULL;

}
void freeList(macronode* head) {/*free linked list*/
    macronode* current = head;
    while (current) {
        macronode* next = current->next;
        free(current);
        current = next;
    }
}

#endif
