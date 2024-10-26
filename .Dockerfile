FROM debian:stable AS base

####################
# Stage: server-base
####################
FROM base AS server-base
# Install dependencies
RUN apt-get update && apt-get install -y build-essential libcriterion-dev
# Set working directory
WORKDIR /root/app
# Copy the application code
COPY ./src ./src
COPY Makefile .
RUN mkdir bin

####################
# Stage: server-dev
####################
FROM server-base AS server-dev
# Dev build
CMD ["make"]
# Run tests
CMD ["./bin/test"]
# Run the application
CMD ["./bin/app"]

####################
# Stage: server-build
####################
FROM server-dev AS server-build
EXPOSE 8080
RUN make release
