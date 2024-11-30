window.onload = () => {
  console.log("hhhh");
  const console_chrome_saw_data_button = document.getElementById(
    "console_chrome_saw_data"
  );
  console.log(console_chrome_saw_data_button);
  console_chrome_saw_data_button.onclick = async () => {
    await consoleChromeSawDataButton();
  };
};

function generateSawWaveData(freq, sampleRate, requestSize, saveArray) {
  // create sawtooth wave data, start from x = 0, y = 0
  // function is y = (1/pi)x <x=[0, pi)>, y = (-1/pi)x <x=(pi, 2pi]>

  const step = (2 * Math.PI * freq) / sampleRate;
  console.log(step);
  const k = 1.0 / Math.PI;
  let currentIdx = 0;
  for (let i = 0; i < requestSize; i++) {
    if (currentIdx > Math.PI) {
      // currentIdx = (currentIdx - pi) + (-pi)
      currentIdx = currentIdx - 2 * Math.PI;
    }

    saveArray[i] = k * currentIdx;

    currentIdx += step;
  }
}

async function consoleChromeSawDataButtonInner() {
  const sawFreqElement = document.getElementById("saw_freq");
  if (!sawFreqElement) {
    return;
  }
  const audioCtx = new AudioContext();
  console.log("hello10");

  const oscillatorNode = new OscillatorNode(audioCtx, {
    channelCount: 10,
    channelCountMode: "explicit",
    channelInterpretation: "discrete"
  });
  console.log(oscillatorNode);
  const audioWorkletNode = await createDataGetAudioWorklet(audioCtx, (data) => {
    console.log(data);
    oscillatorNode.stop();
  });
  oscillatorNode.type = "sawtooth";
  const sawFreq = sawFreqElement.value;
  oscillatorNode.frequency.value = sawFreq;
  oscillatorNode.connect(audioWorkletNode);
  audioWorkletNode.connect(audioCtx.destination);
  oscillatorNode.start();
}

async function consoleChromeSawDataButton() {
  consoleChromeSawDataButtonInner();
  // const sawFreqElement = document.getElementById("saw_freq");
  // if (!sawFreqElement) {
  //   return;
  // }
  // const sawFreq = sawFreqElement.value;
  // const audioCtx = new AudioContext();
  // const oscillatorNode = audioCtx.createOscillator();
  // const audioWorkletNode = await createDataGetAudioWorklet(audioCtx, (data) => {
  //   console.log(data);
  //   oscillatorNode.stop();
  // });
  // oscillatorNode.type = "sawtooth";
  // oscillatorNode.frequency.value = sawFreq;
  // oscillatorNode.connect(audioWorkletNode);
  // audioWorkletNode.connect(audioCtx.destination);
  // oscillatorNode.start();
}

function playGeneratedSawData() {
  const sawFreqElement = document.getElementById("saw_freq");
  if (!sawFreqElement) {
    return;
  }
  const sawFreq = sawFreqElement.value;

  const audioCtx = new AudioContext();
  const audioBufferData = audioCtx.createBuffer(
    2,
    audioCtx.sampleRate,
    audioCtx.sampleRate
  );
  const audioBufferSourceNode = audioCtx.createBufferSource();

  const channelData0 = audioBufferData.getChannelData(0);
  const channelData1 = audioBufferData.getChannelData(1);
  generateSawWaveData(
    sawFreq,
    audioCtx.sampleRate,
    audioCtx.sampleRate,
    channelData0
  );
  generateSawWaveData(
    sawFreq,
    audioCtx.sampleRate,
    audioCtx.sampleRate,
    channelData1
  );

  console.log(Array.from(channelData0));

  audioBufferSourceNode.buffer = audioBufferData;

  audioBufferSourceNode.connect(audioCtx.destination);
  audioBufferSourceNode.start();
}
