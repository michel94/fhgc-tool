
set -e
../louvain/louvain ../louvain/data/youtube.bin -l -1 -q 0 > ../louvain/data/youtube.tree
../louvain/generateLabels.sh youtube
java -cp lib/'*' it.unimi.dsi.webgraph.Transform map data/youtube/random-perm/youtube data/youtube/louvain-compressed/youtube --ascii ../louvain/labels.txt
