<p style="color: red">NOTE: 下面的结论尚未进行实践！</p>
1. socket 是什么？
   简单理解一下，操作系统把网络通信需要用到的所有能力（读，写，发送，接收，不同协议）都集成在了一个叫 `sock` 的文件，提供了一层对外接口方便调用者使用通信能力，这个对外接口就叫做 `socket`

2. 什么是 poll？
   poll 用于检查一组文件描述符负责的事件是否已经发生/是否准备好进行 I/O 操作<sup><a href="#r2">[2]</href></sup>

# Reference

1. https://developer.aliyun.com/article/1173904
2. <p id="r2">man poll in macOS</p>