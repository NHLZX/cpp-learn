# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 项目说明

本目录用于 C++ 学习，包含语法练习、设计模式、算法题等内容。子目录说明：

- `cpp/` — C++ 语法练习
- `factory/` — 工厂模式、线程池实现
- `leetcode/` — LeetCode 算法题
- `shared_from_this/` — `shared_ptr` / `enable_shared_from_this` 示例
- `test/` — `std::bind` 等语法测试
- `threadid/` — 线程 ID 相关练习

## 编译方式

单文件直接编译：

```bash
g++ -std=c++17 -o output main.cpp
```

编译并运行测试（以 leetcode 为例）：

```bash
# 编译统一测试文件
g++ -std=c++17 -o leetcode/leetcode_tests leetcode/leetcode_tests.cpp && ./leetcode/leetcode_tests

# 编译单题并运行 Python 测试脚本
g++ -std=c++17 -o leetcode/leetcode128_test leetcode/leetcode128.cpp
python3 leetcode/run_tests.py

# 编译自定义分配器并运行测试
g++ -std=c++17 -o leetcode/alloctor_test leetcode/alloctor.cpp
python3 leetcode/run_alloctor_tests.py
```

## 代码规范

- 代码注释一律使用**中文**
- 变量、函数、类名使用英文命名
- 避免无必要地新建文件，优先修改已有文件

## 其他说明

- 始终用**中文**与用户交流
- 分析 `.cpp` 文件时，提供详细的功能说明
