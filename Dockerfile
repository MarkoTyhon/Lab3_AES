FROM gcc:latest

RUN apt-get update && apt-get install -y cmake make
RUN apt-get install -y libncurses5-dev libncursesw5-dev
# These commands copy your files into the specified directory in the image
# and set that as the working location
COPY . /usr/src/ACS3
WORKDIR /usr/src/ACS3

# This command compiles your app using GCC, adjust for your source code
RUN cmake -S . -B ./build
RUN cmake --build ./build

# This command runs your application, comment out this line to compile only
CMD ["./build/main"]

LABEL Name=lab3_aes Version=0.0.1
\