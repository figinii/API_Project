#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// two label to identify if I'm going on the road from left to right(forward) or from left to right(backword)
#define FORWARD 'F'
#define BACKWORD 'B'
#define MAX_CAR 512

typedef struct node{
    int val;
    struct node *next;
    struct node *prev;
} node_t;

typedef struct station{
    int position; //the position of the station from the start
    
    node_t *cars; //the head to the list of cars
    int car_num;

    struct station *next; //the next station on the road
    struct station *prev; //the node before
}station_t;

typedef struct s_node{
    station_t* station;
    
    struct s_node *next;
    struct s_node *prev;
}path_node_t;

/*
    function takes in two values: 
    stat = the station i'm considering
    dir = direction; can be f for forward or b for backword
    it returns the most distance hypotetical station (wheter it exists or not) I can travel to
*/
int max_dis(station_t *stat, char dir);

void print_stations(station_t *stat);

/*
    returns what is the furthest station reacheble from a node
    int dir is the direction 1 means forward direction; -1 means baskward
    return 0 if doesn found anything
*/
int furthest_node(station_t *node, int dir, station_t **last_stat);

//compare function to order in decreasing order with qsort(<stdlib.h>)
int compare(const void *first, const void *second);

int digits_counter(int num);

char* int_to_char(int num);

//search the station with position == pos
//return NULL if it isnt present
station_t* search_station(station_t *head, int pos);

node_t* search_car(node_t *head, int car_val);

void deallocate_list(node_t *head);

void add_node(int val, node_t **path);

// insert a new station in the road list and return 1
// if it already exists a station in thet position it returns 0 and doesnt insert it
int add_station(station_t** head, int pos, int car_num, int* car_arr);

int remove_station(station_t** head, int pos);

int add_car(station_t* head, int pos, int car_val);

int remove_car(station_t* head, int pos, int car_val);



void add_node_path(station_t* node, path_node_t** path);
void deallocate_list_path(path_node_t *head);

//pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo
/*
    Richiede di pianificare il percorso con i vincoli sopra indicati.
    tampa attesa come risposta: le tappe in ordine di percorrenza, rappresentate con la distanza delle stazioni
    da inizio autostrada, separate da spazi e alla fine seguite da un a-capo. Devono essere incluse partenza e
    arrivo; se coincidono la stazione viene stampata una sola volta. Se il percorso non esiste, stampa nessun
    percorso. L’azione di pianificazione non altera le stazioni o il loro parco veicoli. Le stazioni date sono
    sicuramente presenti.
*/
int best_path_forward(station_t* head, int start, int end, node_t **result);

int best_path_backward(station_t* head, int start, int end, path_node_t **result);
void best_path_backward_correct(path_node_t *path);

int main(int argc, char *argv[]){
    char c;
    int position; 
    int car;
    int res_code;
    int num_car;
    int *car_arr;
    station_t* road = NULL;
    int start, end;
    node_t *path, *tmp;
    path_node_t *path_back, *tmp_back;
    int scan;
    int direction;
    
    c = getchar();
    while(c != EOF){
        if(c == 'd'){
            while(c != ' '){
                c = getchar();
            }
            scan = scanf("%d", &position);
            
            res_code = remove_station(&road, position);
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
                                
                res_code = add_station(&road, position, num_car, car_arr);

                if(res_code)
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
                if(direction < 0)
                    deallocate_list_path(path_back);
                printf("nessun percorso\n");
            }
                
        }
        /*
            else if(c == '/'){
                while(c != '\n'){
                    c = getchar();
                }
                printf("/\n");
            }
        */
        c = getchar();
    }
    scan += 1;
    
    //print_stations(road);

    return 0;
}



int max_dis(station_t *stat, char dir){
    int result;
    int max_len;
    
    max_len = (stat->cars)->val;

    if(dir == BACKWORD){
        result = stat->position + max_len;
    }else if(dir == FORWARD){
        result = stat->position - max_len;
    }else
        result = -1;
    
    return result;    
}

void print_stations(station_t *stat){
    while(stat->next != NULL){
        printf("%d (%d) -> ", stat->position, stat->cars->val);
        stat = stat->next;
    }
    printf("\n");
}

int furthest_node(station_t *node, int dir, station_t **last_stat){
    station_t *furt = NULL, *start_node;
    int pos = node->position;
    int car = node->cars->val;

    start_node = node;
    if(dir > 0){
        while((node->next != NULL) && ((node->position - pos) - car <= 0)){
            furt = node;
            node = node->next;
        }
        if((node->position - pos) - car <= 0)
            furt = node;
        if(furt == start_node)
            return 0;
        else{
            if((node->position - pos) - car <= 0)
                furt = node;
            *last_stat = furt;
            return 1;
        }
    }else{
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
}

int compare(const void *first, const void *second){
    
    return ( *(int*)second - *(int*)first);
}

int digits_counter(int num){
    int count = 0;

    while(num / 10 != 0){
        count++;
        num = num / 10;
    }
    return count + 1;
}

char* int_to_char(int num){
    char *r;

    r = (char*)malloc(sizeof(char)*digits_counter(num));
    
    sprintf(r, "%d", num);
    
    return r;
}

station_t* search_station(station_t *head, int pos){
    station_t *search;

    search = head;
    while(search->next != NULL){
        if(search->position == pos)
            return search;
        if(search->position > pos)
            return NULL;
        search = search->next;
    }
    if(search->position == pos)
            return search;
    return NULL;
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
    }
    free(head);
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

int add_station(station_t** head, int pos, int car_num, int* car_arr){
    station_t *insert;

    //find the first element greater than the one i want to insert
    insert = *head;
    
    if(insert != NULL){
        while(pos > insert->position && insert->next != NULL){
            insert = insert->next;
        }

        //if it already exists do nothing and return the 0 code
        if(insert->position == pos)
            return 0;
    }

    //creo il nuovo nodo e setto i valori
    station_t *new_station = (station_t*)malloc(sizeof(station_t));
    new_station->position = pos;

    //se è la prima
    if(*head == NULL)
        *head = new_station;
    
    if(insert != NULL && insert->position > pos){
        new_station->prev = insert->prev;
        if(insert->prev != NULL)
            (insert->prev)->next = new_station;
        
        new_station->next = insert;
        insert->prev = new_station;
        
        if(insert == *head)
            *head = new_station;
    }else{
        new_station->next = NULL;
        new_station->prev = insert;
        if(insert != NULL)
            insert->next = new_station;
    }
   
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

    return 1;
}

int remove_station(station_t** head, int pos){
    station_t *to_remove;

    to_remove = search_station(*head, pos);
    if(to_remove == NULL)
        return 0;

    if(to_remove->prev != NULL)
        (to_remove->prev)->next = to_remove->next;
    else
        *head = to_remove->next;
    if(to_remove->next != NULL)
        (to_remove->next)->prev = to_remove->prev;


    deallocate_list(to_remove->cars);
    free(to_remove);

    return 1;
}

int add_car(station_t* head, int pos, int car_val){
    station_t* station;
    node_t *car_list;
    node_t *new_car;
    
    station = search_station(head, pos);
    if(station == NULL)
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

int remove_car(station_t* head, int pos, int car_val){
    station_t *station_list;
    node_t *head_car;
    node_t *car_list;

    station_list = search_station(head, pos);
    if(station_list == NULL)
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

int best_path_forward(station_t* head, int start, int end, node_t **result){
    station_t *start_st, *end_st;
    station_t *best_node, *search_node;
    node_t *path = NULL, *new_node;

    start_st = search_station(head, start);
    end_st = search_station(head, end);
    
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



int best_path_backward(station_t* head, int start, int end, path_node_t **result){
    station_t *search_node, *best_node;
    int possible = 1, res, last = 0;
    int direction = -1; //for the furthest_node search function, means backward search
    path_node_t *path;
    int pos, distance;
    station_t *best_node_fr, *search_node_fr; //fr = furthest reach

    search_node = search_station(head, start);

    path = NULL;
    add_node_path(search_node, &path);

    while(possible && !last && search_node->position != end){
        pos = search_node->position;
        distance = (search_node->cars)->val;

        search_node = search_node->prev;
        best_node = NULL;
        while(search_node != NULL && possible && !last && pos - search_node->position <= distance){
            if(best_node == NULL){
                res = furthest_node(search_node, direction, &search_node_fr);
                if(res){
                    best_node = search_node;
                    best_node_fr = search_node_fr;
                }    
                if(search_node_fr->position - end <= 0)
                    last = 1;
            }else{
                res = furthest_node(search_node, direction, &search_node_fr);
                if(res && search_node_fr->position <= best_node_fr->position){
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
            res = furthest_node(search_node, direction, &search_node_fr);
            if(res && search_node_fr->position <= end)
                best_node = search_node;
            search_node = search_node->prev;
        }
        add_node_path(best_node, &path);
        add_node_path(search_station(head, end), &path);
    }


    *result = path;

    return possible;
    
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