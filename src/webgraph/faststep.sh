# <graph-file> <labels-file> or 
# <graph-file> -c <communities-folder>

set -e
THISDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
EXECDIR=$THISDIR/../..
echo $1
if [[ "$1" = /* ]]; then
	$THISDIR/graph2el.sh $1 /tmp/graph
else
	$THISDIR/graph2el.sh $EXECDIR/$1 /tmp/graph
fi

folder=/tmp/faststep-$(( ( RANDOM % 100000 )  + 1 ))
mkdir -p $folder/communities
cd $folder
rm -f communities/*
$EXECDIR/src/faststep/2recursive AA /tmp/graph

if [ $2 == "-c" ]; then
	for file in communities/*; do mv $file $EXECDIR/$3/; done
else
	cp communities/labels-20div.txt $EXECDIR/$2
fi
