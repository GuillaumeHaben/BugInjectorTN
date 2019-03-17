##This file will explain you how to run the entire project. If you need help on specific part of it, other readme are present at the location of each part.


## Docker

We built the site extractor inside a docker image to ease the long installation process of LLVM/clang.
Here is the [Dockerfile](./Dockerfile) used to build this image.

#### How to use docker:

How to install Docker on Ubuntu: https://docs.docker.com/install/linux/docker-ce/ubuntu/#upgrade-docker-ce

First of all, don't forget to start you docker service

* `docker service start`

Then do the following for the first installation

* `docker load --input site-extractor-built.tar` will load the tarball in a Docker image.
* `docker run --name satese -it -v /path/to/site-extractor:/clang-llvm/llvm/tools/clang/tools/extra/satese site-extractor-built /bin/bash` will run a new container from the previously loaded image.

If the tarball is already loaded and the container already existing:

* `docker start satese` will restart the container if you exited it.

Finally, to open a command line:

* `docker exec -it satese /bin/bash` will open a command line inside the container.

Other usefull command:

* `docker rm satese` will remove the container.

##Cland Matchers
When inside the conatainer, you should link the binary to its proper location to avoid include failures during the execution: `ln -s /clang-llvm/build/bin/satese /usr/local/bin/satese`

Then, you should build the project:

* `$ cd /clang-llvm/build`
* `$ ninja satese`

Finally, to check the matching sites: (if you want to add a new matcher, create a new one in this folder: site-extractor-develop/src/matchers)

##On a single file
* `satese ../llvm/tools/clang/tools/extra/satese/tests/FILE.c --`

Redirect the exit inside an output file in the same folder by adding

* ` > output.txt` at the end of the previous command

--------------------------------------
##On a project

You need to compile your project with bear option to generate a `compile_database.json` file that will allow the site extractor to run through all the project.
Example with Wireshark:
```sh
$ cd your/project
$ ./configure --disable-wireshark --disable-glibtest --disable-dftest CC=clang # Using Clang as compiler
$ bear make -j 6 # To run on 6 cores
$ cd clang-llvm/build
$ satese -b your/project/compile_commands.json -a 2>/dev/null```

--------------------------------------

Open a new command line and go to `path-to-site-extractor/site-extractor-develop/src/Scripts`

And run the script:

* `./var.sh`

You will get an output name `output.json` You should open it to check if there is not any problems (for instance a '{' missing)



##JOERN - module 2

###setup joern

First, add the image neepl/joern to your docker : https://hub.docker.com/r/neepl/joern/

Create a folder where you will put your code, here it will be joern9

Put the code that you want to analyze in it

Launch the following command (adapt the path to your folder)
* `sudo docker run --name joern-construct -v /path/to/your/folder:/code -p 7474:7474 -p 7687:7687 --rm -w /code -it neepl/joern java -jar /joern/bin/joern.jar .`

The database for joern is now created, we will create the container that we will work with.

--------------------------------------

Launch the following command to create the container :
* `sudo docker run --name joern-run -v /path/to/your/folder:/code -p 7474:7474 -p 7687:7687 -it neepl/joern /var/lib/neo4j/bin/neo4j console`

When you see the line `Remote interface ready and available at ...` the container is successfully launched and fonctionnal
You can now stop the container with CTRL+C

You have now a working container with joern and neo4j running on it, you can start and stop it as usual.

####Some commands : 
Launch the container :
	sudo docker start joern-run
Enter in the container :
	sudo docker exec -it joern-run /bin/bash
Stop the container :
	sudo docker stop joern-run

--------------------------------------

###Use the joern UC variables detection script

First you should put the python script in your “/code” folder.
You should also put the output.json from the clang UC variables detection script in your “/code” folder.
As a reminder, the “/code” folder is the folder where you did put the C code where you want to inject vulnerabilities.

You can now (in the joern container) launch the command `python script.py` in the “/code” folder.

It will generate all the outputs for the next module.


