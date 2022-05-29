#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *prev, *next;
}Node;

int Size;

int size()
{
    return Size;
}

void swap(Node *node1, Node *node2)
{
    int tmp = node1->data;
    node1->data = node2->data;
    node2->data = tmp;
}

int get(Node *head, int index)
{
    Node* cur = head->next;
    for(int i = 0 ; i < index ; i++) {
        cur = cur->next;
    }
    return cur->data;
}
Node* get_node(Node *head, int index)
{
    Node* cur = head->next;
    for(int i = 0 ; i < index ; i++) {
        cur = cur->next;
    }
    return cur;
}

void sort(Node *head)
{
    int length = size();

    Node *cur = head;
    for(int i = 1 ; i < length ; i++) {
        cur = get_node(head, i);
        for(int j = i ; j > 0 ; j--) {
            if(get(head, j) < get(head, j-1)) {
                swap(cur, cur->prev);
            }
            cur = cur->prev;
        }
    }
}

int insert_after(Node* cur, int d) // insert after cur node
{
    Node* new = (Node*)malloc(sizeof(Node));

    new->data = d;
    new->next = cur->next;
    new->prev = cur;

    cur->next = new;
    new->next->prev = new;
    Size++;
    return 1;
}

int delete(Node *head, Node *last , int index) //delete cur node
{
    if(Size==0) {
        return 0;
    } else if(Size==1) {
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
    Size--;
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
    
    Size++;
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
    Size++;

    return 1;
}

int pop_back(Node *head, Node *last)
{
    if(Size==0) {
        return 0;
    } else if(Size==1) {
        head->next=NULL;
    }

    last->prev = last->prev->prev;
    free(last->prev->next);
    last->prev->next = NULL;
    Size--;
    return 1;
}

int pop_front(Node *head, Node *last)
{
    if(Size==0) {
        return 0;
    } else if(Size==1) {
        last->prev=NULL;
    }

    head->next = head->next->next;
    free(head->next->prev);
    head->next->prev = NULL;
    Size--;
    return 1;
}

void clear(Node *head, Node *last)
{
    while(head->next != NULL) {
        pop_front(head, last);
    }

    head->prev = NULL;
    head->next = NULL;
    last->prev = NULL;
    head->next = NULL;
}

int empty(Node *head)
{
    if(head->next == NULL) {
        return 1;
    } else {
        return 0;
    }
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
    printf("Size: %d\n", Size);
}

int main(void)
{
    Node *current_node = NULL;

    Node *head, *last;
    head = (Node*)malloc(sizeof(Node));
    last = (Node*)malloc(sizeof(Node));

    //head and last initiallize
    clear(head, last);

    //push 1 to 10
    printf("push 1 to 10 to double linked list\n");
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

    printf("\ndelete front, back & 3rd node\n");
    
    pop_back(head, last);
    pop_front(head, last);
    delete(head, last, 3);
    
    print_status(head);


    //insert 87 and sort
    printf("\ninsert 87, 88, 90\n");
    current_node = get_node(head, 4);
    insert_after(current_node, 87);
    current_node = get_node(head, 3);
    insert_after(current_node, 88);
    current_node = get_node(head, 2);
    insert_after(current_node, 90);

    print_status(head);

    //sort
    printf("\nsort\n");
    sort(head);

    print_status(head);



    return 0;
}