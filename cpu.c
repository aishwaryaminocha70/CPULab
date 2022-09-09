#include<stdio.h>
#include "oslabs.h"

int main(){
struct PCB new_process;
struct PCB current_process;
struct PCB ready_queue[QUEUEMAX];
int queue_cnt=0;

current_process.process_id=1;
current_process.arrival_timestamp=1;
current_process.total_bursttime=4;
current_process.execution_starttime=1;
current_process.execution_endtime=5;
current_process.remaining_bursttime=4;
current_process.process_priority=8;

new_process.process_id=2;
new_process.arrival_timestamp=2;
new_process.total_bursttime=3;
new_process.execution_starttime=0;
new_process.execution_endtime=0;
new_process.remaining_bursttime=3;
new_process.process_priority=6;

struct PCB temp=handle_process_arrival_pp(ready_queue,&queue_cnt,current_process,new_process,2);

printf("ready_queue: ");
for(int i=0;i<queue_cnt;i++)
printf("[PID:%d, AT:%d, TBT:%d, EST:%d, EET:%d, RBT:%d, Priority:%d]\n",ready_queue[i].process_id,ready_queue[i].arrival_timestamp,ready_queue[i].total_bursttime,ready_queue[i].execution_starttime,ready_queue[i].execution_endtime,ready_queue[i].remaining_bursttime,ready_queue[i].process_priority);

printf("queue_cnt: %d\n",queue_cnt);
printf("PCB: ");
printf("[PID:%d, AT:%d, TBT:%d, EST:%d, EET:%d, RBT:%d, Priority:%d]\n",temp.process_id,temp.arrival_timestamp,temp.total_bursttime,temp.execution_starttime,temp.execution_endtime,temp.remaining_bursttime,temp.process_priority);

return 0;
}
