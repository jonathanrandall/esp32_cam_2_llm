#ifndef AUDIO_STUFF_H
#define AUDIO_STUFF_H

#include "global_vars.h"
#include "camera_stuff.h"

#define I2S_DOUT  12
#define I2S_BCLK  13
#define I2S_LRC   15

void play_audio2(){
  Audio audio;
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);

    audio.setVolume(8);
    // startMillis = millis();
    audio.connecttohost("http://jonny-ubuntu.local:8000/output.mp3");
    audio.loop();
    delay(20);
    while (audio.isRunning()){
      audio.loop(); 
      vTaskDelay( 20 / portTICK_PERIOD_MS);
    }
    Serial.println("finished audio");
}

void play_audio(void * params){

  bool rec_cmd;
  

  // Set thevolume (0-100)

  while (true){
    delay(200);
    xQueueReceive(audioPlayQueue, &rec_cmd, portMAX_DELAY);
    xSemaphoreTake(Semaphore_wait,  portMAX_DELAY);
    xSemaphoreTake(Semaphore_Controls,  portMAX_DELAY);
    esp_camera_deinit();
    vTaskDelay(10 / portTICK_PERIOD_MS);
    // play_audio2();

    Audio audio;
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);

    audio.setVolume(8);
    // startMillis = millis();
    audio.connecttohost("http://jonny-ubuntu.local:8000/output.mp3");
    audio.loop();
    delay(20);
    while (audio.isRunning()){
      audio.loop(); 
      vTaskDelay( 20 / portTICK_PERIOD_MS);
    }
    vTaskDelay(20 / portTICK_PERIOD_MS);
    

    vTaskDelay(1 / portTICK_PERIOD_MS);
    xSemaphoreGive(Semaphore_Controls);
    xSemaphoreGive(Semaphore_wait);
    
  }

  // i2s_driver_uninstall(I2S_NUM_0); 
  
}


#endif