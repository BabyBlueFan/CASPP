# Echo Client - Rio (Robust I/O) Package 实现说明

这个项目实现了《深入理解计算机系统》(CS:APP) 中的 Rio (Robust I/O) 包，用于处理网络编程中的缓冲I/O操作。

## Rio 包组件

### 1. 数据结构

```c
#define RIO_BUFSIZE 8192

typedef struct {
    int rio_fd;                /* 文件描述符 */
    int rio_cnt;               /* 缓冲区中剩余的未读字节数 */
    char *rio_bufptr;          /* 指向缓冲区中下一个未读字节 */
    char rio_buf[RIO_BUFSIZE]; /* 内部缓冲区 */
} rio_t;
```

### 2. 主要函数

#### `rio_readinitb`
```c
void rio_readinitb(rio_t *rp, int fd);
```
- **功能**: 初始化rio_t结构体，将其与文件描述符关联
- **参数**: 
  - `rp`: 指向rio_t结构体的指针
  - `fd`: 文件描述符
- **使用**: 在使用其他rio函数之前必须先调用此函数

#### `rio_readlineb`
```c
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
```
- **功能**: 从文件中读取一行文本（带缓冲）
- **参数**:
  - `rp`: 指向rio_t结构体的指针
  - `usrbuf`: 用户缓冲区，存储读取的数据
  - `maxlen`: 缓冲区的最大长度
- **返回值**: 读取的字节数（包括换行符），0表示EOF，-1表示错误
- **特点**: 自动处理部分读取，确保读取完整的一行

#### `rio_readnb`
```c
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);
```
- **功能**: 精确读取n个字节（带缓冲）
- **参数**:
  - `rp`: 指向rio_t结构体的指针
  - `usrbuf`: 用户缓冲区
  - `n`: 要读取的字节数
- **返回值**: 实际读取的字节数，-1表示错误
- **特点**: 确保读取指定数量的字节（除非遇到EOF或错误）

#### `rio_writen`
```c
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
```
- **功能**: 鲁棒地写入n个字节（无缓冲）
- **参数**:
  - `fd`: 文件描述符
  - `usrbuf`: 要写入的数据
  - `n`: 要写入的字节数
- **返回值**: 写入的字节数，-1表示错误
- **特点**: 确保所有字节都被写入（处理部分写入情况）

## 使用示例

### 基本使用流程

```c
#include "csapp.h"

int main() {
    int clientfd;
    rio_t rio;
    char buf[MAXLINE];
    
    // 1. 打开客户端连接
    clientfd = open_clientfd("localhost", "8080");
    
    // 2. 初始化rio
    rio_readinitb(&rio, clientfd);
    
    // 3. 读取一行
    ssize_t n = rio_readlineb(&rio, buf, MAXLINE);
    if (n > 0) {
        printf("Read: %s", buf);
    }
    
    // 4. 写入数据
    const char *message = "Hello, Server!\n";
    rio_writen(clientfd, (void*)message, strlen(message));
    
    // 5. 关闭连接
    close(clientfd);
    return 0;
}
```

### 编译和运行

```bash
# 编译
cd cmakeBuild
cmake ..
make

# 运行
./echo_client localhost 8080
```

## Rio 包的优势

1. **缓冲I/O**: 减少系统调用次数，提高效率
2. **处理部分读写**: 网络编程中read()和write()可能只处理部分数据，rio函数自动处理这种情况
3. **简单易用**: 提供高级接口，隐藏底层细节
4. **鲁棒性**: 正确处理错误和边缘情况（如EINTR信号中断）

## 实现细节

- **内部缓冲**: rio_t包含8KB的内部缓冲区
- **懒加载**: 只有在缓冲区为空时才会调用系统read()填充缓冲区
- **内存高效**: 内部缓冲区和用户缓冲区分离，避免不必要的内存拷贝

## 文件结构

```
echo_client/
├── include/
│   └── csapp.h          # Rio包的函数声明和类型定义
├── src/
│   └── csapp.c          # Rio包的函数实现
├── main.c               # Echo客户端示例
└── CMakeLists.txt       # CMake构建配置
```

这个实现完全遵循CS:APP教材中的设计，是学习网络编程和系统级I/O的优秀示例。