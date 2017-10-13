DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
python3 $DIR/readclusters.py $DIR/data/$1.tree $1 > $DIR/labels.txt