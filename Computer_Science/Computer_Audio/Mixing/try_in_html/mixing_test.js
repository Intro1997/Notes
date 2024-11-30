const AudioContext = window.AudioContext || window.webkitAudioContext;

window.onload = function () {
  document.getElementById("do_mi_button").onclick = async () => {
    await do_mi_buffer();
  };
};

const DOUBLE_PI = 2.0 * Math.PI;
function createSineAudioData(frequency, sampleRate, sampleSize) {
  const step = (DOUBLE_PI * frequency) / sampleRate;
  const retArr = [];
  for (let i = 0; i < sampleSize; i++) {
    retArr[i] = Math.sin(step * i);
  }
  return retArr;
}

function create4ChannelAudioData(audioCtx, dataLength) {
  const do_array = Array.from(
    createSineAudioData(261.6, audioCtx.sampleRate, dataLength)
  );
  const me_array = Array.from(
    createSineAudioData(329.6, audioCtx.sampleRate, dataLength)
  );
  return [do_array, do_array, me_array, me_array];
}

function create4ChannelAudioBufferSource(audioCtx, fourChannalData) {
  const audioBuffer = audioCtx.createBuffer(
    4,
    fourChannalData[0].length,
    audioCtx.sampleRate
  );
  for (let i = 0; i < fourChannalData.length; i++) {
    audioBuffer.copyToChannel(new Float32Array(fourChannalData[i]), i);
  }
  const audioBufferSourceNode = audioCtx.createBufferSource();
  audioBufferSourceNode.buffer = audioBuffer;
  return audioBufferSourceNode;
}

function createDataGetAudioGainNode(audioCtx) {
  const gainNode = audioCtx.createGain();
  gainNode.gain.value = 1.0;
  return gainNode;
}

async function createDataGetAudioWorklet(audioCtx, dataGetCb) {
  await audioCtx.audioWorklet.addModule("process.js");
  const audioWorkletNode = new AudioWorkletNode(audioCtx, "my-processor");
  setTimeout(() => {
    audioWorkletNode.disconnect();
    audioWorkletNode.port.postMessage("getInputAudioData");
    audioWorkletNode.port.onmessage = (e) => {
      dataGetCb(e.data);
    };
  }, 1100);
  return audioWorkletNode;
}

function mix4ChannelToStereo(fourChannelArray) {
  const retLeft = [];
  const retRight = [];
  const src0Left = new Float32Array(fourChannelArray[0]);
  const src0Right = new Float32Array(fourChannelArray[1]);
  const src1Left = new Float32Array(fourChannelArray[2]);
  const src1Right = new Float32Array(fourChannelArray[3]);
  for (let i = 0; i < fourChannelArray[0].length; i++) {
    retLeft[i] = (src0Left[i] + src1Left[i]) / 2;
    retRight[i] = (src0Right[i] + src1Right[i]) / 2;
  }
  return [retLeft, retRight];
}

function compareArray(left, right) {
  console.log("left array = ", left);
  console.log("right array = ", right);

  const offset = 1024; /* use this offset because mixed the result data from
                          gain node has the same audio data at range [0, 1023] 
                          and [1024, 2047], so we compare start with 1024-th
                          element.
                          If you find it's not the correct, you may should check
                          output result of your mixed audio data and modify this 
                          offset.
                        */
  const minChannelLen = left.length < right.length ? left.length : right.length;
  const minSampleLen =
    left[0].length < right[0].length ? left[0].length : right[0].length;
  for (let i = 0; i < minChannelLen; i++) {
    for (let j = 0; j < minSampleLen; j++) {
      if (Math.abs(left[i][j] - right[i][j + offset]) > 1e-6) {
        console.log(
          `too large distance at [${i}][${j + offset}], ${left[i][j]} != ${
            right[i][j + offset]
          }`
        );
        break;
      }
    }
  }
  console.log(
    `compare finished! They are equal in ${minChannelLen} channels and ${minSampleLen} samples.`
  );
}

function create2ChannelAudioBufferSource(audioCtx, twoChannalData) {
  const audioBuffer = audioCtx.createBuffer(
    2,
    twoChannalData[0].length,
    audioCtx.sampleRate
  );
  for (let i = 0; i < twoChannalData.length; i++) {
    audioBuffer.copyToChannel(new Float32Array(twoChannalData[i]), i);
  }
  const audioBufferSourceNode = audioCtx.createBufferSource();
  audioBufferSourceNode.buffer = audioBuffer;
  return audioBufferSourceNode;
}

async function compareGainMixedAndMyMixed(audioCtx, channelAudioData) {
  console.log("compare gain mixed data with my mixed data");
  const myMixRetAudioData = mix4ChannelToStereo(channelAudioData);
  const audioBufferSourceNode = create4ChannelAudioBufferSource(
    audioCtx,
    channelAudioData
  );
  let dataAfterMixedByGainNode = null;
  const gainNode = createDataGetAudioGainNode(audioCtx);
  const dataGetAudioWorkletNode = await createDataGetAudioWorklet(
    audioCtx,
    (data) => {
      dataAfterMixedByGainNode = data;
      console.log(dataAfterMixedByGainNode);
      compareArray(myMixRetAudioData, dataAfterMixedByGainNode);
    }
  );

  audioBufferSourceNode.connect(gainNode);
  gainNode.connect(dataGetAudioWorkletNode);
  audioBufferSourceNode.start();
  dataGetAudioWorkletNode.connect(audioCtx.destination);
}

function playUseMyMixedData(audioCtx, channelAudioData) {
  console.log("playing use my mixed data");
  const myMixRetAudioData = mix4ChannelToStereo(channelAudioData);
  const audioBufferSourceNode = create2ChannelAudioBufferSource(
    audioCtx,
    myMixRetAudioData
  );
  audioBufferSourceNode.connect(audioCtx.destination);
  audioBufferSourceNode.start();
}

async function do_mi_buffer() {
  const audioCtx = new AudioContext();
  const sampleLength = audioCtx.sampleRate;
  const channelAudioData = create4ChannelAudioData(audioCtx, sampleLength);
  await compareGainMixedAndMyMixed(audioCtx, channelAudioData);
  // playUseMyMixedData(audioCtx, channelAudioData);
}
