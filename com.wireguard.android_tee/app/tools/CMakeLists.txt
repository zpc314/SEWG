# SPDX-License-Identifier: Apache-2.0
#
# Copyright © 2018 WireGuard LLC. All Rights Reserved.

cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

# Work around https://github.com/android-ndk/ndk/issues/602
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")

add_executable(libwg-quick.so wireguard/src/tools/wg-quick/android.c ndk-compat/compat.c)
target_compile_options(libwg-quick.so PUBLIC -O3 -std=gnu11 -Wall -pedantic -Wno-missing-field-initializers -include ${CMAKE_CURRENT_SOURCE_DIR}/ndk-compat/compat.h -DWG_PACKAGE_NAME=\"${ANDROID_PACKAGE_NAME}\")

file(GLOB WG_SOURCES wireguard/src/tools/*.c libmnl/src/*.c ndk-compat/compat.c)
add_executable(libwg.so ${WG_SOURCES})
target_compile_options(libwg.so PUBLIC -idirafter "${CMAKE_CURRENT_SOURCE_DIR}/libmnl/include/" -I "${CMAKE_CURRENT_SOURCE_DIR}/wireguard/src/tools/" -O3 -std=gnu11 -D_GNU_SOURCE -include ${CMAKE_CURRENT_SOURCE_DIR}/ndk-compat/compat.h -DHAVE_VISIBILITY_HIDDEN -DRUNSTATEDIR=\"/data/data/${ANDROID_PACKAGE_NAME}/cache\" -Wno-pointer-arith -Wno-unused-parameter)

add_custom_target(libwg-go.so WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/libwg-go" COMMENT "Building wireguard-go" VERBATIM COMMAND make
    ANDROID_ARCH_NAME=arm64
    ANDROID_C_COMPILER=/home/zpc/Android/Sdk/ndk-bundle/toolchains/llvm/prebuilt/linux-x86_64/bin/clang
    ANDROID_TOOLCHAIN_ROOT=/home/zpc/Android/Sdk/ndk-bundle/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64
    ANDROID_LLVM_TRIPLE=aarch64-none-linux-android21
    ANDROID_SYSROOT=/home/zpc/Android/Sdk/ndk-bundle/sysroot
    ANDROID_PACKAGE_NAME=com.wireguard.android.debug
    "CFLAGS=-isystem /home/zpc/Android/Sdk/ndk-bundle/sysroot/usr/include/aarch64-linux-android -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -Wa,--noexecstack -Wformat -Werror=format-security  -Wno-unused-command-line-argument"
    "LDFLAGS=-Wl,--exclude-libs,libgcc.a -Wl,--exclude-libs,libatomic.a -nostdlib++ --sysroot /home/zpc/Android/Sdk/ndk-bundle/platforms/android-21/arch-arm64 -Wl,--build-id -Wl,--warn-shared-textrel -Wl,--fatal-warnings -L/home/zpc/Android/Sdk/ndk-bundle/sources/cxx-stl/llvm-libc++/libs/arm64-v8a -Wl,--no-undefined -Wl,-z,noexecstack -Qunused-arguments -Wl,-z,relro -Wl,-z,now  -fuse-ld=gold"
    DESTDIR=/home/zpc/wireguard/com.wireguard.android_0117/app/build/intermediates/cmake/debug/obj/arm64-v8a
    BUILDDIR=/home/zpc/wireguard/com.wireguard.android_0117/app/build/intermediates/cmake/debug/obj/arm64-v8a/../generated-src
)
# Hack to make it actually build as part of the default target
add_dependencies(libwg.so libwg-go.so)

#ANDROID_ARCH_NAME=arm64
#ANDROID_C_COMPILER=/home/zpc/Android/Sdk/ndk-bundle/toolchains/llvm/prebuilt/linux-x86_64/bin/clang
#ANDROID_TOOLCHAIN_ROOT=/home/zpc/Android/Sdk/ndk-bundle/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64
#ANDROID_LLVM_TRIPLE=aarch64-none-linux-android21
#ANDROID_SYSROOT=/home/zpc/Android/Sdk/ndk-bundle/sysroot
#ANDROID_PACKAGE_NAME=com.wireguard.android.debug
#"CFLAGS=-isystem /home/zpc/Android/Sdk/ndk-bundle/sysroot/usr/include/aarch64-linux-android -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -Wa,--noexecstack -Wformat -Werror=format-security  -Wno-unused-command-line-argument"
#"LDFLAGS=-Wl,--exclude-libs,libgcc.a -Wl,--exclude-libs,libatomic.a -nostdlib++ --sysroot /home/zpc/Android/Sdk/ndk-bundle/platforms/android-21/arch-arm64 -Wl,--build-id -Wl,--warn-shared-textrel -Wl,--fatal-warnings -L/home/zpc/Android/Sdk/ndk-bundle/sources/cxx-stl/llvm-libc++/libs/arm64-v8a -Wl,--no-undefined -Wl,-z,noexecstack -Qunused-arguments -Wl,-z,relro -Wl,-z,now  -fuse-ld=gold"
#DESTDIR=/home/zpc/wireguard/com.wireguard_old.android_0114/app/build/intermediates/cmake/debug/obj/arm64-v8a
#BUILDDIR=/home/zpc/wireguard/com.wireguard_old.android_0114/app/build/intermediates/cmake/debug/obj/arm64-v8a/../generated-src
#DESC = Building wireguard-go