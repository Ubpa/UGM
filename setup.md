# 安装使用说明

## 环境

- VS2019

- CMake 3.16.3 +

## 安装

- 下载源码，以下两种方式选择一种
  - git: `git clone https://github.com/Ubpa/UGM` 
  - https://github.com/Ubpa/UGM/archive/master.zip（解压）
- 打开 CMake-GUI
  - 在 `Where is the source code` 栏中填写 `<your-path-to-UGM>` 
  - 在 `Where to build the binaries` 栏中填写 `<your-path-to-UGM>/build` 
  - 点击 Configure，期间会下载 [UCMake](https://github.com/Ubpa/UCMake) 和 [UTemplate](https://github.com/Ubpa/UTemplate) 到目录 `build/_deps/`（如若下载失败，多试几次或者翻墙再试，直至成功无错）
  - 在 CMake-GUI 中间的配置框中
    - 找到 `CMAKE_INSTALL_PREFIX`（安装位置），改成其他位置（非 C 盘，因为安装到 C 盘需要管理员权限，会有很多麻烦），如 `D:/Program_Files/Ubpa`（注意一定要以 `Ubpa` 结尾），记为 `<install-path>`
    - 你还可以配置其他选项
      - `Ubpa_BuildTest_UCMake`（默认开启）：依赖库 CMake 的示例，可不构建
      - `Ubpa_BuildTest_UGM`（默认开启）：UGM 的示例，建议构建
      - `Ubpa_BuildTest_UTemplate`（默认开启）：UTemplate 的示例，可不构建
      - `Ubpa_USE_SIMD`（默认开启）：确保自己的 CPU 支持 SSE 2
      - `Ubpa_USE_SSE_4_1`（默认开启）：确保自己的 CPU 支持 SSE 4.1
  - 点击 Generate
  - 点击 Open Project，此时会**打开 VS2019** 
- 打开 VS2019 后
  - 在解决方案资源管理器中，找到 `CMakePredefinedTargets/INSTALL`，右键 -> 生成，在输出窗口中可以看到有一些文件（UCMake，UTemplate 和 UGM）安装到了 `<install-path>` 
  - 另外，你也可以运行一下示例（如，在解决方案资源管理器中，找到 `UGM/test/UGM_test_00_val`，右键->设为启动项目，然后在 VS2019 的工具栏中，点击”本地Windows调试器“，从而运行示例）

## 使用

- 配置环境变量

  - 将 `<install-path>` 加入到系统环境变量 `Path` 中（或者新建一个环境变量为 `UGM_DIR`，并将其值设置为 ` <install-path>`），从而使得 CMake 的 `find_package` 可以正确找到 `UGM` 
  - 删掉 `<your-path-to-UGM>/build`，否则 CMake 的 `find_package` 会优先定位到此处，很可能导致错误

- 项目搭建示例（代码可在 [demo/](demo/) 处找到）

  ```c++
  // main.cpp
  #include <UGM/UGM.h>
  #include <iostream>
  using namespace Ubpa;
  int main(){
      pointf3 p{0, 1, 2};
      pointf3 q{2, 1, 0};
      std::cout << p - q << std::endl;
  }
  ```

  ```CMake
  # CMakeLists.txt
  cmake_minimum_required(VERSION 3.14)
  project(demo_project VERSION 1.0.0)
  
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED True)
  
  find_package(UGM REQUIRED)
  
  add_executable(demo main.cpp)
  
  target_link_libraries(demo PUBLIC Ubpa::UGM_core)
  ```

