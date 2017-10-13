# $1: dataset name
set -e
../louvain/louvain ../louvain/data/$1.bin -l -1 -q 0 > ../louvain/data/$1.tree
../louvain/generateLabels.sh $1
java -cp lib/'*' it.unimi.dsi.webgraph.Transform map data/$1/random-perm/$1 data/$1/louvain-compressed/$1 --ascii ../louvain/labels.txt
