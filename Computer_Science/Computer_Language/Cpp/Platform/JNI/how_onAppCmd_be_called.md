# onAppCmd 是如何被调用到的

<font color='red'>NOTE：本文只讨论 ALooper_pollAll 的情况</font>

## 阅读准备

- [Android Code Search](https://cs.android.com/android)（最好使用 main 分支）

## 正文

Android NDK 中，通过调用 ALooper_pollAll 来拦截 Native Activity 的所有事件，并通过调用 `(android_poll_source*)(*outData)->process(...)` 来处理事件。如果我们想要通过自己的方法来处理事件，则需要在 process 调用之前，用自己实现的方法来初始化 android_app 的 onAppCmd 属性。这说明 process 几乎与 onAppCmd 是绑定关系，onAppCmd 通过 process 来被调用。但这个流程是什么样的呢？

通过在 Android Code Search 中对 onAppCmd 的搜索，我们可以发现 android_native_app_glue.c 中的 process_cmd 调用了该方法：

```cpp
static void process_cmd(struct android_app* app, struct android_poll_source* source) {
    int8_t cmd = android_app_read_cmd(app);
    android_app_pre_exec_cmd(app, cmd);
    if (app->onAppCmd != NULL) app->onAppCmd(app, cmd);
    android_app_post_exec_cmd(app, cmd);
}
```

顺着这个线索可以发现，该方法被记录在了 `android_app->cmdPollSource.process` 上

```cpp
static void* android_app_entry(void* param) {
    struct android_app* android_app = (struct android_app*)param;

    android_app->config = AConfiguration_new();
    AConfiguration_fromAssetManager(android_app->config, android_app->activity->assetManager);

    print_cur_config(android_app);

    android_app->cmdPollSource.id = LOOPER_ID_MAIN;
    android_app->cmdPollSource.app = android_app;
    android_app->cmdPollSource.process = process_cmd;
    android_app->inputPollSource.id = LOOPER_ID_INPUT;
    android_app->inputPollSource.app = android_app;
    android_app->inputPollSource.process = process_input;

    ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_addFd(looper, android_app->msgread, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, NULL,
            &android_app->cmdPollSource);
    ...
}
```

ALooper_addFd 定义在 frameworks/base/native/android/looper.cpp 获取了一个 looper 之后，调用了 addFd 方法

```cpp
int ALooper_addFd(ALooper* looper, int fd, int ident, int events,
        ALooper_callbackFunc callback, void* data) {
    return ALooper_to_Looper(looper)->addFd(fd, ident, events, callback, data);
}
```

```cpp
int Looper::addFd(int fd, int ident, int events, Looper_callbackFunc callback, void* data) {
    sp<SimpleLooperCallback> looperCallback;
    if (callback) {
        looperCallback = sp<SimpleLooperCallback>::make(callback);
    }
    return addFd(fd, ident, events, looperCallback, data);
}
```

最内层的 addFd 方法很长，这里选择比较关键的部分解释：

```cpp
int Looper::addFd(int fd, int ident, int events, const sp<LooperCallback>& callback, void* data) {
    ...
    { // acquire lock
        AutoMutex _l(mLock);
        // There is a sequence number reserved for the WakeEventFd.
        if (mNextRequestSeq == WAKE_EVENT_FD_SEQ) mNextRequestSeq++;
        const SequenceNumber seq = mNextRequestSeq++;

        Request request;
        request.fd = fd;
        request.ident = ident;
        request.events = events;
        request.callback = callback;
        request.data = data;
        ...
```

首先这里的 data 就是 `&android_app->cmdPollSource`，这里构建了 Requeset 对象，`request.data` 记录了 `&android_app->cmdPollSource`

```cpp
        ...
        if (seq_it == mSequenceNumberByFd.end()) {
            ...
            mRequests.emplace(seq, request);
            mSequenceNumberByFd.emplace(fd, seq);
        } else {
            ...
            mRequests.erase(oldSeq);
            mRequests.emplace(seq, request);
            seq_it->second = seq;
        }
    } // release lock
    return 1;
}
```

接着，将 request 对象加入到 mRequests 中；mRequests 是一个 Looper.h 内部的一个 Vector 对象，Vector 是 Android Native 内部实现的数据结构。
那么，我们来看看哪里用到了 mRequests；搜索后我们发现一共有两处地方遍历了 mRequests：

1. `int Looper::pollOnce(int timeoutMillis, int* outFd, int* outEvents, void** outData)`
2. `int Looper::pollInner(int timeoutMillis)`
   由于本文只对 ALooper_pollAll 进行讨论，所以这里只讨论第一种情况

```cpp
int Looper::pollOnce(int timeoutMillis, int* outFd, int* outEvents, void** outData) {
    int result = 0;
    for (;;) {
        while (mResponseIndex < mResponses.size()) {
            const Response& response = mResponses.itemAt(mResponseIndex++);
            int ident = response.request.ident;
            if (ident >= 0) {
                int fd = response.request.fd;
                int events = response.events;
                void* data = response.request.data;
#if DEBUG_POLL_AND_WAKE
                ALOGD("%p ~ pollOnce - returning signalled identifier %d: "
                        "fd=%d, events=0x%x, data=%p",
                        this, ident, fd, events, data);
#endif
                if (outFd != nullptr) *outFd = fd;
                if (outEvents != nullptr) *outEvents = events;
                if (outData != nullptr) *outData = data;
                return ident;
            }
        }
        ...
    }
}
```
假设获取到的 response 正是我们设置的 mAppCmd 的 response，当 ident 不为 0 时，outData 接受 `&android_app->cmdPollSource`，当查看 pollOnce 的调用链路时，我们发现顶端刚好就是 ALooper_pollAll

```
Looper::pollOnce <- Looper::pollAll <- ALooper_pollAll   
```
那么其实 android_poll_source.process 调用的就是 (android_app->cmdPollSource).process，而这个 process 就是 process_cmd，process_cmd 内部调用了 onAppCmd。