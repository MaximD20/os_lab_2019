MAXCOUNT=$1

count=1


echo "$MAXCOUNT случайных чисел:"

echo "-----------------"
for ((i=0; i<$1; i++))


do
b=$(od -A n -t d -N 1 /dev/urandom)
echo $b >> numbers.txt

let "count += 1"

done
echo "-----------------"

