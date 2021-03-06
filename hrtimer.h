
/* 
 * For my Laptop PC. This function was ripped from: 
 * http://stackoverflow.com/questions/7935518/is-clock-gettime-adequate-for-submicrosecond-timing
 */
#if __x86_64
__inline__ long long get_hrtimer(void) {
  long long lo, hi;
  __asm__ __volatile__ (      // serialize
  "xorl %%eax,%%eax \n        cpuid"
  ::: "%rax", "%rbx", "%rcx", "%rdx");
  /* We cannot use "=A", since this would use %rax on x86_64 and return only the lower 32bits of the TSC */
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return (long long)hi << 32 | lo;
}
#endif

/* For the Raspberry Pi */
#if __ARMEL__
/* Based on example code for Raspberry PI 1MHz timer ripped from http://mindplusplus.wordpress.com/2013/05/21/accessing-the-raspberry-pis-1mhz-timer/ */
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
 
#define ST_BASE (0x20003000)
#define TIMER_OFFSET (4)
 
__inline__ long long get_hrtimer(void) {
  long long int t, *timer; // 64 bit timer
  int fd;
  void *st_base; // byte ptr to simplify offset math
 
  // get access to system core memory
  if (-1 == (fd = open("/dev/mem", O_RDONLY))) {
    fprintf(stderr, "open() failed.\n");
    return 255;
  }
 
  // map a specific page into process's address space
  if (MAP_FAILED == (st_base = mmap(NULL, 4096,
				    PROT_READ, MAP_SHARED, fd, ST_BASE))) {
    fprintf(stderr, "mmap() failed.\n");
    return 254;
  }
 
  // set up pointer, based on mapped page
  timer = (long long int *)((char *)st_base + TIMER_OFFSET);
 
  // read initial timer
  t = *timer;

  close(fd);

  return t;
}
#endif
