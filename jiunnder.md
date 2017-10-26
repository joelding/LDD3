## Kernel Locking
* Why?
  * Synchronization in certain code paths in the kernel
  * Locking issues arise from
    * SMP (symmetric multiprocessing): True concurrency
	* Interrupt handlers
	* Preemptible kernel
	* Any code can block (go to sleep)

## Kernel Locking --- Semaphore
Multi-process mutual exclusion
  
## Kernel Locking --- Spinlock
Low level driver locking mechanism

* Spinlock functions
```
#include <linux/spinlock.h>
DEFINE_SPINLOCK(lock);
```
  * Lock functions
```
void spin_lock(spinlock_t *lock)
void spin_trylock(spinlock_t *lock)
```
  * Release a spin lock
```
void spin_unlock(spinlock_t *lock)
```
* A thread gets spinlock & later an interrupt requires the same lock
Deadlock if both calls spin\_lock(). Solution: let the thread disables interrupts while .

```
void spin_lock_irq(spinlock_t *lock)
void spin_lock_irqsave(spinlock_t *lock, unsigned long flags)
void spin_lock_bh(spinlock_t *lock, unsigned long flags)

void spin_unlock_irq(spinlock_t *lock)
void spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags)
void spin_unlock_bh(spinlock_t *lock)
```
### Readers-writer spinlock
* Allow any number of readers into a critical section simultaneously, but writers must have exclusive.
```
#include <linux/spinlock.h>

```

reference:
1. kernel  Documentation/locking/spinlocks

## Kernel Locking --- Atmoic
Perform in one uninterruptible operation

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

### Atomic Variables (Bit)
* Defined in asm/bitops.h
* Modify or test single bits in a single step, no interrupt (or other processor) can interfere.
* Atomic functions
```
void set_bit(nr, void *addr);
```
Sets bit number nr in the data item pointed to by addr
```
void clear_bit(nr, void *addr);
```
Clears the specified
```
void change_bit(nr, void *addr);
```
Toggles the bit
```
test_bit(nr, void *addr);
```
Returns the current value of the bit.
```
int test_and_set_bit(nr, void *addr);
```
Set a bit and return its old value
```
int test_and_clear_bit(nr, void *addr);
```
Clear a bit and return its old value
```
int test_and_change_bit(nr, void *addr);
```
Change a bit and return its new value


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
