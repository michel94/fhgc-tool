# $1: <dataset name>, data will be at data/<dataset name>
echo "Creating Dataset" $1
set -e
mkdir -p data/$1/orig data/$1/random-perm data/$1/faststep-compressed data/$1/LLP-compressed data/$1/LLP-compressed/communities data/$1/louvain-compressed ../louvain/data/
./compressAsciiGraph.sh data/$1/orig/data data/$1/orig/$1 data/$1/orig/data.el $2 $3 $4 $5
./compressAsciiGraph.sh data/$1/orig/data data/$1/random-perm/$1 data/$1/random-perm/data.el --shuffle $2 $3 $4 $5
cp indexes.txt data/$1/random-perm/indexes.txt
echo "./LLP-perm.sh" $1 > $1-LLP-perm.sh

tail -n +2 data/$1/random-perm/data.el > ../louvain/data.txt
../louvain/convert -i ../louvain/data.txt -o ../louvain/data/$1.bin
rm ../louvain/data.txt

echo "./louvain-perm.sh $1" > $1-louvain-perm.sh

mkdir -p ../faststep/$1-tests/communities
mkdir -p ../louvain/$1-tests/communities

recScript="../faststep/$1-tests/$1-test.sh"
echo ../2recursive AA ../../webgraph/data/$1/random-perm/data.el > $recScript

chmod +x $1-LLP-perm.sh $1-louvain-perm.sh $recScript 
echo "Created scripts:" $1-LLP-perm.sh $1-louvain-perm.sh $recScript
