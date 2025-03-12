#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#define HNSA_DOWNLOAD_MAGIC_NUMBER 'D'
#include "hnsa_download.h"
#define DEVICE_NAME "/dev/Mailbox"
#define CLOSE_DEBUG_MESSAGE _IO('M', 0)
#define OPEN_DEBUG_MESSAGE _IO('M', 1)
static void printError() {
    switch (errno) {
    case EIO:
        printf("The device generates an internal error\n");
        break;
    case EFAULT:
        printf("Parameter points to a null or invalid address (in the memory management sense)\n");
        break;
    case EBADMSG:
        printf("Authentication of the buffer has failed\n");
        break;
    case EPERM:
        printf("The device is opened in read-only mode and the function requires a read-write permission\n");
        break;
    case EPROTO:
        printf("Event not received HNSA_DOWNLOAD_EVENT_BL_RESOURCES_NEEDED\n");
        break;
    case EOVERFLOW:
        printf("An overflow in event queue occurs, one or more events were lost\n");
        break;
    case EWOULDBLOCK:
        printf("There is no event pending\n");
        break;
    case EINVAL:
        printf("The enumerated value is greater than the last value.\n");
        break;
    default:
        printf("kernel happen other error\n");
        break;
    }
}
static void readParamsTest() {
    int fd = open(DEVICE_NAME, O_RDWR);
    int res = 0;
    hnsa_download_params_t params;
    if (fd < 0) {
        perror("Failed to open the device");
        return;
    }
    res = ioctl(fd, HNSA_DOWNLOAD_READ_PARAMS, &params);
    if (res == -1) {
        printf("read params error\n");
        printError();
    } else {
        printf("read params success\n");
        printf("The content read is as follows: \n");
        printf("params.mode : %d\n", params.mode);
        printf("params.params_type : %d\n", params.params_type);
        printf("params.download_request : %d\n", params.download_request);
        printf("params.ntp_server_address : %s\n", params.ntp_server_address);
        printf("params.timeoffset : %s\n", params.timeoffset);
        printf("params.country : %s\n", params.country);
        printf("params.nb_images : %d\n", params.nb_images);
        for (int i = 0; i < HNSA_DOWNLOAD_MAX_IMAGE; i++) {
            printf("params.settings_images[%d] info : \n", i);
            hnsa_download_settings_image_t *p = &params.settings_images[i];
            printf("\tid_image : %s\n", p->id_image);
            printf("\tnb_protocol_preference : %d\n", p->nb_protocol_preference);
        }
        printf("params.dhcp_option_77 : %s\n", params.dhcp_option_77);
        printf("params.wifi_state : %d\n", params.wifi_state);
        printf("params.wifi_ssid : %s\n", params.wifi_ssid);
        printf("params.wifi_key : %s\n", params.wifi_key);
        printf("params.frequency_band : %d\n", params.frequency_band);
    }
    close(fd);
}
static void writeParamsTest() {
    int fd = open(DEVICE_NAME, O_RDWR);
    int res = 0;
    hnsa_download_params_t params;
    if (fd < 0) {
        perror("Failed to open the device");
        return;
    }
    params.mode = 0;
    params.params_type = 1;
    params.download_request = 1;
    strcpy(params.ntp_server_address, "0.0.0.0");
    strcpy(params.timeoffset, "00");
    strcpy(params.country, "A");
    params.nb_images = 0;
    for (int i = 0; i < HNSA_DOWNLOAD_MAX_IMAGE; i++) {
        hnsa_download_settings_image_t *p = &params.settings_images[i];
        strcpy(p->id_image, "nullptr");
        p->nb_protocol_preference = i;
    }
    strcpy(params.dhcp_option_77, "NULL");
    params.wifi_state = 9;
    strcpy(params.wifi_ssid, "summer");
    strcpy(params.wifi_key, "0000-0000-0000-0000");
    params.frequency_band = 1;
    res = ioctl(fd, HNSA_DOWNLOAD_WRITE_PARAMS, &params);
    if (res == -1) {
        printf("write params error\n");
        printError();
    } else {
        printf("write params success\n");
        printf("The content write is as follows: \n");
        printf("params.mode : %d\n", params.mode);
        printf("params.params_type : %d\n", params.params_type);
        printf("params.download_request : %d\n", params.download_request);
        printf("params.ntp_server_address : %s\n", params.ntp_server_address);
        printf("params.timeoffset : %s\n", params.timeoffset);
        printf("params.country : %s\n", params.country);
        printf("params.nb_images : %d\n", params.nb_images);
        for (int i = 0; i < HNSA_DOWNLOAD_MAX_IMAGE; i++) {
            printf("params.settings_images[%d] info : \n", i);
            hnsa_download_settings_image_t *p = &params.settings_images[i];
            printf("\tid_image : %s\n", p->id_image);
            printf("\tnb_protocol_preference : %d\n", p->nb_protocol_preference);
        }
        printf("params.dhcp_option_77 : %s\n", params.dhcp_option_77);
        printf("params.wifi_state : %d\n", params.wifi_state);
        printf("params.wifi_ssid : %s\n", params.wifi_ssid);
        printf("params.wifi_key : %s\n", params.wifi_key);
        printf("params.frequency_band : %d\n", params.frequency_band);
    }
    close(fd);
}
static void readPrivDataTest() {
    int fd = open(DEVICE_NAME, O_RDWR);
    int res = 0;
    uint32_t private_data;
    if (fd < 0) {
        perror("Failed to open the device");
        return;
    }
    res = ioctl(fd, HNSA_DOWNLOAD_READ_PRIV_DATA, &private_data);
    if (res == -1) {
        printf("read private data error\n");
        printError();
    } else {
        printf("read private data success\n");
        printf("private_data : %d\n", private_data);
    }
    close(fd);
}
static void writePrivDataTest() {
    int fd = open(DEVICE_NAME, O_RDWR);
    int res = 0;
    uint32_t private_data = 99;
    if (fd < 0) {
        perror("Failed to open the device");
        return;
    }
    res = ioctl(fd, HNSA_DOWNLOAD_WRITE_PRIV_DATA, &private_data);
    if (res == -1) {
        printf("write private data error\n");
        printError();
    } else {
        printf("write private data success\n");
        printf("The content write is as follows: \n");
        printf("private_data : %d\n", private_data);
    }
    close(fd);
}
static void getCapabilitiesTest() {
    //
}
static void activateLoaderTest() {
    //
}
static void getVersionTest() {
    //
}
static void getIdImageTest() {
    //
}
static void getReportTest() {
    //
}
static void bootloaderAllocResourceTest() {
    //
}
static void bootloaderFreeResourceTest() {
    //
}
static void getEventTest() {
    //
}
static void bootloaderSearchAvaliableIdImageTest() {
    //
}
static void bootloaderSetAutoSearchPeriodicityTest() {
    int fd = open(DEVICE_NAME, O_RDWR);
    int res = 0;
    uint32_t periodicity = 555;
    if (fd < 0) {
        perror("Failed to open the device");
        return;
    }
    res = ioctl(fd, HNSA_DOWNLOAD_BL_SET_AUTOSEARCH_PERIODICITY, &periodicity);
    if (res == -1) {
        printf("set auto search periodicity error\n");
        printError();
    } else {
        printf("set auto search periodicity success\n");
        printf("The content set is as follows: \n");
        printf("periodicity : %d\n", periodicity);
    }
    close(fd);
}
static void bootloaderGetAutoSearchPeriodicityTest() {
    int fd = open(DEVICE_NAME, O_RDWR);
    int res = 0;
    uint32_t periodicity;
    if (fd < 0) {
        perror("Failed to open the device");
        return;
    }
    res = ioctl(fd, HNSA_DOWNLOAD_BL_GET_AUTOSEARCH_PERIODICITY, &periodicity);
    if (res == -1) {
        printf("get auto search periodicity error\n");
        printError();
    } else {
        printf("get auto search periodicity success\n");
        printf("periodicity : %d\n", periodicity);
    }
    close(fd);
}
static void getAvailableIdImageTest() {
    //
}
static void installIdImageTest() {
    //
}
static void resetReportTest() {
    //
}
static void bootloaderSetCountryTest() {
    //
}
static void closeKernelDebug() {
    int fd = open(DEVICE_NAME, O_RDWR);
    int res = 0;
    if (fd < 0) {
        perror("Failed to open the device");
        return;
    }
    res = ioctl(fd, CLOSE_DEBUG_MESSAGE, NULL);
    if (res == -1) {
        printf("close kernel debug error\n");
        printError();
    } else {
        printf("close kernel debug success\n");
    }
    close(fd);
}
static void openKernelDebug() {
    int fd = open(DEVICE_NAME, O_RDWR);
    int res = 0;
    if (fd < 0) {
        perror("Failed to open the device");
        return;
    }
    res = ioctl(fd, OPEN_DEBUG_MESSAGE, NULL);
    if (res == -1) {
        printf("open kernel debug error\n");
        printError();
    } else {
        printf("open kernel debug success\n");
    }
    close(fd);
}
static void printMenu() {
    printf("\t 0. EXIT\n");
    printf("\t 1. TEST HNSA_DOWNLOAD_GET_CAPABILITIES\n");
    printf("\t 2. TEST HNSA_DOWNLOAD_READ_PARAMS\n");
    printf("\t 3. TEST HNSA_DOWNLOAD_WRITE_PARAMS\n");
    printf("\t 4. TEST HNSA_DOWNLOAD_READ_PRIV_DATA\n");
    printf("\t 5. TEST HNSA_DOWNLOAD_WRITE_PRIV_DATA\n");
    printf("\t 6. TEST HNSA_DOWNLOAD_ACTIVATE_LOADER\n");
    printf("\t 7. TEST HNSA_DOWNLOAD_GET_VERSION\n");
    printf("\t 8. TEST HNSA_DOWNLOAD_GET_ID_IMAGE\n");
    printf("\t 9. TEST HNSA_DOWNLOAD_GET_REPORT\n");
    printf("\t10. TEST HNSA_DOWNLOAD_BL_ALLOC_RESOURCES\n");
    printf("\t11. TEST HNSA_DOWNLOAD_BL_FREE_RESOURCES\n");
    printf("\t12. TEST HNSA_DOWNLOAD_GET_EVENT\n");
    printf("\t13. TEST HNSA_DOWNLOAD_BL_SEARCH_AVAILABLE_ID_IMAGE\n");
    printf("\t14. TEST HNSA_DOWNLOAD_BL_SET_AUTOSEARCH_PERIODICITY\n");
    printf("\t15. TEST HNSA_DOWNLOAD_BL_GET_AUTOSEARCH_PERIODICITY\n");
    printf("\t16. TEST HNSA_DOWNLOAD_GET_AVAILABLE_ID_IMAGE\n");
    printf("\t17. TEST HNSA_DOWNLOAD_INSTALL_ID_IMAGE\n");
    printf("\t18. TEST HNSA_DOWNLOAD_RESET_REPORT\n");
    printf("\t19. TEST HNSA_DOWNLOAD_BL_SET_COUNTRY\n");
    printf("\t20. CLOSE KERNEL DEBUG\n");
    printf("\t21. OPEN KERNEL DEBUG\n");
}
static void (*testFunctions[])(void) = {
    getCapabilitiesTest,
    readParamsTest,
    writeParamsTest,
    readPrivDataTest,
    writePrivDataTest,
    activateLoaderTest,
    getVersionTest,
    getIdImageTest,
    getReportTest,
    bootloaderAllocResourceTest,
    bootloaderFreeResourceTest,
    getEventTest,
    bootloaderSearchAvaliableIdImageTest,
    bootloaderSetAutoSearchPeriodicityTest,
    bootloaderGetAutoSearchPeriodicityTest,
    getAvailableIdImageTest,
    installIdImageTest,
    resetReportTest,
    bootloaderSetCountryTest,
    closeKernelDebug,
    openKernelDebug,
};
int main(int argc, char **argv) {
    while (true) {
        int select = 0;
        printMenu();
        printf("Please select the function you want to test(0 to exit)\n");
        printf(">>> ");
        fflush(stdout);
        scanf("%d", &select);
        if (select == 0) {
            printf("You exited the program\n");
            break;
        }
        if (select >= 22) {
            printf("input error\n");
            break;
        }
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        testFunctions[select - 1]();
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
    }
    return 0;
}
