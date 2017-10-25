# LINUX DEVICE DRIVERS

This is my notes on programming Linux device drivers.

## History
```
	2017/10/25	init
```

## Miscellaneous Character Drivers
* are not found in LDD3 book
* have been existing since 2.0 kernel
* support custom hacks
* show as character devices with major number 10 with the following commands
```
# ls -l /dev
# cat /proc/devices
```
* APIs (kernel 3.18)
```
#include <linux/miscdevice.h>
struct file_operations mydev_fops;
struct miscdevice * misc = {
	.minor	=	MISC_DYNAMIC_MINOR, //a minor number is assigned by kernel
	.name	=	"mydev",
	.fops	=	&mydev_fops,
};
int misc_register(struct miscdevice * misc);
int misc_deregister(struct miscdevice * misc);

```

references:
  * Docs » The Linux driver implementer’s API guide » Miscellaneous Devices [link](https://www.kernel.org/doc/html/v4.12/driver-api/misc_devices.html)
  * Miscellaneous Character Drivers, Jun 30, 1998  By Alessandro Rubini [link](http://www.linuxjournal.com/article/2920)

## Netlink / Netlink sock family
* is a Linux interface used for inter-process communication between both the kernel and userspace processes, and between different userspace processes
* has benefits over other communcations such as system calls, ioctls or proc filesystems
  * is relatively trivial
  * is a full-duplex communication link
  * is asynchronous
  * enables loadable module
  * supports multicast

* uses standard socket APIs for user-space processes
```

```
* uses a special kernel API for kernel modules (kernel 3.18)
```
#include <linux/netlink.h>
struct sock *netlink_kernel_create(struct net *net, int unit, struct netlink_kernel_cfg *cfg);
void netlink_kernel_release(struct sock *sk);

```

references:
  * Wiki [link](https://en.wikipedia.org/wiki/Netlink)
  * Kernel Korner - Why and How to Use Netlink Socket, Jan 05, 2005  By Kevin Kaichuan He [link](http://www.linuxjournal.com/article/7356)
  * Linux中与内核通信的Netlink机制 [link](http://www.linuxidc.com/Linux/2011-07/39085.htm)
