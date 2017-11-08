// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

#define MAXPAGES (PHYSTOP / PGSIZE)

void freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

struct run {
  struct run *next;
};
 // This will take care of the count.
 //An array for all the possible pa.
int kpg_count[PHYSTOP >> PGSHIFT];

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
  // Lab 2: For COW fork, we can't store the run in the
  // physical page, because we need space for the ref
  // count.  Moving the data to the kmem struct.
  struct run runs[MAXPAGES];
} kmem;

// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  initlock(&kmem.lock, "kmem");
  kmem.use_lock = 0;
  freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
  kmem.use_lock = 1;
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE)
    kfree(p);
}
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
    panic("kfree");


  // Fill with junk to catch dangling refs.
  memset(v, 1, PGSIZE);

  if(kmem.use_lock)
    acquire(&kmem.lock);
<<<<<<< HEAD

  // Lab2: because we moved 'runs' to kmem
  //r = (struct run*)v;
  r = &kmem.runs[(V2P(v) / PGSIZE)];
=======
  r = (struct run*)v;

>>>>>>> lab1
  r->next = kmem.freelist;

  kmem.freelist = r;

  //kpg_count[(int)(V2P(r))>>PGSHIFT]= 0;
  if(kmem.use_lock)
    release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;
  char *rv;

  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = kmem.freelist;
  if(r){
    kmem.freelist = r->next;
    kpg_count[(int)(V2P(r))>>PGSHIFT]= 1;
  }
  if(kmem.use_lock)
    release(&kmem.lock);

  // Lab2: because we moved 'runs' to kmem
  //return (char*)r;
  rv = r ? P2V((r - kmem.runs) * PGSIZE) : r;
  return rv;
}

// This is a helper function to increment the count
// of the page.
void kincrement(uint v){

  if(kmem.use_lock)
    acquire(&kmem.lock);
  kpg_count[v>>PGSHIFT]++;
  if(kmem.use_lock)
    release(&kmem.lock);
}

// This is a helper function to decrement the count
// of the page.
void kdecrement(uint v){
  if(kmem.use_lock)
    acquire(&kmem.lock);
  kpg_count[v>>PGSHIFT]--;
  if(kmem.use_lock)
    release(&kmem.lock);
}

uint get_kpg_count(uint v){

  if(kmem.use_lock)
    acquire(&kmem.lock);
   uint count = kpg_count[v>>PGSHIFT];
  if(kmem.use_lock)
    release(&kmem.lock);

  return count;
}
