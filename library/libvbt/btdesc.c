#include <libcrt.h>


int libvbt_desc(struct libvbt_t *st) {
    inquiry_info *devices = NULL;
    int max_devices = 8;  // Maximum number of devices to discover
    int num_devices, i;

    // Get the first available Bluetooth device
    int device_id = hci_get_route(NULL);

    // Open a Bluetooth socket
    int socket = hci_open_dev(device_id);
    if (socket < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Set inquiry parameters
    inquiry_info *ii = (inquiry_info *)malloc(max_devices * sizeof(inquiry_info));
    num_devices = hci_inquiry(device_id, 8, max_devices, NULL, &devices, IREQ_CACHE_FLUSH);

    // Print information about discovered devices
    for (i = 0; i < num_devices; i++) {
        ba2str(&(devices + i)->bdaddr, (char *)&(devices + i)->bdaddr);
        printf("Device Address: %s\n", (devices + i)->bdaddr);
        printf("Device Name: %x %x %x\n", (devices + i)->pscan_rep_mode, (devices + i)->pscan_period_mode, (devices + i)->pscan_mode );
        printf("Device Class: 0x%2X%2X%2X\n", (devices + i)->dev_class[2], (devices + i)->dev_class[1], (devices + i)->dev_class[0]);
        //printf("RSSI: %d\n", (devices + i)->rssi);
        printf("------------------------------\n");
    }

    // Free resources
    free(ii);
    hci_close_dev(socket);

    return 0;
}

