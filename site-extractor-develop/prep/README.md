# Preparation scripts for Satese and Callgrind outputs intersection

> The Site Extractor use Clang/LLVM and LibASTMatchers to find potential sites where to inject bugs for SATE VI test cases in C.
> Callgrind is a profiling tool that records the call history among functions in a program's run.
> The idea behind those scripts is to use Callgrind to get the input files from Wireshark (pcap) going through the different functions and to make the intersection of those results and the ones from Satese.

## Requirements 

Python

## Description

* `./callgrind_all.sh` dumps the call history of all the inputs inside a folder. (Output names: callgrind.out.id) 
* `python format.py` takes all the files generated before and keep only the function name and the file name (Output names: callgrind.out.id.formated)
* `ls *.formated | xargs -L 1 ./sanitize.sh` sorts the content of the files generated before and removes duplicates (Output names: callgrind.out.id.formated.clean)
* `python dictionary.py > occurencesFunctionsWireshark.txt` browses all files and puts the occurence of function calls in a dictionnary alongside their file name. 
* `python satesePrep.py` prepares the occurencesFunctionsWireshark.txt in order to process it easily in C++
* The final step consists of using Satese "intersec" matchers. Those matchers call a different print function: `Matcher::printIntersec` defined in `matcher.cpp`. This function prints intereseting sites only if they are located inside one of our function.

Finally, we should have more interesting results in a more reasonable number. We also have the input calling the functions where are located the sites.

## Meta

Guillaume HABEN – guillaume.haben@nist.gov