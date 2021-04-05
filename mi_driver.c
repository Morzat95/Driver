#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h> /* for put_user */

/*
 * Prototypes - this would normally go in a .h file
 */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "mi_device"
#define BUFF_LEN 80

/*
 * Global variables are declared as static, so are global within the file.
 */

/*
 * Major number assigned to our device driver
 */
static int Major = 0;

/* Is device open? 
 * Used to prevent multiple access to device
 */
static int Device_Open = 0;

/*
 * The message the device will give when asked
 */
static char Message[BUFF_LEN];

/*
 * How far did the process reading the message get?
 * Userful if the message is larger than the size of the
 * buffer we get to fill in device_read.
 */
static char *Message_Ptr;

/*
 * Definimos las operaciones del driver para manipular el Char Device
 */
struct file_operations fops =
{
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

static char *message = "abcd"; /* Default test message */

static int inputMessageLength;

module_param(message, charp, 0);
MODULE_PARM_DESC(n, "A character string");

int init_module(void)
{
    /* Constructor */
    printk(KERN_INFO "================================================== INIT ==================================================\n");

    printk(KERN_INFO "Driver Reversor registrado. El mensaje a reversar es: %s\n", message);

    /* Registramos el Char Device */
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0)
    {
        printk(KERN_ALERT "Registering char device failed with %d\n", Major);
        return Major;
    }

    printk(KERN_INFO "I was assigned major number %d. Talk to\n", Major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");

    printk(KERN_INFO "================================================== END INIT ==================================================\n");

    return SUCCESS;
}

void cleanup_module(void)
{
    /* Destructor */
    printk(KERN_INFO "================================================== CLEANUP ==================================================\n");

    /*
     * Unregister the device
     */
    // int ret = unregister_chrdev(Major, DEVICE_NAME);
    unregister_chrdev(Major, DEVICE_NAME);

    // if (ret < 0)
    //     printk(KERN_ALERT "Error in unregister_chrdev: %d\n", ret);

    printk(KERN_INFO "Driver encriptador desregistrado\n");

    printk(KERN_INFO "================================================== CLEANUP END ==================================================\n");
}

/*
 * Methods
 */

/*
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *file)
{
    // static int counter = 0;

    if (Device_Open)
        return -EBUSY;

    Device_Open++;
    // sprintf(Message, "I already told you %d times Hello world!\n", counter++);
    Message_Ptr = Message;
    try_module_get(THIS_MODULE);

    return SUCCESS;
}

/*
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
    Device_Open--; /* We're now ready for our next caller */

    /*
     * Decrement the usage count, or else once you opened the file, you'll
     * never get rid of the module.
     */
    module_put(THIS_MODULE);

    return 0;
}

/*
 * Called when a process, wich already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read( struct file *file,      /* see include/linux/fs.h    */
                            char __user * buffer,   /* buffer to fill with data  */
                            size_t length,          /* length of the buffer      */
                            loff_t * offset)
{
    /*
     * Number of bytes actually written to the buffer
     */
    int bytes_read = 0;

    /*
     * If we're at the end of the message,
     * return 0 signifying end of file
     */
    if (*Message_Ptr == 0)
        return 0;

    /* Se recorre el mensaje de input de atrás hacia delante */
    Message_Ptr += inputMessageLength; /* Sitúo el puntero al final del mensaje */
    Message_Ptr--; /* elimino salto de línea */
    Message_Ptr--; /* último caracter válido */

    /*
     * Actually put the data into the buffer
     */
    while (length && *Message_Ptr)
    {
        /*
         * The buffer is in the user data segment, not the kernel
         * segment so "*" assignment won't work. We have to use
         * put_user wich copies data from kernel data segment to
         * the user data segment.
         */
        put_user(*(Message_Ptr--), buffer++);

        length--;
        bytes_read++;
    }

#ifdef DEBUG
    printk(KERN_INFO "Read %d bytes, %d left\n", bytes_read, length);
#endif

    /*
     * Most read functions return the number of bytes put into the buffer
     */
    return bytes_read;
    
}

/*
 * Called when a process writes to dev file: echo "hi" > /dev/hello
 */
static ssize_t device_write(struct file *file, const char __user * buffer, size_t length, loff_t * offset)
{

    int i;

    for (i = 0; i < length && i < BUFF_LEN; i++)
    {
        get_user(Message[i], buffer + i);
    }

    Message_Ptr = Message;

    inputMessageLength = i;

    /*
     * Again, return the number of input characters used
     */
    return i;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Federico");
MODULE_DESCRIPTION("Driver para reversar datos");