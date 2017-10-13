# $1: dataset name
set -e
rm -f data/$1/LLP-compressed/communities/*
java -cp law-lib/'*' it.unimi.dsi.law.graph.LayeredLabelPropagation data/$1/random-perm/$1 data/$1/LLP-compressed/LLP-perm -l data/$1/LLP-compressed/communities/communities
java -cp lib/'*' it.unimi.dsi.webgraph.Transform map data/$1/random-perm/$1 data/$1/LLP-compressed/$1 data/$1/LLP-compressed/LLP-perm
for file in data/$1/LLP-compressed/communities/*; do ./labels2txt.sh $file; done
