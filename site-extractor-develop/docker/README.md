## Docker

We built the site extractor inside a docker image to ease the long installation process of LLVM/clang.
Here is the [Dockerfile](./Dockerfile) used to build this image.

#### How to use docker:

How to install Docker on Ubuntu: https://docs.docker.com/install/linux/docker-ce/ubuntu/#upgrade-docker-ce

* `docker load --input site-extractor-built.tar` will load the tarball in a Docker image.
* `docker run --name satese -it -v /path/to/site-extractor:/clang-llvm/llvm/tools/clang/tools/extra/satese site-extractor-built /bin/bash` will run a new container from the previously loaded image.
* `docker start satese` will restart the container if you exited it.
* `docker exec -it satese /bin/bash` will open a command line inside the container.
* `docker rm satese` will remove the container.
