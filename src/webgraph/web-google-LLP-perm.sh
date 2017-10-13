
set -e
rm -f data/web-google/LLP-compressed/communities/*
java -cp law-lib/'*' it.unimi.dsi.law.graph.LayeredLabelPropagation data/web-google/random-perm/web-google data/web-google/LLP-compressed/LLP-perm -l data/web-google/LLP-compressed/communities/communities
java -cp lib/'*' it.unimi.dsi.webgraph.Transform map data/web-google/random-perm/web-google data/web-google/LLP-compressed/web-google data/web-google/LLP-compressed/LLP-perm
for file in data/web-google/LLP-compressed/communities/*; do ./labels2txt.sh $file; done
