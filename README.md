# hardware_section
### TO-DO
*  Setup Hostspot with root node
*  Connect PI to root's hotspot
*  Make requests withing them

```c++
#define SERVER_PATH "http://10.192.18.110:3000/summary/current?accountId="
#define VOLTAGE_PATH "http://10.192.18.110:3000/summary/" + accountId + "/voltage"
String accountId = "62cc2c675065faa3815663ec";
```
<p>The variables above are the most important for data transfer between devices and the server. When you set it up on
the Raspberry  PI, configure your own static ip and replace it here, or use the IP defined here as the static IP </p>


```c++
bool isRoot = true;
```
<p> For the accound Id, you have to write it here manually for now.<p>
<p> Remember to set 'isRoot' to false when uploading to other nodes and true when uploading to the root note <p>


## Start Sequence
1. Start node 1
2. Connect smart phone to HOTSPOT set up by node. If you want to run this with the smart hub, post all the data to the loopback address of the smart hub '127.0.0.1' or the smart hubs IP address
3. Power OTher nodes and wait for a 200 status code in the serial monitor
4. Start the backend server 
```sh
nodemon index.js
```

5. Before demonstrating, run the app and everything that day. Errors may occur in the backend. Just restart the server so the database updates its values

# Kindly report Errors to me

