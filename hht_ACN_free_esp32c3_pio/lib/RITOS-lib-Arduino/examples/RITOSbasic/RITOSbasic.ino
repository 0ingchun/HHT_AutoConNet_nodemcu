#include "ritos.h" 
// RITOS: Real Internet Time Operation Sansystem / RITOS:实时互联网操作系统 for ESP8266

/*
  Please select esp8266 as the development board before compiling
  請先選擇esp8266作為開發板再進行編譯
*/

volatile int a = 1000, b = 3000;
int statemachine = 0;

Ritos tarefa1; // Thread Task One / 綫程任務1
void thread1() {
  a--;
  
  if ( a <= 0 && statemachine==0) {
   Serial.println("Interrupt thread 1!");

   digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
    
   // Attention! Delay() cannot be used within the thread / 注意！綫程内不允許使用Delay()
   a = 1500; // 1500ms. You can be used in this way instead of Delay(1500); 您可以通過此方法延時，代替Delay(1500);
   statemachine=1;
  }

  if ( a <= 0 && statemachine==1) {   
   Serial.println("thread 1 again!");

   digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
    
   a = 2000; // 2000ms
   statemachine=2;
  }

   if ( a <= 0 && statemachine==2) {   
   Serial.println("thread 1 again again!");
     
   digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
     
   a = 1700; // 1700ms
   statemachine=0;
  }
}


Ritos tarefa2; // Thread Task Two / 綫程任務2
void thread2() {
  b--; 
  
  if ( b <= 0 ) {
    Serial.println("Interrupcao thread 2!");
    
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
    
    b = 4300; // 4300ms
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);  // Initialize the LED_BUILTIN pin as an output
  
  // Create threads task / 創建线程任務
  tarefa1.task(thread1);  
  tarefa2.task(thread2);

}

void loop() {
  // Nothing will be run here / 此處不會被運行任何東西
}
