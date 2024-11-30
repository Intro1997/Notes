window.onload = function () {
  document.getElementById("console_origin_sine_data").onclick = async () => {
    await consoleOriginSineData();
  };
  create_preload_sine_data();
};

const PRELOAD_SINE_DATA_SIZE = 4096;
const PRELOAD_SINE_DATA_LEN = PRELOAD_SINE_DATA_SIZE - 1;
let PRELOAD_SINE_DATA = new Float32Array(PRELOAD_SINE_DATA_SIZE);
function create_preload_sine_data() {
  const step = (Math.PI * 2) / 4096;
  for (let i = 0; i < PRELOAD_SINE_DATA_SIZE; i++) {
    PRELOAD_SINE_DATA[i] = Math.sin(step * i);
  }
}

async function consoleOriginSineData() {
  const freq = getFreqFromElementId("sinx_freq");
  const detune = getFreqFromElementId("sinx_detune");
  if (freq && detune) {
    const audioCtx = new AudioContext();
    await getChromeSinxData(
      audioCtx,
      freq,
      detune,
      (chromeData) => {
        console.log(chromeData[0]);
      },
      1000
    );
  }
}

function generate_sine_audio_data(
  freq,
  detune,
  dataLength,
  saveArray,
  sampleRate
) {
  /**
   * 由于 freq 的范围在 [-sampleRate/2, sampleRate/2]
   * 所以 sampleRate/freq 的范围在 [-inf, -2] and [2, inf]
   * 所以 internal 的范围在 [-2048, 2048]
   * 对于 internal，考虑三种情况：
   *    - internal >= 1：求出 floor_idx 和 ceil_idx，两者之间进行插值
   *    - 0 < internal < 1：做两点的线性插值
   *    - internal < 0：取上述两个情况的相反数
   * 对于 freq 为 0 的情况，返回全 0 数组
   */

  if (saveArray.length < dataLength) {
    saveArray = new Float32Array(dataLength);
  }

  const resultSymbol = freq < 0 ? -1 : 1;
  const absFreq = Math.abs(freq * Math.pow(2, detune / 1200));
  if (
    absFreq == 0 ||
    Math.abs(absFreq - sampleRate / 2) < 1e-5 ||
    absFreq > sampleRate / 2
  ) {
    console.log(
      `${absFreq} is large equal than ${sampleRate / 2}, cannot do sampling`
    );
    saveArray.fill(0);
    return;
  }

  let internal = (PRELOAD_SINE_DATA_SIZE / sampleRate) * absFreq;

  if (internal >= Math.floor(PRELOAD_SINE_DATA_SIZE / 2)) {
    console.log(
      `Cannot use internal<${internal}> which large eaqul than half preload data size<${Math.floor(
        PRELOAD_SINE_DATA_SIZE / 2
      )}>`
    );
    return;
  }

  saveArray[0] = PRELOAD_SINE_DATA[0];

  let idx = internal;
  let idx_floor;
  let idx_ceil;
  let percent = 0;
  let final_data;
  for (let i = 1; i < dataLength; i++) {
    if (idx >= PRELOAD_SINE_DATA_SIZE) {
      idx = idx - PRELOAD_SINE_DATA_SIZE;
    }

    idx_floor = Math.floor(idx);
    percent = idx - idx_floor;

    // if (idx_floor >= PRELOAD_SINE_DATA_SIZE) {
    //   idx_floor = idx_floor - PRELOAD_SINE_DATA_SIZE;
    // }

    idx_ceil = idx_floor == PRELOAD_SINE_DATA_LEN ? 0 : idx_floor + 1;

    if (
      idx_floor > PRELOAD_SINE_DATA_LEN ||
      idx_ceil > PRELOAD_SINE_DATA_LEN ||
      idx_floor < 0 ||
      idx_ceil < 0
    ) {
      console.log(
        `error idx! idx_floor = ${idx_floor}, i = ${i}, internal = ${internal}, idx = ${idx}, percent = ${percent}`
      );
      return;
    }

    final_data =
      PRELOAD_SINE_DATA[idx_floor] +
      (PRELOAD_SINE_DATA[idx_ceil] - PRELOAD_SINE_DATA[idx_floor]) * percent;

    // if (i == 4362 || i == 4361) {
    //   console.log(
    //     `i = ${i}\nfinal_data = ${final_data}\nidx_floor = ${idx_floor}\nidx_ceil = ${idx_ceil}\nPRELOAD_SINE_DATA[idx_floor] = ${PRELOAD_SINE_DATA[idx_floor]}\nPRELOAD_SINE_DATA[idx_ceil] = ${PRELOAD_SINE_DATA[idx_ceil]}\npercent = ${percent}\ninternal = ${internal}`
    //   );
    // }

    if (isNaN(final_data)) {
      console.log(
        `final_data is null: idx_floor = ${idx_floor}, idx_ceil = ${idx_ceil}, PRELOAD_SINE_DATA[idx_floor] = ${PRELOAD_SINE_DATA[idx_floor]}, PRELOAD_SINE_DATA[idx_ceil] = ${PRELOAD_SINE_DATA[idx_ceil]}, internal = ${internal}`
      );
    }

    saveArray[i] = resultSymbol * final_data;

    idx += internal;
  }
}

function play_generated_data(freq, detune) {
  const audioCtx = new AudioContext();
  const audioBuffer = audioCtx.createBuffer(
    2,
    audioCtx.sampleRate,
    audioCtx.sampleRate
  );

  for (let channel = 0; channel < audioBuffer.numberOfChannels; channel++) {
    const currentChannel = audioBuffer.getChannelData(channel);

    generate_sine_audio_data(
      freq,
      detune,
      audioBuffer.length,
      currentChannel,
      audioCtx.sampleRate
    );

    console.log(currentChannel);
  }

  const source = audioCtx.createBufferSource();
  source.buffer = audioBuffer;
  source.connect(audioCtx.destination);
  source.start();
}

function play_generated_sine() {
  const freq = getFreqFromElementId("sinx_freq");
  const detune = getFreqFromElementId("sinx_detune");
  if (freq && detune) {
    play_generated_data(freq, detune);
  }
}

function play_origin_sine() {
  const freq = getFreqFromElementId("sinx_freq");
  const detune = getFreqFromElementId("sinx_detune");
  if (freq && detune) {
    const audioCtx = new (window.AudioContext || window.webkitAudioContext)();
    const oscillatorNode = audioCtx.createOscillator();
    oscillatorNode.frequency.value = freq;
    oscillatorNode.detune.value = detune;
    oscillatorNode.connect(audioCtx.destination);
    oscillatorNode.start();
    setTimeout(() => {
      oscillatorNode.stop();
    }, 1000);
  }
}

function compareGeneratedWithNature() {
  const resultElement = document.getElementById("compare_result");
  resultElement.innerHTML = "Comparing...Please wait a minute...";

  const audioCtx = new AudioContext();
  const oscillatorNode = audioCtx.createOscillator();

  const sampleRate = audioCtx.sampleRate;
  const maxFreq = oscillatorNode.frequency.maxValue;
  const minFreq = oscillatorNode.frequency.minValue;

  const duration = 5; // 5 second

  const generatedSamples = new Float32Array(duration * sampleRate);

  let maxDiffAbs = 0;
  let maxDiffFreq = 0;
  let maxDiffIdx = 0;

  console.log(
    `Comparing freq[${minFreq}, ${maxFreq}], this will cost lot of time...`
  );

  const detune = 0.0;

  for (let currentFreq = minFreq; currentFreq <= maxFreq; currentFreq++) {
    if (currentFreq % 100 == 0) {
      console.log(
        `Start comparing freq[${currentFreq} ~ ${currentFreq + 100}]...`
      );
    }

    const natureStep =
      (2 * Math.PI * currentFreq * Math.pow(2, detune / 1200)) / sampleRate;
    let diffAbs = 0;
    let currentSample = 0;
    generatedSamples.fill(0);

    generate_sine_audio_data(
      currentFreq,
      detune,
      generatedSamples.length,
      generatedSamples,
      sampleRate
    );

    for (let t = 0; t < duration * sampleRate; t++) {
      currentSample = Math.sin(t * natureStep);
      diffAbs = Math.abs(currentSample - generatedSamples[t]);
      if (maxDiffAbs < diffAbs) {
        maxDiffAbs = diffAbs;
        maxDiffFreq = currentFreq;
        maxDiffIdx = t;
      }
      if (diffAbs > 0.001) {
        const str0 = `Compare freq<${currentFreq}> with sampleRate<${sampleRate}> failed!\n`;
        const str1 = `currentSample<${currentSample}> != generatedSamples[${t}]<${generatedSamples[t]}>\n`;
        const str2 = `diffAbs = ${diffAbs}\n`;

        resultElement.style.color = "red";
        resultElement.innerHTML = str0 + str1 + str2;

        console.log(str0 + str1 + str2);
        return;
      }
    }
    if (currentFreq % 100 == 0 && currentFreq != minFreq) {
      console.log(
        `\tfreq[${currentFreq - 100} ~ ${currentFreq}] has finished.`
      );
    }

    // const str0 = `Compare freq<${currentFreq}> with sampleRate<${sampleRate}> Success!`;
    // const str1 = `Max abs of diff between nature and generated data is ${maxDiffAbs}`;

    // console.log(str0 + str1);
  }

  resultElement.style.color = "green";
  resultElement.innerHTML = `Compare all freq with sampleRate<${sampleRate}> Success! maxDiffAbs of all = ${maxDiffAbs} in freq<${maxDiffFreq} at idx<${maxDiffIdx}>>`;
}
