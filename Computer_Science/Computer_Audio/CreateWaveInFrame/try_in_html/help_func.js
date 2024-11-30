const AudioContext = window.AudioContext || window.webkitAudioContext;

async function createDataGetAudioWorklet(
  audioCtx,
  dataGetCb,
  disconnectTime = 1000 /* ms */
) {
  try {
    let backDeep = window.location.pathname.split("/").length - 2;
    let backStr = "";
    while (backDeep) {
      backStr += "../";
      backDeep--;
    }
    await audioCtx.audioWorklet.addModule(backStr + "process.js");
  } catch (error) {
    console.log("load module error, " + error);
  }

  const audioWorkletNode = new AudioWorkletNode(audioCtx, "my-processor");

  setTimeout(() => {
    audioWorkletNode.disconnect();
    console.log("on post");
    audioWorkletNode.port.postMessage("getInputAudioData");
    audioWorkletNode.port.onmessage = (e) => {
      dataGetCb(e.data);
    };
  }, disconnectTime);
  return audioWorkletNode;
}

/**
 *
 * @param {*} freq
 * @param {*} cb: function(data) {}
 * @param {*} playTime
 */
async function getChromeSinxData(
  audioCtx,
  freq,
  detune,
  cb,
  playTime = 1000 /* ms */
) {
  const oscillator = audioCtx.createOscillator();
  oscillator.type = "sine";

  oscillator.frequency.value = freq;
  oscillator.detune.value = detune;
  console.log(oscillator.detune)

  const sinxDataAudioWorkletNode = await createDataGetAudioWorklet(
    audioCtx,
    (data) => {
      oscillator.stop();
      cb(data);
    },
    playTime
  );

  oscillator.connect(sinxDataAudioWorkletNode);
  sinxDataAudioWorkletNode.connect(audioCtx.destination);
  oscillator.start();
}

function getFreqFromElementId(id_string) {
  const input_elem = document.getElementById(id_string);
  if (!input_elem) {
    console.error("Cannot find element which id is sinx_freq!");
    return;
  }
  const freq = Number(input_elem.value);
  if (isNaN(freq)) {
    alert("Frequency is not a number!");
    return null;
  }
  return freq;
}

function compare_kernal(left_data, right_data) {
  for (let i = 0; i < left_data.length; i++) {
    // console.log(`${i} : ${Math.abs(left_data[i] - right_data[i])}`);
    // if (i == 4361 || i == 4362) {
    //   console.log(
    //     `compare ${i}: left_data[${i}] = ${left_data[i]} vs right_data[${i}] = ${right_data[i]}`
    //   );
    // }
    if (Math.abs(left_data[i] - right_data[i]) > 0.00001) {
      console.log("not equal at ", i);
      console.log(
        `left_data[${i}] is ${left_data[i]}, but right_data[${i}] is ${right_data[i]}`
      );
      return;
    }
  }
  console.log("total equal!");
}
