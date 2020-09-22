FROM ubuntu:20.04

# Instalacion libpng
RUN apt-get update && \
    apt-get install -y libpng-dev && \
    rm -rf /var/lib/apt/lists/*

RUN mkdir psot1 && mkdir psot1-dstorage

ADD server /psot1

EXPOSE 8080

CMD [ "/psot1/server" ]