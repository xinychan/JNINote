# 最低支持的CMake版本
cmake_minimum_required(VERSION 3.4.1)

# TODO 001--导入 FMOD 模块的头文件
include_directories("inc")

# 批量导入所有源文件
file(GLOB allcpp *.c *.h *.cpp)

add_library(
        native-lib # 生成 libnative-lib.so
        SHARED # SHARED==动态库，生成xxx.so；STATIC==静态库，生成xxx.a
        # native-lib.cpp # 引入单个源文件
        ${allcpp}
)

# TODO 002--导入 FMOD 模块的库文件(xxx.so)
# CMAKE_SOURCE_DIR == CMakeLists.txt 当前所在路径
# CMAKE_ANDROID_ARCH_ABI == 自动识别当前设备CPU架构
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}")
# 硬编码导入库文件路径 /jniLibs/x86
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/x86")

# 在NDK路径中寻找 xxx/xxx/liblog.so
# 找到 liblog.so 路径后，赋值给 log-lib 变量
find_library(
        log-lib
        log)

# TODO 003--把所需的库文件链接到 libnative-lib.so
target_link_libraries(
        native-lib
        ${log-lib} # 把 log-lib 变量对应的so库，链接到 libnative-lib.so
        fmod # 寻找到 fmod.so 库，链接到 libnative-lib.so
        fmodL # 寻找到 fmodL.so 库，链接到 libnative-lib.so
)