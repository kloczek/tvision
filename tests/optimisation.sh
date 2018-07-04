#!/bin/bash

if [ -e ../Makefile ]; then
	make -C .. distclean
fi
if [ ! -e ../configure ]; then
	(cd ..; ./bootstrap.sh)
fi

timestamp="$(date +%F-%T)"

CXXFLAGS_O2="-O2"
CXXFLAGS_O2_lto="-O2 -flto"
CXXFLAGS_Os="-Os"
CXXFLAGS_Os_lto="-Os -flto"
CXXFLAGS_max_opt="-Os -flto -fno-exceptions -fno-rtti"

LDFLAGS_O2="-Wl,--as-needed"
LDFLAGS_O2_lto="-O2 -Wl,--as-needed -flto -fuse-linker-plugin"
LDFLAGS_Os="-Wl,--as-needed"
LDFLAGS_Os_lto="-Os -Wl,--as-needed -flto -fuse-linker-plugin"
LDFLAGS_max_opt="-Os -Wl,--as-needed -flto -fuse-linker-plugin"

all_targets="O2 O2_lto Os Os_lto max_opt"

> optimisation.log

rm -rf $all_targets {demo,background,listbox,load,nomenus,splash,tvedit,tvguid*,tvlife,validator,tvhc,libtvision.so.0.0*}
for i in $all_targets; do
	mkdir $i
	cd $i
	echo "$i configure"
	eval LDFLAGS="$(echo \$LDFLAGS_${i})"
	eval CXXFLAGS="$(echo \$CXXFLAGS_${i})"
	LDFLAGS="$LDFLAGS" \
	CXXFLAGS="$CXXFLAGS" \
	AR="gcc-ar" RANLIB="gcc-ranlib" NM="gcc-nm" \
	../../configure --quiet --enable-maintainer-mode
	echo "$i make"
	make -j20 >/dev/null 2>&1
	echo "------------------------------------------------------" >> ../optimisation.log
	echo LDFLAGS=\""$LDFLAGS"\" >> ../optimisation.log
	echo CXXFLAGS=\""$CXXFLAGS"\" >> ../optimisation.log
	echo $(gcc --version| grep GCC) >>../optimisation.log
	echo "------------------------------------------------------" >> ../optimisation.log
	cd ..
	cp $i/*/.libs/{demo,background,listbox,load,nomenus,splash,tvedit,tvguid*,tvlife,validator,tvhc,libtvision.so.0.0*} .
	size {demo,background,listbox,load,nomenus,splash,tvedit,tvguid*,tvlife,validator,tvhc,libtvision.so.0.0*} >> optimisation.log
	for i in {1..6}; do echo >> optimisation.log; done
	rm -rf {demo,background,listbox,load,nomenus,splash,tvedit,tvguid*,tvlife,validator,tvhc,libtvision.so.0.0*}
done
