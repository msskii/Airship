#include "mpu6050.h"
#include "../i2c_dev.h"

#include <unistd.h>
#include <stdint.h>

Mpu6050::Mpu6050(int addr) : I2CDev(addr) {
	reset();
	writeReg8(MPU6050_REG_PWR_MGMT1, 0x00); //unset sleep bit
}

Mpu6050::~Mpu6050() {
	writeReg8(MPU6050_REG_PWR_MGMT1, 0x40); //set sleep bit
}

void Mpu6050::reset() {
	writeReg8(MPU6050_REG_PWR_MGMT1, 0x80);
	usleep(500); //let mpu6050 complete reset
}

void Mpu6050::setSleepMode(bool on) {
	writeBitReg8(MPU6050_REG_PWR_MGMT1, 6, on);
}

void Mpu6050::setCycleMode(bool on) {
	writeBitReg8(MPU6050_REG_PWR_MGMT1, 5, on);
}

void Mpu6050::setCycleFreq(int freq) {
	writeBitReg8(MPU6050_REG_PWR_MGMT2, 7, freq & 0x02 >> 1);
	writeBitReg8(MPU6050_REG_PWR_MGMT2, 6, freq & 0x01);
}

float Mpu6050::getTemperature() {
	int16_t val = 0xFFFF & readReg16(MPU6050_REG_TEMP);
	return val / 340.0f + 36.53f; //formula from datasheet
}

float Mpu6050::getAccel(int reg) {
	int16_t val = 0xFFFF & readReg16(reg);
	return val / 16384.0f * 9.80665f; //formula from datasheet, only valid for AFS_SEL equal 0
}

float Mpu6050::getGyro(int reg) {
	int16_t val = 0xFFFF & readReg16(reg);
	return val / 131.0f; //formula from datasheet, only valid for FS_SEL equal 0
}

float Mpu6050::getAccelX() {
	return getAccel(MPU6050_REG_AC_X);
}

float Mpu6050::getAccelY() {
	return getAccel(MPU6050_REG_AC_Y);
}

float Mpu6050::getAccelZ() {
	return getAccel(MPU6050_REG_AC_Z);
}

float Mpu6050::getGyroX() {
	return getGyro(MPU6050_REG_GY_X);
}

float Mpu6050::getGyroY() {
	return getGyro(MPU6050_REG_GY_Y);
}

float Mpu6050::getGyroZ() {
	return getGyro(MPU6050_REG_GY_Z);
}
