#include <libcrt.h>

#define BLUEZ_SERVICE "org.bluez"
#define ADAPTER_INTERFACE "org.freedesktop.DBus.Properties"
#define ADAPTER_PROPERTY_ALL "Devices"
#define DEVICE_INTERFACE "org.freedesktop.DBus.Properties"
#define DEVICE_PROPERTY_ADDR "Address"
#define DEVICE_PROPERTY_CONN "Connected"
#define DEVICE_PROPERTY_NAME "Name"
#define ADAPTER_PROPERTY_MIC "Discovering"




int libdbus_address(struct libvbt_t *st) {
    DBusError err;
    DBusConnection *conn;
    DBusMessage *msg;
    DBusMessageIter iter;
    DBusMessageIter arrayIter;
    DBusMessageIter variantIter;
    char *device_path;
    char *device_address;

    // Initialize the D-Bus error object
    dbus_error_init(&err);

    // Connect to the system bus
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "D-Bus connection error: %s\n", err.message);
        dbus_error_free(&err);
        return 1;
    }

    // Get the object path of the Bluetooth adapter
    const char *adapter_path = "/org/bluez/hci0";

    // Create a message to get the adapter properties
    msg = dbus_message_new_method_call(BLUEZ_SERVICE, adapter_path, ADAPTER_INTERFACE, "Get");
    if (!msg) {
        fprintf(stderr, "Error creating D-Bus message\n");
        return 1;
    }

    // Append the interface, property, and interface to the message
    printf("loading .. %s@%d \n", __FUNCTION__, __LINE__);
    dbus_message_append_args(msg, DBUS_TYPE_STRING, &ADAPTER_INTERFACE, DBUS_TYPE_STRING, &ADAPTER_PROPERTY_ALL, DBUS_TYPE_INVALID);
    printf("loading .. %s@%d \n", __FUNCTION__, __LINE__);

    // Send the message and get a reply
    if (!dbus_connection_send_with_reply_and_block(conn, msg, -1, &err)) {
        fprintf(stderr, "D-Bus method call error: %s\n", err.message);
        dbus_error_free(&err);
        return 1;
    }

    // Parse the reply
    if (!dbus_message_iter_init(msg, &iter)) {
        fprintf(stderr, "Error initializing D-Bus message iterator\n");
        return 1;
    }

    // Iterate through the returned properties
    while (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_INVALID) {
        dbus_message_iter_recurse(&iter, &variantIter);

        if (dbus_message_iter_get_arg_type(&variantIter) == DBUS_TYPE_ARRAY) {
            dbus_message_iter_recurse(&variantIter, &arrayIter);
            while (dbus_message_iter_get_arg_type(&arrayIter) == DBUS_TYPE_OBJECT_PATH) {
                dbus_message_iter_get_basic(&arrayIter, &device_path);

                // Create a message to get the device properties
                DBusMessage *deviceMsg = dbus_message_new_method_call(BLUEZ_SERVICE, device_path, DEVICE_INTERFACE, "Get");
                if (!deviceMsg) {
                    fprintf(stderr, "Error creating D-Bus message for device\n");
                    return 1;
                }
                printf("loading .. %s@%d \n", __FUNCTION__, __LINE__);
                // Append the interface, property, and interface to the device message
                dbus_message_append_args(deviceMsg, DBUS_TYPE_STRING, &DEVICE_INTERFACE, DBUS_TYPE_STRING, &DEVICE_PROPERTY_ADDR, DBUS_TYPE_INVALID);
                printf("loading .. %s@%d \n", __FUNCTION__, __LINE__);
                // Send the device message and get a reply
                if (!dbus_connection_send_with_reply_and_block(conn, deviceMsg, -1, &err)) {
                    fprintf(stderr, "D-Bus method call error for device: %s\n", err.message);
                    dbus_error_free(&err);
                    return 1;
                }

                // Parse the device reply
                DBusMessageIter deviceIter;
                if (!dbus_message_iter_init(deviceMsg, &deviceIter)) {
                    fprintf(stderr, "Error initializing D-Bus message iterator for device\n");
                    return 1;
                }

                // Iterate through the device properties
                while (dbus_message_iter_get_arg_type(&deviceIter) != DBUS_TYPE_INVALID) {
                    dbus_message_iter_recurse(&deviceIter, &variantIter);

                    if (dbus_message_iter_get_arg_type(&variantIter) == DBUS_TYPE_STRING) {
                        dbus_message_iter_get_basic(&variantIter, &device_address);
                        printf("Device Address: %s\n", device_address);
                        break;
                    }

                    dbus_message_iter_next(&deviceIter);
                }

                // Free resources for device message
                dbus_message_unref(deviceMsg);

                dbus_message_iter_next(&arrayIter);
            }
        }

        dbus_message_iter_next(&iter);
    }

    // Free resources for adapter message
    dbus_message_unref(msg);
    dbus_connection_unref(conn);

    return 0;
}

int libdbus_audio(struct libvbt_t *st) {
    DBusError err;
    DBusConnection *conn;
    DBusMessage *msg;
    DBusMessageIter iter;
    int connected = 0;

    // Initialize the D-Bus error object
    dbus_error_init(&err);

    // Connect to the system bus
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "D-Bus connection error: %s\n", err.message);
        dbus_error_free(&err);
        return 1;
    }

    // Replace "hci0" with the correct Bluetooth adapter
    const char *adapter_path = "/org/bluez/hci0";

    // Replace "00:00:00:00:00:00" with the correct Bluetooth device address
    const char *device_address = "00:00:00:00:00:00";

    // Create a message to get the device properties
    char device_path[64];
    snprintf(device_path, sizeof(device_path), "%s/dev_%s", adapter_path, device_address);
    msg = dbus_message_new_method_call(BLUEZ_SERVICE, device_path, DEVICE_INTERFACE, "GetAll");
    if (!msg) {
        fprintf(stderr, "Error creating D-Bus message\n");
        return 1;
    }

    // Append the interface and property to the message
    dbus_message_append_args(msg, DBUS_TYPE_STRING, &DEVICE_INTERFACE, DBUS_TYPE_INVALID);

    // Send the message and get a reply
    if (!dbus_connection_send_with_reply_and_block(conn, msg, -1, &err)) {
        fprintf(stderr, "D-Bus method call error: %s\n", err.message);
        dbus_error_free(&err);
        return 1;
    }

    // Parse the reply
    if (!dbus_message_iter_init(msg, &iter)) {
        fprintf(stderr, "Error initializing D-Bus message iterator\n");
        return 1;
    }

    // Iterate through the returned properties
    while (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_INVALID) {
        DBusMessageIter variantIter;
        dbus_message_iter_recurse(&iter, &variantIter);

        if (dbus_message_iter_get_arg_type(&variantIter) == DBUS_TYPE_BOOLEAN) {
            dbus_message_iter_get_basic(&variantIter, &connected);
            printf("Audio status: %s\n", connected ? "Connected" : "Disconnected");
            break;
        }

        dbus_message_iter_next(&iter);
    }

    // Free resources
    dbus_message_unref(msg);
    dbus_connection_unref(conn);

    return 0;
}

int libdbus_devices(struct libvbt_t *st) {
    DBusError err;
    DBusConnection *conn;
    DBusMessage *msg;
    DBusMessageIter iter;
    DBusMessageIter arrayIter;
    DBusMessageIter variantIter;
    char *device_path;

    // Initialize the D-Bus error object
    dbus_error_init(&err);

    // Connect to the system bus
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "D-Bus connection error: %s\n", err.message);
        dbus_error_free(&err);
        return 1;
    }

    // Get the object path of the Bluetooth adapter
    const char *adapter_path = "/org/bluez/hci0";

    // Create a message to get the adapter properties
    msg = dbus_message_new_method_call(BLUEZ_SERVICE, adapter_path, ADAPTER_INTERFACE, "Get");
    if (!msg) {
        fprintf(stderr, "Error creating D-Bus message\n");
        return 1;
    }

    // Append the interface, property, and interface to the message
    dbus_message_append_args(msg, DBUS_TYPE_STRING, &ADAPTER_INTERFACE, DBUS_TYPE_STRING, &ADAPTER_PROPERTY_ALL, DBUS_TYPE_INVALID);

    // Send the message and get a reply
    if (!dbus_connection_send_with_reply_and_block(conn, msg, -1, &err)) {
        fprintf(stderr, "D-Bus method call error: %s\n", err.message);
        dbus_error_free(&err);
        return 1;
    }

    // Parse the reply
    if (!dbus_message_iter_init(msg, &iter)) {
        fprintf(stderr, "Error initializing D-Bus message iterator\n");
        return 1;
    }

    // Iterate through the returned properties
    while (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_INVALID) {
        dbus_message_iter_recurse(&iter, &variantIter);

        if (dbus_message_iter_get_arg_type(&variantIter) == DBUS_TYPE_ARRAY) {
            dbus_message_iter_recurse(&variantIter, &arrayIter);
            while (dbus_message_iter_get_arg_type(&arrayIter) == DBUS_TYPE_OBJECT_PATH) {
                dbus_message_iter_get_basic(&arrayIter, &device_path);
                printf("Device Path: %s\n", device_path);

                // You can add more code here to get additional information about each device if needed

                dbus_message_iter_next(&arrayIter);
            }
        }

        dbus_message_iter_next(&iter);
    }

    // Free resources
    dbus_message_unref(msg);
    dbus_connection_unref(conn);

    return 0;
}

int libdbus_micophone(struct libvbt_t *st) {
    DBusError err;
    DBusConnection *conn;
    DBusMessage *msg;
    DBusMessageIter iter;
    int discovering = 0;

    // Initialize the D-Bus error object
    dbus_error_init(&err);

    // Connect to the system bus
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "D-Bus connection error: %s\n", err.message);
        dbus_error_free(&err);
        return 1;
    }

    // Create a message to get the adapter properties
    msg = dbus_message_new_method_call(BLUEZ_SERVICE, "/", ADAPTER_INTERFACE, "GetAll");
    if (!msg) {
        fprintf(stderr, "Error creating D-Bus message\n");
        return 1;
    }

    // Append the interface and property to the message
    dbus_message_append_args(msg, DBUS_TYPE_STRING, &ADAPTER_INTERFACE, DBUS_TYPE_INVALID);

    // Send the message and get a reply
    if (!dbus_connection_send_with_reply_and_block(conn, msg, -1, &err)) {
        fprintf(stderr, "D-Bus method call error: %s\n", err.message);
        dbus_error_free(&err);
        return 1;
    }

    // Parse the reply
    if (!dbus_message_iter_init(msg, &iter)) {
        fprintf(stderr, "Error initializing D-Bus message iterator\n");
        return 1;
    }

    // Iterate through the returned properties
    while (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_INVALID) {
        DBusMessageIter variantIter;
        dbus_message_iter_recurse(&iter, &variantIter);

        if (dbus_message_iter_get_arg_type(&variantIter) == DBUS_TYPE_BOOLEAN) {
            dbus_message_iter_get_basic(&variantIter, &discovering);
            printf("Microphone status: %s\n", discovering ? "On" : "Off");
            break;
        }

        dbus_message_iter_next(&iter);
    }

    // Free resources
    dbus_message_unref(msg);
    dbus_connection_unref(conn);

    return 0;
}


int libdbus_name(struct libvbt_t *st) {
    DBusError err;
    DBusConnection *conn;
    DBusMessage *msg;
    DBusMessageIter iter;
    DBusMessageIter arrayIter;
    DBusMessageIter variantIter;
    char *device_path;
    char *device_name;

    // Initialize the D-Bus error object
    dbus_error_init(&err);

    // Connect to the system bus
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "D-Bus connection error: %s\n", err.message);
        dbus_error_free(&err);
        return 1;
    }

    // Get the object path of the Bluetooth adapter
    const char *adapter_path = "/org/bluez/hci0";

    // Create a message to get the adapter properties
    msg = dbus_message_new_method_call(BLUEZ_SERVICE, adapter_path, ADAPTER_INTERFACE, "Get");
    if (!msg) {
        fprintf(stderr, "Error creating D-Bus message\n");
        return 1;
    }

    // Append the interface, property, and interface to the message
    dbus_message_append_args(msg, DBUS_TYPE_STRING, &ADAPTER_INTERFACE, DBUS_TYPE_STRING, &ADAPTER_PROPERTY_ALL, DBUS_TYPE_INVALID);

    // Send the message and get a reply
    if (!dbus_connection_send_with_reply_and_block(conn, msg, -1, &err)) {
        fprintf(stderr, "D-Bus method call error: %s\n", err.message);
        dbus_error_free(&err);
        return 1;
    }

    // Parse the reply
    if (!dbus_message_iter_init(msg, &iter)) {
        fprintf(stderr, "Error initializing D-Bus message iterator\n");
        return 1;
    }

    // Iterate through the returned properties
    while (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_INVALID) {
        dbus_message_iter_recurse(&iter, &variantIter);

        if (dbus_message_iter_get_arg_type(&variantIter) == DBUS_TYPE_ARRAY) {
            dbus_message_iter_recurse(&variantIter, &arrayIter);
            while (dbus_message_iter_get_arg_type(&arrayIter) == DBUS_TYPE_OBJECT_PATH) {
                dbus_message_iter_get_basic(&arrayIter, &device_path);

                // Create a message to get the device properties
                DBusMessage *deviceMsg = dbus_message_new_method_call(BLUEZ_SERVICE, device_path, DEVICE_INTERFACE, "Get");
                if (!deviceMsg) {
                    fprintf(stderr, "Error creating D-Bus message for device\n");
                    return 1;
                }

                // Append the interface, property, and interface to the device message
                dbus_message_append_args(deviceMsg, DBUS_TYPE_STRING, &DEVICE_INTERFACE, DBUS_TYPE_STRING, &DEVICE_PROPERTY_NAME, DBUS_TYPE_INVALID);

                // Send the device message and get a reply
                if (!dbus_connection_send_with_reply_and_block(conn, deviceMsg, -1, &err)) {
                    fprintf(stderr, "D-Bus method call error for device: %s\n", err.message);
                    dbus_error_free(&err);
                    return 1;
                }

                // Parse the device reply
                DBusMessageIter deviceIter;
                if (!dbus_message_iter_init(deviceMsg, &deviceIter)) {
                    fprintf(stderr, "Error initializing D-Bus message iterator for device\n");
                    return 1;
                }

                // Iterate through the device properties
                while (dbus_message_iter_get_arg_type(&deviceIter) != DBUS_TYPE_INVALID) {
                    dbus_message_iter_recurse(&deviceIter, &variantIter);

                    if (dbus_message_iter_get_arg_type(&variantIter) == DBUS_TYPE_STRING) {
                        dbus_message_iter_get_basic(&variantIter, &device_name);
                        printf("Device Name: %s\n", device_name);
                        break;
                    }

                    dbus_message_iter_next(&deviceIter);
                }

                // Free resources for device message
                dbus_message_unref(deviceMsg);

                dbus_message_iter_next(&arrayIter);
            }
        }

        dbus_message_iter_next(&iter);
    }

    // Free resources for adapter message
    dbus_message_unref(msg);
    dbus_connection_unref(conn);

    return 0;
}


