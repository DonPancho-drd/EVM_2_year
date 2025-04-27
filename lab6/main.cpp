#include <libusb-1.0/libusb.h>
#include <stdio.h>

int main() {
    libusb_context *ctx = NULL;
    libusb_device **devs;
    ssize_t cnt;

    // Инициализация libusb
    if (libusb_init(&ctx) < 0) {
        fprintf(stderr, "Ошибка инициализации libusb\n");
        return 1;
    }

    // Получение списка USB-устройств
    cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0) {
        fprintf(stderr, "Ошибка получения списка устройств\n");
        libusb_exit(ctx);
        return 1;
    }

    // Вывод информации о каждом устройстве
    for (ssize_t i = 0; i < cnt; i++) {
        struct libusb_device_descriptor desc;
        libusb_get_device_descriptor(devs[i], &desc);
        printf("Устройство: VID=%04x, PID=%04x\n", desc.idVendor, desc.idProduct);
    }

    // Освобождение ресурсов
    libusb_free_device_list(devs, 1);
    libusb_exit(ctx);
    return 0;
}
