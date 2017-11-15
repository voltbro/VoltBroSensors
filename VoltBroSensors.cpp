#include "VoltBroSensors.h"
#include <Arduino.h>

static void VoltBroSensors::I2C_WriteReg(uint8_t dev_addr, uint8_t register_addr, uint8_t data) {
  Wire.beginTransmission(dev_addr);           //  указываем ID адрес сенсора на шине I2C
  Wire.write(register_addr);                //  указываем адрес регистра в который будет записан байт данных
  Wire.write(data);                        //  указываем данные которые нужно записать в регистр
  Wire.endTransmission();                 //  совершаем передачу по шине I2C
}

static void VoltBroSensors::I2C_ReadBytes(uint8_t dev_addr, uint8_t register_addr, uint8_t num, uint8_t *buffer)
{
  uint8_t i;
  Wire.beginTransmission(dev_addr);            //  указываем ID адрес сенсора на шине I2C
  Wire.write(register_addr);                 //  указываем адрес регистра из которого будет производится чтение (а так же следующих за ним, если мы читаем больше 1 байта)
  Wire.endTransmission();                   //  совершаем передачу по шине I2C
  Wire.requestFrom(dev_addr, num);
  for (i = 0; i < num; i++) {
    *buffer = Wire.read();
    buffer++;
  }
}

// Write byte to register
static void VoltBroSensors::I2C_writeRegister8(uint8_t dev_addr, uint8_t register_addr, uint8_t value)
{
  Wire.beginTransmission(dev_addr);
  Wire.write(register_addr);
  Wire.write(value);
  Wire.endTransmission();
}

// Read byte to register
static uint8_t VoltBroSensors::I2C_fastRegister8(uint8_t dev_addr, uint8_t register_addr)
{
  uint8_t value;
  Wire.beginTransmission(dev_addr);
  Wire.write(register_addr);
  Wire.endTransmission();

  Wire.requestFrom(dev_addr, (uint8_t)1);
  value = Wire.read();
  Wire.endTransmission();

  return value;
}

static int32_t VoltBroSensors::I2C_getRegister32(uint8_t dev_addr, uint8_t register_addr, int num, boolean unsign){
      int32_t		result32=0;
      int16_t		result16=0;
      int8_t		result8 =0;

			Wire.beginTransmission(dev_addr);													//	указываем ID адрес сенсора на шине I2C
			Wire.write(register_addr);																//	указываем адрес регистра из которого будет производится чтение (а так же следующих за ним, если мы читаем больше 1 байта)
			Wire.endTransmission();															//	совершаем передачу по шине I2C
			Wire.requestFrom(0x77, num);												//	указываем ID адрес сенсора на шине I2C и количество байт которое мы хотим прочитать
			for(int i=0; i<num; i++){result32<<=8;		result32+=Wire.read();	}		//	читаем полученные данные
			if (unsign && num==1)		{result8 =result32;	result32 =result8;		}		//	добавляем знак (если Uns==true)
			if (unsign && num==2)		{result16=result32;	result32 =result16;		}		//	добавляем знак (если Uns==true)
			return result32;
}
