FROM debian:stable AS base

# Install dependencies
RUN apt-get update && apt-get install -y build-essential libcriterion-dev;

# Set working directory
WORKDIR /root/app

# Copy the application code
COPY ./src ./src
COPY Makefile .
# COPY watch.sh .
# RUN chmod +x watch.sh;
RUN mkdir bin;

# Setup env
ENV PORT=8080
ENV WWW_DIR=/srv/www
EXPOSE 8080

# Run tests
FROM base AS dev
# RUN apt-get install -y inotify-tools;
# RUN make;
# RUN ./bin/test;
# CMD ["./bin/app"]
CMD ["sh", "-c", "make && ./bin/app"]
