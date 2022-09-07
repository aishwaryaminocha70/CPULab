oslabs.h

memory.c

Source code for oslabs.h:

#define QUEUEMAX 10

#define MAPMAX 10

#define TABLEMAX 10

#define POOLMAX 10

#define REFERENCEMAX 20

#define MAX( a, b ) ( ( a > b) ? a : b )

#define MIN( a, b ) ( ( a > b) ? b : a )

struct RCB {

int request_id;

int arrival_timestamp;

int cylinder;

int address;

int process_id;

};

struct PCB {

int process_id;

int arrival_timestamp;

int total_bursttime;

int execution_starttime;

int execution_endtime;

int remaining_bursttime;

int process_priority;

};

struct MEMORY_BLOCK {

int start_address;

int end_address;

int segment_size;

int process_id; //0 indicates a free block

};

struct PTE {

int is_valid;

int frame_number;

int arrival_timestamp;

int last_access_timestamp;

int reference_count;

};

struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);

struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt);

struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);

struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder);

struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);

struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, int current_cylinder, int scan_direction);

struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id);

struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id);

struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id);

struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id, int last_address);

void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt);

int process_page_access_fifo(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp);

int count_page_faults_fifo(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);

int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp);

int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);

int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp);

int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp);

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp);

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp);

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp);

struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum);

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum);









#include "oslabs.h"

#include <stdio.h>

#include <string.h>

#include <stdbool.h>

struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {

struct MEMORY_BLOCK temp_memory_block, allocated_memory;

allocated_memory.end_address = 0;

allocated_memory.start_address = 0;

allocated_memory.process_id = 0;

allocated_memory.segment_size = 0;

bool match = false;

int memory_map_index = 0 , best_fit_segmant = 0;

for (int i = 0; i <= *map_cnt - 1; i++) { //finding best fit

if (request_size == memory_map[i].segment_size) { //perfect match, save memory index and then break

match = true;

memory_map_index = i;

best_fit_segmant = request_size;

break;

}

else if ((memory_map[i].segment_size > request_size) && (memory_map[i].process_id == 0)) { // good match, keep looking for best match

if (match == false){ //first match

match = true;

memory_map_index = i;

best_fit_segmant = memory_map[i].segment_size;

}

else { //found a better match than before

if (best_fit_segmant > memory_map[i].segment_size){

memory_map_index = i;

best_fit_segmant = memory_map[i].segment_size;

}

}

}

}

if (match == true){ //found match, now swap

temp_memory_block = memory_map[memory_map_index];

allocated_memory.start_address = temp_memory_block.start_address;

allocated_memory.end_address = allocated_memory.start_address + request_size - 1;

allocated_memory.process_id = process_id;

allocated_memory.segment_size = request_size;

*map_cnt = *map_cnt + 1;

for (int i = memory_map_index; i <= *map_cnt; i++){

memory_map[memory_map_index+1] = memory_map[i];

}

memory_map[memory_map_index+1].start_address = allocated_memory.end_address + 1;

memory_map[memory_map_index+1].end_address = memory_map[memory_map_index].end_address;

memory_map[memory_map_index+1].process_id = memory_map[memory_map_index].process_id;

memory_map[memory_map_index+1].segment_size = memory_map[memory_map_index].segment_size - request_size;

memory_map[memory_map_index] = allocated_memory;

}

return allocated_memory;

}

struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {

struct MEMORY_BLOCK temp_memory_block, allocated_memory;

allocated_memory.end_address = 0;

allocated_memory.start_address = 0;

allocated_memory.process_id = 0;

allocated_memory.segment_size = 0;

bool match = false;

int memory_map_index = 0 , best_fit_segmant = 0;

for (int i = 0; i <= *map_cnt - 1; i++) { //finding best fit

if (request_size <= memory_map[i].segment_size) { //perfect match, save memory index and then break

match = true;

memory_map_index = i;

best_fit_segmant = request_size;

break;

}

}

if (match == true){ //found match, now swap

temp_memory_block = memory_map[memory_map_index];

allocated_memory.start_address = temp_memory_block.start_address;

allocated_memory.end_address = allocated_memory.start_address + request_size - 1;

allocated_memory.process_id = process_id;

allocated_memory.segment_size = request_size;

*map_cnt = *map_cnt + 1;

for (int i = memory_map_index; i <= *map_cnt; i++){

memory_map[memory_map_index+1] = memory_map[i];

}

memory_map[memory_map_index+1].start_address = allocated_memory.end_address + 1;

memory_map[memory_map_index+1].end_address = memory_map[memory_map_index].end_address;

memory_map[memory_map_index+1].process_id = memory_map[memory_map_index].process_id;

memory_map[memory_map_index+1].segment_size = memory_map[memory_map_index].segment_size - request_size;

memory_map[memory_map_index] = allocated_memory;

}

return allocated_memory;

}

struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {

struct MEMORY_BLOCK temp_memory_block, allocated_memory;

allocated_memory.end_address = 0;

allocated_memory.start_address = 0;

allocated_memory.process_id = 0;

allocated_memory.segment_size = 0;

bool match = false;

int memory_map_index = 0 , worst_fit_segmant = 0;

for (int i = 0; i <= *map_cnt - 1; i++) { //finding worst fit

if ((memory_map[i].segment_size >= request_size) && (memory_map[i].process_id == 0)) { // good match, keep looking for best match

if (match == false){ //first match

match = true;

memory_map_index = i;

worst_fit_segmant = memory_map[i].segment_size;

}

else { //found a worst match than before

if (worst_fit_segmant < memory_map[i].segment_size){

memory_map_index = i;

worst_fit_segmant = memory_map[i].segment_size;

}

}

}

}

if (match == true){ //found match, now swap

temp_memory_block = memory_map[memory_map_index];

allocated_memory.start_address = temp_memory_block.start_address;

allocated_memory.end_address = allocated_memory.start_address + request_size - 1;

allocated_memory.process_id = process_id;

allocated_memory.segment_size = request_size;

*map_cnt = *map_cnt + 1;

for (int i = memory_map_index; i <= *map_cnt; i++){

memory_map[memory_map_index+1] = memory_map[i];

}

memory_map[memory_map_index+1].start_address = allocated_memory.end_address + 1;

memory_map[memory_map_index+1].end_address = memory_map[memory_map_index].end_address;

memory_map[memory_map_index+1].process_id = memory_map[memory_map_index].process_id;

memory_map[memory_map_index+1].segment_size = memory_map[memory_map_index].segment_size - request_size;

memory_map[memory_map_index] = allocated_memory;

}

return allocated_memory;

}

struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id, int last_address) {

struct MEMORY_BLOCK temp_memory_block, allocated_memory;

allocated_memory.end_address = 0;

allocated_memory.start_address = 0;

allocated_memory.process_id = 0;

allocated_memory.segment_size = 0;

bool match = false;

int memory_map_index = 0 , best_fit_segmant = 0;

int index = 0;

for (int i = last_address; i <= *map_cnt+ last_address - 1; i++) { //finding next fit

if (i >= *map_cnt){

index = i - *map_cnt;

}

else

index = i;

if ((request_size <= memory_map[index].segment_size) && (memory_map[i].process_id == 0)){ // match, save memory index and then break

match = true;

memory_map_index = index;

best_fit_segmant = request_size;

break;

}

}

if (match == true){ //found match, now swap

temp_memory_block = memory_map[memory_map_index];

allocated_memory.start_address = temp_memory_block.start_address;

allocated_memory.end_address = allocated_memory.start_address + request_size - 1;

allocated_memory.process_id = process_id;

allocated_memory.segment_size = request_size;

*map_cnt = *map_cnt + 1;

for (int i = memory_map_index; i <= *map_cnt; i++){

memory_map[memory_map_index+1] = memory_map[i];

}

memory_map[memory_map_index+1].start_address = allocated_memory.end_address + 1;

memory_map[memory_map_index+1].end_address = memory_map[memory_map_index].end_address;

memory_map[memory_map_index+1].process_id = memory_map[memory_map_index].process_id;

memory_map[memory_map_index+1].segment_size = memory_map[memory_map_index].segment_size - request_size;

memory_map[memory_map_index] = allocated_memory;

}

return allocated_memory;

}

void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt) {

bool flag = false;

if ((*map_cnt == 1) && (memory_map[0].end_address == 0) && (memory_map[0].start_address == 0) && (memory_map[0].process_id == 0) && (memory_map[0].segment_size == 0))

return;

else {

for (int i = 0; i < *map_cnt; i++){ //looking for the memory block index to free

if((freed_block.start_address == memory_map[i].start_address) && (freed_block.end_address == memory_map[i].end_address) && (freed_block.process_id == memory_map[i].process_id)) {

memory_map[i].process_id = 0;

if (i > 0){

if (memory_map[i-1].process_id == 0){

memory_map[i-1].end_address = freed_block.end_address;

memory_map[i-1].segment_size = memory_map[i-1].segment_size + freed_block.segment_size;

for (int index = i; index <= *map_cnt; index++){

memory_map[index] = memory_map[index + 1];

}

*map_cnt = *map_cnt - 1;

flag = true;

}

}

if (i < *map_cnt-1){

if (flag == false){

i = i+1;

}

if (memory_map[i].process_id == 0){

memory_map[i].start_address = memory_map[i-1].start_address;

memory_map[i].segment_size = memory_map[i].end_address -memory_map[i].start_address+1;

for (int index = i; index <= *map_cnt; index++){

memory_map[index-1] = memory_map[index];

}

*map_cnt = *map_cnt - 1;

}

}

break;

}

}

}

}



