# $1: dataset
# clusters faststep/louvain/LLP <dataset> <output-basename> [options]
echo "$@"
set -e
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo $4
if [ "$#" -eq 4 ]; then
	rm -f $DIR/../../$4-*
fi
java -cp $DIR/law-lib/'*' it.unimi.dsi.law.graph.LayeredLabelPropagation "$@" # -g -0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11 -> Using these gammas results in the same labelling results when clusters are passed directly
#java -cp $DIR/lib/'*' it.unimi.dsi.webgraph.Transform map $1 $2 data/$1/LLP-compressed/LLP-perm
#echo "output-basename" $2
if [ "$#" -eq 4 ]; then
	for file in $DIR/../../$4-*; do $DIR/labels2txt.sh $file; rm $file; done
else
	$DIR/labels2txt.sh $2
	mv $2.txt $2
fi
