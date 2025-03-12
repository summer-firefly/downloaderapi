#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/blkdev.h>
#include <linux/vmalloc.h>
#include <stdbool.h>
struct timeval {
    long tv_sec;
    long tv_usec;
};
#include "hnsa_download.h"
MODULE_IMPORT_NS(VFS_internal_I_am_really_a_filesystem_and_am_NOT_a_driver);
#define HNSA_DOWNLOAD_MAGIC_NUMBER 'D'
#define CLOSE_DEBUG_MESSAGE _IO('M', 0)
#define OPEN_DEBUG_MESSAGE _IO('M', 1)
#define HNSA_DOWNLOAD_CAPABILITY_INDEX 0
#define HNSA_DOWNLOAD_PARAMS_INDEX 1
#define UINT32_T_PRIVATE_DATA_INDEX 2
#define HNSA_DOWNLOAD_VERSION_INDEX 3
#define HNSA_DOWNLOAD_ID_IMAGE_INDEX 4
#define HNSA_DOWNLOAD_REPORT_INDEX 5
#define HNSA_DOWNLOAD_BL_RESOURCES_INDEX 6
#define HNSA_DOWNLOAD_EVENT_INDEX 7
#define UINT32_T_PERIODICITY_INDEX 8
#define HNSA_DOWNLOAD_BL_COUNTRY_INDEX 9
#define MAX_INDEX HNSA_DOWNLOAD_BL_COUNTRY_INDEX
#define START_ADDRESS 0x00
#define CRC_LENGTH sizeof(uint32_t)
#define REDUNDANT_SIZE (1024 * 1024) // 1kb redundant size
#define PARTITION_PATH "/dev/mmcblk0p7"
#define DEVICE_NAME "Mailbox"
#define DOWNLOADER_API_VERSION "2025.3.11_date"
static uint64_t startAddresses[MAX_INDEX + 1];
static struct block_device *bdev;
static struct file *file;
static struct class *my_class = NULL;
static struct device *my_device = NULL;
static struct cdev myCdev;
static int majorNumber;
static bool debug = true;

static uint32_t crc_table[256] = {
    0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b,
    0x1a864db2, 0x1e475005, 0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
    0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd, 0x4c11db70, 0x48d0c6c7,
    0x4593e01e, 0x4152fda9, 0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
    0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011, 0x791d4014, 0x7ddc5da3,
    0x709f7b7a, 0x745e66cd, 0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
    0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5, 0xbe2b5b58, 0xbaea46ef,
    0xb7a96036, 0xb3687d81, 0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
    0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49, 0xc7361b4c, 0xc3f706fb,
    0xceb42022, 0xca753d95, 0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,
    0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d, 0x34867077, 0x30476dc0,
    0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
    0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16, 0x018aeb13, 0x054bf6a4,
    0x0808d07d, 0x0cc9cdca, 0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,
    0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02, 0x5e9f46bf, 0x5a5e5b08,
    0x571d7dd1, 0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
    0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e, 0xbfa1b04b, 0xbb60adfc,
    0xb6238b25, 0xb2e29692, 0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
    0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a, 0xe0b41de7, 0xe4750050,
    0xe9362689, 0xedf73b3e, 0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
    0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34,
    0xdc3abded, 0xd8fba05a, 0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,
    0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb, 0x4f040d56, 0x4bc510e1,
    0x46863638, 0x42472b8f, 0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
    0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5,
    0x3f9b762c, 0x3b5a6b9b, 0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
    0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623, 0xf12f560e, 0xf5ee4bb9,
    0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
    0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f, 0xc423cd6a, 0xc0e2d0dd,
    0xcda1f604, 0xc960ebb3, 0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
    0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b, 0x9b3660c6, 0x9ff77d71,
    0x92b45ba8, 0x9675461f, 0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
    0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640, 0x4e8ee645, 0x4a4ffbf2,
    0x470cdd2b, 0x43cdc09c, 0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,
    0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24, 0x119b4be9, 0x155a565e,
    0x18197087, 0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
    0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d, 0x2056cd3a,
    0x2d15ebe3, 0x29d4f654, 0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,
    0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c, 0xe3a1cbc1, 0xe760d676,
    0xea23f0af, 0xeee2ed18, 0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
    0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662,
    0x933eb0bb, 0x97ffad0c, 0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
    0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4};

static uint32_t calculate_crc32(void *data, int len) {
    register int i;
    char *cdata = (char *)data;
    uint32_t crc = 0xffffffff;
    for (i = 0; i < len; i++) {
        crc = (crc << 8) ^ crc_table[((crc >> 24) ^ *cdata++) & 0xff];
    }
    return crc;
}
static void logDebug(const char *file, const char *func, int line, const char *message) {
    if (debug) {
        printk(KERN_INFO "Debug info: File: %s, Function: %s, Line: %d , message : %s\n", file, func, line, message);
    }
}
static int device_open(struct inode *inode, struct file *file) {
    logDebug(__FILE__, __func__, __LINE__, "downloader API kernel driver code version : " DOWNLOADER_API_VERSION);
    logDebug(__FILE__, __func__, __LINE__, "call device_open");
    return 0;
}
static int device_release(struct inode *inode, struct file *file) {
    logDebug(__FILE__, __func__, __LINE__, "call device_release");
    return 0;
}
static int open(void) {
    bdev = blkdev_get_by_path(PARTITION_PATH, FMODE_READ | FMODE_WRITE, NULL);
    if (IS_ERR(bdev)) {
        logDebug(__FILE__, __func__, __LINE__, "open error");
        return -1;
    }
    file = filp_open(PARTITION_PATH, O_RDWR, 0);
    if (IS_ERR(file)) {
        logDebug(__FILE__, __func__, __LINE__, "open error");
        blkdev_put(bdev, FMODE_READ | FMODE_WRITE);
        return -1;
    }
    return 0;
}
static int write(void *data, int len, loff_t pos) {
    return kernel_write(file, data, len, &pos);
}
static int read(void *data, int len, loff_t pos) {
    return kernel_read(file, data, len, &pos);
}
static void close(void) {
    filp_close(file, NULL);
    blkdev_put(bdev, FMODE_READ | FMODE_WRITE);
}

static void initStartAddresses(void) {
    uint64_t address = START_ADDRESS;
    startAddresses[HNSA_DOWNLOAD_CAPABILITY_INDEX] = address;
    address += sizeof(hnsa_download_capability_t) + CRC_LENGTH + REDUNDANT_SIZE;
    startAddresses[HNSA_DOWNLOAD_PARAMS_INDEX] = address;
    address += sizeof(hnsa_download_params_t) + CRC_LENGTH + REDUNDANT_SIZE;
    startAddresses[UINT32_T_PRIVATE_DATA_INDEX] = address;
    address += sizeof(uint32_t) + CRC_LENGTH + REDUNDANT_SIZE;
    startAddresses[HNSA_DOWNLOAD_VERSION_INDEX] = address;
    address += sizeof(hnsa_download_version_t) + CRC_LENGTH + REDUNDANT_SIZE;
    startAddresses[HNSA_DOWNLOAD_ID_IMAGE_INDEX] = address;
    address += sizeof(hnsa_download_id_image_t) + CRC_LENGTH + REDUNDANT_SIZE;
    startAddresses[HNSA_DOWNLOAD_REPORT_INDEX] = address;
    address += sizeof(hnsa_download_report_t) + CRC_LENGTH + REDUNDANT_SIZE;
    startAddresses[HNSA_DOWNLOAD_BL_RESOURCES_INDEX] = address;
    address += sizeof(hnsa_download_bl_resources_t) + CRC_LENGTH + REDUNDANT_SIZE;
    startAddresses[HNSA_DOWNLOAD_EVENT_INDEX] = address;
    address += sizeof(hnsa_download_event_t) + CRC_LENGTH + REDUNDANT_SIZE;
    startAddresses[UINT32_T_PERIODICITY_INDEX] = address;
    address += sizeof(uint32_t) + CRC_LENGTH + REDUNDANT_SIZE;
    startAddresses[HNSA_DOWNLOAD_BL_COUNTRY_INDEX] = address;
    address += sizeof(hnsa_download_bl_country_t) + CRC_LENGTH + REDUNDANT_SIZE;
}
static long readAndVerification(uint64_t offset, void *params, int size) {
    int read_crc = -1;
    int check_crc = 1;
    int len = 0;
    char message[128] = {0};
    if (open() < 0) {
        logDebug(__FILE__, __func__, __LINE__, "open error");
        close();
        return -1;
    }
    len = read(&read_crc, CRC_LENGTH, offset);
    if (len == -1) {
        logDebug(__FILE__, __func__, __LINE__, "read crc error");
        close();
        return -1;
    }
    sprintf(message, "partition storage crc is 0x%x", read_crc);
    logDebug(__FILE__, __func__, __LINE__, message);
    len = read(params, size, offset + CRC_LENGTH);
    if (len == -1) {
        logDebug(__FILE__, __func__, __LINE__, "read error");
        close();
        return -1;
    }
    logDebug(__FILE__, __func__, __LINE__, "Read actual content successfully");
    check_crc = calculate_crc32(params, size);
    sprintf(message, "calculate actual content crc is 0x%x", check_crc);
    logDebug(__FILE__, __func__, __LINE__, message);
    if (check_crc != read_crc) {
        logDebug(__FILE__, __func__, __LINE__, "Verification failed");
        close();
        return -1;
    }
    close();
    return 0;
}
static long writeWithCRC_checksum(uint64_t offset, void *params, int size) {
    uint32_t crc = 0;
    int len = 0;
    char message[128] = {0};
    if (open() < 0) {
        logDebug(__FILE__, __func__, __LINE__, "open error");
        close();
        return -1;
    }
    crc = calculate_crc32(params, size);
    sprintf(message, "calculate crc is 0x%x", crc);
    logDebug(__FILE__, __func__, __LINE__, message);
    len = write(&crc, CRC_LENGTH, offset);
    if (len == -1) {
        logDebug(__FILE__, __func__, __LINE__, "write crc error");
        close();
        return -1;
    }
    logDebug(__FILE__, __func__, __LINE__, "write crc success");
    len = write(params, size, offset + CRC_LENGTH);
    if (len == -1) {
        logDebug(__FILE__, __func__, __LINE__, "write content error");
        close();
        return -1;
    }
    logDebug(__FILE__, __func__, __LINE__, "write content success");
    close();
    return 0;
}
static long getCapabilities(unsigned long arg) {
    hnsa_download_capability_t *cap = (hnsa_download_capability_t *)vmalloc(sizeof(hnsa_download_capability_t));
    uint64_t offset = startAddresses[HNSA_DOWNLOAD_CAPABILITY_INDEX];
    logDebug(__FILE__, __func__, __LINE__, "call getCapabilities");
    if (!cap) {
        logDebug(__FILE__, __func__, __LINE__, "vmalloc error");
        return -EIO;
    }
    if (readAndVerification(offset, cap, sizeof(hnsa_download_capability_t)) == -1) {
        vfree(cap);
        return -EIO;
    }
    if (copy_to_user((hnsa_download_capability_t __user *)arg, cap, sizeof(hnsa_download_capability_t))) {
        vfree(cap);
        return -EFAULT;
    }
    vfree(cap);
    return 0;
}
static long readParams(unsigned long arg) {
    hnsa_download_params_t *download_read = (hnsa_download_params_t *)vmalloc(sizeof(hnsa_download_params_t));
    uint64_t offset = startAddresses[HNSA_DOWNLOAD_PARAMS_INDEX];
    logDebug(__FILE__, __func__, __LINE__, "call readParams");
    if (!download_read) {
        logDebug(__FILE__, __func__, __LINE__, "vmalloc error");
        return -EIO;
    }
    if (readAndVerification(offset, download_read, sizeof(hnsa_download_params_t)) == -1) {
        vfree(download_read);
        return -EIO;
    }
    if (copy_to_user((hnsa_download_params_t __user *)arg, download_read, sizeof(hnsa_download_params_t))) {
        vfree(download_read);
        return -EFAULT;
    }
    vfree(download_read);
    return 0;
}
static long writeParams(unsigned long arg) {
    hnsa_download_params_t *download_params = (hnsa_download_params_t *)vmalloc(sizeof(hnsa_download_params_t));
    uint64_t offset = startAddresses[HNSA_DOWNLOAD_PARAMS_INDEX];
    logDebug(__FILE__, __func__, __LINE__, "call writeParams");
    if (!download_params) {
        logDebug(__FILE__, __func__, __LINE__, "vmalloc error");
        return -EIO;
    }
    if (copy_from_user(download_params, (hnsa_download_params_t __user *)arg, sizeof(hnsa_download_params_t))) {
        vfree(download_params);
        return -EFAULT;
    }
    if (writeWithCRC_checksum(offset, download_params, sizeof(hnsa_download_params_t)) == -1) {
        vfree(download_params);
        return -EIO;
    }
    vfree(download_params);
    return 0;
}
static long readPrivData(unsigned long arg) {
    uint32_t private_data;
    uint64_t offset = startAddresses[UINT32_T_PRIVATE_DATA_INDEX];
    logDebug(__FILE__, __func__, __LINE__, "call readPrivData");
    if (readAndVerification(offset, &private_data, sizeof(private_data)) == -1) {
        return -EIO;
    }
    if (copy_to_user((uint32_t __user *)arg, &private_data, sizeof(private_data))) {
        return -EFAULT;
    }
    return 0;
}
static long writePrivData(unsigned long arg) {
    uint32_t private_data;
    uint64_t offset = startAddresses[UINT32_T_PRIVATE_DATA_INDEX];
    logDebug(__FILE__, __func__, __LINE__, "call writePrivData");
    if (copy_from_user(&private_data, (uint32_t __user *)arg, sizeof(uint32_t))) {
        return -EFAULT;
    }
    if (writeWithCRC_checksum(offset, &private_data, sizeof(private_data)) < 0) {
        return -EIO;
    }
    return 0;
}
static long activateLoader(unsigned long arg) {
    // The parameter passed in by the user layer is a null pointer
    logDebug(__FILE__, __func__, __LINE__, "call activateLoader");
    return 0;
}
static long getVersion(unsigned long arg) {
    hnsa_download_version_t *version = (hnsa_download_version_t *)vmalloc(sizeof(hnsa_download_version_t));
    uint64_t offset = startAddresses[HNSA_DOWNLOAD_VERSION_INDEX];
    logDebug(__FILE__, __func__, __LINE__, "call getVersion");
    if (!version) {
        logDebug(__FILE__, __func__, __LINE__, "vmalloc error");
        return -EIO;
    }
    if (readAndVerification(offset, version, sizeof(hnsa_download_version_t)) == -1) {
        vfree(version);
        return -EIO;
    }
    if (copy_to_user((hnsa_download_version_t __user *)arg, version, sizeof(hnsa_download_version_t))) {
        vfree(version);
        return -EFAULT;
    }
    vfree(version);
    return 0;
}
static long getIdImage(unsigned long arg) {
    hnsa_download_id_image_t *id_image = (hnsa_download_id_image_t *)vmalloc(sizeof(hnsa_download_id_image_t));
    uint64_t offset = startAddresses[HNSA_DOWNLOAD_ID_IMAGE_INDEX];
    logDebug(__FILE__, __func__, __LINE__, "call getIdImage");
    if (!id_image) {
        logDebug(__FILE__, __func__, __LINE__, "vmalloc error");
        return -EIDRM;
    }
    if (readAndVerification(offset, id_image, sizeof(hnsa_download_id_image_t)) == -1) {
        vfree(id_image);
        return -EIO;
    }
    if (copy_to_user((hnsa_download_id_image_t __user *)arg, id_image, sizeof(hnsa_download_id_image_t))) {
        vfree(id_image);
        return -EFAULT;
    }
    vfree(id_image);
    return 0;
}
static long getReport(unsigned long arg) {
    hnsa_download_report_t *report = (hnsa_download_report_t *)vmalloc(sizeof(hnsa_download_report_t));
    uint64_t offset = startAddresses[HNSA_DOWNLOAD_REPORT_INDEX];
    logDebug(__FILE__, __func__, __LINE__, "call getReport");
    if (!report) {
        logDebug(__FILE__, __func__, __LINE__, "vmalloc error");
        return -EIO;
    }
    if (readAndVerification(offset, report, sizeof(hnsa_download_report_t)) == -1) {
        vfree(report);
        return -EIO;
    }
    if (copy_to_user((hnsa_download_report_t __user *)arg, report, sizeof(hnsa_download_report_t))) {
        vfree(report);
        return -EFAULT;
    }
    vfree(report);
    return 0;
}
static long bootloaderAllocResource(unsigned long arg) {
    // hnsa_download_bl_resources_t resource;
    logDebug(__FILE__, __func__, __LINE__, "call bootloaderAllocResource");
    return 0;
}
static long bootloaderFreeResource(unsigned long arg) {
    // The parameter passed in by the user layer is a null pointer
    logDebug(__FILE__, __func__, __LINE__, "call bootloaderFreeResource");
    return 0;
}
static long getEvent(unsigned long arg) {
    hnsa_download_event_t *event = (hnsa_download_event_t *)vmalloc(sizeof(hnsa_download_event_t));
    uint64_t offset = startAddresses[HNSA_DOWNLOAD_EVENT_INDEX];
    logDebug(__FILE__, __func__, __LINE__, "call getEvent");

    if (!event) {
        logDebug(__FILE__, __func__, __LINE__, "vmalloc error");
        return -EIO;
    }
    if (readAndVerification(offset, event, sizeof(hnsa_download_event_t)) == -1) {
        vfree(event);
        return -EIO;
    }
    if (copy_to_user((hnsa_download_event_t __user *)arg, event, sizeof(hnsa_download_event_t))) {
        vfree(event);
        return -EFAULT;
    }
    vfree(event);
    return 0;
}
static long bootloaderSearchAvaliableIdImage(unsigned long arg) {
    // The parameter passed in by the user layer is a null pointer
    logDebug(__FILE__, __func__, __LINE__, "call bootloaderSearchAvaliableIdImage");
    return 0;
}
static long bootloaderSetAutoSearchPeriodicity(unsigned long arg) {
    uint32_t periodicity;
    uint64_t offset = startAddresses[UINT32_T_PERIODICITY_INDEX];
    logDebug(__FILE__, __func__, __LINE__, "call bootloaderSetAutoSearchPeriodicity");
    if (copy_from_user(&periodicity, (uint32_t __user *)arg, sizeof(periodicity))) {
        return -EFAULT;
    }
    if (writeWithCRC_checksum(offset, &periodicity, sizeof(periodicity)) < 0) {
        return -EIO;
    }
    return 0;
}
static long bootloaderGetAutoSearchPeriodicity(unsigned long arg) {
    uint32_t periodicity;
    uint64_t offset = startAddresses[UINT32_T_PERIODICITY_INDEX];
    logDebug(__FILE__, __func__, __LINE__, "call bootloaderGetAutoSearchPeriodicity");
    if (readAndVerification(offset, &periodicity, sizeof(periodicity)) == -1) {
        return -EIO;
    }
    if (copy_to_user((uint32_t __user *)arg, &periodicity, sizeof(periodicity))) {
        return -EFAULT;
    }
    return 0;
}
static long getAvailableIdImage(unsigned long arg) {
    // hnsa_download_id_image_t available_id_image;
    logDebug(__FILE__, __func__, __LINE__, "call getAvailableIdImage");
    return 0;
}
static long installIdImage(unsigned long arg) {
    // hnsa_download_id_image_t install_id_image;
    logDebug(__FILE__, __func__, __LINE__, "call installIdImage");
    return 0;
}
static long resetReport(unsigned long arg) {
    // The parameter passed in by the user layer is a null pointer
    logDebug(__FILE__, __func__, __LINE__, "call resetReport");
    return 0;
}
static long bootloaderSetCountry(unsigned long arg) {
    // hnsa_download_bl_country_t bl_country;
    logDebug(__FILE__, __func__, __LINE__, "call bootloaderSetCountry");
    return 0;
}
static void openDebug(void) {
    logDebug(__FILE__, __func__, __LINE__, "call openDebug");
    debug = true;
}
static void closeDebug(void) {
    logDebug(__FILE__, __func__, __LINE__, "call closeDebug");
    debug = false;
}
static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    int result = 0;
    logDebug(__FILE__, __func__, __LINE__, "call device_ioctl");
    switch (cmd) {
    case HNSA_DOWNLOAD_READ_PARAMS:
        result = readParams(arg);
        break;
    case HNSA_DOWNLOAD_WRITE_PARAMS:
        result = writeParams(arg);
        break;
    case HNSA_DOWNLOAD_READ_PRIV_DATA:
        result = readPrivData(arg);
        break;
    case HNSA_DOWNLOAD_WRITE_PRIV_DATA:
        result = writePrivData(arg);
        break;
    case HNSA_DOWNLOAD_GET_CAPABILITIES:
        result = getCapabilities(arg);
        break;
    case HNSA_DOWNLOAD_ACTIVATE_LOADER:
        result = activateLoader(arg);
        break;
    case HNSA_DOWNLOAD_GET_VERSION:
        result = getVersion(arg);
        break;
    case HNSA_DOWNLOAD_GET_ID_IMAGE:
        result = getIdImage(arg);
        break;
    case HNSA_DOWNLOAD_GET_REPORT:
        result = getReport(arg);
        break;
    case HNSA_DOWNLOAD_BL_ALLOC_RESOURCES:
        result = bootloaderAllocResource(arg);
        break;
    case HNSA_DOWNLOAD_BL_FREE_RESOURCES:
        result = bootloaderFreeResource(arg);
        break;
    case HNSA_DOWNLOAD_GET_EVENT:
        result = getEvent(arg);
        break;
    case HNSA_DOWNLOAD_BL_SEARCH_AVAILABLE_ID_IMAGE:
        result = bootloaderSearchAvaliableIdImage(arg);
        break;
    case HNSA_DOWNLOAD_BL_SET_AUTOSEARCH_PERIODICITY:
        result = bootloaderSetAutoSearchPeriodicity(arg);
        break;
    case HNSA_DOWNLOAD_BL_GET_AUTOSEARCH_PERIODICITY:
        result = bootloaderGetAutoSearchPeriodicity(arg);
        break;
    case HNSA_DOWNLOAD_GET_AVAILABLE_ID_IMAGE:
        result = getAvailableIdImage(arg);
        break;
    case HNSA_DOWNLOAD_INSTALL_ID_IMAGE:
        result = installIdImage(arg);
        break;
    case HNSA_DOWNLOAD_RESET_REPORT:
        result = resetReport(arg);
        break;
    case HNSA_DOWNLOAD_BL_SET_COUNTRY:
        result = bootloaderSetCountry(arg);
        break;
    case HNSA_DOWNLOAD_SET_BOOT_COUNTER:
        result = -1;
        break;
    case OPEN_DEBUG_MESSAGE:
        openDebug();
        break;
    case CLOSE_DEBUG_MESSAGE:
        closeDebug();
        break;
    default:
        result = -1;
        break;
    }
    return result;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .unlocked_ioctl = device_ioctl,
    .compat_ioctl = device_ioctl,
};

static int initWork(void) {
    char message[128] = {0};
    initStartAddresses();
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
        logDebug(__FILE__, __func__, __LINE__, "Failed to register character device");
        return -1;
    }
    sprintf(message, "Device registered with major number is %d", majorNumber);
    logDebug(__FILE__, __func__, __LINE__, message);
    my_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(my_class)) {
        unregister_chrdev(majorNumber, DEVICE_NAME);
        logDebug(__FILE__, __func__, __LINE__, "Failed to register device class");
        return -1;
    }
    logDebug(__FILE__, __func__, __LINE__, "mychardev: device class registered correctly");
    my_device = device_create(my_class, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(my_class)) {
        class_destroy(my_class);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        logDebug(__FILE__, __func__, __LINE__, "Failed to create the device");
        return -1;
    }
    logDebug(__FILE__, __func__, __LINE__, "mychardev: device class created correctly");
    cdev_init(&myCdev, &fops);
    myCdev.owner = THIS_MODULE;
    if (cdev_add(&myCdev, MKDEV(majorNumber, 0), 1) == -1) {
        device_destroy(my_class, MKDEV(majorNumber, 0));
        class_destroy(my_class);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        logDebug(__FILE__, __func__, __LINE__, "Failed to add cdev");
        return -1;
    }
    return 0;
}
static void exitWork(void) {
    cdev_del(&myCdev);
    device_destroy(my_class, MKDEV(majorNumber, 0));
    class_unregister(my_class);
    class_destroy(my_class);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    logDebug(__FILE__, __func__, __LINE__, "Device unregistered");
}

static int __init DownloadAPI_KernelInit(void) {
    int res = 0;
    logDebug(__FILE__, __func__, __LINE__, "call DownloadAPI_KernelInit");
    res = initWork();
    if (res < 0) {
        logDebug(__FILE__, __func__, __LINE__, "init error");
    }
    return res;
}
static void __exit DownloadAPI_KernelExit(void) {
    exitWork();
    logDebug(__FILE__, __func__, __LINE__, "call DownloadAPI_KernelExit");
}
module_init(DownloadAPI_KernelInit);
module_exit(DownloadAPI_KernelExit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("HeSiWen@skyworth.com");
MODULE_DESCRIPTION("Downloader API kernel code");