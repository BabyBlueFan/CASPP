# Echo 服务器使用说明

## 项目概览

这个项目现在包含两个可执行程序：

- **echo_client**: Echo 客户端程序
- **echo_server**: Echo 服务器程序

## 编译完成

✅ **编译成功！** 两个可执行程序已生成：
- `cmakeBuild/echo_client` (34KB)
- `cmakeBuild/echo_server` (35KB)

## 使用方法

### 1. 启动服务器

```bash
# 启动 echo 服务器，监听 8888 端口
./cmakeBuild/echo_server 8888
```

**服务器输出：**
```
Echo Server started on port 8888
Waiting for connections...
```

### 2. 启动客户端

在另一个终端窗口中：

```bash
# 连接到本地服务器的 8888 端口
./cmakeBuild/echo_client localhost 8888
```

**客户端输出：**
```
Connected to localhost:8888
Enter text to send (Ctrl+D to exit):
```

## 功能特点

### 服务器特性
- ✅ **并发处理**: 使用多进程处理多个客户端
- ✅ **自动清理**: 防止僵尸进程
- ✅ **Echo 服务**: 将接收到的数据原样返回
- ✅ **连接信息**: 显示客户端连接和断开信息
- ✅ **退出命令**: 输入 "quit" 可以优雅断开连接

### 客户端特性
- ✅ **交互式输入**: 从标准输入读取用户输入
- ✅ **实时响应**: 显示服务器返回的数据
- ✅ **鲁棒I/O**: 使用 Rio 包处理网络I/O

## 测试示例

### 基本测试

**服务器端：**
```bash
./cmakeBuild/echo_server 8888
```

**客户端：**
```bash
./cmakeBuild/echo_client localhost 8888
```

**输入测试：**
```
Enter text to send (Ctrl+D to exit):
Hello, Server!
Server response: Hello, Server!

How are you?
Server response: How are you?

quit
Client requested to quit
Connection with localhost:8888 closed
```

### 并发测试

**启动多个客户端连接同一个服务器：**

```bash
# 终端1
./cmakeBuild/echo_client localhost 8888

# 终端2  
./cmakeBuild/echo_client localhost 8888

# 终端3
./cmakeBuild/echo_client localhost 8888
```

**服务器端会显示：**
```
Connected to localhost:8888
Connected to localhost:8888
Connected to localhost:8888
```

## 技术实现

### 服务器架构
```
主进程 (监听socket)
  ↓
  accept() 接受连接
  ↓
  fork() 创建子进程
  ↓
  子进程处理 echo 服务
  ↓
  close() 关闭连接
```

### 数据流
```
客户端 → 发送数据 → 网络 → 服务器接收
  ↑                        ↓
  ←────────────────────────←
  服务器返回 echo 响应
```

## 代码结构

### 主要文件
- `main.c` - 客户端主程序
- `mainServer.c` - 服务器主程序  
- `src/csapp.c` - Rio 包和Socket函数实现
- `include/csapp.h` - 头文件声明

### 关键函数

**服务器端：**
- `main()` - 主循环，接受连接
- `echo()` - Echo 服务实现
- `sigchld_handler()` - 信号处理，避免僵尸进程

**客户端：**
- `main()` - 连接服务器，处理用户I/O
- 使用 Rio 包进行缓冲I/O

## 故障排除

### 端口被占用
```bash
# 检查端口是否被占用
lsof -i :8888

# 更换端口
./cmakeBuild/echo_server 9999
```

### 连接失败
```bash
# 检查服务器是否启动
ps aux | grep echo_server

# 检查防火墙设置
# 确保端口未被阻止
```

### 编译问题
```bash
# 清理并重新编译
rm -rf cmakeBuild
mkdir cmakeBuild && cd cmakeBuild && cmake .. && make
```

## 扩展功能建议

1. **日志记录**: 添加详细的服务器日志
2. **配置文件**: 支持配置端口和其他参数
3. **客户端命令**: 添加更多客户端命令（如 stats、help）
4. **性能监控**: 添加连接数统计和性能指标
5. **安全增强**: 添加身份验证和加密

## 学习要点

通过这个项目，你将学习到：

1. ✅ **Socket 编程**: TCP 客户端/服务器实现
2. ✅ **并发处理**: 多进程服务器设计
3. ✅ **信号处理**: SIGCHLD 处理避免僵尸进程
4. ✅ **鲁棒I/O**: Rio 包的使用和实现
5. ✅ **网络协议**: TCP 连接建立和终止
6. ✅ **系统调用**: socket、bind、listen、accept、fork 等

这个项目是学习《深入理解计算机系统》网络编程章节的绝佳实践！