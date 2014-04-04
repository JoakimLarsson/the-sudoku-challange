/*
 * This function was ripped from: 
 * http://stackoverflow.com/questions/7935518/is-clock-gettime-adequate-for-submicrosecond-timing
 */
__inline__ long long get_hrtimer(void) {
  long long lo, hi;
  __asm__ __volatile__ (      // serialize
  "xorl %%eax,%%eax \n        cpuid"
  ::: "%rax", "%rbx", "%rcx", "%rdx");
  /* We cannot use "=A", since this would use %rax on x86_64 and return only the lower 32bits of the TSC */
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return (long long)hi << 32 | lo;
}
