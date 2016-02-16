#! /bin/sh

basepath=$(cd `dirname $0`; pwd)

for a in `ls -1 $basepath/*.h `
do
    prev_word=""
    prev_word2=""
    for b in `cat $a `
    do
        if ( [ "$prev_word" = "class" ] )
        then
            if ( [ "$prev_word2" = "MCD_EXPORT" ] )
            then
                rm -f $basepath/$b
                echo "#include \"`basename $a`\"" >> $basepath/$b
            fi
        fi
        prev_word=$prev_word2
        prev_word2=$b
    done
done
