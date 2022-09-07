#include "oslabs.h"
#include <stdio.h>
#include <string.h>

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
if ((current_process.process_id == 0) && (current_process.total_bursttime == 0) && (current_process.execution_endtime == 0) && (current_process.remaining_bursttime == 0) && (current_process.execution_starttime == 0) && (current_process.arrival_timestamp == 0) && (current_process.process_priority == 0)) {// NULLPCB , New process priorty
new_process.execution_starttime = timestamp;
new_process.execution_endtime = timestamp + new_process.total_bursttime;
new_process.remaining_bursttime = new_process.total_bursttime;
return new_process;
}
else if ( new_process.process_priority >= current_process.process_priority) { //New process goes to queue
new_process.execution_starttime = 0;
new_process.execution_endtime = 0;
new_process.remaining_bursttime = new_process.total_bursttime;
ready_queue[*queue_cnt] = new_process;
*queue_cnt = *queue_cnt + 1;
return current_process;
}
else { //current process goes to queue
new_process.execution_starttime = timestamp;
new_process.execution_endtime = timestamp + new_process.total_bursttime;
new_process.remaining_bursttime = new_process.total_bursttime;
current_process.remaining_bursttime = current_process.execution_endtime - timestamp;
current_process.execution_endtime = 0;
ready_queue[*queue_cnt] = current_process;
*queue_cnt = *queue_cnt + 1;
return new_process;
}
}

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
if (*queue_cnt > 0) {
struct PCB next_process;
int priorty = ready_queue[0].process_priority;
int temp_queue = 0;
for (int i = 1 ; i <= *queue_cnt - 1; i++){
if (priorty > ready_queue[i].process_priority){
priorty = ready_queue[i].process_priority;
temp_queue = i;
}
}
next_process = ready_queue[temp_queue];
if (*queue_cnt == 1) {
ready_queue[0].process_id = 0;
ready_queue[0].arrival_timestamp = 0;
ready_queue[0].total_bursttime = 0;
ready_queue[0].execution_starttime = 0;
ready_queue[0].execution_endtime = 0;
ready_queue[0].remaining_bursttime = 0;
ready_queue[0].process_priority = 0;
}
else
ready_queue[temp_queue] = ready_queue[*queue_cnt - 1];
*queue_cnt = *queue_cnt - 1;
next_process.execution_starttime = timestamp;
next_process.execution_endtime = timestamp + next_process.total_bursttime;
return next_process;
}
else {
struct PCB null_PCB;
null_PCB.process_id = 0;
null_PCB.arrival_timestamp = 0;
null_PCB.total_bursttime = 0;
null_PCB.execution_starttime = 0;
null_PCB.execution_endtime = 0;
null_PCB.remaining_bursttime = 0;
null_PCB.process_priority = 0;
return null_PCB;
  
}
}

//***********************************

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp){
if ((current_process.process_id == 0) && (current_process.total_bursttime == 0) && (current_process.execution_endtime == 0) && (current_process.remaining_bursttime == 0) && (current_process.execution_starttime == 0) && (current_process.arrival_timestamp == 0) && (current_process.process_priority == 0)) {// NULLPCB , New process priority
new_process.execution_starttime = time_stamp;
new_process.execution_endtime = time_stamp + new_process.total_bursttime;
new_process.remaining_bursttime = new_process.total_bursttime;
return new_process;
}
else if ( new_process.total_bursttime > current_process.remaining_bursttime) { //new process goes to the queue
new_process.execution_starttime = 0;
new_process.execution_endtime = 0;
new_process.remaining_bursttime = new_process.total_bursttime;
ready_queue[*queue_cnt] = new_process;
*queue_cnt = *queue_cnt + 1;
return current_process;
}
else { //current process goes to the queue
new_process.execution_starttime = time_stamp;
new_process.execution_endtime = time_stamp + new_process.total_bursttime;
new_process.remaining_bursttime = new_process.total_bursttime;
current_process.remaining_bursttime = current_process.execution_endtime - time_stamp;
current_process.execution_endtime = 0;
current_process.execution_starttime = 0;
ready_queue[*queue_cnt] = current_process;
*queue_cnt = *queue_cnt + 1;
return new_process;
}
}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
if (*queue_cnt > 0) {
struct PCB next_process;
int r_bursttime = ready_queue[0].remaining_bursttime;
int temp_queue = 0;
for (int i = 1 ; i <= *queue_cnt - 1; i++){
if (r_bursttime > ready_queue[i].remaining_bursttime){ //looking for the smallest bursttime
r_bursttime = ready_queue[i].remaining_bursttime;
temp_queue = i;
}
}
next_process = ready_queue[temp_queue];
if (*queue_cnt == 1) {
ready_queue[0].process_id = 0;
ready_queue[0].arrival_timestamp = 0;
ready_queue[0].total_bursttime = 0;
ready_queue[0].execution_starttime = 0;
ready_queue[0].execution_endtime = 0;
ready_queue[0].remaining_bursttime = 0;
ready_queue[0].process_priority = 0;
}
else
ready_queue[temp_queue] = ready_queue[*queue_cnt - 1];
*queue_cnt = *queue_cnt - 1;
next_process.execution_starttime = timestamp;
next_process.execution_endtime = timestamp + next_process.remaining_bursttime;
return next_process;
}
else {
struct PCB null_PCB;
null_PCB.process_id = 0;
null_PCB.arrival_timestamp = 0;
null_PCB.total_bursttime = 0;
null_PCB.execution_starttime = 0;
null_PCB.execution_endtime = 0;
null_PCB.remaining_bursttime = 0;
null_PCB.process_priority = 0;
return null_PCB;
  
}
}

//***********************************

struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum) {
if ((current_process.process_id == 0) && (current_process.total_bursttime == 0) && (current_process.execution_endtime == 0) && (current_process.remaining_bursttime == 0) && (current_process.execution_starttime == 0) && (current_process.arrival_timestamp == 0) && (current_process.process_priority == 0)) {// NULLPCB , New process priorty
new_process.execution_starttime = timestamp;
if (time_quantum <= new_process.total_bursttime){
new_process.execution_endtime = timestamp + time_quantum;
}
else {
new_process.execution_endtime = timestamp + new_process.total_bursttime;
}
new_process.remaining_bursttime = new_process.total_bursttime;
return new_process;
}
else {
new_process.execution_starttime = 0;
new_process.execution_endtime = 0;
new_process.remaining_bursttime = new_process.total_bursttime;
ready_queue[*queue_cnt] = new_process;
*queue_cnt = *queue_cnt + 1;
return current_process;
}
}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum) {
if (*queue_cnt > 0) {
struct PCB next_process;
int arr_timestamp = ready_queue[0].arrival_timestamp;
int temp_queue = 0;
for (int i = 1 ; i <= *queue_cnt - 1; i++){
if (arr_timestamp > ready_queue[i].arrival_timestamp){
arr_timestamp = ready_queue[i].arrival_timestamp;
temp_queue = i;
}
}
next_process = ready_queue[temp_queue];
if (*queue_cnt == 1) {
ready_queue[0].process_id = 0;
ready_queue[0].arrival_timestamp = 0;
ready_queue[0].total_bursttime = 0;
ready_queue[0].execution_starttime = 0;
ready_queue[0].execution_endtime = 0;
ready_queue[0].remaining_bursttime = 0;
ready_queue[0].process_priority = 0;
}
else
ready_queue[temp_queue] = ready_queue[*queue_cnt - 1];
*queue_cnt = *queue_cnt - 1;
next_process.execution_starttime = timestamp;
if (time_quantum <= next_process.remaining_bursttime){
next_process.execution_endtime = timestamp + time_quantum;
}
else {
next_process.execution_endtime = timestamp + next_process.remaining_bursttime;
}
return next_process;
}
else {
struct PCB null_PCB;
null_PCB.process_id = 0;
null_PCB.arrival_timestamp = 0;
null_PCB.total_bursttime = 0;
null_PCB.execution_starttime = 0;
null_PCB.execution_endtime = 0;
null_PCB.remaining_bursttime = 0;
null_PCB.process_priority = 0;
return null_PCB;
  
}
}

oslabs.h

  
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
