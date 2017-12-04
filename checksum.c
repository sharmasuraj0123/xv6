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
#define WRITESIZE 12
#define READSIZE 8

typedef struct {
  char buf[BUFFERSIZE];
  int head;
  int tail;
  int checksum;
  cond_var_t *cv;
  mutex_t *m;
  int done;
} shared_t;

void consumer(shared_t* sa);
void producer(shared_t* sa);
void testMutextLock(shared_t *sa);

int
main(int argc, char *argv[]) {

  printf(1, "Beginning the READING-WRITING TEST\n");
  shared_t *sa = (shared_t *) shmbrk(sizeof(shared_t *));


  sa->head = 0;
  sa->tail = 0;
  sa->checksum = 0;
  sa->done =0;
  sa->m = (mutex_t *) shmbrk(sizeof(mutex_t *));
  sa->cv = (cond_var_t *) shmbrk(sizeof(cond_var_t *));
  //sa->buf = (char *)shmbrk(sizeof(BUFFERSIZE));
  //memset(sa->buf, 0, BUFFERSIZE);

  //Also to distinguish between a reader of writer.
  int bytes_read =0;
  int bytes_written =0;
  int i;

  mutex_init(sa->m);
  cv_init(sa->cv);
  //sa->cv->locked =1;
  printf(1, "lol\n");

  int fd;
  //char *c = malloc(sizeof(char*));
  char c[132];
  //*c = 'a';
   fd = open("README", O_RDONLY);

   if(fd<0)
   printf(1,"dssdsdgh\n" );

   int count =1;
   int ck=0;
  while(count){
    count = read(fd, c, 1);
    //printf(1, "%c",*c);
    ck+= *c;
  }
  close(fd);

  printf(1,"ck %d\n",ck);

  int pid;
  pid = fork();
  //Child will Start Creating processes.

  //printf(1,"count: %d\n", count);


  // if(pid==0)
  //   consumer(sa);
  // else
  // producer(sa);


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
      producer(sa);
    else consumer(sa);
    //testMutextLock(sa);
    while(wait()!= -1)
    ;
    exit();
  }



  while(wait()!= -1)
  ;
  printf(1,"YESS\n");
    printf(1,"checksum after everything: %d\n",sa->checksum);
  exit();


  return 0;
}


void testMutextLock(shared_t *sa){

  mutex_lock(sa->m);

  printf(1,"Everyone\n");
  printf(1,"IS\n");
  printf(1,"In\n");
  printf(1,"Sync\n");
  printf(1,"Here\n");
  printf(1,"\n\n\n\n\n\n\n");

  mutex_unlock(sa->m);


}

void consumer(shared_t* sa){
  char nread =0;
  int checksum = 0;
  int i;

while(sa->done !=4){
  mutex_lock(sa->m);
  for (i=0; i < READSIZE;i++){
    //printf(1,"Consumer Starts reading\n");
    //If the Buffer is already empty;
    if (sa->head== sa->tail){
      //printf(1,"Here\n");
      // cv_bcast(sa->cv);
      // cv_wait(sa->cv,sa->m);
      //  mutex_unlock(sa->m);
      //  mutex_lock(sa->m);
      break;
      if(sa->done ==4 && (sa->head == sa->tail)){
        cv_bcast(sa->cv);
        sa->checksum += checksum;
        printf(1,"hsschecksum: %d\n",sa->checksum);
        return;
      }
    }

    nread = sa->buf[sa->head%BUFFERSIZE];
      //printf(1,"%c",sa->buf[sa->head%BUFFERSIZE]);
    (sa->head)++;
    //sa->head= (sa->head);
    checksum+=nread;


    //Closing Condition
    // if(nread == '\0'){
    //   cv_bcast(sa->cv);
    //   sa->checksum += checksum;
    //   printf(1,"rchecksum: %d\n",(int)sa->checksum);
    //   mutex_unlock(sa->m);
    //   return;
    // }
  }
    //printf(1,"\n");
  cv_bcast(sa->cv);
  mutex_unlock(sa->m);
}

  printf(1,"hchecksummmm: %d\n",(int)sa->checksum);
  cv_bcast(sa->cv);
    sa->checksum += checksum;

}
void producer(shared_t* sa){
  //Open the file for the process.
  int fd,i;

  int count =10;
  fd = open("README", O_RDWR);


  while(count){
    mutex_lock(sa->m);
  for(i=0; i< WRITESIZE;i++){
    //Buffer is full
    //printf(1,"Producer Starts\n");
    if ((sa->tail) == (sa->head)+BUFFERSIZE){
      //printf(1,"What\n");
      //cv_bcast(sa->cv);
      //mutex_unlock(sa->m);
      //cv_wait(sa->cv,sa->m);
      // mutex_unlock(sa->m);
      // mutex_lock(sa->m);
      break;

    }
    char c[10];
    count = read(fd, c, 1);

    sa->buf[sa->tail%BUFFERSIZE] = *c;
    //printf(1," CHar c: %c",sa->buf[sa->tail]);
    sa->tail++;
    //sa->tail= (sa->tail);

    if(count ==0)
      break;

  }
  //cv_bcast(sa->cv);
  cv_wait(sa->cv,sa->m);
  mutex_unlock(sa->m);
}
    printf(1,"HUH\n");
    sa->done++;
  close (fd);
}
