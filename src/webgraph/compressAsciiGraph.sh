set -e
#echo "${@:2}"
./parseGraph webgraph=tmp.ascii faststep=/tmp/fs.el $3 $4 $5 $6 $7 $8 $9 < $1
cat tmp.ascii | java -cp lib/'*' it.unimi.dsi.webgraph.BVGraph -g ASCIIGraph -1 null $2
rm tmp.ascii
# java -cp lib/'*' it.unimi.dsi.webgraph.ASCIIGraph data/amazon/random-perm/amazon graph # Obtain ASCIIGraph from BVGraph
