set -e
D=$(dirname $2)
mkdir -p $D
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"/../..
cd src/webgraph
./compressAsciiGraph.sh $DIR/$1 $DIR/$2 $DIR/$2/tmp/data.el "${@:3}"
