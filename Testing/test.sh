trap 'increment' 3 6 9 14 15 11
increment()
{
  echo "Caught SIGNAL ..."
  cp file "Crach_$VAR_$(uuidgen)"
}
### main script
VAR=0
while :
do
  echo "================ TEST NUMBER = $VAR =================="
  cat /dev/random | head -c 1000 > file && cat file | $1
  ## you can close stdin and stderr to supress output to the terminal
  VAR=`expr ${VAR} + 1`
  if [ "${VAR}" -gt "100" ]
      then
      exit 1
    fi
    sleep 1
done
