class MyProcessor extends AudioWorkletProcessor {
  constructor() {
    super();
    this.inputAudioData = [];
    this.port.onmessage = (e) => {
      if (e.data === "getInputAudioData") {
        this.port.postMessage(this.inputAudioData);
      } else {
        console.error("Error reqest: ", e.data);
      }
    };
  }

  process(inputs, outputs, parameters) {
    const input = inputs[0];
    const output = outputs[0];

    if (input && output) {
      for (let channel = 0; channel < input.length; channel++) {
        const inputData = input[channel];
        const outputData = output[channel];

        if (!this.inputAudioData[channel]) {
          this.inputAudioData.push(Array.from(inputData));
        }

        this.inputAudioData[channel] = this.inputAudioData[channel].concat(
          Array.from(inputData)
        );

        for (let i = 0; i < inputData.length; i++) {
          outputData[i] = inputData[i];
        }
      }
    }
    return true;
  }
}

try {
  registerProcessor("my-processor", MyProcessor);
} catch (e) {
  console.error("register process node failed, ", e);
}
