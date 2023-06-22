#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define word_max 120

typedef struct  pos{
    int first_index;
    int second_index;
}pos;


typedef struct word{
    char text[120];
    struct word * next;
}word;


typedef struct map{
    word * table[95][96];
}map;

//map word_map;

void clear_table(map * my_map){
    for(int  i = 0 ; i < 95 ; i++){
        for(int j = 0 ; j < 96 ; j ++){
            my_map -> table[i][j] = NULL;
        }
    }
}


pos hash(char * w){//32 till 126 // 95 letters
    pos ret ; 
    ret.first_index= w[0] - 32;
    ret.second_index = 0;
    if(w[1] != 0){
        ret.second_index = w[1] - 31;
    }
    return ret;
}


void add(map * my_map ,word * w){
    pos index = hash(w -> text);
    if(my_map -> table[index.first_index][index.second_index] == NULL){
        my_map -> table[index.first_index][index.second_index] = w;
        return;
    } else if(!strcmp(my_map -> table[index.first_index][index.second_index] -> text , w -> text)){
        return;
    } else for(word * try = my_map -> table[index.first_index][index.second_index] ; try != NULL ; try = try -> next){
        if(try -> next == NULL){
            try -> next = w;
            return;
        } else if(!strcmp(try -> next -> text , w -> text)){
            return;
        }
    }
}


word * remove_element(map * my_map , char * w){
    pos index = hash(w);
    if(my_map -> table[index.first_index][index.second_index] == NULL){
        return NULL;
    }
    
    else if(!strcmp(my_map -> table[index.first_index][index.second_index]  -> text , w )){
        word * tmp = my_map -> table[index.first_index][index.second_index];
        if(tmp != NULL){
            my_map -> table[index.first_index][index.second_index]  = tmp -> next;
            
        }else{
            my_map -> table[index.first_index][index.second_index] = NULL;
        }
        return tmp;
    }

    for(word * search = my_map -> table[index.first_index][index.second_index] ; search != NULL && search -> next != NULL ; search = search ->next){
        if(!strcmp(search -> next -> text , w)){
            word * tmp = search -> next;
            if(tmp -> next != NULL){
                search -> next = search -> next -> next;
            } else {
                search -> next = NULL;
            }
            return tmp;
        }
    }

    return NULL;
}


word *lookup(map my_map , char *w){
    pos index = hash(w);
    if(my_map.table[index.first_index][index.second_index] == NULL){
        return NULL;
    }
    
    else if(!strcmp(my_map.table[index.first_index][index.second_index]  -> text , w )){
        return my_map.table[index.first_index][index.second_index];
    }

    for(word * search = my_map.table[index.first_index][index.second_index] ; search != NULL && search -> next != NULL ; search = search ->next){
        if(!strcmp(search -> next -> text , w)){
            if(search -> next != NULL){
                return search -> next;
            }
        }
    }

    return NULL;
}