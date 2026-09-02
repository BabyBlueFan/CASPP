#!/bin/bash

# 快速验证服务器和客户端是否编译成功

echo "=== Echo 系统快速验证 ==="
echo ""

# 检查可执行文件
if [ -f "cmakeBuild/echo_server" ] && [ -f "cmakeBuild/echo_client" ]; then
    echo "✅ 编译成功！"
    echo ""
    echo "生成的可执行文件："
    ls -lh cmakeBuild/echo_server cmakeBuild/echo_client
    echo ""
    echo "🚀 使用方法："
    echo ""
    echo "启动服务器："
    echo "  ./cmakeBuild/echo_server 8888"
    echo ""
    echo "启动客户端（在另一个终端）："
    echo "  ./cmakeBuild/echo_client localhost 8888"
    echo ""
    echo "然后输入任意文本，服务器会echo返回相同内容"
    echo "输入 'quit' 可以退出连接"
else
    echo "❌ 编译失败或文件不存在"
    echo "请运行: mkdir -p cmakeBuild && cd cmakeBuild && cmake .. && make"
fi