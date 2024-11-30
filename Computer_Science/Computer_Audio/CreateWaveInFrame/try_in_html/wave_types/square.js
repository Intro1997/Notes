window.onload = () => {
  const console_chrome_square_data_button = document.getElementById(
    "console_chrome_square_data"
  );
  if (console_chrome_square_data) {
    console_chrome_square_data.onclick = async () => {
      await consoleChromeSquareData();
    };
  }
};

async function consoleChromeSquareData() {
  const squareFreqElement = document.getElementById("square_freq");
  if (squareFreqElement) {
    const squareFreq = squareFreqElement.value;
    const audioCtx = new AudioContext();

    const oscillatorNode = audioCtx.createOscillator();
    const audioWorkletNode = await createDataGetAudioWorklet(
      audioCtx,
      (data) => {
        console.log(data);
        oscillatorNode.stop();
      }
    );

    oscillatorNode.type = "square";
    oscillatorNode.frequency.value = squareFreq;
    oscillatorNode.connect(audioWorkletNode);
    audioWorkletNode.connect(audioCtx.destination);
    oscillatorNode.start();
  }
}

const PI_2 = Math.PI * 2;
function generateSquareWaveData(freq, sampleRate, requestSize, saveArray) {
  const step = (PI_2 * freq) / sampleRate;
  let currentIdx = 0;
  for (let i = 0; i < requestSize; i++) {
    if (currentIdx >= PI_2) {
      currentIdx = currentIdx - PI_2;
    }
    saveArray[i] = currentIdx >= Math.PI ? -1 : 1;
    currentIdx += step;
  }
}

function playGeneratedSquareData() {
  const squareFreqElement = document.getElementById("square_freq");
  if (squareFreqElement) {
    const squareFreq = squareFreqElement.value;
    const audioCtx = new AudioContext();
    const audioBufferData = audioCtx.createBuffer(
      2,
      audioCtx.sampleRate,
      audioCtx.sampleRate
    );

    const audioBufferDataChannel0 = audioBufferData.getChannelData(0);
    const audioBufferDataChannel1 = audioBufferData.getChannelData(1);

    generateSquareWaveData(
      squareFreq,
      audioCtx.sampleRate,
      audioBufferDataChannel0.length,
      audioBufferDataChannel0
    );

    generateSquareWaveData(
      squareFreq,
      audioCtx.sampleRate,
      audioBufferDataChannel1.length,
      audioBufferDataChannel1
    );

    const audioBufferSourceNode = audioCtx.createBufferSource();
    audioBufferSourceNode.buffer = audioBufferData;
    audioBufferSourceNode.connect(audioCtx.destination);
    audioBufferSourceNode.start();
  }
}
