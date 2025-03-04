int startBottom = 2;		// กำหนดพินปุ่ม Start
int resetBottom = 3;  		// กำหนดพินปุ่ม Reset
int ledStart = 8;    		// กำหนดพิน LED สำหรับแสดงสถานะ Start
int StartState = LOW;  	// กำหนดสถานะเริ่มต้นของปุ่ม Start เป็น LOW
int gaspin = 0;       		// กำหนดพินสำหรับอ่านค่าจากเซนเซอร์ (analog pin 0)
float sum = 0.0;      		// ตัวแปรสะสมค่าแรงดันไฟฟ้าที่อ่านได้

void setup() {
  Serial.begin(9600);  			// เริ่มการสื่อสารแบบ Serial ที่ความเร็ว 9600
  pinMode(ledStart, OUTPUT);   	// กำหนดพิน LED เป็น OUTPUT
  pinMode(startBottom, INPUT); 	// กำหนดพินปุ่ม Start เป็น INPUT
  pinMode(resetBottom, INPUT); 	// กำหนดพินปุ่ม Reset เป็น INPUT
  Serial.println("CLEARDATA"); 		// ล้างข้อมูลที่เคยเก็บ
  Serial.println("LABEL,CLOCK,TIME,VARIABLE"); 	// พิมพ์หัวข้อของข้อมูลที่แสดง
}


void loop() {
  // เช็คว่าปุ่ม Start ถูกกด (LOW) และสถานะยังไม่เป็น HIGH
  if(digitalRead(startBottom) == LOW && StartState != HIGH) {
    StartState = HIGH;  			// เปลี่ยนสถานะ Start เป็น HIGH
    Serial.println("start");  		// พิมพ์ข้อความ "start" เมื่อเริ่มทำงาน
    digitalWrite(ledStart, StartState);  	// เปิดไฟ LED แสดงสถานะ Start

    // ลูปทำงานต่อเนื่องจนกว่าจะกดปุ่ม Reset (ค่าของ resetBottom == LOW)
    while(digitalRead(resetBottom) != LOW) {
      // อ่านค่าแรงดันจาก analog pin 0 และแปลงค่าเป็นโวลต์
      float v = (analogRead(0) * 5) / 1024.0;
      Serial.println(v);  			// แสดงผลค่าที่อ่านได้ทาง Serial
      // ถ้าแรงดันไฟฟ้ามีค่าเกิน 0.2 โวลต์ ให้นำค่ามาสะสมใน sum
      if(v >= 0.2){
        sum = sum + v;
      }
      delay(1000);  			// หน่วงเวลา 1 วินาที ก่อนการอ่านครั้งต่อไป
    }

    StartState = LOW;			// เมื่อกดปุ่ม Reset สถานะ Start จะเปลี่ยนเป็น LOW
    Serial.print("reset Sum = ");  	// แสดงค่าที่สะสมได้เมื่อกด Reset
    Serial.println(sum);
    sum = 0.0;  				// รีเซ็ตค่า sum เป็น 0
    digitalWrite(ledStart, StartState);  	// ปิดไฟ LED แสดงสถานะ
    delay(200);  			// หน่วงเวลา 200 มิลลิวินาที
  }
}
