// Include required libraries
#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"
#include "wifi_stuff.h"
#include "camera_stuff.h"
#include "audio_stuff.h"
#include "oled_stuff.h"

// #include "camera_stuff.h"

#include "soc/soc.h" // Disable brownout problems
#include "soc/rtc_cntl_reg.h" // Disable brownout problems
// Define I2S connections

// Wifi Credentials

#define button_pin 4


// Audio audio;
bool playAudio = false;  // Flag to control when audio is played
static unsigned long startMillis;

void show_ip(){
  display.clearDisplay();
    display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.println(WiFi.localIP());
  display.display();
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  Serial.begin(115200);
  Semaphore_Controls = xSemaphoreCreateMutex();
  Semaphore_wait = xSemaphoreCreateMutex();
  audioPlayQueue = xQueueCreate(1, sizeof(bool));
  // audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  
  // delay(1000);
  // esp_camera_deinit();
  pinMode(button_pin, INPUT);


  // Connect to Wi-Fi
  init_wifi();
  delay(50);
  Serial.print("starting camera");
  init_camera();
  delay(50);
  startCameraServer();
  Serial.println("starging oled");

  init_oled();
  Serial.println("showing ip");
  show_ip();

  // display.setTextSize(1);
  // display.setTextColor(SSD1306_WHITE);
  // display.setCursor(10, 10);
  // display.println(WiFi.localIP());
  // display.display();

  xTaskCreatePinnedToCore(
       play_audio, /* Function to implement the task */
       "rm",    /* Name of the task */
       1024*10,            /* Stack size in words */
       NULL,            /* Task input parameter */
       2,               /* Priority of the task */
       NULL,     /* Task handle. */
       1);              /* Core where the task should run */

  // delay(3000);
  // capture_camera = false;
  // delay(400);
  // esp_camera_deinit();
  // delay(500);
  // startCameraServer();

   // Connect MAX98357 I2S Amplifier Module
  //  i2s_driver_uninstall(I2S_NUM_0);
  // play_audio();

  // Serial.println("audio finished");
  // // delay(200);
  // while(init_camera()<0){
  //   Serial.println("init camera");
  // }
  // capture_camera = true;
}
bool next_cmd = true;
void loop() {

  
  // audio.loop(); 
  // delay(20);
  // // Simulate doing some task
  // Serial.println("Performing a task...");
  delay(100);  // Simulate task for 5 seconds

  if(digitalRead(button_pin))
  // if (next_cmd)
  {
    xSemaphoreTake(Semaphore_Controls,  portMAX_DELAY);
    Serial.println("button read");
    esp_camera_deinit();
    while(digitalRead(button_pin)){
      //wait for led to turn off
    }
    delay(200);
    request_inference();
    Serial.println("requesting");
    waiting_resp = true;
    // delay(500);
    // digitalWrite(4, LOW);           // Set GPIO 4 to HIGH (turn on LED, for example)
  
    // rtc_gpio_hold_en(GPIO_NUM_4);
    // xSemaphoreTake(Semaphore_Controls,  portMAX_DELAY);
    while (waiting_resp){
      delay(500);
      oled_write("waiting");
    }
    oled_write("received");
    play_audio2();
    xSemaphoreGive(Semaphore_Controls);
    // xSemaphoreGive(Semaphore_wait);
    // xQueueSend(audioPlayQueue, &next_cmd, portMAX_DELAY);
    // delay(10);
    // xSemaphoreTake(Semaphore_wait,  portMAX_DELAY);
    delay(200);
    if(init_camera()<0){
      Serial.println("init camera failed");
      oled_write("init cam failed");
      vTaskDelay( 500 / portTICK_PERIOD_MS);
      ESP.restart();
    }
    show_ip();
    next_cmd = false;
  }

  // // Set the flag to start playing audio after the task is complete
  // if (!playAudio) {
  //   playAudio = true;  // Set the flag to true to trigger audio playback
  //   Serial.println("Task completed, starting audio...");

  //   // Start streaming the audio from the local HTTP server
  //   audio.connecttohost("http://jonny-ubuntu.local:8000/output.wav");
  // }

  // // Check if audio is playing and call audio.loop() to keep it playing
  // if (playAudio) {
  //   audio.loop();  // This ensures the audio continues to play
  // }

  // // Stop the audio after a condition (e.g., after a certain time)
  // if (false && audio.isRunning() && someConditionToStop()) {
  //   Serial.println("Stopping audio...");
  //   audio.stopSong();  // Stop the current song
  //   playAudio = false;  // Reset flag so it doesn't loop the audio
  // }
}

// Define a function that stops the audio after a certain condition (e.g., time)
bool someConditionToStop() {
  // Example condition: stop after a delay or when task finishes
  
  if (millis() - startMillis > 20000) {  // Stop after 15 seconds of playing
    startMillis = millis();
    return true;
  }
  return false;
}
