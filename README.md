# fhgc-tool
A tool for hierarchical graph clustering and succint representation of graphs, supporting three different methods.

Check example.sh for some examples of usage.

|    Command    | Arguments |
|---------------|----|
|**create-dataset**|<**raw-data**> <**graph-out**> **options**|
|   |*creates a Webgraph graph, with .graph, .properties and .offsets files, given the data file and the output base file* <br> <**raw-data**>: path to a file containing one line $r_i$ per edge. Each line contains two space separated integers $r_{i,0}$ and $r_{i,1}$, the endpoints of edge $i$ of the graph. By default, the edges are interpreted as undirected and the numbering of the vertices starts at 1. <br> <**graph-out**>: base path to the webgraph files: .graph, .properties and .offsets. <br> Options: <br> **--sub0**: vertex numbering starts at 0 <br> **--directed**: edge ($r_{i,0}$, $r_{i,1}$) is interpreted as a directed edge from $r_{i,0}$ to $r_{i,1}$ |
| **clusters**           | **faststep**/**louvain**/**LLP** <**graph**> <**output-folder**> |
| |*generates the clustering for a graph using some method*  <br> **faststep/louvain/LLP**: available methods for clustering <br> <**graph**>: graph base path <br> <**output-folder**>: output folder where communities file will be written |
| **labels**           | **faststep**/**louvain**/**LLP** <**graph**> <**output-file**> |
| |*generates the reordering for a graph using some method* <br> **faststep/louvain/LLP**: available methods for reordering. <br> <**graph**>: graph base path. <br> <**output-file**>: output file where the reordering will be written. |
| **clusterings2labels** | <**graph**> <**clusterings-prefix**> <**output-file**> |
| |*generates the reordering for a graph using a set of clusterings (which may or may not be hierarchical), provided externally* <br> <**graph**>: graph base path <br> <**clusterings-prefix**>: the prefix of the files containing the clusterings. Each clustering file provided must have a $n$ lines, where each line contains a single integer $c_i$, the community node $i$ belongs to. <br> <**output-file**>: output file where the reordering will be written. |
| **reorder** | <**graph-in**> <**graph-out**> <**indexes**> |
| |*reorders a graph using the provided reordering file* <br> <**graph-in**>: input graph base path. <br> <**graph-out**>: output graph base path. <br> <**indexes**>: output file where the reordering will be written. |
| **compare-clusters** | <**cluster-file-1**> <**cluster-file-2**> [Options] |
| |*compares clusterings using a metric: either Jaccard distance or NMI (default metric)*  <br> <**cluster-file-1**>: clustering file path. The clustering file provided must have a $n$ lines, where each line contains a single integer $c_i$, the community node $i$ belongs to   <br> <**cluster-file-2**>: output graph base path. Same file format.  <br> Options: <br> **--nmi**: Uses NMI as clustering metric <br> **--jaccard**: Uses Jaccard Distance as clustering metric |
| **compare-clusters-list** | <**cluster-file-1**> <**cluster-file-2**> [Options] |
| | *compares a set of clusterings with another set of clusterings, using a metric: either Jaccard distance or NMI (default metric)* <br> **regex-clusters-1**: multiple clustering files. This argument supports regex (using the same syntax of the command line tool "ls"), which must be defined between quotation marks. Each clustering file provided must have a $n$ lines, where each line contains a single integer $c_i$, the community node $i$ belongs to  <br> **regex-clusters-2**: multiple clustering files. Same file format. <br> Options: <br> **--nmi**: Uses NMI as clustering metric <br> **--jaccard**: Uses Jaccard Distance as clustering metric |
| **size** | <**graph**> <**more graphs**> |
| | *Returns the size of one or more graph files* <br> <**graph**>: graph base path <br> <**more graphs**>: base paths of more graph files |
| **help** | |
| | *Show usage information* |
