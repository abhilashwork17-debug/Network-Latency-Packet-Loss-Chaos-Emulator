#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/string.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "chaos_driver"
#define CLASS_NAME "chaos"
#define CONFIG_SIZE 256

#define CHAOS_IOCTL_MAGIC 'C'

struct chaos_config
{
    int latency;
    int packet_loss;
    int jitter;
};

#define CHAOS_IOCTL_SET_CONFIG \
    _IOW(CHAOS_IOCTL_MAGIC, 1, struct chaos_config)

#define CHAOS_IOCTL_GET_CONFIG \
    _IOR(CHAOS_IOCTL_MAGIC, 2, struct chaos_config)

static dev_t chaos_dev;
static struct cdev chaos_cdev;
static struct class *chaos_class;
static struct device *chaos_device;

static DEFINE_MUTEX(chaos_mutex);

static char configuration[CONFIG_SIZE] = "No configuration received\n";

static struct chaos_config current_config = {
    .latency = 0,
    .packet_loss = 0,
    .jitter = 0};

static int chaos_open(struct inode *inode, struct file *file)
{
    pr_info("chaos_driver: device opened\n");
    return 0;
}

static int chaos_release(struct inode *inode, struct file *file)
{
    pr_info("chaos_driver: device closed\n");
    return 0;
}

static ssize_t chaos_read(
    struct file *file,
    char __user *buffer,
    size_t length,
    loff_t *offset)
{
    size_t message_length;

    mutex_lock(&chaos_mutex);

    if (*offset == 0)
        message_length = strlen(configuration);
    else
        message_length = 0;

    if (message_length == 0)
    {
        mutex_unlock(&chaos_mutex);
        return 0;
    }

    if (length > message_length)
        length = message_length;

    if (copy_to_user(buffer, configuration, length))
    {
        mutex_unlock(&chaos_mutex);
        return -EFAULT;
    }

    *offset += length;

    mutex_unlock(&chaos_mutex);

    return length;
}

static ssize_t chaos_write(
    struct file *file,
    const char __user *buffer,
    size_t length,
    loff_t *offset)
{
    size_t copy_length;

    if (length == 0)
        return 0;

    copy_length = length;

    if (copy_length >= CONFIG_SIZE)
        copy_length = CONFIG_SIZE - 1;

    mutex_lock(&chaos_mutex);

    memset(configuration, 0, CONFIG_SIZE);

    if (copy_from_user(configuration, buffer, copy_length))
    {
        mutex_unlock(&chaos_mutex);
        return -EFAULT;
    }

    configuration[copy_length] = '\0';

    pr_info("chaos_driver: configuration received: %s",
            configuration);

    mutex_unlock(&chaos_mutex);

    return copy_length;
}

static long chaos_ioctl(
    struct file *file,
    unsigned int command,
    unsigned long argument)
{
    struct chaos_config config;

    switch (command)
    {

    case CHAOS_IOCTL_SET_CONFIG:

        if (copy_from_user(
                &config,
                (struct chaos_config __user *)argument,
                sizeof(config)))
            return -EFAULT;

        if (config.latency < 0 ||
            config.packet_loss < 0 ||
            config.packet_loss > 100 ||
            config.jitter < 0)
            return -EINVAL;

        mutex_lock(&chaos_mutex);

        current_config = config;

        mutex_unlock(&chaos_mutex);

        pr_info(
            "chaos_driver: config set - latency=%d ms, loss=%d%%, jitter=%d ms\n",
            config.latency,
            config.packet_loss,
            config.jitter);

        return 0;

    case CHAOS_IOCTL_GET_CONFIG:

        mutex_lock(&chaos_mutex);

        config = current_config;

        mutex_unlock(&chaos_mutex);

        if (copy_to_user(
                (struct chaos_config __user *)argument,
                &config,
                sizeof(config)))
            return -EFAULT;

        return 0;

    default:
        return -ENOTTY;
    }
}

static const struct file_operations chaos_fops = {
    .owner = THIS_MODULE,
    .open = chaos_open,
    .read = chaos_read,
    .write = chaos_write,
    .unlocked_ioctl = chaos_ioctl,
    .release = chaos_release,
};

static int __init chaos_driver_init(void)
{
    int ret;

    pr_info("chaos_driver: initializing\n");

    ret = alloc_chrdev_region(&chaos_dev, 0, 1, DEVICE_NAME);
    if (ret < 0)
    {
        pr_err("chaos_driver: failed to allocate device number\n");
        return ret;
    }

    cdev_init(&chaos_cdev, &chaos_fops);
    chaos_cdev.owner = THIS_MODULE;

    ret = cdev_add(&chaos_cdev, chaos_dev, 1);
    if (ret < 0)
    {
        pr_err("chaos_driver: failed to add cdev\n");
        unregister_chrdev_region(chaos_dev, 1);
        return ret;
    }

    chaos_class = class_create(CLASS_NAME);

    if (IS_ERR(chaos_class))
    {
        pr_err("chaos_driver: failed to create class\n");
        cdev_del(&chaos_cdev);
        unregister_chrdev_region(chaos_dev, 1);
        return PTR_ERR(chaos_class);
    }

    chaos_device = device_create(
        chaos_class,
        NULL,
        chaos_dev,
        NULL,
        DEVICE_NAME);

    if (IS_ERR(chaos_device))
    {
        pr_err("chaos_driver: failed to create device\n");
        class_destroy(chaos_class);
        cdev_del(&chaos_cdev);
        unregister_chrdev_region(chaos_dev, 1);
        return PTR_ERR(chaos_device);
    }

    mutex_init(&chaos_mutex);

    pr_info("chaos_driver: loaded successfully\n");

    return 0;
}

static void __exit chaos_driver_exit(void)
{
    device_destroy(chaos_class, chaos_dev);
    class_destroy(chaos_class);
    cdev_del(&chaos_cdev);
    unregister_chrdev_region(chaos_dev, 1);

    pr_info("chaos_driver: unloaded\n");
}

module_init(chaos_driver_init);
module_exit(chaos_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhilash Mishra");
MODULE_DESCRIPTION("Linux character device driver for Network Latency and Packet Loss Chaos Emulator");
MODULE_VERSION("1.0");
