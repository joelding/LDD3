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
