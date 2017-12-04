#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"
#include "vdso.h"

char*
strcpy(char *s, char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
stat(char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, void *vsrc, int n)
{
  char *dst, *src;

  dst = vdst;
  src = vsrc;
  while(n-- > 0)
    *dst++ = *src++;
  return vdst;
}

uint
vdso_getticks()
{
  static vdso_getticks_t _getticks_func = 0;

  // upon the first use, get the entry from the kernel
  if (0 == _getticks_func) {
    _getticks_func = vdso_entry(VDSO_GETTICKS);
  }

  // call the function
  return _getticks_func();
}

uint
vdso_getpid()
{
  static vdso_getpid_t _getpid_func = 0;

  // upon the first use, get the entry from the kernel
  if (0 == _getpid_func) {
    _getpid_func = vdso_entry(VDSO_GETPID);
  }

  // call the function
  return _getpid_func();
}


void mutex_init(mutex_t *m){
  m->locked = 0;
}
void mutex_lock(mutex_t *m){

  //pushcli(); // disable interrupts to avoid deadlock.

  //Keep on sleeping until the lock is available.
  while (xchg(&m->locked, 1) != 0)
    futex_wait((int *)&(m->locked),1);

}
int  mutex_trylock(mutex_t *m){
  if(xchg(&m->locked, 1) != 0)
    return 0;

  return -1;
}
void mutex_unlock(mutex_t *m){

  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that all the stores in the critical
  // section are visible to other cores before the lock is released.
  // Both the C compiler and the hardware may re-order loads and
  // stores; __sync_synchronize() tells them both not to.
  __sync_synchronize();
  // Release the lock, equivalent to lk->locked = 0.
  // This code can't use a C assignment, since it might
  // not be atomic. A real OS would use C atomics here.
  asm volatile("movl $0, %0" : "+m" (m->locked) : );
  //popcli();
  //Wake up all the resources waiting for the lock.
  futex_wake((int *)&(m->locked));
}
void cv_init(cond_var_t *cv){
  cv->locked = 0;
}
void cv_wait(cond_var_t *cv, mutex_t *m){
  //Try to get the cv lock.
  if (xchg(&cv->locked, 1) != 0){
    mutex_unlock(m);
    //If not then first release the lock.
    //Then sleep.
    futex_wait((int *)&(cv->locked),1);
    //Aquire the mutex lock first thing after wakeup
    mutex_lock(m);
  }
}
void cv_bcast(cond_var_t *cv){

  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that all the stores in the critical
  // section are visible to other cores before the lock is released.
  // Both the C compiler and the hardware may re-order loads and
  // stores; __sync_synchronize() tells them both not to.
  __sync_synchronize();
  // Release the lock, equivalent to lk->locked = 0.
  // This code can't use a C assignment, since it might
  // not be atomic. A real OS would use C atomics here.
  asm volatile("movl $0, %0" : "+m" (cv->locked) : );
  //popcli();
  //Wake up all the resources waiting for the lock.
  futex_wake((int *)&(cv->locked));
}
