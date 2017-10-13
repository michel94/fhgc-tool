set -e
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
java -cp $DIR/lib/'*' it.unimi.dsi.webgraph.ASCIIGraph $1 $2
$DIR/parseGraphTxt $2.graph-txt $2
