#include <AccelStepper.h>

#define DIR 19
#define STP 21
#define EN  22

#define STEPS_PER_MM 400.0  // calibrate จริง: สั่ง 10mm ได้ 40mm จริง (16000 step / 40mm)

AccelStepper stepper(1, STP, DIR); // DRIVER mode: (interface, stepPin, dirPin)

String inputBuffer = "";

void setup()
{
   Serial.begin(115200);

   stepper.setEnablePin(EN);
   stepper.setPinsInverted(false, false, true); // true = enable active-low
   stepper.enableOutputs();

   stepper.setMaxSpeed(6400);       // step/s ปรับตามที่ไดรเวอร์/มอเตอร์ไหว
   stepper.setAcceleration(1600);   // step/s^2

   stepper.setCurrentPosition(0);   // กำหนดจุดเริ่มต้นเป็น 0mm

   Serial.println("พิมพ์ตำแหน่งเป้าหมาย (mm) แล้วกด Enter");
}

void moveToMM(float mm)
{
   long targetSteps = round(mm * STEPS_PER_MM);
   stepper.moveTo(targetSteps);
}

void handleSerialInput()
{
   while (Serial.available() > 0) {
      char c = Serial.read();
      if (c == '\n' || c == '\r') {
         if (inputBuffer.length() > 0) {
            float mm = inputBuffer.toFloat();
            Serial.print("สั่งไปที่ ");
            Serial.print(mm);
            Serial.println(" mm");
            moveToMM(mm);
            inputBuffer = "";
         }
      } else {
         inputBuffer += c;
      }
   }
}

void loop()
{
   stepper.run();          // ต้องเรียกถี่ๆ ห้ามมี delay() คั่น
   handleSerialInput();    // เช็ค input ที่พิมพ์เข้ามาแต่ละรอบ loop โดยไม่บล็อก
}