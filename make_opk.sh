#!/bin/sh

OPK_NAME=griffon.opk
echo ${OPK_NAME}

FLIST="build/griffon.dge"
FLIST="${FLIST} build/objectdb.dat build/readme.txt"
FLIST="${FLIST} build/art build/data"
FLIST="${FLIST} build/mapdb build/music"
FLIST="${FLIST} build/sfx"
FLIST="${FLIST} build/default.gcw0.desktop"
FLIST="${FLIST} build/griffon.png"

cat > build/default.gcw0.desktop <<EOF
[Desktop Entry]
Name=Griffon Legend
Comment=Action RPG game
Exec=griffon.dge
Terminal=false
Type=Application
StartupNotify=true
Icon=griffon
Categories=games;
EOF

rm -f ${OPK_NAME}
mksquashfs ${FLIST} ${OPK_NAME}

cat default.gcw0.desktop
