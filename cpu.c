#include "oslabs.h"
#include <stdio.h>
#include <string.h>

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
if ((current_process.process_id == 0) && (current_process.total_bursttime == 0) && (current_process.execution_endtime == 0) &&
(current_process.remaining_bursttime == 0) && (current_process.execution_starttime == 0) && (current_process.arrival_timestamp == 0) && (current_process.process_priority == 0)) {
// NULLPCB , New process priorty
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

//*******************************************************************************************************

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp){
if ((current_process.process_id == 0) && (current_process.total_bursttime == 0) && (current_process.execution_endtime == 0) && (current_process.remaining_bursttime == 0) &&
(current_process.execution_starttime == 0) && (current_process.arrival_timestamp == 0) && (current_process.process_priority == 0)) {// NULLPCB , New process priority
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

//*****************************************************************************************************

struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum) {
if ((current_process.process_id == 0) && (current_process.total_bursttime == 0) && (current_process.execution_endtime == 0) && (current_process.remaining_bursttime == 0) &&
(current_process.execution_starttime == 0) && (current_process.arrival_timestamp == 0) && (current_process.process_priority == 0)) {// NULLPCB , New process priorty
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

//***************************************************************************************************
/*
int main (){
struct PCB process_1;
struct PCB process_2;
struct PCB process_3;
struct PCB process_4;
struct PCB NULL_PCB;

process_1.process_id = 1;
process_1.arrival_timestamp = 1;
process_1.total_bursttime = 5;
process_1.execution_starttime = 3;
process_1.execution_endtime = 8;
process_1.remaining_bursttime = 3;
process_1.process_priority = 3;

process_2.process_id = 2;
process_2.arrival_timestamp = 3;
process_2.total_bursttime = 6;
process_2.execution_starttime = 6;
process_2.execution_endtime = 12;
process_2.remaining_bursttime = 0;
process_2.process_priority = 2;

process_3.process_id = 3;
process_3.arrival_timestamp = 5;
process_3.total_bursttime = 3;
process_3.execution_starttime = 5;
process_3.execution_endtime = 8;
process_3.remaining_bursttime = 3;
process_3.process_priority = 5;

process_4.process_id = 4;
process_4.arrival_timestamp = 5;
process_4.total_bursttime = 3;
process_4.execution_starttime = 2;
process_4.execution_endtime = 4;
process_4.remaining_bursttime = 3;
process_4.process_priority = 6;

NULL_PCB.process_id = 0;
NULL_PCB.arrival_timestamp = 0;
NULL_PCB.total_bursttime = 0;
NULL_PCB.execution_starttime = 0;
NULL_PCB.execution_endtime = 0;
NULL_PCB.remaining_bursttime = 0;
NULL_PCB.process_priority = 0;

struct PCB ready_queue[QUEUEMAX];
int cnt = 0;
int *queue_cnt;
queue_cnt = &cnt;
int timestamp = 0;
struct PCB current_process, next_process;


printf("queue_cnt value is: %d \n", *queue_cnt);
printf("%d", process_1.process_id);

next_process = handle_process_completion_pp(ready_queue ,queue_cnt, timestamp);

current_process = handle_process_arrival_pp(ready_queue, queue_cnt, process_1, process_2, timestamp);
current_process = handle_process_arrival_pp(ready_queue, queue_cnt, current_process, process_3, timestamp);
timestamp = 3;
next_process = handle_process_completion_pp(ready_queue ,queue_cnt, timestamp);
current_process = handle_process_arrival_pp(ready_queue, queue_cnt, current_process, process_4, timestamp);


return 0;
}
