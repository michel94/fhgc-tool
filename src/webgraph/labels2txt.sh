DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
java -cp $DIR/law-lib/'*' it.unimi.dsi.law.io.tool.DataInput2Text $1 $1.txt -t int