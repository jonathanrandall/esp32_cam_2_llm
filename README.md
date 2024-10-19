# esp32_cam_2_llm
connect an esp32 cam to llm and back again

The project connects an esp32 cam to an LLM, which takes as input the prompt "describe this picture?" together with the image, and then returns a description of the image. The returned description is then transformed into speech. The LLM is running locally on my machine. I'm using the [ollama library](https://ollama.com/) with LLAVA model. The __LLAVA open source model__ is good for doing image analysis. I'm using an MSI laptop with an NVIDIA 4070 GPU, which has 8GB of vram.

### repository files:

* __llm_2_esp32_cam.py__: this file is running on my local maching.
* __esp32_2_llm__: this contains the test internet radio file from the audio [esp32-i2s library](https://github.com/schreibfaul1/ESP32-audioI2S). Try to get this working first to troubleshoot any i2s difficulties.
* __esp32_2_llmv4__: this contains the code running on the eps32 cam that communicates with the llm.
* __fritzing__: fritzing file and gerbers for pcb.

### equipment list

* __esp32 cam__: I'm using the AI thinker.
* __MAX 98357__: Audio amplifier.
* __speakers__: 8 ohm or 4 ohm speakers. It's worth getting good ones. Cheaper ones are at aliexpress (https://www.aliexpress.com/item/1005006056014552.html). More expensive ones are at Amazon (https://www.amazon.com.au/dp/B01LN8ONG4).
* 
