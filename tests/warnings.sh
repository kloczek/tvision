#!/usr/bin/sh

dnf install -y gpm-devel ncurses-devel doxygen gcc-c++ clang llvm

if [ -e ../Makefile ]; then
	make -C .. distclean
fi
if [ ! -e ../configure ]; then
	(cd ..; autoreconf -fiv)
fi

rm -rf warnings-*
mkdir warnings-{gcc,clang}

cd warnings-clang

LDFLAGS="-Wl,--as-needed -flto -fuse-linker-plugin" \
CXXFLAGS="-O -Weverything -Wno-c++98-compat -fno-caret-diagnostics" \
AR=/usr/bin/llvm-ar RANLIB=/usr/bin/llvm-ranlib NM=/usr/bin/llvm-nm \
CXX=/usr/bin/clang++ \
../../configure --enable-maintainer-mode
echo make
make >/dev/null 2> >(tee ../warnings-clang.err >&2)

cd -
cd warnings-gcc

LDFLAGS="-Wl,--as-needed -flto -fuse-linker-plugin" \
CXXFLAGS="-Wall -Wextra -fno-diagnostics-show-caret -Os -Wp,-D_FORTIFY_SOURCE=2 -fdiagnostics-show-option -Wcomment -Wfloat-conversion -Wfloat-equal -Wformat -Wformat-security -Wformat-signedness -Wimplicit-fallthrough -Wignored-qualifiers -Wlogical-not-parentheses -Wmaybe-uninitialized -Wmissing-braces -Wmissing-declarations -Wmissing-field-initializers -Wmissing-format-attribute -Wno-long-long -Wno-unreachable-code -Wparentheses -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point -Wshadow -Wsign-compare -Wswitch -Wswitch-default -Wtype-limits -Wundef -Wuninitialized -Wunreachable-code -Wunused-but-set-variable -Wunused-function -Wunused-label -Wunused-parameter -Wunused-result -Wunused-value -Wunused-variable -Wwrite-strings" \
AR="/usr/bin/gcc-ar" RANLIB="/usr/bin/gcc-ranlib" NM="/usr/bin/gcc-nm" \
../../configure --enable-maintainer-mode
echo make
make >/dev/null 2> >(tee ../warnings-gcc.err >&2)

cd -

grep -- -W warnings-gcc.err | sed 's/.*\[//; s/\]//' | sort | uniq -c | sort -nr > warnings-gcc.log
echo >> warnings-gcc.log
echo "----------------------------------------------------------------" >> warnings-gcc.log
cat warnings-gcc.err >> warnings-gcc.log

grep -- -W warnings-clang.err | sed 's/.*\[//; s/\]//' | sort | uniq -c | sort -nr > warnings-clang.log
echo >> warnings-clang.log
echo "----------------------------------------------------------------" >> warnings-clang.log
cat warnings-clang.err >> warnings-clang.log

rm -rf warnings-*.err

(cd warnings-gcc/doc; doxygen 2>&1) | sed "s,$(cd ..; pwd),," >warnings-doxygen.log
