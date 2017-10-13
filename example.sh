NAME=airports

DIR=data/$NAME
DS=$DIR/graph
./fhgc create-dataset $DIR/raw $DIR/graph

mkdir -p $DIR/clusters-LLP $DIR/clusters-louvain $DIR/clusters-faststep

./fhgc clusters louvain $DS $DIR/clusters-louvain
./fhgc clusters LLP $DS $DIR/clusters-LLP
./fhgc clusters faststep $DS $DIR/clusters-faststep

./fhgc labels louvain $DS $DIR/labels-louvain.txt
./fhgc labels LLP $DS $DIR/labels-LLP.txt
./fhgc labels faststep $DS $DIR/labels-faststep.txt

./fhgc reorder $DS $DIR/LLP-compressed $DIR/labels-LLP.txt
./fhgc reorder $DS $DIR/louvain-compressed $DIR/labels-louvain.txt
./fhgc reorder $DS $DIR/faststep-compressed $DIR/labels-faststep.txt

./fhgc size $DIR/graph $DIR/LLP-compressed $DIR/louvain-compressed $DIR/faststep-compressed
