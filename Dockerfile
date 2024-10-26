FROM alpine:3.20.3

# Install dependencies
RUN apk add --no-cache build-base

# Set working directory
WORKDIR /root/app

# Copy the application code
COPY ./src ./src
COPY Makefile .
RUN mkdir bin
RUN make

# Run the application
EXPOSE 8080
CMD ["./bin/app"]
