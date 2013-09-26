#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

MODULE_LICENSE ( "Dual BSD/GPL" ) ; 

#define CACHESIZE (128*1024)

void *ptr[10];
struct kmem_cache *cache;

static int slab_init(void)
{

	int i;
	int ret=0;
        printk("slab driver installed.\n");

	memset(ptr, 0, sizeof(ptr));

        cache = kmem_cache_create("slab_alloc", CACHESIZE, 0, 0, NULL);
        if (cache == NULL) {
                ret = -ENOMEM;
                goto error;
        }

        for (i = 0 ; i < 10 ; i++) {
                ptr[i] = kmem_cache_alloc(cache, GFP_KERNEL);
                if (ptr[i] == NULL) {
                        ret = -ENOMEM;
                        goto error;
                }
                memset(ptr[i], 0, CACHESIZE);
                printk("ptr[%d]=%p, size=%d\n", i, ptr[i], ksize(ptr[i]));
        }
        return 0;

error:
        for (i = 0 ; i < 10 ; i++) {
                if (ptr[i])
                        kmem_cache_free(cache, ptr[i]);
        }

        if (cache)
                kmem_cache_destroy(cache);

        return (ret);

}

static void slab_exit(void)
{
	int i;
        printk("slab driver removed.\n");
        for (i = 0 ; i < 10 ; i++) {
                if (ptr[i])
                        kmem_cache_free(cache, ptr[i]);
        }

        if (cache)
                kmem_cache_destroy(cache);


}

module_init(slab_init);
module_exit(slab_exit);
