//Jason Huang
//October 10, 2022
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "shakespeare.h"
#define BUFFER 1024


Node *add_to_list(char *w, Node *l) {
    Node* new_word;
    new_word = (Node*)calloc(1, sizeof(Node));
    new_word->word = strdup(w);
    new_word->counter =1;
    new_word->next = l;
    return new_word;
}

void add_to_LinkedList(char *w, LinkedList*l) {
    int len = 0;
    for (int i = 0; w[i] != '\0'; i++){
        len++;
    }

    Node *linkedlist = l->ll[len-5];
    l->ll[len-5] = add_to_list(w, linkedlist);
    return;
}

bool look_up(char *w, LinkedList *list) {
    int len =0;

    for (int i = 0; w[i] != '\0'; i++){
        
        len++;
    }
    // printf("%s : %d\n", w, len);
    // if(len <5 ){
    //     return false;
    // }
    Node *here = list->ll[len-5];
    while(here!=NULL) {
        if (strcmp(here->word, w) == 0) {
            here->counter++;
            return true;
        }
    here = here->next;
  }
  return false;
}

void slowfast(Node* s,Node** f, struct Node** b)
{
    Node* fast;
    Node* slow;
    slow = s;
    fast = s->next;
    //fast moves 2 for evrye slow moves 1 to find middle
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
 
    *f = s;
    *b = s->next;
    s->next = NULL;
}

Node* alpha(Node *x, Node*y){
    Node* z = NULL;
    //base
    if (x == NULL) return (y);
    else if (y == NULL) return (x);
 
    //choose x or y
    if (x->counter == y->counter && strcmp(x->word, y->word)<0) {
        z = x;
        z->next = alpha(x->next, y);
    }
    else {
        z = y;
        z->next = alpha(x, y->next);
    }
    return (z);    
}

void lexi(Node **h){

    Node* head = *h;
    Node* x, *y =NULL;
    //base case for recursive apporach
    if ((head == NULL) || (head->next == NULL)) return;
    //split the list into 2
    slowfast(head, &x, &y);
    //sort the sublists 
    lexi(&x);
    lexi(&y);
    //merge the 2 lists beack together
    *h = alpha(x, y);
}

Node* Sorted(Node* x, Node* y)
{
    Node* z = NULL;
    //base
    if (x == NULL)
        return (y);
    else if (y == NULL)
        return (x);
 
    //choose x or y but in descending order
    if (x->counter >= y->counter) {
        z = x;
        z->next = Sorted(x->next, y);
    }
    else {
        z = y;
        z->next = Sorted(x, y->next);
    }
    return (z);
}


void Merge(Node** h)
{
    Node* head = *h;
    Node* x, *y =NULL;
    //base case for recursive apporach
    if ((head == NULL) || (head->next == NULL)) return;
    //split the list into 2
    slowfast(head, &x, &y);
    //sort the sublists 
    Merge(&x);
    Merge(&y);
    //merge the 2 lists beack together
    *h = Sorted(x, y);
}


LinkedList *build_list(size_t num) {
  LinkedList* output = (LinkedList*)calloc(1, sizeof(LinkedList));
  output->n = num;
  output->ll = calloc(num, sizeof(Node*));

  return output;
}

void remove_new_lines(char *s){
    if(s == NULL){
        return;
    }
    char *newline;
    newline = strchr(s, '\n');
    if (newline){
        *newline = '\0';
    }
}

void PrintList(LinkedList *list) {

  for (int i=0; i < list->n; i++) {
    Node *here = list->ll[i];
    while(here) {
        printf("index: %d, %s: %ld\n", i, here->word, here->counter);
        here = here->next;
    }
  }
  return;
}

int main(int argc, char **argv){

    FILE *input;
    FILE *output;
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
    if(argc<3){
        printf("Error Couldn't Open File.\n");
    }       

    LinkedList *list = build_list(32);
    FILE *file;
    file = fopen("shakespeare-cleaned5.txt", "r");
    if(!file) exit(1);
    //printf("opened\n");
    char buff[BUFFER];
    char b[BUFFER];
    // Node *t = NULL;
    int i =0;
    // int countme=0;
    char *word;
    char* save = "%s\n";
    while (fgets(buff, BUFFER, file) != NULL){
        // remove_new_lines(buff);
        sscanf(buff, "%s ", b);
        // printf("Read this: %s. \n", b);
        word = strdup(b);

        // word = strdup(buff);
        //printf("copied: %s. \n", word); 
        i = look_up(word, list);
        if(i ==0){
            add_to_LinkedList(word, list);

        }
    }

    for(int i = 0; list->ll[i]; i++){
        lexi(&list->ll[i]);
    }

    for(int i = 0; list->ll[i]; i++){
        Merge(&list->ll[i]);
    }

    //PrintList(list);

    int length;
    int rank;
    char*store;
    

    while(fgets(buff, BUFFER, input)!=NULL){
        remove_new_lines(buff);
        store = strtok(buff, " ");
        length = atoi(store);
        store = strtok(NULL, " ");
        rank = atoi(store);
        int counter = 0;
        char*save_file;
        char* dash = "-";
        if (length<=list->n){
            Node *here = list->ll[length-5];
            if (here == NULL){
                fprintf(output, save, dash);
            }
            while(here!=NULL){
                save_file=NULL;
                if(counter == rank){
                    save_file = strdup(here->word);
                    fprintf(output, save, save_file);
                    break;
                }
                
                counter++;
                here = here->next;
                if (here == NULL){
                    fprintf(output, save, dash);
                }
            }

        } else if (length> list->n){
            fprintf(output, save, dash);
        }
    }
        
    fclose(input);
    fclose(output);
    fclose(file);
}
