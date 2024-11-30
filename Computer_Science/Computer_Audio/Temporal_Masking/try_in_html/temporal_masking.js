const SOL_TONE_FREQ = 392;
const DOUBLE_PI = 2 * Math.PI;

function get_a_period_sine_wave_audio_data(freq, sampleRate) {
  const step = (DOUBLE_PI * freq) / sampleRate;
  const less_than_one_peroid_step_count = Math.floor(DOUBLE_PI / step);
  const audio_data_for_one_period = [];
  let ret = 0;
  for (let i = 0; i < less_than_one_peroid_step_count; i++) {
    ret = Math.sin(step * i);
    audio_data_for_one_period.push(ret);
  }
  return audio_data_for_one_period;
}

function get_empty_data(empty_time_ms, sampleRate) {
  const sample_size = Math.floor(empty_time_ms / 1000 / (1.0 / sampleRate));
  const empty_data_array = [];
  for (let i = 0; i < sample_size; i++) {
    empty_data_array.push(0);
  }
  return empty_data_array;
}

function playing_sol(empty_time_ms) {
  const audioCtx = new (window.AudioContext || window.webkitAudioContext)();

  const on_period_audio_data = get_a_period_sine_wave_audio_data(
    SOL_TONE_FREQ,
    audioCtx.sampleRate
  );
  const empty_data = get_empty_data(empty_time_ms, audioCtx.sampleRate);
  const segment_data = on_period_audio_data.concat(empty_data);
  const segment_cnt = Math.floor(audioCtx.sampleRate / segment_data.length);
  const sample_size = segment_data.length * segment_cnt;

  const myArrayBuffer = audioCtx.createBuffer(
    2,
    // audioCtx.sampleRate * 3,
    sample_size,
    audioCtx.sampleRate
  );

  for (let channel = 0; channel < myArrayBuffer.numberOfChannels; channel++) {
    const nowBuffering = myArrayBuffer.getChannelData(channel);
    for (let seg_idx = 0; seg_idx < segment_cnt; seg_idx++) {
      for (
        let i = seg_idx * segment_data.length, seg_data_idx = 0;
        i < (seg_idx + 1) * segment_data.length;
        i++, seg_data_idx++
      ) {
        nowBuffering[i] = segment_data[seg_data_idx];
      }
    }
  }
  const source = audioCtx.createBufferSource();
  source.buffer = myArrayBuffer;
  source.connect(audioCtx.destination);
  source.start();
}

function click_to_start() {
  const interval_ms_time = parseInt(
    document.getElementById("interval_time_ms").value
  );
  if (
    interval_ms_time !== interval_ms_time ||
    interval_ms_time < 0 ||
    interval_ms_time > 500
  ) {
    alert("Please enter invalid integer [0, 500]!");
  } else {
    const hint = document.getElementById("operation_hint");
    hint.innerHTML =
      "Each audio data will wait " +
      interval_ms_time +
      "ms after playing finished!";
    playing_sol(interval_ms_time);
  }
}
