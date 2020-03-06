t=0
count=0
if (($#>1)); then
for param in $@
do
t=$[t+param]
count=$[count+1]
done
result=$[t/count]
else
for param in $(cat $1)
do
t=$[t+param]
count=$[count+1]
done
result=$[t/count]
fi
echo "Количество чисел: $count"
echo "-----------------"
echo "Среднее арифметическое: $result"