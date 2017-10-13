set -e
java -cp jars/runtime/'*':build it.unimi.dsi.law.graph.LayeredLabelPropagation ../LLP/data/youtube-random-perm/youtube perm -g 2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,32 -l labels
cd ../webgraph
java -cp lib/'*' it.unimi.dsi.webgraph.Transform map data/youtube-random-perm/youtube data/youtube-faststep-compressed/youtube ../law+faststep/perm
cd ../law+faststep
