# $1: dataset name
# <graph-file> <labels-file> or 
# <graph-file> -c <communities-folder>

set -e
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
L=$DIR/../louvain
#echo "louvain" $1 $2
$DIR/graph2el.sh $DIR/../../$1 /tmp/graph
cd /tmp
mkdir -p /tmp/communities
rm -f /tmp/communities/*

mkdir -p /tmp/louvain
rm -rf /tmp/louvain/*
tail -n +2 /tmp/graph > /tmp/louvain/data.txt
$L/convert -i /tmp/louvain/data.txt -o /tmp/louvain/graph.bin

$L/louvain /tmp/louvain/graph.bin -l -1 -q 0 > /tmp/louvain/graph.tree 2> /tmp/plsno.txt
python2 $L/readclusters.py /tmp/louvain/graph.tree /tmp/communities/communities > /tmp/louvain/labels.txt
#java -cp lib/'*' it.unimi.dsi.webgraph.Transform map data/$1/random-perm/$1 data/$1/louvain-compressed/$1 --ascii $L/labels.txt

if [ $2 == "-c" ]; then
	for file in /tmp/communities/*; do mv $file $DIR/../../$3/; done
else
	cp /tmp/louvain/labels.txt $DIR/../../$2
fi
