set -e
java -cp jars/runtime/'*':build it.unimi.dsi.law.graph.LayeredLabelPropagation ../LLP/data/youtube-random-perm/youtube perm -g 5000div -l labels
cd ../webgraph
java -cp lib/'*' it.unimi.dsi.webgraph.Transform map data/youtube-random-perm/youtube data/youtube-2rec/youtube ../law+faststep/perm
cd ../law+faststep
