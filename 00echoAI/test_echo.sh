#!/bin/bash

# Echo 客户端/服务器测试脚本

echo "=== Echo 系统测试脚本 ==="
echo ""

# 检查可执行文件是否存在
if [ ! -f "cmakeBuild/echo_server" ] || [ ! -f "cmakeBuild/echo_client" ]; then
    echo "❌ 可执行文件不存在，请先编译项目"
    echo "运行: mkdir -p cmakeBuild && cd cmakeBuild && cmake .. && make"
    exit 1
fi

echo "✅ 找到可执行文件"
echo ""

# 获取测试端口（默认 8888）
PORT=${1:-8888}
echo "🚀 启动测试，端口: $PORT"
echo ""

# 启动服务器（后台运行）
echo "📡 启动 echo 服务器..."
./cmakeBuild/echo_server $PORT > /tmp/echo_server_$$.log 2>&1 &
SERVER_PID=$!
echo "✅ 服务器启动 (PID: $SERVER_PID)"
echo ""

# 等待服务器启动
sleep 1

# 检查服务器是否成功启动
if ! kill -0 $SERVER_PID 2>/dev/null; then
    echo "❌ 服务器启动失败"
    cat /tmp/echo_server_$$.log
    exit 1
fi

echo "✅ 服务器运行正常"
echo ""

# 测试连接
echo "🧪 测试1: 基本连接测试"
echo "Hello, Server!" | timeout 5 ./cmakeBuild/echo_client localhost $PORT > /tmp/echo_client_$$.log 2>&1 &
CLIENT_PID=$!

# 等待客户端完成
sleep 2

# 检查客户端输出
if grep -q "Hello, Server!" /tmp/echo_client_$$.log; then
    echo "✅ 基本连接测试通过"
else
    echo "❌ 基本连接测试失败"
    cat /tmp/echo_client_$$.log
fi

echo ""
echo "🧪 测试2: 多行数据测试"
echo -e "Line 1\nLine 2\nLine 3" | timeout 5 ./cmakeBuild/echo_client localhost $PORT > /tmp/echo_client_$$.log 2>&1 &
CLIENT_PID=$!

sleep 2

if grep -q "Line 1" /tmp/echo_client_$$.log && grep -q "Line 2" /tmp/echo_client_$$.log; then
    echo "✅ 多行数据测试通过"
else
    echo "❌ 多行数据测试失败"
    cat /tmp/echo_client_$$.log
fi

echo ""
echo "🧪 测试3: 退出命令测试"
echo "quit" | timeout 5 ./cmakeBuild/echo_client localhost $PORT > /tmp/echo_client_$$.log 2>&1 &
CLIENT_PID=$!

sleep 2

if grep -q "quit" /tmp/echo_client_$$.log; then
    echo "✅ 退出命令测试通过"
else
    echo "❌ 退出命令测试失败"
fi

echo ""
echo "📊 服务器日志："
echo "-------------------"
cat /tmp/echo_server_$$.log
echo "-------------------"

# 清理
echo ""
echo "🧹 清理进程..."
kill $SERVER_PID 2>/dev/null
rm -f /tmp/echo_server_$$.log /tmp/echo_client_$$.log

echo "✅ 测试完成"
echo ""
echo "📝 手动测试方法："
echo "1. 启动服务器: ./cmakeBuild/echo_server $PORT"
echo "2. 启动客户端: ./cmakeBuild/echo_client localhost $PORT"
echo "3. 在客户端输入文本，查看 echo 响应"
echo "4. 输入 'quit' 或 Ctrl+D 退出"