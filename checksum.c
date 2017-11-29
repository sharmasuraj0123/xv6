#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"

#define BUFFERSIZE 128

typedef struct {
  char buf[BUFFERSIZE];
  char * head;
  char *tail;
  int checksum;
  cond_var_t *cv;
  mutex_t *m;
} shared_t;

void consumer();
void producer();

int
main(int argc, char *argv[]) {

  printf(1, "Beginning the READING-WRITING TEST\n");
  shared_t *sa = (shared_t *) shmbrk(4096);
  memset(sa, 0, sizeof(shared_t));

  //Set the head & tail to the first element.
  sa->head = (sa->buf);
  sa->tail = (sa->buf);

  //Also to distinguish between a reader of writer.
  int bytes_read =0;
  int bytes_written =0;
  int i;
  int pid;
  pid = fork();
  //Child will Start Creating processes.
  if(pid==0){
    for(i=0; i<3; i++){
      pid =fork();
      if(pid==0){
        bytes_read = -1;
        bytes_written =0;
      }
      else {
       bytes_written = -1;
        bytes_read =0;
      }
    }
    if(bytes_written==0 && bytes_read ==-1)
      producer();
    else consumer();
    while(wait()!= -1)
    ;
    exit();
  }

  while(wait()!= -1)
  ;
  //exit();
  return 0;
}

void consumer(){
  int nread;
}
void producer(){

}
