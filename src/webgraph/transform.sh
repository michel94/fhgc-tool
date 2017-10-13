set -e
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
java -cp $DIR/lib/'*' it.unimi.dsi.webgraph.Transform map $1 $2 --ascii $3