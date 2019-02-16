# 20 Ways Relay Array Control on MT7688 Duo
This is a project to control two 8 ways relay array (total 16 ways) by one MT7688 Duo.
Control and status check are done by sending REST calls.

## Install
### Install Arduino side code
- source: `mcu/relay-array-control.ino`

### Install SSH-KEY to OpenWRT to login without password
1. generate key
```
λ ssh-keygen -t rsa -b 2048
Generating public/private rsa key pair.
Enter file in which to save the key (...): id_rsa
Enter passphrase (empty for no passphrase):
Enter same passphrase again:
Your identification has been saved in id_rsa.
Your public key has been saved in id_rsa.pub.
The key fingerprint is:
...
The key's randomart image is:
...
```
2. install public key to OpenWRT
`ssh root@192.168.1.127 "tee -a /etc/dropbear/authorized_keys" < id_rsa.pub`
3. try login
`ssh -i id_rsa root@192.168.1.127`

### Deploy app to OpenWRT
1. copy app
`scp -i id_rsa mpu/app.js root@192.168.1.127:/root`
2. run manually on device
`/usr/bin/node --harmony /root/app.js`

### Install service
1. install service (so it will run on startup)
`scp -i id_rsa mpu/app root@192.168.1.127:/etc/init.d`
2. make the service executable
`ssh -i id_rsa root@192.168.1.127 chmod 755 /etc/init.d/app`
3. start the service
`ssh -i id_rsa root@192.168.1.127 /etc/init.d/app enable`

### Install test script (optional)
1. copy app
`scp -i id_rsa mpu/test.sh root@192.168.1.127:/root`
2. make it executable
`ssh -i id_rsa root@192.168.1.127 chmod 755 /root/test.sh`
3. run test
`ssh -i id_rsa root@192.168.1.127 /root/test.sh`

## Usage
### Turn on / off test mode
- When multiple boards exist, we may want to identify which one is which.
To do so, turn one board to test mode, the LED light (pin 13) will blink very fast.
`curl 192.168.1.127:8080/test`
- A normal read will turn the test mode off
`curl 192.168.1.127:8080`
- The response is a JSON object with pin index as the key, and on/off as a boolean values (true is ON).
```
{
    "0": false,
    "1": false,
    "2": false,
    "3": false,
    "4": false,
    "5": false,
    "6": false,
    "7": false,
    "8": true,
    "9": false,
    "10": true,
    "11": true,
    "12": false,
    "13": false,
    "14": false,
    "15": false,
    "16": false,
    "17": false,
    "18": false,
    "19": false
}
```

### Turn one pin on and off
- Take the response JSON format as the request body, and use method: POST, for example:
`curl -X POST -d "{\"2\": true}" http://192.168.1.127:8080`
- Only list the pin and set it to `true` if you want to turn it on.  Others will be turned off.

## Technical Details

### Pin-out
- Digital pin D17, D0, and D1 are reserved for the system. These pins having random signals during boot up.
- Digital pin D13 is for LED for test and signal indicator purpose.

ID | PIN | LABEL
-- | --- | -----
0  | D14 | S0
1  | D15 | S1
2  | D16 | S2
3  | D2  | D2
4  | D3  | D3
5  | D4  | D4
6  | D5  | D5
7  | D6  | D6
8  | D7  | D7
9  | D8  | D8
10 | D9  | D9
11 | D10 | D10
12 | D11 | D11
13 | D12 | D12
14 | D18 | A0
15 | D19 | A1
16 | D20 | A2
17 | D21 | A3
18 | D22 | A4
19 | D23 | A5

### Signal
- The serial communication between MPU and MCU is a 3 bytes packet.
- byte[0] bit 0 ~ 7 maps to pin id 0 ~ 7
- byte[1] bit 0 ~ 7 maps to pin id 8 ~ 15
- byte[2] bit 0 ~ 3 maps to pin id 16 ~ 19
- byte[2] bit 6 -> 0: test mode off, 1: test mode on
- byte[2] bit 7 -> 0: read, 1: write
