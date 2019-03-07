#!/bin/bash
#Ce script va permettre de trouver le nom des variables générées grace
#à un scanf

fileMatcher="@"
lineMatcher=":"
fctMatcher="i"
varMatcher="v"
scanfmatcher="s"
fopenmatcher="f"
fopenvarmatcher="t"

#Si scanf 0, si fopen 1
fct=-1

eg="="
count=0
printed=0
tokenscanf="&"
tokenfopen="("
tokenequal="="
espace=" "
par=","
fin=")"
temp=0
temp2=0
rm -r output
echo '{"data": [' >> output
for line in $(cat /root/Desktop/BugInjectorTN/site-extractor-develop/tests/output.txt)
do
j=${#line}
let j--
for i in `seq 0 $j`
do

  #Pour récuperer le fichier et la ligne
  if [ ${line:i:1} = $fileMatcher ]; then
    temp2=-1
  fi
  let temp2++

  if [ ${line:i:1} = $lineMatcher ]; then
    let i++
      if [ ${line:$(($i)):1} = $fctMatcher ]; then
        i=$(($i-temp2))
        let temp2--
        rm -r outputtemp
        #echo ";" >> output.txt
        echo '{"file": "'${line:0:$(($i-1))}'",' >> output
        echo '"function line": "'${line:$(($i)):$(($temp2))}'",' >> output

        echo '{"file": "'${line:0:$(($i-1))}'",' >> outputtemp
        echo '"function line": "'${line:$(($i)):$(($temp2))}'",' >> outputtemp
      fi

      #On regarde si c'est bien la récupération d'une variable utilisateur
      if [ ${line:$(($i)):1} = $varMatcher ]; then
        #On regarde si c'est le cas scanf
        if [ ${line:$(($i+1)):1} = $scanfmatcher ]; then
          i=$(($i-temp2))
          let temp2--
          echo '"scanf ligne": "'${line:$(($i)):$(($temp2))}'",'>> output
          echo '"scanf ligne": "'${line:$(($i)):$(($temp2))}'",'>> outputtemp
          #On sauvegarde le cas
          fct=0
        fi
    #On regarde si c'est le cas fopen
        if [ ${line:$(($i+1)):1} = $fopenmatcher ]; then
          i=$(($i-temp2))
          let temp2--
          echo '"fopen line": "'${line:$(($i)):$(($temp2))}'",' >> output
          #On sauvegarde le cas
          fct=1
        fi
      fi
    #On regarde si c'est le cas d'une variable implémentée avec un fopen
    if [ ${line:$(($i)):1} = $fopenvarmatcher ]; then
      l=$(($i+1))
      m=$(($i))
      count=0
      printed=0
      while [ $m -lt $j ]; do

        #Ici on cherche le = marquant la fin du nom de la variable de fin
        if [ "${line:l:1}" = "$tokenequal" ]; then
          echo '"variable": "'${line:$(($i+1)):$(($count))}'",' >> output
          i=$(($m))
          m=$(($j))
          let printed++
        fi
        let m++
        let l++
        let count++
      done
      #Si l'utilisateur met un espace entre le nom de sa variable et le = :
      if [[ "$printed" -eq 0 ]]; then
        echo '"variable": "'${line:$(($i+1)):$(($count))}'",' >> output
        printed=1
      fi
    fi

  fi

  #Cas scanf
  if [ "$fct" -eq "0" ]; then
    #On regarde quand on trouve &
    if [ ${line:i:1} = $tokenscanf ]; then
      l=$(($i+1))
      m=$(($i))
      while [ $m -lt $j ]; do

        #Si on a trouvé &, c'est qu'on a potentiellement une variable,
        #Il faut chercher la fin de la variable, marquée par , ou )

        #Ici on cherche ,
        if [ "${line:l:1}" = "$par" ]; then
          echo '"variable": "'${line:$(($i+1)):$(($m))}'"}' >> output
          cat outputtemp >> output
          i=$(($m))
          m=$(($j))
        fi

        #Ici on cherche )
        if [ "${line:l:1}" = "$fin" ]; then
          echo '"variable": "'${line:$(($i+1)):$(($m))}'"},' >> output
        fi
        let m++
        let l++
      done

    fi
  fi


  #Cas fopen
  if [ "$fct" -eq "1" ] && [ "$printed" -eq "0" ]; then
    #On regarde quand on trouve &
    if [ ${line:i:1} = $tokenfopen ]; then
      l=$(($i+1))
      m=$(($i))
      count=0
      while [ $m -lt $j ]; do

        #Ici on cherche le guilletmet de fin
        if [ "${line:l:1}" = "$par" ]; then
          echo '"file": '${line:$(($i+1)):$(($count))}',' >> output
          i=$(($m))
          m=$(($j))
        fi

        let m++
        let l++
        let count++
      done

    fi
  fi
  let i++
done
done

sed '$ s/.$//' output > output2
sed '$ s/.$//' output > output2
echo "}]" >> output2
echo "}" >> output2

cat output2 > output.json

rm -r output
rm -r output2
rm -r outputtemp

#tr -d '\n' < output2 > output.json
