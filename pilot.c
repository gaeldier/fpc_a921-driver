#include <stdio.h>
#include <libusb-1.0/libusb.h>

#define VENDOR_ID 0x10A5 // v.id 
#define PRODUCT_ID 0x9201 // v.product


// test : parler avec le lecteur

int main() {
    libusb_context *ctx = NULL;
    libusb_device_handle *dev_handle = NULL;
    
    if (libusb_init(&ctx) < 0) {
        printf("erreur init libusb\n");
        return 1;
    }

    dev_handle = libusb_open_device_with_vid_pid(ctx, VENDOR_ID, PRODUCT_ID);
    if (dev_handle == NULL) {
        printf("capteur introuvable\n");
        libusb_exit(ctx);
        return 1;
    }

    // si jamais, pour détacher le pilote du kernel
    if (libusb_kernel_driver_active(dev_handle, 0) == 1) {
        libusb_detach_kernel_driver(dev_handle, 0);
    }

    if (libusb_claim_interface(dev_handle, 0) < 0) {
        printf("impossible de claim l'interface\n");
        libusb_close(dev_handle);
        libusb_exit(ctx);
        return 1;
    }

    // les paramètres (bmRequestType, bRequest, wValue, wIndex) donnés par Wireshark
    unsigned char data[64];
    int bytes_transferred = libusb_control_transfer(
        dev_handle, 
        LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT,
        0x01, 
        0x0000, 
        0x0000, 
        data, 
        sizeof(data), 
        1000 // timeout en ms
    );

    libusb_release_interface(dev_handle, 0);
    libusb_close(dev_handle);
    libusb_exit(ctx);

    return 0;


// todo: libfprint 
}