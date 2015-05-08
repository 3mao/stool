buf=`pwd`
tmp=`perl -MPOSIX  -e 'print ((uname)[1])'`;

echo "aaa:$buf $tmp"
