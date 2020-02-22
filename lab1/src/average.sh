t=0
for param in "$@"
do
echo $param
t=$[t+param]
done
result=$[t/$#]
echo $#
echo $result
