#!/bin/bash
#Find the name of variable, generated with a fopen or a scanf

fileMatcher="@"
lineMatcher=":"
fctMatcher="i"
varMatcher="v"
scanfmatcher="s"
fopenmatcher="f"
fopenvarmatcher="t"

#Will change this value: 0 if scanf, 1 if fopen
fct=-1

eg="="
count=0
printed=0
tokenscanf="&"
tokenfopen="("
tokenequal="="
tokenguillemet='"'
espace=" "
par=","
fin=")"
temp=0
temp2=0
echo '' > outputtemp

echo '{"data": [' >> output
#Deleting the spaces, that scripting isn't working well with
sed -e "s/ //g" /root/Desktop/BugInjectorTN/site-extractor-develop/tests/output.txt > /root/Desktop/BugInjectorTN/site-extractor-develop/tests/outputtemp
#Deleting '*', that are not working well with script (need to make more tests to know if this won't create problems with pointers but for now it is working)
sed -e "s/*//g" /root/Desktop/BugInjectorTN/site-extractor-develop/tests/outputtemp > /root/Desktop/BugInjectorTN/site-extractor-develop/tests/output
rm -r /root/Desktop/BugInjectorTN/site-extractor-develop/tests/outputtemp
#Path to the output file of Clang
for line in $(cat /root/Desktop/BugInjectorTN/site-extractor-develop/tests/output)
do
j=${#line}
let j--
for i in `seq 0 $j`
do

  #To get the file path
  if [ ${line:i:1} = $fileMatcher ]; then
    temp2=-1
  fi
  let temp2++

  #To get the line
  if [ ${line:i:1} = $lineMatcher ]; then
    let i++
      if [ ${line:$(($i)):1} = $fctMatcher ]; then
        i=$(($i-temp2))
        let temp2--
        rm -r outputtemp
        echo '{"file": "'${line:0:$(($i-1))}'",' >> output
        echo '"function line": "'${line:$(($i)):$(($temp2))}'",' >> output
        printed=0
#We are saving in two different files. The second one is temporary,
#to save the line and the file path in case there is more than one variable on the same line
        echo '{"file": "'${line:0:$(($i-1))}'",' >> outputtemp
        echo '"function line": "'${line:$(($i)):$(($temp2))}'",' >> outputtemp
      fi

      #Looking for the token defined in the matcher
      if [ ${line:$(($i)):1} = $varMatcher ]; then
        #scanf case
        if [ ${line:$(($i+1)):1} = $scanfmatcher ]; then
          i=$(($i-temp2))
          let temp2--
          echo '"scanf ligne": "'${line:$(($i)):$(($temp2))}'",'>> output
          echo '"scanf ligne": "'${line:$(($i)):$(($temp2))}'",'>> outputtemp
          #We save that it's scanf
          fct=0
        fi
    #fopen case
        if [ ${line:$(($i+1)):1} = $fopenmatcher ]; then
          i=$(($i-temp2))
          let temp2--
          echo '"fopen line": "'${line:$(($i)):$(($temp2))}'",' >> output
          #Save
          fct=1
        fi
      fi
    #Looking for the token corresponding to the variable (in the matcher)
    #It's the beginning of the variable name
    if [ ${line:$(($i)):1} = $fopenvarmatcher ]; then
      l=$(($i+1))
      m=$(($i))
      count=0
      printed=0
      while [ $m -lt $j ]; do

        #Looking for the token corresponding to the end of the variable name '='
        if [ "${line:l:1}" = "$tokenequal" ]; then
          echo '"variable": "'${line:$(($i+1)):$(($count))}'"}' >> output
          i=$(($m))
          m=$(($j))
          let printed++
        fi
        let m++
        let l++
        let count++
      done
    fi

  fi

  #Scanf case
  if [ "$fct" -eq "0" ]; then
    #Token corresponding to the beginning of the varaible name '&'
    if [ ${line:i:1} = $tokenscanf ]; then
      l=$(($i+1))
      m=0

      while [ $m -lt $j ]; do

        #The end of the variable is ',' or '('

        #Looking for ','
        #Case scanf
        if [ "${line:l:1}" = "," ] && [ "$fct" -eq "0" ]; then
          echo '"variable": "'${line:$(($i+1)):$(($m))}'"}' >> output
          printed=0
          cat outputtemp >> output
          i=$(($m))
          m=$(($j))
        fi
        #Case fopen
        if [ "${line:l:1}" = "$par" ] && [ "$fct" -eq "1" ]; then
          echo '"variable": "'${line:$(($i+1)):$(($m))}'""}' >> output
          printed=0
          cat outputtemp >> output
          i=$(($m))
          m=$(($j))
        fi

        #Looking for ')'
        if [ "${line:l:1}" = "$fin" ]; then
          echo '"variable": "'${line:$(($i+1)):$(($m))}'"},' >> output
          printed=0
        fi
        let m++
        let l++
      done

    fi
  fi


  #fopen case
  if [ "$fct" -eq "1" ] && [ "$printed" -eq "0" ]; then
    #Looking for the '(' in fopen(...) corresponding to the beginning of the file name
    if [ ${line:i:1} = $tokenfopen ]; then
      l=$(($i+1))
      m=$(($i))
      count=0
      while [ $m -lt $j ]; do

        #Looking for the ',' corresponding to the end of the file name
        if [ "${line:l:1}" = "$par" ]; then
          if [ ${line:$(($i+1)):1} = $tokenguillemet ]; then
            echo '"file": '${line:$(($i+1)):$(($count))}',' >> output
          else
            echo '"file": "'${line:$(($i+1)):$(($count))}'",' >> output
          fi
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

#Format changes to look like a Json file
sed '$ s/.$//' output > output2
sed '$ s/.$//' output > output2
echo "}]" >> output2
echo "}" >> output2

cat output2 > output.json

#Cleaning the folder
rm -r output
rm -r output2
rm -r outputtemp

#tr -d '\n' < output2 > output.json
