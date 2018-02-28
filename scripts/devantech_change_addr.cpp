#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <assert.h>
#include <chrono>
#include <thread>

int main (int argc, char *argv[]) 
{
    if (argc != 4) {
        std::cout << "Invalid number of arguments. " << argc << std::cout;
        return -1;
    }
    char *end;
    unsigned long bus = std::strtol(argv[1], &end, 0);
    unsigned long device_addr = std::strtol(argv[2], &end, 0);
    uint8_t command = 0x00;
    unsigned long new_addr = std::strtol(argv[3], &end, 0);
    const int delay = 100;

    char filename[20];
    snprintf(filename, sizeof(filename), "/dev/i2c-%d", bus);
    filename[sizeof(filename) - 1] = '\0';
    
    new_addr = new_addr << 1;
    
    if (new_addr < 0xE0 || new_addr > 0xEE) {
        std::cout << "Unsupported new address." << std::cout;
        return -1;
    }
    int file = open(filename, O_RDWR);
    if (file < 0) {
        std::cout << "Failed to open the i2c bus: " << filename << "." << std::endl;
        return -1;
    }
    if (ioctl(file, I2C_SLAVE, device_addr) < 0) {
        std::cout << "Failed to acquire bus access or talk to device. Addr: " << device_addr << std::endl;
        return -1;
    }
    uint8_t buffer[2];
    buffer[0] = command;
    buffer[1] = 0xA0;
    if (write(file, buffer, 2) != 2) {
        std::cout << "Failed to send 0xA0 to device." << std::endl;
        return -1;
    }
    buffer[1] = 0xAA;
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    if (write(file, buffer, 2) != 2) {
        std::cout << "Failed to send 0xAA to device." << std::endl;
        return -1;
    }
    buffer[1] = 0xA5;
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    if (write(file, buffer, 2) != 2) {
        std::cout << "Failed to send 0xA5 to device." << std::endl;
        return -1;
    }
    buffer[1] = new_addr;
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    if (write(file, buffer, 2) != 2) {
        std::cout << "Failed to change addr." << std::endl;
        return -1;
    }
    std::cout << "Successfully changed addr." << std::endl;
    return 0;
}
