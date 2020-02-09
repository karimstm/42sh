# trap 'increment' 2 3 6 9 14 15 11

# increment()
# {
#   echo "Caught SIGNAL ..."
#   cp file "$X"
# }

# ### main script
# X=0
# while :
# do
#   echo "X=$X"
#   cat /dev/random | head -c 1000 > file && cat file | ./42sh
#   cat file | bash
#   cat file | zsh
#   X=`expr ${X} + 1`
#   echo $X
#   if [ "${X}" -gt "2000" ]
#     then
#         exit 1
#     fi
#     sleep 1
# done