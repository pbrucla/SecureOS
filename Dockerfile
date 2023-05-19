FROM ubuntu:20.04

RUN apt-get update -y
RUN apt-get install build-essential gcc-i686-linux-gnu -y
RUN apt-get install nasm -y

RUN mkdir -p /app/
COPY ./Makefile /app/Makefile
COPY ./source /app/source

CMD ["make", "image"]
