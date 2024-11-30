window.onload = () => {
  const console_chrome_triangle_data_button = document.getElementById(
    "console_chrome_triangle_data"
  );
  if (console_chrome_triangle_data) {
    console_chrome_triangle_data.onclick = async () => {
      await consoleChromeTriangleData();
    };
  }
};

async function consoleChromeTriangleData() {
  const triangleFreqElement = document.getElementById("triangle_freq");
  if (triangleFreqElement) {
    const triangleFreq = triangleFreqElement.value;
    const audioCtx = new AudioContext();

    const oscillatorNode = audioCtx.createOscillator();
    const audioWorkletNode = await createDataGetAudioWorklet(
      audioCtx,
      (data) => {
        console.log(data);
        oscillatorNode.stop();
      }
    );

    oscillatorNode.type = "triangle";
    oscillatorNode.frequency.value = triangleFreq;
    oscillatorNode.connect(audioWorkletNode);
    audioWorkletNode.connect(audioCtx.destination);
    oscillatorNode.start();
  }
}

const PI_2 = Math.PI * 2;
const PI_half = Math.PI / 2;
const PI_3_half = (3 * Math.PI) / 2;
function generateTriangleWaveData(freq, sampleRate, requestSize, saveArray) {
  // create triangle wave data, start from x = 0, y = 0
  // function is y = (1/pi)x (x belongs to [0, pi) )
  //             y = (-1/pi)(x - 2pi) (x belongs to [pi, 2pi) )

  const step = (PI_2 * freq) / sampleRate;
  // 1 / (Math.PI/2)
  const k = 2.0 / Math.PI;
  let currentIdx = 0;
  for (let i = 0; i < requestSize; i++) {
    if (currentIdx >= PI_2) {
      currentIdx = currentIdx - PI_2;
    }

    if (currentIdx > PI_half && currentIdx <= PI_3_half) {
      saveArray[i] = -k * (currentIdx - Math.PI);
    } else if (currentIdx > PI_3_half && currentIdx <= PI_2) {
      saveArray[i] = k * (currentIdx - PI_2);
    } else {
      saveArray[i] = k * currentIdx;
    }

    currentIdx += step;
  }
}

function playGeneratedTriangleData() {
  const triangleFreqElement = document.getElementById("triangle_freq");
  if (triangleFreqElement) {
    const triangleFreq = triangleFreqElement.value;
    const audioCtx = new AudioContext();
    const audioBufferData = audioCtx.createBuffer(
      2,
      audioCtx.sampleRate,
      audioCtx.sampleRate
    );

    const audioBufferDataChannel0 = audioBufferData.getChannelData(0);
    const audioBufferDataChannel1 = audioBufferData.getChannelData(1);

    generateTriangleWaveData(
      triangleFreq,
      audioCtx.sampleRate,
      audioBufferDataChannel0.length,
      audioBufferDataChannel0
    );

    generateTriangleWaveData(
      triangleFreq,
      audioCtx.sampleRate,
      audioBufferDataChannel1.length,
      audioBufferDataChannel1
    );

    console.log(Array.from(audioBufferDataChannel0));

    const audioBufferSourceNode = audioCtx.createBufferSource();
    audioBufferSourceNode.buffer = audioBufferData;
    audioBufferSourceNode.connect(audioCtx.destination);
    audioBufferSourceNode.start();
  }
}
