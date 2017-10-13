#set -e
./transform.sh data/$1/random-perm/$1 data/2rec-results/$1 ../faststep/$1-tests/communities/labels-20div.txt		> /tmp/tmp.txt; echo -n "max comm size 20:	" && ls -s data/2rec-results/$1.graph | awk '{print $1;}';
./transform.sh data/$1/random-perm/$1 data/2rec-results/$1 ../faststep/$1-tests/communities/labels-50div.txt		> /tmp/tmp.txt; echo -n "max comm size 50:	" && ls -s data/2rec-results/$1.graph | awk '{print $1;}';
./transform.sh data/$1/random-perm/$1 data/2rec-results/$1 ../faststep/$1-tests/communities/labels-100div.txt		> /tmp/tmp.txt; echo -n "max comm size 100:	" && ls -s data/2rec-results/$1.graph | awk '{print $1;}';
./transform.sh data/$1/random-perm/$1 data/2rec-results/$1 ../faststep/$1-tests/communities/labels-200div.txt		> /tmp/tmp.txt; echo -n "max comm size 200:	" && ls -s data/2rec-results/$1.graph | awk '{print $1;}';
./transform.sh data/$1/random-perm/$1 data/2rec-results/$1 ../faststep/$1-tests/communities/labels-500div.txt		> /tmp/tmp.txt; echo -n "max comm size 500:	" && ls -s data/2rec-results/$1.graph | awk '{print $1;}';
./transform.sh data/$1/random-perm/$1 data/2rec-results/$1 ../faststep/$1-tests/communities/labels-1000div.txt		> /tmp/tmp.txt; echo -n "max comm size 1000:	" && ls -s data/2rec-results/$1.graph | awk '{print $1;}';
./transform.sh data/$1/random-perm/$1 data/2rec-results/$1 ../faststep/$1-tests/communities/labels-2000div.txt		> /tmp/tmp.txt; echo -n "max comm size 2000:	" && ls -s data/2rec-results/$1.graph | awk '{print $1;}';
./transform.sh data/$1/random-perm/$1 data/2rec-results/$1 ../faststep/$1-tests/communities/labels-5000div.txt		> /tmp/tmp.txt; echo -n "max comm size 5000:	" && ls -s data/2rec-results/$1.graph | awk '{print $1;}';
./transform.sh data/$1/random-perm/$1 data/2rec-results/$1 ../faststep/$1-tests/communities/labels-10000div.txt		> /tmp/tmp.txt; echo -n "max comm size 10000:	" && ls -s data/2rec-results/$1.graph | awk '{print $1;}';
./transform.sh data/$1/random-perm/$1 data/2rec-results/$1 ../faststep/$1-tests/communities/labels-20000div.txt		> /tmp/tmp.txt; echo -n "max comm size 20000:	" && ls -s data/2rec-results/$1.graph | awk '{print $1;}';
./transform.sh data/$1/random-perm/$1 data/2rec-results/$1 ../faststep/$1-tests/communities/labels-50000div.txt		> /tmp/tmp.txt; echo -n "max comm size 50000:	" && ls -s data/2rec-results/$1.graph | awk '{print $1;}';
./transform.sh data/$1/random-perm/$1 data/2rec-results/$1 ../faststep/$1-tests/communities/labels-100000div.txt > /tmp/tmp.txt; echo -n "max comm size 100000:	" && ls -s data/2rec-results/$1.graph | awk '{print $1;}';
echo -n "Random perm:	" && ls -s data/$1/random-perm/$1.graph | awk '{print $1;}';
echo -n "LLP perm:	" && ls -s data/$1/LLP-compressed/$1.graph | awk '{print $1;}';
echo -n "Louvain perm:	" && ls -s data/$1/louvain-compressed/$1.graph | awk '{print $1;}';
