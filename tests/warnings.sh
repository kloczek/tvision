#!/bin/bash

if [ -e ../Makefile ]; then
	make -C .. distclean
fi
if [ ! -e ../configure ]; then
	(cd ..; ./bootstrap.sh)
fi

rm -rf warnings
mkdir warnings
cd warnings
LDFLAGS="-Wl,--as-needed -flto -fuse-linker-plugin" \
CXXFLAGS="-Wall -Wextra -fno-diagnostics-show-caret -Os -Wp,-D_FORTIFY_SOURCE=2" \
AR="gcc-ar" RANLIB="gcc-ranlib" NM="gcc-nm" \
../../configure --quiet --enable-maintainer-mode
echo make
make >/dev/null 2> >(tee ../warnings.err >&2)
cd ..
grep -- -W warnings.err | sed 's/.*\[//; s/\]//' | sort | uniq -c | sort -nr > warnings.log
echo >> warnings.log
echo "----------------------------------------------------------------" >> warnings.log
cat warnings.err >> warnings.log
rm -rf warnings.err
