# Dockerfile for SmartBP

FROM ubuntu:22.04

WORKDIR /home/app

COPY . /home/app/

CMD make station
