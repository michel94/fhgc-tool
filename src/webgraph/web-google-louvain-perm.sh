
set -e
../louvain/louvain ../louvain/data/web-google.bin -l -1 -q 0 > ../louvain/data/web-google.tree
../louvain/generateLabels.sh web-google
java -cp lib/'*' it.unimi.dsi.webgraph.Transform map data/web-google/random-perm/web-google data/web-google/louvain-compressed/web-google --ascii ../louvain/labels.txt
