[settings]
arch=x86_64
build_type=Release
compiler=apple-clang
compiler.cppstd=17
compiler.libcxx=libc++
compiler.version=14
os=Macos

[conf]
tools.cmake.cmaketoolchain:generator=Ninja

[options]
peakeater/*:signed=True