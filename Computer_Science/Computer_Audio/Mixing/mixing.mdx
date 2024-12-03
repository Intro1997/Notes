~~1. 多通道 mix 逻辑可以参考 chromium 的 [third_party/webrtc/modules/audio_mixer/audio_frame_manipulator.cc -- RemixFrame()](https://source.chromium.org/chromium/chromium/src/+/main:third_party/webrtc/modules/audio_mixer/audio_frame_manipulator.cc;drc=90cac1911508d3d682a67c97aa62483eb712f69a;bpv=1;bpt=1;l=58?gsn=RemixFrame&gs=KYTHE%3A%2F%2FKoQDCr8Ba3l0aGU6Ly9jaHJvbWl1bS5nb29nbGVzb3VyY2UuY29tL2NvZGVzZWFyY2gvY2hyb21pdW0vc3JjLy9tYWluP2xhbmc9YyUyQiUyQj9wYXRoPXRoaXJkX3BhcnR5L3dlYnJ0Yy9tb2R1bGVzL2F1ZGlvX21peGVyL2F1ZGlvX2ZyYW1lX21hbmlwdWxhdG9yLmNjI05pM2dKMkJJTlo2WlVydkE5VnQ5cE0tU2F0RGs0VG95RnJWM2htOUxMbFkKvwFreXRoZTovL2Nocm9taXVtLmdvb2dsZXNvdXJjZS5jb20vY29kZXNlYXJjaC9jaHJvbWl1bS9zcmMvL21haW4_bGFuZz1jJTJCJTJCP3BhdGg9dGhpcmRfcGFydHkvd2VicnRjL21vZHVsZXMvYXVkaW9fbWl4ZXIvYXVkaW9fZnJhbWVfbWFuaXB1bGF0b3IuY2MjdXR6dUZORlpBUnJSLTdLQmxFbWlYZ0JXYmlLTTdMYW9EamtRNVV3ZHR1dw%3D%3D)。这个逻辑被用于 WebRTC 的音频数据混合。因为 WebRTC 要保证实时性，所以使用的方法简单粗暴：~~

~~- 对于低于 3 通道的音频数据：~~
~~- 下采样：使用简单的除 2~~
~~- 上采样：拷贝相同样本数据到另一个通道~~
~~- 对于高于 3 通道的音频数据使用 [ChannelMixer](https://source.chromium.org/chromium/chromium/src/+/main:third_party/webrtc/audio/utility/channel_mixer.h;drc=90cac1911508d3d682a67c97aa62483eb712f69a;l=32) 完成~~

## Chromium 内的混合方式

多通道混合可以参考 AudioNodeInput 的 [SumAllConnections()](https://source.chromium.org/chromium/chromium/src/+/main:third_party/blink/renderer/modules/webaudio/audio_node_input.cc;drc=cc5be7150eef183a1b9a6716d42a396ab7c59733;bpv=1;bpt=1;l=111?gsn=SumAllConnections&gs=KYTHE%3A%2F%2FKoADCr0Ba3l0aGU6Ly9jaHJvbWl1bS5nb29nbGVzb3VyY2UuY29tL2NvZGVzZWFyY2gvY2hyb21pdW0vc3JjLy9tYWluP2xhbmc9YyUyQiUyQj9wYXRoPXRoaXJkX3BhcnR5L2JsaW5rL3JlbmRlcmVyL21vZHVsZXMvd2ViYXVkaW8vYXVkaW9fbm9kZV9pbnB1dC5jYyNVTXVOTGlIWDB3U1lYOWg5WTVqVkhDdlNEUGtlNVYtQzNxdDJGT3NoVjBZCr0Ba3l0aGU6Ly9jaHJvbWl1bS5nb29nbGVzb3VyY2UuY29tL2NvZGVzZWFyY2gvY2hyb21pdW0vc3JjLy9tYWluP2xhbmc9YyUyQiUyQj9wYXRoPXRoaXJkX3BhcnR5L2JsaW5rL3JlbmRlcmVyL21vZHVsZXMvd2ViYXVkaW8vYXVkaW9fbm9kZV9pbnB1dC5jYyNqQmNxVFpiYlNMSF9qZk9xUVVDRWxFUGhsR0JxUDBRY3dOZzE3ZXNHYlF3)，它对所有保存输入节点输出数据的 AudioBus 进行了混合，最终调用到 [AudioBus::SumFrom()](https://source.chromium.org/chromium/chromium/src/+/main:third_party/blink/renderer/platform/audio/audio_bus.cc;drc=cc5be7150eef183a1b9a6716d42a396ab7c59733;bpv=1;bpt=1;l=261?gsn=SumFrom&gs=KYTHE%3A%2F%2FKu4CCrQBa3l0aGU6Ly9jaHJvbWl1bS5nb29nbGVzb3VyY2UuY29tL2NvZGVzZWFyY2gvY2hyb21pdW0vc3JjLy9tYWluP2xhbmc9YyUyQiUyQj9wYXRoPXRoaXJkX3BhcnR5L2JsaW5rL3JlbmRlcmVyL3BsYXRmb3JtL2F1ZGlvL2F1ZGlvX2J1cy5jYyMyNlNLaW0xcktBQTNPSGRIYjhCd2QxNE80U2JONHRmQVhKNGNiZHRESTNvCrQBa3l0aGU6Ly9jaHJvbWl1bS5nb29nbGVzb3VyY2UuY29tL2NvZGVzZWFyY2gvY2hyb21pdW0vc3JjLy9tYWluP2xhbmc9YyUyQiUyQj9wYXRoPXRoaXJkX3BhcnR5L2JsaW5rL3JlbmRlcmVyL3BsYXRmb3JtL2F1ZGlvL2F1ZGlvX2J1cy5jYyNDN3lCN0hDbjJZMlMyX0pjelFnV2sxOFRCTkRUWk5MMWxUZjBIX1hRVGdv)
对于 4 -> 2 的降采样，他居然只是简单的除以 2！！

```cpp
void AudioBus::SumFromByDownMixing(const AudioBus& source_bus) {
  unsigned number_of_source_channels = source_bus.NumberOfChannels();
  unsigned number_of_destination_channels = NumberOfChannels();

  if (number_of_source_channels == 2 && number_of_destination_channels == 1) {
    // Down-mixing: 2 -> 1
    //   output = 0.5 * (input.L + input.R)
  } else if (number_of_source_channels == 4 &&
             number_of_destination_channels == 1) {
    // Down-mixing: 4 -> 1
    //   output = 0.25 * (input.L + input.R + input.SL + input.SR)
  } else if (number_of_source_channels == 6 &&
             number_of_destination_channels == 1) {
    // Down-mixing: 5.1 -> 1
    //   output = sqrt(1/2) * (input.L + input.R) + input.C
    //            + 0.5 * (input.SL + input.SR)
  } else if (number_of_source_channels == 4 &&
             number_of_destination_channels == 2) {
    // Down-mixing: 4 -> 2
    //   output.L = 0.5 * (input.L + input.SL)
    //   output.R = 0.5 * (input.R + input.SR)
  } else if (number_of_source_channels == 6 &&
             number_of_destination_channels == 2) {
    // Down-mixing: 5.1 -> 2
    //   output.L = input.L + sqrt(1/2) * (input.C + input.SL)
    //   output.R = input.R + sqrt(1/2) * (input.C + input.SR)
  } else if (number_of_source_channels == 6 &&
             number_of_destination_channels == 4) {
    // Down-mixing: 5.1 -> 4
    //   output.L = input.L + sqrt(1/2) * input.C
    //   output.R = input.R + sqrt(1/2) * input.C
    //   output.SL = input.SL
    //   output.SR = input.SR
  } else {
    // All other cases, fall back to the discrete sum. This will perform
    // channel-wise sum until the destination channels run out.
    DiscreteSumFrom(source_bus);
  }
}

```

## 使用 Chrome 进行验证

我在 `mixing_test.js` 中创建了一个包含 4 通道音频数据的 AudioBufferSourceNode，前两个通道和后两个通道的数据分别是 261.6Hz(do) 和 329.6Hz(mi)，并且将这个 AudioBufferSourceNode 连接到 AudioGainNode 上，通过 AudioWorklet 获取 AudioGainNode 的输出；同时，本地使用 `mixedLeft = (left0 + left1) / 2.0` 的方式对之前的 4 通道音频数据进行混合，然后对比它们，发现误差小于 `1e-6`，这对于 SLint16 的数据来说完全足够（`32767 * 1e-5 < 1`）。

## 发现的问题

对于 `mixing_test.js` 中的例子，AudioGainNode 获取到的音频数据，其范围在 [0, 1023] 和 [1024, 2047] 的数据出现重复，每个通道都是这样，这个还不清楚是什么原因导致的，所以对两个通道的比较都是从 1024 开始的
