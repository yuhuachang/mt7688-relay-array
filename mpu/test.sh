#!/bin/sh

echo "test mode on"
curl http://127.0.0.1:8080/test 2>&1 1>/dev/null

echo "all on"
curl -X POST -d "{\"0\": true,\"1\": true,\"2\": true,\"3\": true,\"4\": true,\"5\": true,\"6\": true,\"7\": true,\"8\": true,\"9\": true,\"10\": true,\"11\": true,\"12\": true,\"13\": true,\"14\": true,\"15\": true,\"16\": true,\"17\": true,\"18\": true,\"19\": true}" http://127.0.0.1:8080/ 2>&1 1>/dev/null
sleep 2

echo "all off"
curl -X POST -d "{\"0\": false,\"1\": false,\"2\": false,\"3\": false,\"4\": false,\"5\": false,\"6\": false,\"7\": false,\"8\": false,\"9\": false,\"10\": false,\"11\": false,\"12\": false,\"13\": false,\"14\": false,\"15\": false,\"16\": false,\"17\": false,\"18\": false,\"19\": false}" http://127.0.0.1:8080/ 2>&1 1>/dev/null
sleep 2

for i in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
do
    echo "$i on"
    curl -X POST -d "{\"${i}\": true}" http://127.0.0.1:8080/ 2>&1 1>/dev/null
    sleep 0.2
done

echo "all off"
curl -X POST -d "{}" http://127.0.0.1:8080/ 2>&1 1>/dev/null

echo "test mode off"
curl http://127.0.0.1:8080/ 2>&1 1>/dev/null

echo "test complete!"
