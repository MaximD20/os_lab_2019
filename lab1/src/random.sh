#
#do
#cat $RANDOM > numbers.txt
#done
MAXCOUNT=$1

count=1

echo

echo "$MAXCOUNT случайных чисел:"

echo "-----------------"
for ((i=0; i<$1; i++))


do
b=$(od -A n -t d -N 1 /dev/urandom)
echo $b >> numbers.txt

let "count += 1" # Нарастить счетчик.

done
#cat numbers.txt | tr -s '\r\n' ' ' > numbers.txt
echo "-----------------"

