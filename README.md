# Distributed-Key-Value-Store-using-gRPC
 
 This project is a part of a course 6210- Advanced Operating Systems taken in spring 2022
 
 
## 1. Introduction
Implemented GT Store, the distributed key-value store system,
by network calls for simulating the shopping processes.
GT Store is a centralized manager to multiple data nodes.
For the clients, the driver application, will perform gets and sets to key/value pairs while we manually take storage nodes offline to ensure that the service can recover from node failure.
 
 ### System Components

1. **GT Store**
   1. **Centralized manager**: One user-space process will handle all the control path decisions of the key-value store. The manager's tasks include, but are not limited to:
      1. Membership management
      2. Load balancing of clients across replica nodes
      3. Index management. E.g., you could (but not required) allow clients first contact centralized manager on initialization to figure out the data nodes where they should send their requests.

   2. **Storage/Data nodes**: N user-space processes will act as the storage nodes in the key-value store.
They will store in-memory and provide the key-value pairs, those they are responsible for.
These processes represent the actual key-value store.

2. **Driver application**: ~~M user-space processes (or threads)~~ A single process [that interacts with GT Store](./project_4_demo_and_report.md).
In a typical interaction, the application
	1. reads the latest version of the object
	2. mutates it, and
	3. stores it back.

3. **Client library**: The driver applications should use a simple programming interface, in order to interact with the key-value store. The base API includes;
   1. `init()`: initialize the client session with the manager and other control pathoperations.
   2. `put(key, value)`
   3. `get(key)`
   4. `finalize()`: control path cleanup operation.
