#!/bin/sh

# Copies a file at a directory 6 times, with the appropriate cube suffixes.
# Useful with ogre cube materials, skyboxes.
# Example usage: sh tools/cubify.sh data/textures/skybox.jpg
#
# Soultatos Stefanos 2022

path=$1
extension="${path##*.}"
filename="${path%.*}"

echo "cubifying texture: $filename"

cp $path $filename"_bk."$extension
cp $path $filename"_dn."$extension
cp $path $filename"_fr."$extension
cp $path $filename"_lf."$extension
cp $path $filename"_rt."$extension
cp $path $filename"_up."$extension
