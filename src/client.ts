import {readLines} from "https://deno.land/std/io/mod.ts";
// Import necessary Deno API
const addr = {hostname: "127.0.0.1", port: 8080};

// Connect to the TCP server
const conn = await Deno.connect(addr);
console.log("Connected to the server");

while (true) {
  console.log("Please enter some lines of input:");

  for await (const line of readLines(Deno.stdin)) {
    console.log(`You entered: ${line}`);

    // Message to send to the server
    const encoder = new TextEncoder();
    // const message = "Hello from Deno TCP client!";
    const message = line;
    await conn.write(encoder.encode(message));
    console.log(`Sent message: ${message}`);

    // Read response from server
    const buffer = new Uint8Array(1024);
    const bytesRead = await conn.read(buffer);
    if (bytesRead) {
      const decoder = new TextDecoder();
      const response = decoder.decode(buffer.subarray(0, bytesRead));
      console.log(`Received message: ${response}`);
    }
  }
}

// Close the connection
conn.close();
