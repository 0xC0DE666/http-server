FROM debian:stable AS base

# Install dependencies
RUN apt-get update && apt-get install -y build-essential libcriterion-dev;

# Set working directory
WORKDIR /root/app

# Copy the application code
COPY ./src ./src
COPY Makefile .
RUN mkdir bin;

# Setup env
ENV PORT=8080
ENV WWW_DIR=/srv/www
EXPOSE 8080

# Dev image
FROM base AS dev
RUN make;
RUN ./bin/test;
CMD ["./bin/app"]
