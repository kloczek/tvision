#!/bin/bash

if [ -e ../Makefile ]; then
	make -C .. distclean
fi

rm -rf warnings
mkdir warnings
cd warnings
LDFLAGS="-Wl,--as-needed -flto -fuse-linker-plugin" \
CXXFLAGS="-Wall" \
AR="gcc-ar" RANLIB="gcc-ranlib" NM="gcc-nm" \
../../configure --quiet --enable-maintainer-mode
echo make
make >/dev/null 2> >(tee ../warnings.err >&2)
cd ..
grep -- -W warnings.err | sed 's/.*\[//; s/\]//' | sort | uniq -c | sort -nr > warnings.log
echo >> warnings.log
echo "----------------------------------------------------------------" >> warnings.log
cat warnings.err >> warnings.log
rm -rf warnings warnings.err
