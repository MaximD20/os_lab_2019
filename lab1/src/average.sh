t=0
count=0
for param in $(cat $1)
do
t=$[t+param]
count=$[count+1]
done
result=$[t/count]
echo "Количество чисел: $count"
echo "-----------------"
echo "Среднее арифметическое: $result"
