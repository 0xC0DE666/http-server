FROM debian:stable AS base

# Install dependencies
RUN apt-get update && apt-get install -y build-essential libcriterion-dev

# Set working directory
WORKDIR /root/app

# Copy the application code
COPY ./src ./src
COPY Makefile .
RUN mkdir bin
RUN make release

# Run tests
CMD ["./bin/test"]

# Run the application
EXPOSE 8080
CMD ["./bin/app"]
