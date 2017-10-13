
set -e
rm -f data/youtube/LLP-compressed/communities/*
java -cp law-lib/'*' it.unimi.dsi.law.graph.LayeredLabelPropagation data/youtube/random-perm/youtube data/youtube/LLP-compressed/LLP-perm -l data/youtube/LLP-compressed/communities/communities
java -cp lib/'*' it.unimi.dsi.webgraph.Transform map data/youtube/random-perm/youtube data/youtube/LLP-compressed/youtube data/youtube/LLP-compressed/LLP-perm
for file in data/youtube/LLP-compressed/communities/*; do ./labels2txt.sh $file; done
