# Bet-Game
Bet-Game is a concurrent TCP betting game server that allows multiple clients to connect, place a bet within a defined number range, and receive the result of the game every 15 seconds. This implementation uses gRPC as the communication protocol for efficient and structured client-server messaging.

## 📦 Features
- Accepts **IPv4 and IPv6** connections on TCP port **2222**
- Supports up to **64,500 simultaneous clients**
- Uses **gRPC** for binary communication
- Generates a **random winning number** every 15 seconds
- Disconnects clients after each betting round
- **Logs all winners** to stdout or syslog
- Designed to run **indefinitely without user interaction**


## 📐 Game Flow

1. A client connects and sends an `open` request.
2. If the server has not reached the client limit:
   - It responds with an `Accept` message containing:
     - A unique `Client ID`
3. The client must send exactly **one** `bet` message with a number within the allowed range.
4. Every **15 seconds**, if there are betting clients:
   - The server draws a winning number.
   - Sends a `result` message to each participating client.
   - Indicates whether the client won.
   - Closes the client connection.
5. All winners are printed to the log or stdout.
