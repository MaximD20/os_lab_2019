t=0
count=0
while read line
do
let "t = t + line" 
let "count = count + 1"
done
let "result = t / count"
echo "Количество чисел: $count"
echo "-----------------"
echo "Среднее арифметическое: $result"