## Kernel Locking
* Why?
  * Synchronization in certain code paths in the kernel
  * Locking issues arise from
    * SMP (symmetric multiprocessing): True concurrency
	* Interrupt handlers
	* Preemptible kernel
	* Any code can block (go to sleep)

## Kernel Locking --- Semaphore
  * Multi-process mutual exclusion
  
## Kernel Locking --- Spinlock
  * Low level driver locking mechanism

## Kernel Locking --- Atmoic
  * Perform in one uninterruptible operation

### Atomic Variables (Integer)
* Defined in asm/atomic.h   
```
typedef struct {
	volatile int counter;
} atomic_t;
```
* Operation compiled as one single instruction
* Macros
```
#define atomic_read(v) ((v)->counter)
```
Return the current value of v.
```
#define atomic_set(v,i) (((v)->counter) = (i))
```
Set the atomic variable v to the integer value i.
```
#define ATOMIC_INIT(i) {(i)}
atomic_t v = ATOMIC_INIT(0);
```
Initialize an atomic variable.
* Atomic functions:
```
void atomic_add(int i, atomic_t *v)
```
Add i to the atomic variable pointed to by v.
```
void atomic_sub(int i, atomic_t *v)
```
Subtract i from \*v.
```
void atomic_dec(atomic_t *v)
void atomic_inc(atomic_t *v)
```
Increment or decrement an atomic variable.
```
void atomic_dec_and_test(atomic_t *v)
void atomic_inc_and_test(atomic_t *v)
void atomic_sub_and_test(int i, atomic_t *v)
```
Perform the specified operation and test the result; if, after the operation, the atomic value is -, then the return value is true; otherwise, it is false.

## Kernel Locking --- Caveats
  * Semaphore:
    * The lock-held time may be long.
	* Can not synchronize the interrupt context.
  * Spinlock:
    * Do not to re-acquire a spinlock you already hold
	* Do not hold for a long time.
	* Do not do anything that will sleep.
  * Atomic:
    * Some architectures do not expect atomic_t to have more than 24 usable bits.
	* Cannot pass anything to an atomic operator except an atomic_t.
