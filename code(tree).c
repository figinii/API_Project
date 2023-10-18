#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FORWARD 'F'
#define BACKWORD 'B'
#define MAX_CAR 512
#define RED 1
#define BLACK 0

typedef struct node {
    int val;
    struct node *next;
    struct node *prev;
} node_t;

typedef struct station{
    int position; //the position of the station from the start
    
    struct station *leftChild;
    struct station *rightChild;
    struct station *parent;

    int color; // 0 = black, 1 = red

    struct station *next; //the next station on the road
    struct station *prev;

    node_t *cars; //the head to the list of cars
    int car_num;
    
}station_t;

typedef struct{
    station_t* nil;
    station_t *root;
    
    station_t *first;
    station_t *last;
}tree_t;

typedef struct s_node{
    station_t* station;
    
    struct s_node *next;
    struct s_node *prev;
}path_node_t;

tree_t* plant_seed(void);

int compare(const void *first, const void *second);

void left_rotate(tree_t* T, station_t* node);
void right_rotate(tree_t* T, station_t* node);

void insert_leaf(tree_t* T, station_t* node);
void rb_insert_fixup(tree_t* T, station_t* node);

station_t* add_station(tree_t* head, int pos, int car_num, int* car_arr);
void insert_leaf(tree_t* T, station_t* node);
void rb_insert_fixup(tree_t* T, station_t* node);
station_t* in_order_successor(tree_t* T, station_t* node);
station_t* min_node(station_t *root, station_t *leaf);

station_t* search_station(station_t *root, station_t *leaf, int position);

int remove_station(tree_t *T, int pos);
void remove_leaf(tree_t* T, station_t* node);
station_t* rb_transplant(tree_t* T, station_t* original, station_t* replacement);
void rb_remove_fixup(tree_t* T, station_t* node);

int add_car(tree_t* head, int pos, int car_val);
int remove_car(tree_t* head, int pos, int car_val);
node_t* search_car(node_t *head, int car_val);

void deallocate_list(node_t *head);

station_t* furthest_node_backward(tree_t* T, station_t *start_node, int end);
int furthest_node(station_t *node, station_t **last_stat);
int best_path_forward(tree_t* head, int start, int end, node_t **result);
int best_path_backward(tree_t* head, int start, int end, path_node_t **result);
void add_node(int val, node_t **path);

void best_path_backward_correct(path_node_t *path);

void deallocate_all(tree_t *T);

void add_node_path(station_t* node, path_node_t** path);
void deallocate_list_path(path_node_t *head);


int main(int argc, char *argv[]){
    char c;
    int scan;

    int position, direction; 
    int car, res_code, num_car, *car_arr;
    int start, end;
    node_t *path, *tmp;
    station_t* res_stat;
    tree_t* road = plant_seed();
    path_node_t *path_back, *tmp_back;


    c = getchar();
    while(c != EOF){
        if(c == 'd'){
            while(c != ' '){
                c = getchar();
            }
            scan = scanf("%d", &position);
            
            res_code = remove_station(road, position);
            if(res_code)
                printf("demolita\n");
            else
                printf("non demolita\n");
        }else if(c == 'r'){
            while(c != ' '){
                c = getchar();
            }

            scan = scanf("%d", &position);
            c = getchar();
            scan = scanf("%d", &car);
            
            res_code = remove_car(road, position, car);
            if(res_code)
                printf("rottamata\n");
            else
                printf("non rottamata\n");
        }else if(c == 'a'){
            while(c != '-')
                c = getchar();
            c = getchar();
            if(c == 's'){
                while(c != ' ')
                    c = getchar();
                
                scan = scanf("%d", &position);
                c = getchar();
                scan = scanf("%d", &num_car);
                c = getchar();

                car_arr = (int*)malloc(sizeof(int) * num_car);
                for(int i = 0; i < num_car; i++){
                    scan = scanf("%d", &car);
                    car_arr[i] = car;
                    c = getchar();
                }

                qsort(car_arr, num_car, sizeof(int), compare);
                                
                res_stat = add_station(road, position, num_car, car_arr);

                if(res_stat != NULL)
                    printf("aggiunta\n");
                else
                    printf("non aggiunta\n");
                free(car_arr);

            }else if(c == 'a'){
                while(c != ' ')
                    c = getchar();
                
                scan = scanf("%d", &position);
                c = getchar();
                scan = scanf("%d", &car);
                
                res_code = add_car(road, position, car);
                if(res_code)
                    printf("aggiunta\n");
                else
                    printf("non aggiunta\n");
            }
        }else if(c == 'p'){
            while(c != ' ')
                c = getchar();

            scan = scanf("%d", &start);

            c = getchar();

            scan = scanf("%d", &end);

            if(end >= start){
                res_code = best_path_forward(road, start, end, &path); 
                direction = 1;
            }else{
                res_code = best_path_backward(road, start, end, &path_back);
                direction = -1;
            }
            if(res_code){
                if(direction > 0){
                    while(path != NULL){
                        printf("%d ", (path->val));
                        tmp = path->next;
                        free(path);
                        path = tmp;
                    }
                    printf("%d\n", end);
                }else{
                    best_path_backward_correct(path_back);
                    while(path_back->next != NULL){
                        path_back = path_back->next;
                    }
                    
                    while(path_back->prev != NULL){
                        printf("%d ", path_back->station->position);
                        tmp_back = path_back->prev;
                        free(path_back);
                        path_back = tmp_back;
                    }
                    printf("%d", path_back->station->position);
                    printf("\n");
                }
                    
            }else{
                printf("nessun percorso\n");
            }
          
        }
        c = getchar();
    }
    scan += 1; 
    /*
        while(road->root != road->nil){
            remove_station(road, road->root->position);
        }
        free(road->nil);
        free(road);
    */

    //deallocate_all(road);
    return 0;
}


tree_t* plant_seed(void){
    tree_t *earth;

    earth = (tree_t*)malloc(sizeof(tree_t));
    earth->nil = (station_t*)malloc(sizeof(station_t));
    (earth->nil)->position = -1;
    (earth->nil)->color = BLACK;
    earth->root = earth->nil;
    earth->nil->rightChild = earth->nil;
    earth->nil->leftChild = earth->nil;

    earth->first = NULL;
    earth->last = NULL;
    
    return earth;
}

int compare(const void *first, const void *second){
    
    return ( *(int*)second - *(int*)first);
}


station_t* add_station(tree_t* head, int pos, int car_num, int* car_arr){
    
    if(search_station(head->root, head->nil, pos) != head->nil)
        return NULL;
    
    //creo il nuovo nodo e setto i valori
    station_t *new_station = (station_t*)malloc(sizeof(station_t));
    new_station->position = pos;

    
    //inserisci nell'albero
    insert_leaf(head, new_station);
   
    new_station->car_num = car_num;
    //trasformo l'array di macchine in una lista; alla base aggiungo uno 0
    node_t *head_cars;
    node_t *new_car = (node_t*)malloc(sizeof(node_t));
    node_t *tmp = NULL;
    
    head_cars = new_car;
    new_station->cars = head_cars;

    for(int i = 0; i < car_num; i++){

        new_car->val = car_arr[i];
        new_car->prev = tmp;
        new_car->next = (node_t*)malloc(sizeof(node_t));
        tmp = new_car;
        new_car = new_car->next;
    }
    new_car->val = 0;
    new_car->prev = tmp;
    new_car->next = NULL;

    return new_station;
}
void insert_leaf(tree_t* T, station_t* node){
    station_t *search_node, *son;
    station_t *successor;

    search_node = T->nil;
    son = T->root;

    while(son != T->nil){
        search_node = son;
        if(node->position < son->position)
            son = son->leftChild;
        else
            son = son->rightChild;
    }
    node->parent = search_node;
    if(search_node == T->nil){
        T->root = node;
    }else if(node->position < search_node->position)
        search_node->leftChild = node;
    else
        search_node->rightChild = node;
    node->leftChild = T->nil;
    node->rightChild = T->nil;
    node->color = RED;
    
    rb_insert_fixup(T, node);

    if(T->first == NULL){
        T->first = node;
        node->next = NULL;
        node->prev = NULL;
        T->last = node;
    }else{
        successor = in_order_successor(T, node);
        if(successor == T->nil){
            T->last->next = node;
            node->prev = T->last;
            node->next = NULL;
            T->last = node;
        }else{
            node->next = successor;
            node->prev = successor->prev;
            successor->prev = node;
            if(node->prev != NULL)
                node->prev->next = node;
            if(successor == T->first)
                T->first = node;
                
        }
            
    }
}
void rb_insert_fixup(tree_t* T, station_t* node){
    station_t *search_node;

    while((node->parent)->color == RED){
        if(node->parent == node->parent->parent->leftChild){
            search_node = node->parent->parent->rightChild;
            if(search_node->color == RED){
                node->parent->color = BLACK;
                search_node->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }else{
                if(node == node->parent->rightChild){
                    node = node->parent;
                    left_rotate(T, node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                right_rotate(T, node->parent->parent);                
            }
                
        }else{
            search_node = node->parent->parent->leftChild;
            if(search_node->color == RED){
                node->parent->color = BLACK;
                search_node->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }else{
                if(node == node->parent->leftChild){
                    node = node->parent;
                    right_rotate(T, node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                left_rotate(T, node->parent->parent);                
            }
        } 
    }
    T->root->color = BLACK;
}
station_t* in_order_successor(tree_t* T, station_t* node){
    station_t *search_node, *left_partent;
    
    left_partent = T->nil;
    search_node = T->root;
    while(search_node != node){
        if(search_node->position > node->position){
            left_partent = search_node;
            search_node = search_node->leftChild;
        }else
            search_node = search_node->rightChild;
    }
    if (search_node->rightChild != T->nil)
        return min_node(node->rightChild, T->nil);
    else
        return left_partent;
        
}
station_t* min_node(station_t *root, station_t *leaf){
    if(root->leftChild == leaf)
        return root;
    else
        return min_node(root->leftChild, leaf);
}


void left_rotate(tree_t* T, station_t* node){
    station_t* rotated;

    rotated = node->rightChild;
    node->rightChild = rotated->leftChild;
    rotated->parent = node->parent;

    if(rotated->leftChild != T->nil)
        (rotated->leftChild)->parent = node;
    if(node->parent == T->nil)
        T->root = rotated;
    else if(node == node->parent->leftChild)
        (node->parent)->leftChild = rotated;
    else
        (node->parent)->rightChild = rotated;
    rotated->leftChild = node;
    node->parent = rotated;
}

void right_rotate(tree_t* T, station_t* node){
    station_t* rotated;

    rotated = node->leftChild;
    node->leftChild = rotated->rightChild;
    rotated->parent = node->parent;

    if(rotated->rightChild != T->nil)
        (rotated->rightChild)->parent = node;
    if(node->parent == T->nil)
        T->root = rotated;
    else if(node == node->parent->rightChild)
        (node->parent)->rightChild = rotated;
    else
        (node->parent)->leftChild = rotated;
    rotated->rightChild = node;
    node->parent = rotated;
}


station_t* search_station(station_t *root, station_t *leaf, int position){
    if(root->position == position || root == leaf)
        return root;
    else if(root->position > position)
        return(search_station(root->leftChild, leaf, position));
    else
        return(search_station(root->rightChild, leaf, position));
}
station_t* rb_transplant(tree_t* T, station_t* original, station_t* replacement){
    if(original->parent == T->nil)
        T->root = replacement;
    else if(original == original->parent->leftChild)
        original->parent->leftChild = replacement;
    else
        original->parent->rightChild = replacement;
    replacement->parent = original->parent;

    return original;
}



int remove_station(tree_t* T, int pos){
    station_t* to_remove;

    to_remove = search_station(T->root, T->nil, pos);

    if(to_remove == T->nil)
        return 0;
    
    remove_leaf(T, to_remove);
    T->root->color = BLACK;
    
    if(to_remove->next != NULL)
        to_remove->next->prev = to_remove->prev;
    else
        T->last = to_remove->prev;

    if(to_remove->prev != NULL)
        to_remove->prev->next = to_remove->next;
    else
        T->first = to_remove->next;

    deallocate_list(to_remove->cars);
    free(to_remove);
    return 1;
}
void remove_leaf(tree_t* T, station_t* node){
    station_t *successor;
    
    //caso 1: node non ha figli 
    if(node->leftChild == T->nil && node->rightChild == T->nil){
        if(node->color == BLACK)
            rb_remove_fixup(T, node);
        
        node = rb_transplant(T, node, T->nil);
        
    //caso 2: node ha un solo figlio
    }else if(node->leftChild == T->nil){
        if(node->color == BLACK)
            rb_remove_fixup(T, node);

        node = rb_transplant(T, node, node->rightChild);
    }else if(node->rightChild == T->nil){
        if(node->color == BLACK)
            rb_remove_fixup(T, node);

        node = rb_transplant(T, node, node->leftChild);
    //caso 3: ha entrambi i figli
    }else{
        successor = min_node(node->rightChild, T->nil);
        remove_leaf(T, successor);
        // successor->parent->leftChild = successor->rightChild;
        node = rb_transplant(T, node, successor);
        successor->leftChild = node->leftChild;
        successor->rightChild = node->rightChild;
        successor->leftChild->parent = successor;
        node->rightChild->parent = successor;
        successor->color = node->color;
    }

}
void rb_remove_fixup(tree_t* T, station_t* node){
    station_t* brother;
    
    //se è la radice quella che tolgo, basta tirar su il figlio e colorarlo di nero se necessario
    if(node == T->root){
        T->root->color = BLACK;
        return;
    }
    // se il mio nodo è il figlio sinistro
    if(node == node->parent->leftChild){  
        brother = node->parent->rightChild;
        
        // se non ha un fratello 
        if(brother == T->nil){
            if(node->parent->color == RED)
                node->parent->color = BLACK;
            else{
                rb_remove_fixup(T, node->parent);
            }
        }else if(brother->color == RED){
            node->parent->color = RED;
            brother->color = BLACK;
            left_rotate(T, node->parent);
            rb_remove_fixup(T, node);
        }else{
            if(brother->leftChild->color == BLACK && brother->rightChild->color == BLACK)
                brother->color = RED;
            else if(brother->rightChild->color == RED){
                brother->color = node->parent->color;
                node->parent->color = BLACK;
                brother->rightChild->color = BLACK;
                left_rotate(T, node->parent);
            }else{
                brother->color = RED;
                brother->leftChild->color = BLACK;
                right_rotate(T, brother);
                rb_remove_fixup(T, node);
            }
        }
    }else{        
        brother = node->parent->leftChild;
        
        if(brother == T->nil){
            if(node->parent->color == RED)
                node->parent->color = BLACK;
            else{
                rb_remove_fixup(T, node->parent);
            }
        }else if(brother->color == RED){
            node->parent->color = RED;
            brother->color = BLACK;
            right_rotate(T, node->parent);
            rb_remove_fixup(T, node);
        }else{
            if(brother->rightChild->color == BLACK && brother->leftChild->color == BLACK)
                brother->color = RED;
            else if(brother->leftChild->color == RED){
                brother->color = node->parent->color;
                node->parent->color = BLACK;
                brother->leftChild->color = BLACK;
                right_rotate(T, node->parent);
            }else{
                brother->color = RED;
                brother->rightChild->color = BLACK;
                left_rotate(T, brother);
                rb_remove_fixup(T, node);
            }
        }
    }
    T->root->color = BLACK;
}   

int add_car(tree_t* head, int pos, int car_val){
    station_t* station;
    node_t *car_list;
    node_t *new_car;
    
    station = search_station(head->root, head->nil, pos);
    if(station == head->nil)
        return 0;

    if(station->car_num == MAX_CAR)
        return 0;
    
    new_car = (node_t*)malloc(sizeof(node_t));
    new_car->val = car_val;

    if(station->car_num == 0){
        
        new_car->next = station->cars;
        station->cars = new_car;
        
        new_car->next->prev = new_car;
        new_car->prev = NULL; 
        

        station->car_num += 1;
        return 1;    
    }
    car_list = station->cars;
    while(car_list->next != NULL){
        if(car_val >= car_list->val){
            new_car->next = car_list;
            new_car->prev = car_list->prev;

            if(car_list->prev != NULL)
                (car_list->prev)->next = new_car;
            else
                station->cars = new_car;

            car_list->prev = new_car;

            station->car_num += 1;
            return 1;  
        }
        car_list = car_list->next;
    }
    new_car->next = car_list;
    new_car->prev = car_list->prev;

    if(car_list->prev != NULL)
        (car_list->prev)->next = new_car;

    car_list->prev = new_car;

    station->car_num += 1;
    return 1;  

}
int remove_car(tree_t* head, int pos, int car_val){
    station_t *station_list;
    node_t *head_car;
    node_t *car_list;

    station_list = search_station(head->root, head->nil, pos);
    if(station_list == head->nil)
        return 0;
    
    head_car = station_list->cars;
    car_list = search_car(head_car, car_val);
    if(car_list == NULL)
        return 0;
    
    if(car_list->prev != NULL)
        (car_list->prev)->next = car_list->next;
    
    if(car_list->next != NULL){
        (car_list->next)->prev = car_list->prev;
        if(car_list->prev == NULL)
            station_list->cars = car_list->next;
    }
    
    free(car_list);
    station_list->car_num -= 1;

    return 1;
}
node_t* search_car(node_t *head, int car_val){
    node_t *search;

    search = head;
    while(search->next != NULL){
        if(search->val == car_val)
            return search;
        if(search->val < car_val)
            return NULL;
        search = search->next;
    }
    if(search->val == car_val)
            return search;
    return NULL;
}

void deallocate_list(node_t *head){
    node_t *tmp;
    
    while(head->next != NULL){
        tmp = head->next;
        free(head);
        head = tmp;
        tmp = NULL;
    }
    if(head != NULL)
        free(head);
}

int best_path_forward(tree_t* head, int start, int end, node_t **result){
    station_t *start_st, *end_st;
    station_t *best_node, *search_node;
    node_t *path = NULL, *new_node;

    start_st = search_station(head->root, head->nil, start);
    end_st = search_station(head->root, head->nil, end);
    
    if(start_st == head->nil || end_st == head->nil)
        return 0;

    while(start_st != end_st){
        best_node = NULL;        
        search_node = end_st->prev;
        
        while(search_node != start_st->prev){
            if(search_node->position + search_node->cars->val >= end_st->position){
                best_node = search_node;
            }
            search_node = search_node->prev;
        }

        if(best_node == NULL){
            if(path != NULL)
                deallocate_list(path);
            return 0;
        }else{
            new_node = (node_t*)malloc(sizeof(node_t));
            new_node->val = best_node->position;

            if(path == NULL){
                path = new_node;
                new_node->next = NULL;
                new_node->next = NULL;
            }else{//inserire il nodo mantenendo l'ordine
                path->prev = new_node;
                new_node->next = path;
                new_node->prev = NULL;
                path = new_node;
            }            
        }
        end_st = best_node;
    }
    *result = path;
    return 1;
}
int best_path_backward(tree_t* head, int start, int end, path_node_t **result){
    station_t *search_node, *best_node = NULL;
    int possible = 1, last = 0;
    path_node_t *path;
    int pos, distance;
    station_t *best_node_fr, *search_node_fr; //fr = furthest reach
        
    
    search_node = search_station(head->root, head->nil, start);

    if(search_node == head->nil)
        possible = 0;

    path = NULL;
    add_node_path(search_node, &path);

    pos = search_node->position;

    while(possible && !last && search_node->position != end){
        pos = search_node->position;
        distance = (search_node->cars)->val;

        search_node = search_node->prev;
        best_node = NULL;
        while(search_node != NULL && possible && !last && pos - search_node->position <= distance){
            if(best_node == NULL){
                search_node_fr = furthest_node_backward(head, search_node, end);
                    best_node = search_node;
                    best_node_fr = search_node_fr;  
                if(search_node_fr->position - end <= 0)
                    last = 1;
            }else{
                search_node_fr = furthest_node_backward(head, search_node, end);
                if(search_node_fr->position <= best_node_fr->position){
                    best_node = search_node;
                    best_node_fr = search_node_fr;
                }
                if((search_node_fr->position <= end))
                    last = 1;
            }
            search_node = search_node->prev;
        }
        if(best_node == NULL)
            possible = 0;
        else if(!last){
            add_node_path(best_node, &path);
            search_node = best_node;
        }
    }
    if(last && possible){
        while(pos - search_node->position <= distance && search_node->position > end){
            search_node_fr = furthest_node_backward(head, search_node, end);
            if(search_node_fr->position <= end)
                best_node = search_node;
            search_node = search_node->prev;
        }
        add_node_path(best_node, &path);
        add_node_path(search_station(head->root, head->nil, end), &path);
    }



    if(!possible){
        deallocate_list_path(path);
        return 0;
    }else{


        *result = path;
        return 1;
    }
}

int furthest_node(station_t *node, station_t **last_stat){
    station_t *furt = NULL, *start_node = NULL;
    int pos = node->position;
    int car = node->cars->val;

    while((node->prev != NULL) && ((pos - node->position) - car <= 0)){
        furt = node;
        node = node->prev;
    }
    if((pos - node->position) - car <= 0)
        furt = node;
    if(furt == start_node)
        return 0;
    else{
        if((pos - node->position) - car <= 0)
            furt = node;
        *last_stat = furt;
        return 1;
    }

}

void add_node(int val, node_t **path){
    node_t *new_node;

    new_node = (node_t*)malloc(sizeof(node_t));
    new_node->val = val;

    if(*path == NULL){
        *path = new_node;
        new_node->next = NULL;
        new_node->next = NULL;
    }else{//inserire il nodo mantenendo l'ordine
        (*path)->prev = new_node;
        new_node->next = *path;
        new_node->prev = NULL;
        *path = new_node;
    }
}
void deallocate_all(tree_t *T){
    if(T->root == T->nil)
        return;
    else{
        remove_station(T, T->root->position);
        deallocate_all(T);
    }
    return;
}

void best_path_backward_correct(path_node_t *path){
    station_t *search_node, *best_node;
    
    best_node = NULL;
    if(path->next->next != NULL)
        path = path->next->next;
    while(path->next != NULL){
        
        search_node = path->station->prev;
        while(search_node != (path->prev->prev)->station){
            if((search_node->position - search_node->cars->val <= path->prev->prev->station->position) && 
            (search_node->position >= path->station->position - path->station->cars->val))
                best_node = search_node;
            search_node = search_node->prev;
        }
        if(best_node != NULL)
            path->prev->station = best_node;
        path = path->next;
    }
    search_node = path->station->prev;
    while(search_node != (path->prev->prev)->station){
        if((search_node->position - search_node->cars->val <= path->prev->prev->station->position) && 
        (search_node->position >= path->station->position - path->station->cars->val))
            best_node = search_node;
        search_node = search_node->prev;
    }
    path->prev->station = best_node;
}
void add_node_path(station_t* node, path_node_t** path){
    path_node_t *new_node;

    new_node = (path_node_t*)malloc(sizeof(path_node_t));
    new_node->station = node;

    if(*path == NULL){
        *path = new_node;
        new_node->next = NULL;
        new_node->prev = NULL;
    }else{
        (*path)->prev = new_node;
        new_node->next = *path;
        new_node->prev = NULL;
        *path = new_node;
    }
}
void deallocate_list_path(path_node_t *head){
    path_node_t *tmp;
    
    while(head->next != NULL){
        tmp = head->next;
        free(head);
        head = tmp;
    }
    free(head);
}

station_t* furthest_node_backward(tree_t* T, station_t *start_node, int end){
    int distance = start_node->position - start_node->cars->val;
    station_t *search_node;

    if(distance <= end)
        return search_station(T->root, T->nil, end);
    
    
    search_node = search_station(T->root, T->nil, distance);
    if(search_node == T->nil){
        add_station(T, distance, 0, NULL);
        search_node = search_station(T->root, T->nil, distance);
        search_node = search_node->next;
        remove_station(T, distance);
    }
    return search_node;
}