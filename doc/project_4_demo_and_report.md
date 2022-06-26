# Project 4 Demo & Report
For project 4, we will have you demo your system handling some failure use cases. We will also have you run a few performance tests and include those in a report with a description of your design. Your client/service implementation should be able to correctly handle the following test sequences:

## Starting the Service
You should have some mechanism for starting the service with `n` storage nodes and `k` replicas. You can start the service from a single binary and fork the storage nodes or you can launch them separately.
```shell
./start_service --nodes 1 --rep 1 #Start the service. Your particular interface may be different
```
or as the current starter code is structured:

```shell
./bin/manager &
./bin/storage &
./bin/storage &
...
```

Because we are allowing you to demo your code, you can choose whatever is easiest for you. You must also provide some way to kill storage processes. This can be done directly by sending a signal to the storage process pid e.g. `kill -9 pid`, or you can design a different way of doing this. We will use this capability to test for availability in the case of storage node permanent failure.

## Client Driver
Please write a client driver program that connects to the service, and performs a single GET or PUT operation:

```shell
./client --put <key> --val <value>
> OK, server id

./client --get <key>
> key, value, server id
```

The client can take other parameters like the master's address `--master "0.0.0.0:50051"` or anything else you want to add. You will use this program to step through different failure situations while interacting with the server. To make parsing easier, you can assume both and key and value should be stored as strings, and neither contains spaces.

## Test 1: Basic Single Server GET/PUT
Should be able to store, retrieve, and overwrite key/value pairs.

```shell
./start_service --nodes 1 --rep 1

./client --put key1 --val value1
> OK, server1

./client --get key1
> key1, value1, server1

./client --put key1 --val value2
> OK, server1

./client --put key2 --val value3
> OK, server1

./client --put key3 --val value4
> OK, server1

./client --get key1
> key1, value2, server1

./client --get key2
> key2, value3, server1

./client --get key3
> key3, value4, server1
```

## Test 2: Basic Multi-Server GET/PUT
Same trace as above, but with multiple storage nodes. The hashed keyspace should be distributed across the nodes, so the keys should not all go the same server. The server id's in the log sequences should be self-consistent.

```shell
./start_service --nodes 5 --rep 3

./client --put key1 --val value1
> OK, server...

./client --get key1
> key1, value1, server1

./client --put key1 --val value2
> OK, server...

./client --put key2 --val value3
> OK, server...

./client --put key3 --val value4
> OK, server...

./client --get key1
> key1, value2, server...

./client --get key2
> key2, value3, server...

./client --get key3
> key3, value4, server...
```

## Handling Node Failure:
The service should handle a node failure as follows:
1. The node is manually killed e.g. `kill -9 <pid>`
2. The master detects this and organizes the service to adjust in the following way:
    - ~~The key space is rebalanced, meaning the regions that the failed node was the primary for are split evenly and re-assigned to the other nodes~~ The keys that the failed node was the primary for should be assumed by a different node/nodes.
    - ~~The keys that the failed node was backing up, must be copied to other nodes so that each entry is backed backed up `k` times again.~~ Requirement eliminated.
3. The client's table of servers is now out of date. If the client tries to contact the failed node, it should time out, and the client should ask the master for a new table, and then re-send the request.

## Test 3: Availability through a single node failure

```shell
./start_service --nodes 3 --rep 2

./client --put key1 --val value1
> OK,serverX
./client --put key1 --val newvalue1
> OK,serverX

#Insert more key,value pairs until 3 of them are mapped to serverX

## kill serverX

./client --get key1
> key1, newvalue1, server...

# do gets for the other keys that were mapped to serverX, they should be on some other node now with the same value.
```

There are different ways of doing the redistribution upon failure, so your results may look different, they just need to be self-consistent. The latest version of the key should be available ~~, and there should always be `k` replicas of an inserted key.~~

## Test 4: Availability through multiple node failures:

We are not showing a full hypothetical output for this example, you can ask clarifying questions on piazza if you are unsure if an output is reasonable. Because this test features a larger number of insertions, you may find it easier to write another client program which has the trace and expected outputs as static data, and then prints some summarized version of the output.

```shell
./start_service --nodes 7 --rep 3

# Insert around 20 keys, so that they are split across at least 4-5 different servers. Overwrite at least 3 of them.
# Kill 2 servers
# Query the inserted keys again, make sure the most recent version of the values are returned.
```

## Performance Tests (Report)

Write a different client program that will perform the following more intensive performance benchmarks:

### Throughput

Perform 200k operations (50/50 read/write mix), on 7 nodes, with 1, 3, and 5 replicas. Plot the troughput in (Ops/s) against number of replicas in a bar chart.

### Loadbalance

Perform 100k inserts, on 7 nodes, with ~~3~~ 1 replica, and then print a histogram of number of keys vs. node id, to show that they keyspace is balanced across nodes. ~~Then kill 3 of the nodes and create another histogram showing the re-balanced keyspace.~~ Requirement removed, just do the first chart.

