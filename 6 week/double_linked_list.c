#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *prev, *next;
}Node;

int size;

int insert_after(Node* cur, int d) // insert after cur node
{
    Node* new = malloc(sizeof(Node));
    new->data = d;
    new->next = cur->next;
    new->prev = cur;

    cur->next = new;
    new->next->prev = new;
    size++;
    return 1;
}

int delete(Node *head, Node *last , int index) //delete cur node
{
    if(size==0) {
        return 0;
    } else if(size==1) {
        head->next=NULL;
    }

    Node* cur = head->next;
    for(int i = 0 ; i < index ; i++) {
        cur = cur->next;
        if(cur == NULL) {
            return 0;
        }
    }

    

    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;

    free(cur);
    size--;
    return 1;
}

int push_back(Node *head, Node *last, int d)
{
    Node* new = (Node*)malloc(sizeof(Node));
    new->data = d;
    new->next = NULL;
    new->prev = last->prev;
    
    if(new->next == NULL&& new->prev==NULL) {
        head->next = new;
    }

    if(last->prev) {
        last->prev->next = new;
    }
    last->prev = new;
    
    size++;
    return 1;
}

int push_front(Node *head, Node *last, int d)
{
    Node* new = (Node*)malloc(sizeof(Node));
    new->data = d;
    new->next = head->next;
    new->prev = NULL;

    if(new->next == NULL && new->prev==NULL) {
        last->prev = new;
    }

    if(head->next != NULL) {
        head->next->prev = new;
    }
    
    head->next = new;
    size++;

    return 1;
}

int pop_back(Node *head, Node *last)
{
    if(size==0) {
        return 0;
    } else if(size==1) {
        head->next=NULL;
    }

    last->prev = last->prev->prev;
    free(last->prev->next);
    last->prev->next = NULL;
    size--;
    return 1;
}

int pop_front(Node *head, Node *last)
{
    if(size==0) {
        return 0;
    } else if(size==1) {
        last->prev=NULL;
    }

    head->next = head->next->next;
    free(head->next->prev);
    head->next->prev = NULL;
    size--;
    return 1;
}

void print_list(Node* node) {
    while(node != NULL) {
        printf("%d -> ", node->data);
        node = node->next;
    }
    printf("NULL\n");
}

void print_status(Node *head) {
    print_list(head->next);
    printf("size: %d\n", size);
}

int main(void)
{
    Node *current_node = NULL;

    Node *head, *last;
    head = (Node*)malloc(sizeof(Node));
    last = (Node*)malloc(sizeof(Node));

    //head and last initiallize
    head->next = NULL;
    head->prev = NULL;
    last->next = NULL;
    last->prev = NULL;


    //push 1 to 10
    push_back(head, last, 1);
    push_back(head, last, 2);
    push_back(head, last, 3);
    push_back(head, last, 4);
    push_back(head, last, 5);
    push_back(head, last, 6);
    push_back(head, last, 7);
    push_back(head, last, 8);
    push_back(head, last, 9);
    push_back(head, last, 10);

    print_status(head);

    //delete front, back & any node
    
    pop_back(head, last);
    pop_front(head, last);
    delete(head, last, 3);
    
    print_status(head);

    return 0;
}