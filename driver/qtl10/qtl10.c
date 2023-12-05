#include "libqtl10.h"

#define GPIO_NUMBER    149     //User LED 0. GPIO number 149. Page 71 of BB-xM Sys Ref Manual.
#define PAGE_SIZE      2048
#define MAX_PATH       256

#define CLASS_NAME     "chardev"
#define REGION_NAME    "crl_drv"
#define DEVICE_NAME    QTDEVICE_NAME


static int progress_value =-1;
static int poweroff_value = 1;

static dev_t first;         // Global variable for the first device number
static struct cdev c_dev;     // Global variable for the character device structure
static struct class *cl;     // Global variable for the device class
 
static int init_result;

struct unicode_cdevice{
    unsigned int port0;
    unsigned int status;
    unsigned int position;
};

struct unicode_cstring{
     char           *buf;
     unsigned int   len;
};

static struct unicode_cstring  *mcstr=NULL;
static struct task_struct *etx_thread=NULL;

static int lsbc_open( struct inode *inode, struct file *file );
static int lsbc_close( struct inode *inode, struct file *file );
static loff_t	lsbc_llseek (struct file *f, loff_t position, int flags);
static ssize_t lsbc_write( struct file* F, const char *buf, size_t count, loff_t *f_pos );
static ssize_t lsbc_read( struct file* F, char *buf, size_t count, loff_t *f_pos );
static long lsbc_ioctl (struct file *f, unsigned int flags, unsigned long value);
static long lsbc_ioctl2 (struct file *f, unsigned int flags, unsigned long value);


static ssize_t lsbc_read( struct file* F, char *buf, size_t count, loff_t *f_pos )
{
    char buffer[10];
    unsigned int len=0;
    unsigned int size=0;
    printk(KERN_ALERT "Executing READ. %x=%d\n", buf, count);
    //int temp = gpio_get_value(GPIO_NUMBER);
    if(buf==NULL || count <= 0){
        printk(KERN_ALERT "Segment memory failed.\n");
        return -1;    
    }
    sprintf( buffer, "%d" , (unsigned int)count );
 
    count = sizeof( buffer );
    if(mcstr==NULL){
        printk(KERN_ALERT "Globl memory failed.\n");
        return -1; 
    }
    size = mcstr->len;
    if(mcstr->len > count) size= count;
    if(count > PAGE_SIZE) {
       printk(KERN_ALERT "Segment length failed.\n");     
       return -1;
    }
    printk(KERN_ALERT "Stack Output READ. %x=%d \n", mcstr->buf, mcstr->len);
    if((len= copy_to_user( buf, mcstr->buf, size )) )
    {
        printk(KERN_ALERT "Copy memory failed.\n");
        return -EFAULT;
    }
    buf[size]='\0';
    printk(KERN_ALERT "Output READ. %x=%d (%s)\n", buf, len, buf);
    if( *f_pos == 0 )
    {
        *f_pos += 1;
        return 1;
    }
    else
    {        
        return 0;
    }
}
 
static ssize_t lsbc_write( struct file* F, const char *buf, size_t count, loff_t *f_pos )
{
    unsigned int len=0;
    printk(KERN_ALERT "Executing WRITE. %x=%d (%s)\n", buf, count, buf);
    

    if(count > PAGE_SIZE || strlen(buf)>PAGE_SIZE){
        printk(KERN_ALERT "Buffer maxinium.\n");
        return -1;
    }
    if(mcstr==NULL){
        printk(KERN_ALERT "Globl memory failed.\n");
        return -1; 
    }
    memset(mcstr->buf, 0, PAGE_SIZE);
    if(mcstr->buf==NULL){
        printk(KERN_ALERT "Globl variable memory failed.\n");
        return -1; 
    }
    len = copy_from_user( mcstr->buf, buf, count );
    mcstr->len = count;    
    //msg[len]='\0';
    //switch( buf[0] )
    //{
    //    case '0':
        //gpio_set_value(GPIO_NUMBER, 0);
    //    break;
 
    //    case '1':
        //gpio_set_value(GPIO_NUMBER, 1);
    //    break;
 
    //    default:
    //    printk(KERN_ALERT "Wrong option.\n");
    //    break;
    //}
    return count;
}
 
static int lsbc_open( struct inode *inode, struct file *file )
{
    char *cwd;
    char path[MAX_PATH];
    //char *buf = (char *)kmalloc(MAX_PATH, GFP_KERNEL);
    cwd = d_path(&file->f_path,path, sizeof(path));
    printk(KERN_ALERT "Executing OPEN.(%s)\n", cwd);
    //if(buf!=NULL)
    // kfree(buf);
    return 0;
}

static int lsbc_close( struct inode *inode, struct file *file )
{
    printk(KERN_ALERT "Executing CLOSE.\n");
    return 0;
}
static long lsbc_ioctl (struct file *f, unsigned int flags, unsigned long value)
{
    printk(KERN_ALERT "Executing IOCTL 1.(%x)(%x) = %d\n", flags, (unsigned int *)value, (unsigned int )value);
    
    return lsbc_ioctl2(f,flags,value);
}

static long lsbc_ioctl2 (struct file *f, unsigned int flags, unsigned long value)
{
    printk(KERN_ALERT "Executing IOCTL 2.(%x)(%x) = %d\n", flags, (unsigned int *)value, (unsigned int )value);
    switch(flags) {
        case CTL10_DEVINIT:
            printk(KERN_ALERT "Init Data Err!\n");
        break;
        case CTL10_PROGWRITE:
            if( copy_from_user(&progress_value ,(int32_t*) value, sizeof(progress_value)) )
            {
                printk(KERN_ALERT "Data Write Err!\n");
            }
            printk(KERN_ALERT "Write Value = %d\n", progress_value);
        break;
        case CTL10_PROGREAD:
            if( copy_to_user((int32_t*) value, &progress_value, sizeof(progress_value)) )
            {
                printk(KERN_ALERT "Data Read : Err!\n");
            }
            printk(KERN_ALERT "Read Value = %d\n", value);
        break;
        case CTL10_RESTART:
            if( copy_from_user(&poweroff_value ,(int32_t*) value, sizeof(poweroff_value)) )
            {
                printk(KERN_ALERT "Data Write Err!\n");
            }
            if(poweroff_value!=1){
                if( copy_from_user(&poweroff_value ,(int32_t*) 1, sizeof(poweroff_value)) )
                {
                    printk(KERN_ALERT "Data Write Err!\n");
                }            
            }
            printk(KERN_ALERT "Write Value = %d\n", poweroff_value);
        break;
        case CTL10_POWEROFF:
            if( copy_from_user(&poweroff_value ,(int32_t*) value, sizeof(poweroff_value)) )
            {
                printk(KERN_ALERT "Data Write Err!\n");
            }
            printk(KERN_ALERT "Write Value = %d\n", poweroff_value);
        break;
        case CTL10_STATEOFF:
            if( copy_to_user((int32_t*) value, &poweroff_value, sizeof(poweroff_value)) )
            {
                printk(KERN_ALERT "Data Read : Err!\n");
            }
            printk(KERN_ALERT "Read Value = %d\n", value);
        break;

        default:
            //pr_info("Default\n");
        break;
    }    
    return 0;
}

static loff_t	lsbc_llseek (struct file *f, loff_t position, int flags){
    printk(KERN_ALERT "Executing LSEEK (%d)(%d)\n", flags, position);    
    switch(flags){
        case SEEK_SET:
            printk(KERN_ALERT "Control seek begin \n"); 
        break;    
        case SEEK_END:
            printk(KERN_ALERT "Control seek final \n");
        break;
        case SEEK_CUR:
            printk(KERN_ALERT "Control seek current \n");
        break;
    }
    return 0;
}

static struct file_operations FileOps =
{
    .owner        = THIS_MODULE,
    .open         = lsbc_open,
    .read         = lsbc_read,
    .write        = lsbc_write,
    .release      = lsbc_close,
    .compat_ioctl = lsbc_ioctl,
    .unlocked_ioctl= lsbc_ioctl2,
    .llseek       = lsbc_llseek
};
 
int thread_function(void *pv) 
{
    int i=0;
    while(!kthread_should_stop())
    {
        #ifdef DEBUG_C
        printk(KERN_ALERT "In Thread Function %d\n", i++);
        #endif
        msleep(10);
    } 
    return 0; 
}

static int qtl_init(void)
{
    //init_result = register_chrdev( 0, "gpio", &FileOps );
    

    init_result = alloc_chrdev_region( &first, 0, 1, REGION_NAME );
 
    if( 0 > init_result )
    {
        printk( KERN_ALERT "Device Registration failed\n" );
        return -1;
    }
    else
    {
        #ifdef DEBUG_C
        printk( KERN_ALERT "Major number is : %d\n",init_result );
        #endif        
        //return 0;
    }
    #ifdef DEBUG_C
    printk(KERN_ALERT "Device loading 10%%. -D %x=%d\n", first, first);
    #endif    
    if ( (cl = class_create( THIS_MODULE, CLASS_NAME ) ) == NULL )
    {
        printk( KERN_ALERT "Class creation failed\n" );
        goto r_device;
    }
    #ifdef DEBUG_C
    printk(KERN_ALERT "Device loading 20%%. -D %x=%d\n", cl,  first);
    #endif
    if( device_create( cl, NULL, first, NULL, DEVICE_NAME ) == NULL )
    {
        printk( KERN_ALERT "Device creation failed\n" );
        goto r_device;
    }
    #ifdef DEBUG_C
    printk(KERN_ALERT "Device loading 40%%. -D %x=%d\n", FileOps, first);
    #endif
    cdev_init( &c_dev, &FileOps );
    FileOps.owner = THIS_MODULE;
    #ifdef DEBUG_C
    printk(KERN_ALERT "Device loading 60%%. -D %x=%d\n", c_dev, first);
    #endif
    if( cdev_add( &c_dev, first, 1 ) == -1)
    {
        printk( KERN_ALERT "Device addition failed\n" );
        goto r_device;
    }
    mcstr = (struct unicode_cstring*)kmalloc(sizeof(struct unicode_cstring), GFP_KERNEL);
	if (mcstr ==NULL){
		printk(KERN_ALERT "allocator address: 0x%p\n", mcstr);
        goto r_device;
    }
    mcstr->buf = (char *) kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (mcstr->buf ==NULL){
		printk(KERN_ALERT "allocator address: 0x%p\n", mcstr->buf);
        goto r_device;
    }
    mcstr->len = 0;
    
    etx_thread = kthread_create(thread_function,NULL,"eTx Thread"); 
    if(etx_thread) 
    {
        #ifdef DEBUG_C
        printk(KERN_ALERT "Kthread Created Successfully...\n");
        #endif
        wake_up_process(etx_thread); 
    }    
    else 
    {
        printk(KERN_ALERT "Cannot create kthread\n"); 
        goto r_device;
    }
    #ifdef KRUN_C
    etx_thread = kthread_run(thread_function,NULL,"eTx Thread");
    if(etx_thread) {
        printk(KERN_ALERT "Kthread Created Successfully...\n");
    } else {
        printk(KERN_ALERT "Cannot create kthread\n");
        goto r_device;
    }
    #endif
    printk(KERN_ALERT "QTL Device initialized success\n");

    return 0;
    r_device:
        if(etx_thread!=NULL) kthread_stop(etx_thread); 
        if(mcstr!=NULL){
            if(mcstr->buf!=NULL) kfree(mcstr->buf);             
            kfree(mcstr);
        }        
        device_destroy( cl, first );
        class_destroy( cl );
        unregister_chrdev_region( first, 1 );
    
    return -1;
}
 
void qtl_exit(void)
{
    kthread_stop(etx_thread);
    //unregister_chrdev( init_result, "gpio" );
    if(mcstr!=NULL){
        if(mcstr->buf!=NULL){
            kfree(mcstr->buf);        
        }
        kfree(mcstr);
    }
    cdev_del( &c_dev );
    device_destroy( cl, first );
    class_unregister( cl); 
    class_destroy( cl );
    unregister_chrdev_region( first, 1 );
 
    printk(KERN_ALERT "QTL Device unregistered\n");
}

module_init(qtl_init);
module_exit(qtl_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("mathawarin kongmoeng <songkeart@gmail.com>");
MODULE_DESCRIPTION("Our First Character Driver");
MODULE_VERSION("1.0");

